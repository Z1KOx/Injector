#include "process.hpp"

#include <algorithm>
#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <Psapi.h>

Process::Process()
{
	m_systemProcess = windowsSystemProcessNames();

	getAllNoneWindowsProcesses();
}

bool Process::isSystemProcess(const std::string& processName) const
{
	for (const auto& systemProcess : m_systemProcess)
	{
		if (processName == systemProcess)
			return true;
	}
	return false;
}

void Process::getAllNoneWindowsProcesses()
{
	const auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	for (bool proc = Process32First(hSnapshot, &pe); proc; proc = Process32Next(hSnapshot, &pe))
	{
		const auto hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
		if (hProcess)
		{
			TCHAR procName[MAX_PATH] = TEXT("<unknown>");
			if (GetModuleBaseName(hProcess, nullptr, procName, sizeof(procName) / sizeof(TCHAR)))
			{
				std::string sProcessName(procName, procName + _tcslen(procName));
				if (!isSystemProcess(sProcessName))
					m_runningProcesses.push_back(sProcessName);
			}
			CloseHandle(hProcess);
		}
	}

	CloseHandle(hSnapshot);
}

constexpr std::vector<std::string> Process::windowsSystemProcessNames()
{
	return { "SystemProcess", "injector.exe" };
}