#include "injector.hpp"

#include <TlHelp32.h>
#include <cassert>
#include <sstream>

Injector::Injector(const char* dllPath, const char* procName)
    : m_dllPath(dllPath), m_procName(procName)
{
    if (dllPath == nullptr || procName == nullptr) {
        m_lastErrorCode = ErrorCode::OpenProcessFailed;
        return;
    }
}

Injector::~Injector() noexcept
{
    MessageBeep(MB_OK);

    if (m_hThread != nullptr) {
        CloseHandle(m_hThread);
    }
    if (m_hProcess != nullptr) {
        CloseHandle(m_hProcess);
    }
}

void Injector::injectDll() noexcept
{
    getPID();

    m_lpBaseAddress = VirtualAllocEx(
        m_hProcess,
        nullptr,
        strlen(m_dllPath) + 1,
        MEM_COMMIT,
        PAGE_READWRITE);
    if (m_lpBaseAddress == nullptr) {
        m_lastErrorCode = ErrorCode::VirtualAllocExFailed;
        return;
    }

    if (!WriteProcessMemory(
        m_hProcess,
        m_lpBaseAddress,
        m_dllPath,
        strlen(m_dllPath) + 1,
        nullptr)) {
        m_lastErrorCode = ErrorCode::WriteProcessMemoryFailed;
        return;
    }

    m_hThread = CreateRemoteThread(
        m_hProcess,
        nullptr,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(M_KERNEL32_BASE, "LoadLibraryA")),
        m_lpBaseAddress,
        0,
        nullptr);
    if (m_hThread == nullptr) {
        m_lastErrorCode = ErrorCode::CreateRemoteThreadFailed;
        return;
    }

    WaitForSingleObject(m_hThread, INFINITE);
    CloseHandle(m_hThread);
}

void Injector::getPID() noexcept
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    assert(hSnapshot != INVALID_HANDLE_VALUE && "Failed to create snapshot of processes");

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);

    if (!Process32First(hSnapshot, &entry)) {
        assert(false && "Failed to retrieve the first process entry");
        CloseHandle(hSnapshot);
        return;
    }

    do {
        if (_stricmp(entry.szExeFile, m_procName) == 0) {
            m_PID = entry.th32ProcessID;
            CloseHandle(hSnapshot);
            
            m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_PID);
            if (m_hProcess == nullptr) {
                m_lastErrorCode = ErrorCode::OpenProcessFailed;
            }
            return;
        }
    } while (Process32Next(hSnapshot, &entry));

    [[maybe_unused]] std::stringstream oss;
    oss << "Process " << m_procName << " not found.";
    MessageBox(nullptr, oss.str().c_str(), "PROCESS NOT FOUND", MB_OK | MB_ICONERROR);

    CloseHandle(hSnapshot);
}
