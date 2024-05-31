#ifndef FILEDIALOG_HPP
#define FILEDIALOG_HPP

#include <windows.h>

class FileDialog
{
public:
	void open() noexcept;
	[[nodiscard]] const char* getPath() noexcept;

private:
	OPENFILENAME m_ofn{ 0 };
	char m_szFile[260]{ 0 };
};

#endif // FILEDIALOG_HPP