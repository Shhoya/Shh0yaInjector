#include <stdio.h>
#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxA(NULL, "Inject", "Shh0ya", MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}