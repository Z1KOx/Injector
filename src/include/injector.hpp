#ifndef INJECTOR_HPP
#define INJECTOR_HPP

#include <windows.h>
#include <string>

class Injector {
public:
    explicit Injector(const char* dllPath, const char* procName);
    ~Injector() noexcept;

    Injector(const Injector& other) = delete;
    Injector& operator=(const Injector& other) = delete;

    void injectDll() noexcept;
    
    const char* getOpeningDLLStatus()       const { return m_openingDLL; }
    const char* getPIDStatus()              const { return m_obtainingPID; }
    const char* getOpeningTargetStatus()    const { return m_openingTargetProc; }
    const char* getAllocMemStatus()         const { return m_allocMem; }
    const char* getWriteMemStatus()         const { return m_writeMem; }
    const char* getCreateThreadStatus()     const { return m_createRemoteThread; }
    const char* getThreadFinishStatus()     const { return m_threadFinish; }
    const char* getSuccessfulInjectStatus() const { return m_successfulInject; }

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

    const char* m_openingDLL{ nullptr };
    const char* m_obtainingPID{ nullptr };
    const char* m_openingTargetProc{ nullptr };
    const char* m_allocMem{ nullptr };
    const char* m_writeMem{ nullptr };
    const char* m_createRemoteThread{ nullptr };
    const char* m_threadFinish{ nullptr };
    const char* m_successfulInject{ nullptr };
};

#endif