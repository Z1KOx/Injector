#include "include/fileDialog.hpp"

void FileDialog::open() noexcept
{
	ZeroMemory(&m_ofn, sizeof(m_ofn));
	m_ofn.lStructSize = sizeof(m_ofn);
	m_ofn.hwndOwner = nullptr;
	m_ofn.lpstrFile = m_szFile;
	m_ofn.lpstrFile[0] = '\0';
	m_ofn.nMaxFile = sizeof(m_szFile);
	m_ofn.lpstrFilter = "DLL Files\0"
					    "*.dll\0"
	                    "All Files\0"
	                    "*.*\0";
	m_ofn.nFilterIndex = 1;
	m_ofn.lpstrFileTitle = nullptr;
	m_ofn.nMaxFileTitle = 0;
	m_ofn.lpstrInitialDir = nullptr;
	m_ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

const char* FileDialog::getPath() noexcept
{
	return (GetOpenFileName(&m_ofn) == TRUE) ? m_ofn.lpstrFile
						                     : "Failed to get dll path";
}