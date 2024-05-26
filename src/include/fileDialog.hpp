#ifndef FILEDIALOG_HPP
#define FILEDIALOG_HPP

#include <windows.h>
#include <string>

class FileDialog {
public:
	FileDialog();

	~FileDialog() noexcept { };

	std::string OpenFileDialog() noexcept;

	FileDialog(const FileDialog& other) = delete;
	FileDialog& operator=(const FileDialog& other) = delete;
private:
	OPENFILENAME m_ofn{ 0 };
	char m_szFile[260]{ 0 };
};

#endif // FILEDIALOG_HPP