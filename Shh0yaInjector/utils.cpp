#include "Injection.h"

PVOID GetAddress(LPCSTR ModuleName, LPCSTR ProcName, PINJECT_DATA pData OPTIONAL)
{
	if (pData != NULL)
	{
		PVOID TargetAddress = GetProcAddress(GetModuleHandleA(ModuleName), ProcName);
		if (TargetAddress == NULL)
		{
			pData->ErrorType = INJ_ERR_NOT_FOUND_ROUTINE;
			return NULL;
		}
		return TargetAddress;
	}
	return GetProcAddress(GetModuleHandleA(ModuleName), ProcName);
}

VOID GetThread(PINJECT_DATA pData)
{
	THREADENTRY32 ThreadEntry = { 0, };
	ThreadEntry.dwSize = sizeof(THREADENTRY32);
	int i = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	Thread32First(hSnapshot, &ThreadEntry);
	while (Thread32Next(hSnapshot, &ThreadEntry))
	{
		if (ThreadEntry.th32OwnerProcessID == pData->ProcessId)
		{
			if (pData->ExecutionMethod == ThreadHijacking)
			{
				pData->MainThread = OpenThread(THREAD_ALL_ACCESS, FALSE, ThreadEntry.th32ThreadID);
				break;
			}
			else
			{
				pData->ThreadId[i] = ThreadEntry.th32ThreadID;
				i++;
			}
		}
	}
	pData->ThreadCount = i;
}

BOOL GetFileInformation(PINJECT_DATA pData, PFILE_INFO pFile)
{
	pFile->FileHandle = CreateFile(pData->DllPathW, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (pFile->FileHandle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	pFile->FileSize = GetFileSize(pFile->FileHandle, NULL);
	if (!pFile->FileSize)
	{
		CloseHandle(pFile->FileHandle);
		return FALSE;
	}

	pFile->ReadBuffer = VirtualAlloc(NULL, pFile->FileSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pFile->ReadBuffer)
	{
		CloseHandle(pFile->FileHandle);
		return FALSE;
	}

	if (!ReadFile(pFile->FileHandle, pFile->ReadBuffer, pFile->FileSize, NULL, NULL))
	{
		CloseHandle(pFile->FileHandle);
		VirtualFree(pFile->ReadBuffer, 0, MEM_RELEASE);
		return FALSE;
	}

	return TRUE;
}

BOOL ValidCheckDLL(PINJECT_DATA pData, PFILE_INFO pFile, PPE_DATA pPeData)
{
	pPeData->pDosHeader = (PIMAGE_DOS_HEADER)pFile->ReadBuffer;
	if (pPeData->pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}

	pPeData->pNtHeaders = (PIMAGE_NT_HEADERS)((DWORD64)pFile->ReadBuffer + pPeData->pDosHeader->e_lfanew);
	if (pPeData->pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

	if (!(pPeData->pNtHeaders->FileHeader.Characteristics & IMAGE_FILE_DLL))
	{
		return FALSE;
	}
	return TRUE;

}