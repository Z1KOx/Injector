#include "include/fileDialog.hpp"

void FileDialog::open() noexcept
{
	ZeroMemory(&m_ofn, sizeof(m_ofn));
	m_ofn.lStructSize = sizeof(m_ofn);
	m_ofn.hwndOwner = NULL;
	m_ofn.lpstrFile = m_szFile;
	m_ofn.lpstrFile[0] = '\0';
	m_ofn.nMaxFile = sizeof(m_szFile);
	m_ofn.lpstrFilter = "DLL Files\0*.dll\0All Files\0*.*\0";
	m_ofn.nFilterIndex = 1;
	m_ofn.lpstrFileTitle = NULL;
	m_ofn.nMaxFileTitle = 0;
	m_ofn.lpstrInitialDir = NULL;
	m_ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

const char* FileDialog::getPath() noexcept {
	if (GetOpenFileName(&m_ofn) == TRUE) {
		return m_ofn.lpstrFile;
	}
	else {
		return "Failed to get dll path";
	}
}