#include "process.hpp"

#include <TlHelp32.h>

[[nodiscard]] std::set<Process::info> Process::processInformation() const
{
	std::set<info> processes;

	const auto hSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

	std::set<const char*> sysProc{ "injector.exe", "[System Process]" };

	if ( hSnapshot != INVALID_HANDLE_VALUE )
	{
		info procInfo;

		PROCESSENTRY32 lppe;
		lppe.dwSize = sizeof( lppe );

		for ( bool proc = Process32First(hSnapshot, &lppe); proc; proc = Process32Next(hSnapshot, &lppe) )
		{
			if ( std::ranges::find_if(sysProc, [&](const char* procName) -> bool {
				return strcmp(procName, lppe.szExeFile) == 0;
				}) != sysProc.end() ) {
				continue;
			}

			BOOL isWow64{  };
			auto hProcess = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, lppe.th32ProcessID );
			if ( hProcess != nullptr )
			{
				IsWow64Process( hProcess, &isWow64 );
				CloseHandle( hProcess );
			}

			procInfo.pid = lppe.th32ProcessID;
			strcpy_s( procInfo.name, lppe.szExeFile );
			procInfo.is64Bit = !isWow64;

			processes.insert( procInfo );
		}

		CloseHandle( hSnapshot );
	}


	return processes;
}
