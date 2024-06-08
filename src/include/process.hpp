#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <set>
#include <string>
#include <windows.h>
#include <algorithm>

class Process
{
public:
	Process() = default;
	~Process() noexcept { }

	Process(const Process& other) = delete;
	Process(Process&& other) = delete;
	Process& operator=(const Process& other) = delete;
	Process& operator=(Process&& other) = delete;

public:
	struct info
	{
		CHAR name[MAX_PATH];
		DWORD pid{ 0 };
		BOOL is64Bit{ false };

		bool operator<(const info& other) const {
			std::string thisName(name);
			std::string otherName(other.name);
			std::ranges::transform(thisName.begin(), thisName.end(), thisName.begin(), ::tolower);
			std::ranges::transform(otherName.begin(), otherName.end(), otherName.begin(), ::tolower);
			return thisName < otherName;
		}
	};

	[[nodiscard]] std::set<info> processInformation() const;
};

#endif // PROCESS_HPP
