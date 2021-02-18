#include "Injection.h"

BOOL InitializeMapData(PINJECT_DATA pData, PMANUAL_MAP_DATA pMapData, PPE_DATA pPeData, PBYTE pReadBuffer)
{
	PVOID remoteBase =
		VirtualAllocEx(
			pData->ProcessHandle,
			NULL, 
			pPeData->pNtHeaders->OptionalHeader.SizeOfImage, 
			MEM_COMMIT | MEM_RESERVE,
			PAGE_EXECUTE_READWRITE
		);


	if (!WriteProcessMemory(pData->ProcessHandle, remoteBase, pReadBuffer, pPeData->pNtHeaders->OptionalHeader.SizeOfHeaders, NULL))
	{
		return FALSE;
	}

	pPeData->pSectionHeader = IMAGE_FIRST_SECTION(pPeData->pNtHeaders);
	PCHAR szDebug = (PCHAR)pPeData->pSectionHeader[0].Name;
	if(!strcmp(szDebug, ".textbss"))
	{
		MessageBoxA(NULL, "Not support debug build DLL, should be release build", "Shh0ya", MB_OK);
	}

	for (int i = 0; i < pPeData->pNtHeaders->FileHeader.NumberOfSections; i++)
	{
		if (!WriteProcessMemory(
			pData->ProcessHandle,
			(PVOID)((DWORD64)remoteBase + pPeData->pSectionHeader[i].VirtualAddress),
			(PVOID)((DWORD64)pReadBuffer + pPeData->pSectionHeader[i].PointerToRawData),
			pPeData->pSectionHeader[i].SizeOfRawData,
			NULL
		))
		{
			DWORD dwErr = GetLastError();
			MessageBoxA(NULL, "Er2", NULL, MB_OK);
			return FALSE;
		}
	}

	pMapData->ImageBase			= remoteBase;
	pMapData->pDosHeader		= (PIMAGE_DOS_HEADER)remoteBase;
	pMapData->fnLoadLibraryA	= LoadLibraryA;
	pMapData->fnLoadLibraryW	= LoadLibraryW;
	pMapData->fnGetProcAddress	= GetProcAddress;
	pMapData->MessageBoxT		= MessageBoxA;

	

	return TRUE;
}


VOID WINAPI ShellCode(PMANUAL_MAP_DATA pMapData)
{
	PBYTE pBase = (PBYTE)pMapData->ImageBase;
	pMapData->pNtHeaders = (PIMAGE_NT_HEADERS)((DWORD64)pBase + pMapData->pDosHeader->e_lfanew);
	pMapData->pSectionHeader = IMAGE_FIRST_SECTION(pMapData->pNtHeaders);
	PBYTE pDelta = pBase - pMapData->pNtHeaders->OptionalHeader.ImageBase;

	pMapData->pBaseReloc = (PIMAGE_BASE_RELOCATION)((DWORD64)pBase + pMapData->pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

	while (pMapData->pBaseReloc->VirtualAddress)
	{
		DWORD Count = (pMapData->pBaseReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
		PWORD pList = (PWORD)(pMapData->pBaseReloc + 1);

		for (int i = 0; i < Count; i++)
		{
			if (pList[i])
			{
				PDWORD64 ptr = (PDWORD64)((DWORD64)pMapData->ImageBase + (pMapData->pBaseReloc->VirtualAddress + (pList[i] & 0xFFF)));
				*ptr += (DWORD64)pDelta;
			}
		}
		pMapData->pBaseReloc = (PIMAGE_BASE_RELOCATION)(pMapData->pBaseReloc + pMapData->pBaseReloc->SizeOfBlock);
	}

	// Import Descriptor
	pMapData->pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD64)pBase + pMapData->pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	while (pMapData->pImportDesc->Name)
	{
		PCHAR szModule = (PCHAR)((DWORD64)pBase + pMapData->pImportDesc->Name);
		HMODULE hModule = pMapData->fnLoadLibraryA(szModule);

		PIMAGE_THUNK_DATA pOriginalThunk	= (PIMAGE_THUNK_DATA)((DWORD64)pBase + pMapData->pImportDesc->OriginalFirstThunk);
		PIMAGE_THUNK_DATA pFirstThunk		= (PIMAGE_THUNK_DATA)((DWORD64)pBase + pMapData->pImportDesc->FirstThunk);

		if (!pOriginalThunk)
		{
			pOriginalThunk = pFirstThunk;
		}

		while (pOriginalThunk->u1.AddressOfData)
		{
			if (IMAGE_SNAP_BY_ORDINAL(pOriginalThunk->u1.Ordinal))
			{
				pFirstThunk->u1.Function = (DWORD64)pMapData->fnGetProcAddress(hModule, (LPCSTR)(pOriginalThunk->u1.Ordinal & 0xFFFF));
			}
			else
			{
				PIMAGE_IMPORT_BY_NAME pImportName = (PIMAGE_IMPORT_BY_NAME)((DWORD64)pBase + pOriginalThunk->u1.AddressOfData);
				pFirstThunk->u1.Function = (DWORD64)pMapData->fnGetProcAddress(hModule, pImportName->Name);
			}

			pOriginalThunk++;
			pFirstThunk++;
		}

		pMapData->pImportDesc++;
	}

	if (pMapData->pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size)
	{
		PIMAGE_TLS_DIRECTORY pTls = 
			(PIMAGE_TLS_DIRECTORY)((DWORD64)pBase + pMapData->pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
		
		PIMAGE_TLS_CALLBACK* pTlsCallback = (PIMAGE_TLS_CALLBACK*)pTls->AddressOfCallBacks;
		while (*pTlsCallback)
		{
			(*pTlsCallback)(pBase, DLL_PROCESS_ATTACH, NULL);
			pTlsCallback++;
		}
	}


	pMapData->fnDllMain = (DLL_ENTRY_POINT_t)((DWORD64)pMapData->ImageBase + pMapData->pNtHeaders->OptionalHeader.AddressOfEntryPoint);
	pMapData->fnDllMain((HMODULE)pMapData->ImageBase, DLL_PROCESS_ATTACH, NULL);

}

VOID ManualMapper(PINJECT_DATA pData)
{
	FILE_INFO FileInfo = { 0, };
	MANUAL_MAP_DATA MapData = { 0, };
	PE_DATA PeData = { 0, };

	if (!GetFileInformation(pData, &FileInfo))
	{
		return;
	}

	if (!ValidCheckDLL(pData, &FileInfo, &PeData))
	{
		CloseHandle(FileInfo.FileHandle);
		VirtualFree(FileInfo.ReadBuffer, 0, MEM_RELEASE);
		return;
	}

	if (!InitializeMapData(pData, &MapData,&PeData, (PBYTE)FileInfo.ReadBuffer))
	{
		CloseHandle(FileInfo.FileHandle);
		VirtualFree(FileInfo.ReadBuffer, 0, MEM_RELEASE);

		return;
	}

	PVOID pShell = VirtualAllocEx(pData->ProcessHandle, NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!WriteProcessMemory(pData->ProcessHandle, pShell, ShellCode, 0x1000, NULL))
	{
		CloseHandle(FileInfo.FileHandle);
		VirtualFree(FileInfo.ReadBuffer, 0, MEM_RELEASE);
		return;
	}


	PVOID pArgument = VirtualAllocEx(pData->ProcessHandle, NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!WriteProcessMemory(pData->ProcessHandle, pArgument, &MapData, 0x1000, NULL))
	{
		CloseHandle(FileInfo.FileHandle);
		VirtualFree(FileInfo.ReadBuffer, 0, MEM_RELEASE);
		return;
	}

	CodeExecution(pData, pShell, pArgument);

	CloseHandle(FileInfo.FileHandle);
	VirtualFree(FileInfo.ReadBuffer, 0, MEM_RELEASE);

}