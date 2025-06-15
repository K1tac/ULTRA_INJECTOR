// ULTRA_INJECTOR SOURCE CODE 
// FEEL FREE TO EDIT THIS HOWEVER YOU WANT, I LITERALLY DON'T CARE.

// Injector.cpp - Compile this as a DLL, not an EXE.
// This exposes a function you can call from C# or anywhere else to inject a DLL into ULTRAKILL.exe

#include <Windows.h>
#include <TlHelp32.h>
#include <string>

// Hardcoded name of the target process to inject into
constexpr const wchar_t* TARGET_PROCESS = L"ULTRAKILL.exe";

// Scans running processes and returns the PID of ULTRAKILL.exe
DWORD FindProcessId() {
    PROCESSENTRY32W entry = { sizeof(entry) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, TARGET_PROCESS) == 0) {
                DWORD pid = entry.th32ProcessID;
                CloseHandle(snapshot);
                return pid;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return 0;
}

// Core injection logic
// Takes the PID and DLL path, allocates memory inside the target,
// writes the path, and calls LoadLibraryW in a remote thread
bool InjectInternal(DWORD pid, const std::wstring& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) return false;

    // Allocate memory in the target process for the DLL path
    void* remoteStr = VirtualAllocEx(hProcess, NULL, (dllPath.size() + 1) * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);
    if (!remoteStr) {
        CloseHandle(hProcess);
        return false;
    }

    // Write the DLL path into the target process's memory
    if (!WriteProcessMemory(hProcess, remoteStr, dllPath.c_str(), (dllPath.size() + 1) * sizeof(wchar_t), NULL)) {
        VirtualFreeEx(hProcess, remoteStr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Run LoadLibraryW remotely with the DLL path as argument
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)LoadLibraryW, remoteStr, 0, NULL);

    if (!hThread) {
        VirtualFreeEx(hProcess, remoteStr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Wait for the injection thread to finish, clean up
    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(hProcess, remoteStr, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return true;
}

// Exported DLL function
// Call this from C# or another app and pass in the path to your cheat DLL.
// It will inject that DLL into ULTRAKILL.exe (as long as it's running).
extern "C" __declspec(dllexport)
bool __cdecl Inject(const wchar_t* dllPath) {
    DWORD pid = FindProcessId();
    if (!pid) return false; // ULTRAKILL.exe not found

    return InjectInternal(pid, dllPath);
}
