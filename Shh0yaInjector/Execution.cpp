#include "Injection.h"

/*
Shell Code, size 0x4F(79)
push rax - r15(general register)
pushfq
mov rax, pFunction
mov rcx, pArgument
sub rsp, 0x28
call rax
add rsp, 0x28
popfq
pop r15 - rax
*/
BYTE bHijacking[] = {
	0x50,0x51,0x52,0x53,0x55,0x56,0x57,0x41,	//0x00 - 0x07
	0x50,0x41,0x51,0x41,0x52,0x41,0x53,0x41,	//0x08 - 0x0F
	0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x9C,	//0x10 - 0x17
	0x48,0xB8,0x00,0x00,0x00,0x00,0x00,0x00,	//0x18 - 0x1F	0x1A(pFunction)
	0x00,0x00,0x48,0xB9,0x00,0x00,0x00,0x00,	//0x20 - 0x27	0x24(Argument)
	0x00,0x00,0x00,0x00,0x48,0x83,0xEC,0x28,	//0x28 - 0x2F		
	0xFF,0xD0,0x48,0x83,0xC4,0x28,0x9D,0x41,	//0x30 - 0x37
	0x5F,0x41,0x5E,0x41,0x5D,0x41,0x5C,0x41,	//0x38 - 0x3F
	0x5B,0x41,0x5A,0x41,0x59,0x41,0x58,0x5F,	//0x40 - 0x47
	0x5E,0x5D,0x5B,0x5A,0x59,0x58,0xC3			//0x48 - 0x4E		
};


VOID CodeExecution(PINJECT_DATA pData, PVOID StartRoutine, PVOID Argument)
{
	switch (pData->ExecutionMethod)
	{
	case CreateRemoteThreadMethod:
	{
		HANDLE hThread = CreateRemoteThread(pData->ProcessHandle, NULL, 0, (LPTHREAD_START_ROUTINE)StartRoutine, Argument, 0, NULL);
		if (hThread)
		{
			CloseHandle(hThread);
		}
		return;
	}

	case NtCreateThreadExMethod:
	{
		NtCreateThreadEx_t NtCreateThreadEx = (NtCreateThreadEx_t)GetAddress("ntdll.dll", "NtCreateThreadEx", pData);
		HANDLE hThread = NULL;
		NtCreateThreadEx(&hThread, THREAD_ALL_ACCESS, NULL, pData->ProcessHandle, StartRoutine, Argument, FALSE, NULL, NULL, NULL, NULL);
		if (hThread)
		{
			CloseHandle(hThread);
		}
		return;
	}

	case ThreadHijacking:
	{
		HANDLE hThread = 0;
		CONTEXT Context = { 0, };
		Context.ContextFlags = CONTEXT_ALL;
		GetThread(pData);

		if (SuspendThread(pData->MainThread) != (DWORD)-1)
		{
			if (!GetThreadContext(pData->MainThread, &Context))
			{
				return;
			}
			else
			{
				PVOID ShellExec = VirtualAllocEx(pData->ProcessHandle, 0, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				if (ShellExec)
				{
					memcpy(&bHijacking[0x1A], &StartRoutine, 8);
					memcpy(&bHijacking[0x24], &Argument, 8);
					WriteProcessMemory(pData->ProcessHandle, ShellExec, bHijacking, sizeof(bHijacking), NULL);
					Context.Rip = (DWORD_PTR)ShellExec;
					SetThreadContext(pData->MainThread, &Context);
					ResumeThread(pData->MainThread);
					return;
				}
			}
		}
		return;
	}

	case NtQueueApcThreadMethod:
	{
		GetThread(pData);
		NtQueueApcThread_t NtQueueApcThread = (NtQueueApcThread_t)GetAddress("ntdll.dll", "NtQueueApcThread", pData);
		if (pData->InjectionMethod != ManualMapMethod)
		{
			for (int i = 0; i < pData->ThreadCount; i++)
			{
				HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pData->ThreadId[i]);
				NtQueueApcThread(hThread, (PPS_APC_ROUTINE)StartRoutine, Argument, NULL, NULL);
			}
		}
		else
		{
			MessageBoxA(NULL, "Not support Manual mapping&APC Queue method", "Shh0ya", MB_OK);
			return;
		}

		return;

	}

	}
}