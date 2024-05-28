#include "injector.hpp"

#include <TlHelp32.h>
#include <sstream>

#define CREATE_THREAD(process, baseAddress) \
    __pragma(warning(push)) \
    __pragma(warning(disable:6387)) \
    CreateRemoteThread((process), nullptr, 0, \
    reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA")), \
    (baseAddress), 0, nullptr) \
    __pragma(warning(pop))



Injector::Injector(const char* dllPath, const char* procName)
    : m_dllPath(dllPath), m_procName(procName) 
{
}

Injector::~Injector() noexcept
{
    if (m_hThread)  CloseHandle(m_hThread);
    if (m_hProcess) CloseHandle(m_hProcess);
}



void Injector::obtainPID() noexcept
{
    m_openingDLL = "Opening the DLL";
    m_obtainingPID = "Obtaining PID";

    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE) {
        showError("Failed to create snapshot of processes", "Snapshot Error");
        return;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    bool found{ false };
    for (bool process = Process32First(hSnapShot, &pe); process; process = Process32Next(hSnapShot, &pe))
    {
        if (_stricmp(pe.szExeFile, m_procName))
            continue;

        m_PID = pe.th32ProcessID;
        found = true;
        break;
    }

    CloseHandle(hSnapShot);

    if (!found) {
        showError("Process not found", "Process Error");
    }
}



bool Injector::openProcess()
{
    m_openingTargetProc = "Opening target process";

    m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, m_PID);
    if (!m_hProcess) {
        showError("OpenProcess failed", "Process Error");
        return false;
    }
    return true;
}

bool Injector::allocateMemory()
{
    m_allocMem = "Allocating memory";

    m_lpBaseAddress = VirtualAllocEx(m_hProcess, nullptr, strlen(m_dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!m_lpBaseAddress) {
        showError("VirtualAllocEx failed", "Memory Error");
        return false;
    }
    return true;
}

bool Injector::writeMemory()
{
    m_writeMem = "Writing to memory";

    if (!WriteProcessMemory(m_hProcess, m_lpBaseAddress, m_dllPath, strlen(m_dllPath) + 1, nullptr)) {
        showError("WriteProcessMemory failed", "Memory Error");
        return false;
    }
    return true;
}

bool Injector::createRemoteThread()
{
    m_createRemoteThread = "Creating remote thread";

    m_hThread = CREATE_THREAD(m_hProcess, m_lpBaseAddress);
    if (!m_hThread) {
        showError("CreateRemoteThread failed", "Thread Error");
        return false;
    }
    return true;
}

void Injector::injectDll() noexcept {
    obtainPID();

    if (!openProcess())        return;
    if (!allocateMemory())     return;
    if (!writeMemory())        return;
    if (!createRemoteThread()) return;

    m_threadFinish = "Waiting for thread to finish";
    if (m_hThread) {
        WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
    }

    m_successfulInject = "Injection successful";
}



[[nodiscard]] std::string Injector::getLastErrorAsString() const noexcept
{
    DWORD errorMsgID = ::GetLastError();
    if (errorMsgID == 0)
        return std::string();

    LPSTR msgBuffer{ nullptr };
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, errorMsgID,
                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msgBuffer, 0, nullptr);

    std::string msg(msgBuffer, size);
    LocalFree(msgBuffer);
    return msg;
}

void Injector::showError(const std::string& msg, const std::string& title) const noexcept
{
    std::ostringstream oss;
    oss << msg << "\nError code: " << GetLastError() << '\n' << getLastErrorAsString();
    MessageBox(nullptr, oss.str().c_str(), title.c_str(), MB_OK | MB_ICONERROR);
    ExitProcess(EXIT_FAILURE);
}