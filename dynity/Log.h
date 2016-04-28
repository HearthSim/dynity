#include <Windows.h>
#include <stdio.h>

void LogInit();
void LogWrite(char *);
void LogGetLastError();

// Emit log messages via OutputDebugString
#define LOG_USE_OUTPUTDEBUGSTRING 0
// Emit log messages to a console window
#define LOG_USE_STDERR 1
// Emit log messages to a log file
#define LOG_USE_FILE 0
#define LOG_FILE_NAME "dynity_log.txt"

#define LOG_USE_ANY (LOG_USE_OUTPUTDEBUGSTRING || LOG_USE_STDERR || LOG_USE_FILE)

#if LOG_USE_ANY
#define LOG__DEBUG_BUF_SIZE 2048

extern char gDebugBuf[LOG__DEBUG_BUF_SIZE];

#define LogPrint(x) LogWrite(x)

#define LogPrintln(x) LogWrite(x "\n")

#define LogPrintf(x, ...)                                                                                              \
	{                                                                                                                  \
		sprintf_s(gDebugBuf, LOG__DEBUG_BUF_SIZE, x, __VA_ARGS__);                                                     \
		LogWrite(gDebugBuf);                                                                                           \
	}
#else
#define LogPrint(x) (void)0
#define LogPrintln(x) (void)0
#define LogPrintf(x, ...) (void)0
#endif