#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <vector>
#include <string>

class Process
{
public:
    Process();

    Process(const Process& other) = delete;
    Process(Process&& other) = delete;
    Process& operator=(const Process& other) = delete;
    Process& operator=(Process&& other) = delete;

    [[nodiscard]] std::vector<std::string> getCurrent() const { return m_runningProcesses; }

private:
    bool isSystemProcess(const std::string& processName) const;
    void getAllNoneWindowsProcesses();
    static constexpr std::vector<std::string> windowsSystemProcessNames();

private:
    std::vector<std::string> m_runningProcesses;
    std::vector<std::string> m_systemProcess;
    std::string m_processName;
};

#endif // PROCESS_HPP