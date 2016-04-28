#define _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>

#define DLL_NAME "dynity.dll"
#define PROC_NAME "Hearthstone.exe"

static HANDLE GetProcessByName(char *name) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 entry = {sizeof(PROCESSENTRY32)};
	size_t len = strlen(name);
	Process32First(snapshot, &entry);
	do {
		if (_strnicmp(name, entry.szExeFile, len) == 0) {
			return OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID);
		}
	} while (Process32Next(snapshot, &entry));
	return 0;
}

int main() {
	HANDLE proc = 0;
	HANDLE procThread = 0;
	bool notified = false;
	while (proc == 0) {
		proc = GetProcessByName(PROC_NAME);
		if (proc == 0) {
			if (!notified) {
				printf("proc doesn't exist, waiting... gle=%x\n", GetLastError());
				notified = true;
			}
			Sleep(1);
		}
	}

	void *paramAddr = VirtualAllocEx(proc, 0, 0x1000, MEM_COMMIT, PAGE_READWRITE);
	SIZE_T bytesWritten = 0;
	char dllPath[MAX_PATH];
	int len = GetCurrentDirectory(MAX_PATH, dllPath);
	strcpy(dllPath + len, "\\" DLL_NAME);

	WriteProcessMemory(proc, paramAddr, dllPath, 0x100, &bytesWritten);
	HANDLE thread = CreateRemoteThread(proc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, (LPVOID)paramAddr, 0, 0);
	if (procThread) {
		ResumeThread(procThread);
	}
}
