#include "Injection.h"

VOID InjLoadLibrary(PINJECT_DATA pData)
{
	PVOID pRemoteBuffer = VirtualAllocEx(pData->ProcessHandle, 0, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE);
	if (!pRemoteBuffer)
	{

	}
	else
	{
		if (!WriteProcessMemory(pData->ProcessHandle, pRemoteBuffer, pData->DllPath, strlen(pData->DllPath) + 1, NULL))
		{

		}
		else
		{
			CodeExecution(pData, LoadLibraryA, pRemoteBuffer);
		}
	}
}