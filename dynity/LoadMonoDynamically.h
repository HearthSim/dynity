#include <Windows.h>

#include "Log.h"

/**
  * The code in LoadMonoDynamically.h, MonoTypes.h, and MonoFunctions.h is derived from code in Unity's fork of the mono
  * project:
  * <https://github.com/Unity-Technologies/mono/tree/unity-staging/unity/smalltestcases/asyncsocketshutdown>
  */

#include "MonoTypes.h"

// define a ProcPtr type for each API
#define DO_API(r, n, p) typedef r(*fp_##n) p;
#include "MonoFunctions.h"

// declare storage for each API's function pointers
#define DO_API(r, n, p) fp_##n n = NULL;
#include "MonoFunctions.h"

HMODULE gMonoModule;

void LoadMono() {
	gMonoModule = LoadLibrary(TEXT(UNITY_DATA_PATH) TEXT("\\Mono\\mono.dll"));
	if (!gMonoModule) {
		LogPrintln("failed to load mono.dll");
		return;
	}

	bool success = true;
#define DO_API(r, n, p)                                                                                                \
	n = (fp_##n)GetProcAddress(gMonoModule, #n);                                                                       \
	if (!n) {                                                                                                          \
		LogPrintf("failed to GetProcAddress for %s\n", #n);                                                            \
		success = false;                                                                                               \
	}
#include "MonoFunctions.h"

	if (success) {
		LogPrintln("mono successfully loaded");
	}
}
