#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <set>
#include <string>
#include <windows.h>

class Process
{
public:
    Process() = default;

    Process(const Process& other) = delete;
    Process(Process&& other) = delete;
    Process& operator=(const Process& other) = delete;
    Process& operator=(Process&& other) = delete;

public:
    struct ProcessInfo
	{
        CHAR name[MAX_PATH];
        DWORD pid;

        bool operator<(const ProcessInfo& other) const {
            return std::string(name) < std::string(other.name);
        }
    };

    [[nodiscard]] std::set<ProcessInfo> processInformation() const;
};

#endif // PROCESS_HPP
