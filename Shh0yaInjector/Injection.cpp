#include "Injection.h"

#pragma warning(disable:4996)

VOID InjectMain(int pid, char* filepath, EXECUTION_METHOD execType, INJECTION_METHOD injectType)
{
	/*char test[256] = { 0, };
	sprintf(test, "%d %d %d", pid, execType, injectType);*/

	INJECT_DATA InjectData = { 0, };
	InjectData.ProcessId = pid;
	InjectData.DllPath = filepath;
	InjectData.ExecutionMethod = execType;
	InjectData.InjectionMethod = injectType;
	SIZE_T size = strlen(filepath) + 1;
	wchar_t* tmpPath = new wchar_t[size];
	mbstowcs(tmpPath, filepath, size);
	InjectData.DllPathW = tmpPath;

	InjectData.ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (InjectData.ProcessHandle == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "Invalid Handle", "Shh0ya", MB_ICONERROR|MB_OK);
		return;
	}

	switch (injectType)
	{
	case LoadLibraryMethod:
		InjLoadLibrary(&InjectData);
		break;
	case LdrLoadDllMethod:
		InjLdrLoadDll(&InjectData);
		break;
	case ManualMapMethod:
		ManualMapper(&InjectData);

	default:
		break;
	}
	
	CloseHandle(InjectData.ProcessHandle);
}

VOID EjectMain(int pid, char* filepath)
{
	MessageBoxA(NULL, "Not implemented", "Shh0ya", MB_ICONWARNING);
	
}
