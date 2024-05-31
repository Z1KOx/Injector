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



Injector::Injector(const char* dllPath, const char* procName) noexcept
	: m_dllPath(dllPath), m_procName(procName)
{
}

Injector::~Injector() noexcept
{
	if (m_hThread)  CloseHandle(m_hThread);
}



void Injector::injectDll()
{
	m_progress.emplace_back("Opening the DLL");

	// Injection process
	try
	{
		obtainPID();          // Obtain the Process ID of the target application
		openProcess();        // Open a handle to the target process
		allocateMemory();     // Allocate memory within the target process
		writeMemory();        // Write the DLL path to the allocated memory
		createRemoteThread(); // Create a remote thread in the target process to load the DLL
	}
	catch (const std::exception& e) {
		showError(e.what(), "Injection Error");
	}

	// Thread process
	if (m_hThread)
	{
		m_progress.emplace_back("Waiting for thread to finish");
		
		// Wait for the remote thread to complete its execution
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}

	m_progress.emplace_back("Injection successful");
}

void Injector::obtainPID()
{
	m_progress.emplace_back("Obtaining PID");

	// Create a snapshot of currently running processes
	const auto hSnapShot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
	if (hSnapShot == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Failed to create snapshot of processes");

	// Initialize pe struct
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	auto found{ false };
	// Loop through all the processes in the snapshot
	for (bool process = Process32First(hSnapShot, &pe); process; process = Process32Next(hSnapShot, &pe))
	{
		// If the process is not found
		if (_stricmp(pe.szExeFile, m_procName) != 0)
			continue;

		// Assign m_PID to the found PID
		m_PID = pe.th32ProcessID;
		found = true;
		break;
	}

	CloseHandle(hSnapShot);

	if (!found)
		throw std::runtime_error("Process not found");
}



void Injector::openProcess()
{
	m_progress.emplace_back("Opening target process");

	// Open a handle to the target process with all access rights
	m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, m_PID);
	if (!m_hProcess)
		throw std::runtime_error("OpenProcess failed");
}

void Injector::allocateMemory()
{
	m_progress.emplace_back("Allocating memory");

	// Allocate memory in the target process for our DLL
	m_lpBaseAddress = VirtualAllocEx(m_hProcess, nullptr, strlen(m_dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (!m_lpBaseAddress)
		throw std::runtime_error("VirtualAllocEx failed");
}

void Injector::writeMemory()
{
	m_progress.emplace_back("Writing to memory");

	// Write our DLL memory into the target process
	if (!WriteProcessMemory(m_hProcess, m_lpBaseAddress, m_dllPath, strlen(m_dllPath) + 1, nullptr))
		throw std::runtime_error("WriteProcessMemory failed");
}

void Injector::createRemoteThread()
{
	m_progress.emplace_back("Creating remote thread");

	// Create a thread inside the target process so our DLL can run there
	m_hThread = CREATE_THREAD(m_hProcess, m_lpBaseAddress);
	if (!m_hThread)
		throw std::runtime_error("CreateRemoteThread failed");
}



[[nodiscard]] std::string Injector::getLastErrorAsString() const noexcept
{
	// Retrieve the error message ID
	const auto errorMsgID = ::GetLastError();
	if (errorMsgID == 0)
		return "errorMsgID = 0";

	// Retrieve the error message string
	LPSTR msgBuffer{ nullptr };
	const size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, errorMsgID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&msgBuffer), 0, nullptr);

	// Convert the message to a std::string
	std::string msg(msgBuffer, size);

	// Free the buffer allocated by FormatMessage
	LocalFree(msgBuffer);

	return msg;
}

void Injector::showError(const std::string& msg, const std::string& title) const noexcept
{
	// Construct an error message with additional information
	std::ostringstream oss;
	oss << msg << "\nError code: " << GetLastError() << '\n' << getLastErrorAsString();

	// Display the error message in a message box
	MessageBox(nullptr, oss.str().c_str(), title.c_str(), MB_OK | MB_ICONERROR);

	// Terminate the process with an error code
	ExitProcess(EXIT_FAILURE);
}