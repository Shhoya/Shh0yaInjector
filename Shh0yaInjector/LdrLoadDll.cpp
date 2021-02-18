#include "Injection.h"

VOID LdrStub(PLDR_DLL_DATA pLdr)
{
	pLdr->LdrLoadDll(NULL, NULL, &pLdr->ModuleFileName, &pLdr->ModuleHandle);
}

VOID InjLdrLoadDll(PINJECT_DATA pData)
{
	LDR_DLL_DATA LdrData = { 0, };
	UNICODE_STRING ModulePath = { 0, };
	RtlInitUnicodeString(&ModulePath, pData->DllPathW);
	
	LdrLoadDll_t LdrLoadDll = (LdrLoadDll_t)GetAddress("ntdll.dll", "LdrLoadDll", pData);
	LdrData.LdrLoadDll = LdrLoadDll;
	LdrData.ModuleFileName = ModulePath;


	PVOID pRemoteBuffer = VirtualAllocEx(pData->ProcessHandle, 0, sizeof(LDR_DLL_DATA), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	PVOID pModuleName = VirtualAllocEx(pData->ProcessHandle, 0, ModulePath.MaximumLength, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	PVOID pFunction     = VirtualAllocEx(pData->ProcessHandle, 0, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);


	if (pRemoteBuffer&& pModuleName &&pFunction)
	{

		WriteProcessMemory(pData->ProcessHandle, pModuleName, pData->DllPathW, (wcslen(ModulePath.Buffer)+1)*2, NULL);
		LdrData.ModuleFileName.Buffer = (PWCHAR)pModuleName;

		WriteProcessMemory(pData->ProcessHandle, pFunction, LdrStub, 0x1000, NULL);
		WriteProcessMemory(pData->ProcessHandle, pRemoteBuffer, &LdrData, sizeof(LDR_DLL_DATA), NULL);

		CodeExecution(pData, pFunction, pRemoteBuffer);

	}


}