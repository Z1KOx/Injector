#ifndef INJECTOR_HPP
#define INJECTOR_HPP

#include <windows.h>
#include <string>
#include <vector>

class Injector {
public:
    explicit Injector(const char* dllPath, const char* procName);
    ~Injector() noexcept;

    Injector(const Injector& other) = delete;
    Injector& operator=(const Injector& other) = delete;

    void injectDll() noexcept;
    
    [[nodiscard]] std::vector<const char*> getProgress() const { return m_progress; }

private:
    void obtainPID() noexcept;

    bool openProcess();
    bool allocateMemory();
    bool writeMemory();
    bool createRemoteThread();

    [[nodiscard]] std::string getLastErrorAsString() const noexcept;
    void showError(const std::string& msg, const std::string& title) const noexcept;

private:
    const char* m_dllPath{ nullptr };
    const char* m_procName{ nullptr };

    DWORD m_PID{ 0 };
    HANDLE m_hProcess{ nullptr };

    HANDLE m_hThread{ nullptr };
    void* m_lpBaseAddress{ nullptr };

    std::vector<const char*> m_progress;
};

#endif