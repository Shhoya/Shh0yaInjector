#pragma once
#include "DllMain.h"
#include <winternl.h>
#include <TlHelp32.h>

#pragma comment(lib,"ntdll.lib")

#pragma warning(disable:4996)

/*
* Base structure & enumeration
*/
enum EXECUTION_METHOD{
	CreateRemoteThreadMethod,
	NtCreateThreadExMethod,
	ThreadHijacking,
	NtQueueApcThreadMethod,
	SetWindowsHookExMethod
};

enum INJECTION_METHOD {
	LoadLibraryMethod,
	LdrLoadDllMethod,
	ManualMapMethod
};

typedef struct _INJECT_DATA {
	HANDLE				ProcessHandle;
	int					ProcessId;
	HANDLE				MainThread;
	int					ThreadId[256];
	int					ThreadCount;
	char*				DllPath;
	wchar_t*			DllPathW;
	INJECTION_METHOD	InjectionMethod;
	EXECUTION_METHOD	ExecutionMethod;
	DWORD				ErrorType;
}INJECT_DATA, * PINJECT_DATA;

/*
* Nt Undocument Structure & Enumeration
*/
typedef struct _PS_ATTRIBUTE
{
	ULONG_PTR Attribute;
	SIZE_T Size;
	union
	{
		ULONG_PTR Value;
		PVOID ValuePtr;
	};
	PSIZE_T ReturnLength;
} PS_ATTRIBUTE, *PPS_ATTRIBUTE;

typedef struct _PS_ATTRIBUTE_LIST
{
	SIZE_T			TotalLength;
	PS_ATTRIBUTE	Attributes[1];
} PS_ATTRIBUTE_LIST, *PPS_ATTRIBUTE_LIST;

typedef VOID (*PPS_APC_ROUTINE)(
	PVOID SystemArgument1,
	PVOID SystemArgument2,
	PVOID SystemArgument3,
	PCONTEXT ContextRecord
	);

/*
* Nt Undocument API
*/

typedef NTSTATUS(*NtCreateThreadEx_t)(
	_Out_ PHANDLE					ThreadHandle,
	_In_ ACCESS_MASK				DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES		ObjectAttributes,
	_In_ HANDLE						ProcessHandle,
	_In_ PVOID						StartRoutine,
	_In_opt_ PVOID					Argument,
	_In_ ULONG						CreateFlags,
	_In_ SIZE_T						ZeroBits,
	_In_ SIZE_T						StackSize,
	_In_ SIZE_T						MaximumStackSize,
	_In_opt_ PPS_ATTRIBUTE_LIST		AttributeList
	);

typedef NTSTATUS(*LdrLoadDll_t)(
	_In_opt_	PWCHAR				PathToFile,
	_In_opt_	ULONG				Flags,
	_In_		PUNICODE_STRING		ModuleFileName,
	_Out_		PHANDLE				ModuleHandle
	);

typedef NTSTATUS(*NtQueueApcThread_t)(
	IN HANDLE ThreadHandle,
	IN PPS_APC_ROUTINE ApcRoutine,
	IN PVOID SystemArgument1 OPTIONAL,
	IN PVOID SystemArgument2 OPTIONAL,
	IN PVOID SystemArgument3 OPTIONAL
);

/*
* LdrLoadDll Data
*/

typedef struct _LDR_DLL_DATA {
	LdrLoadDll_t	LdrLoadDll;
	PWCHAR			PathToFile;
	PULONG			Flags;
	UNICODE_STRING ModuleFileName;
	HANDLE			ModuleHandle;

}LDR_DLL_DATA,*PLDR_DLL_DATA;

/*
* Manual Mapper Data
*/

typedef HMODULE(*LoadLibraryA_t)(LPCSTR lpLibFileName);
typedef HMODULE(*LoadLibraryW_t)(LPCWSTR lpLibFileName);
typedef FARPROC(*GetProcAddress_t)(HINSTANCE hModule, LPCSTR lpProcName);
typedef BOOL(*DLL_ENTRY_POINT_t)(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved);

typedef struct _FILE_INFO {
	HANDLE FileHandle;
	DWORD FileSize;
	PVOID ReadBuffer;
}FILE_INFO,*PFILE_INFO;

typedef int(*MESSAGEBOXT)(
	HWND hWnd,
	LPCSTR Text,
	LPCSTR Title,
	UINT Flag
	);

typedef struct _MANUAL_MAP_DATA {
	PVOID ImageBase;
	LoadLibraryA_t fnLoadLibraryA;
	LoadLibraryW_t fnLoadLibraryW;
	GetProcAddress_t fnGetProcAddress;
	DLL_ENTRY_POINT_t fnDllMain;
	MESSAGEBOXT	MessageBoxT;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeaders;
	PIMAGE_SECTION_HEADER pSectionHeader;
	PIMAGE_BASE_RELOCATION pBaseReloc;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	PIMAGE_TLS_DIRECTORY pTlsDir;
}MANUAL_MAP_DATA,*PMANUAL_MAP_DATA;

typedef struct _PE_DATA {
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeaders;
	PIMAGE_SECTION_HEADER pSectionHeader;
}PE_DATA,*PPE_DATA;


extern "C" __declspec(dllexport) VOID InjectMain(int pid, char* filepath, EXECUTION_METHOD execType, INJECTION_METHOD injectType);
extern "C" __declspec(dllexport) VOID EjectMain(int pid, char* filepath);

PVOID GetAddress(LPCSTR ModuleName, LPCSTR ProcName, PINJECT_DATA pData OPTIONAL);
VOID GetThread(PINJECT_DATA pData);
BOOL GetFileInformation(PINJECT_DATA pData, PFILE_INFO pFile);
BOOL ValidCheckDLL(PINJECT_DATA pData, PFILE_INFO pFile, PPE_DATA pMapData);

VOID InjLoadLibrary(PINJECT_DATA pData);
VOID InjLdrLoadDll(PINJECT_DATA pData);
VOID ManualMapper(PINJECT_DATA pData);
VOID CodeExecution(PINJECT_DATA pData, PVOID StartRoutine, PVOID Argument);