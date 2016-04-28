#define _CRT_SECURE_NO_WARNINGS 1

#include "Log.h"

#if LOG_USE_ANY
CRITICAL_SECTION logSect;
HANDLE logFile;
char gDebugBuf[2048];
#endif

void LogInit() {
#if LOG_USE_ANY
	InitializeCriticalSection(&logSect);
#if LOG_USE_FILE
	logFile = CreateFile(TEXT(LOG_FILE_NAME), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
	                     FILE_ATTRIBUTE_NORMAL, NULL);
#endif

#if LOG_USE_STDERR
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif
#endif
}

void LogWrite(char *buf) {
#if LOG_USE_ANY
	EnterCriticalSection(&logSect);
	DWORD written = 0;
	int len = strlen(buf);
#if LOG_USE_FILE
	WriteFile(logFile, buf, len, &written, NULL);
#endif
#if LOG_USE_OUTPUTDEBUGSTRING
	OutputDebugStringA(buf);
#endif
#if LOG_USE_STDERR
	fwrite(buf, 1, len, stderr);
#endif
	LeaveCriticalSection(&logSect);
#endif
}

void LogGetLastError() {
#if LOG_USE_ANY
	char *buf;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
	               GetLastError(), 0, (LPSTR)&buf, 0, NULL);
	LogWrite(buf);
	LocalFree(buf);
#endif
}
