
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char *argv[])
{
    // Child process
    if (argc == 3 && strcmp(argv[1], "--child") == 0)
    {
        printf("Child process1 id: %lu\n",
               (unsigned long)GetCurrentProcessId());

        printf("Parent process1 id: %s\n", argv[2]);

        printf("\nFiles and directories:\n\n");
        system("dir");

        return 0;
    }

    // Parent process
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    char commandLine[1024];
    char exePath[MAX_PATH];

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    DWORD parent = GetCurrentProcessId();

    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    sprintf(commandLine, "\"%s\" --child %lu",
            exePath, (unsigned long)parent);

    if (!CreateProcessA(
            NULL,
            commandLine,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
    {
        printf("Process creation failed: %lu\n",
               (unsigned long)GetLastError());
        return 1;
    }

    printf("Parent id: %lu\n",
           (unsigned long)GetCurrentProcessId());

    printf("Parent will be waiting for child to complete\n");

    // Wait for child process to finish
    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
