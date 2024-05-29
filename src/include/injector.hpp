#ifndef INJECTOR_HPP
#define INJECTOR_HPP

#include <windows.h>
#include <string>
#include <vector>

class Injector {
public:
    explicit Injector(const char* dllPath, const char* procName) noexcept;
    ~Injector() noexcept;

    // Deleted copy constructor and copy assignment operator to prevent copying
    Injector(const Injector& other) = delete;
    Injector& operator=(const Injector& other) = delete;

    // Executes the DLL injection process
    void injectDll();
    
    // Retrieves the progress of the injection process
    [[nodiscard]] const std::vector<std::string>& getProgress() const noexcept { return m_progress; }

private:
    // Obtains the process ID (PID) of the target process
    void obtainPID();

    // Injection process functions
    void openProcess();
    void allocateMemory();
    void writeMemory();
    void createRemoteThread();

    // Utility functions for handling errors
    [[nodiscard]] std::string getLastErrorAsString() const noexcept;
    void showError(const std::string& msg, const std::string& title) const noexcept;

private:
    const char* m_dllPath{ nullptr };
    const char* m_procName{ nullptr };

    DWORD m_PID{ 0 };
    HANDLE m_hProcess{ nullptr };

    HANDLE m_hThread{ nullptr };
    void* m_lpBaseAddress{ nullptr };

    std::vector<std::string> m_progress;
};

#endif