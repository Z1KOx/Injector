#ifndef INJECTOR_HPP
#define INJECTOR_HPP

#include <windows.h>

class Injector {
public:
    explicit Injector(const char* dllPath, const char* procName);
    ~Injector() noexcept;

    Injector(const Injector& other) = delete;
    Injector& operator=(const Injector& other) = delete;

    enum class ErrorCode {
        Success,
        OpenProcessFailed,
        VirtualAllocExFailed,
        WriteProcessMemoryFailed,
        GetModuleHandleFailed,
        CreateRemoteThreadFailed,
        WaitForSingleObjectFailed,

        MaxItems
    };

    [[nodiscard]] ErrorCode getLastErrorCode() const noexcept { return m_lastErrorCode; }
    void injectDll() noexcept;
    void getPID() noexcept;

private:
    ErrorCode m_lastErrorCode{ ErrorCode::Success };
    const char* m_dllPath{ nullptr };
    const char* m_procName{ nullptr };
    DWORD m_PID{ 0 };
    HANDLE m_hProcess{ nullptr };
    HANDLE m_hThread{ nullptr };
    void* m_lpBaseAddress{ nullptr };
    const HMODULE M_KERNEL32_BASE = GetModuleHandle("kernel32.dll");
};

#endif