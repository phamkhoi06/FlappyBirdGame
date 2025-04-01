#include <windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);

    LPCWSTR exeName = L"bin\\FlappyBirdGame.exe";
    LPCWSTR workingDir = NULL;

    // Create the process
    if (!CreateProcess(
        exeName,   // Application name
        NULL,      // Command line arguments
        NULL,      // Process handle not inheritable
        NULL,      // Thread handle not inheritable
        FALSE,     // Set handle inheritance to FALSE
        0,         // No creation flags
        NULL,      // Use parent's environment block
        workingDir,// Set working directory
        &si,       // Pointer to STARTUPINFO structure
        &pi))    // Pointer to PROCESS_INFORMATION structure
    {
        MessageBox(NULL, L"Failed to start FlappyBirdGame.exe", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }


    WaitForSingleObject(pi.hProcess, INFINITE);


    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
