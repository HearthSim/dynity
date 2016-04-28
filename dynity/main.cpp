#include <Windows.h>

#include "Log.h"
#include "Unity.h"

DWORD WINAPI MainThreadProc(LPVOID lpParameter) {
	LogInit();
	UnityInit();
	UnityTest();
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, MainThreadProc, NULL, 0, NULL);
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
