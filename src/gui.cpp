#include "render.hpp"

#include "..\..\dependencies\imgui\imgui.h"

#include <string>

class FileDialog {
public:
	FileDialog() {
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

	~FileDialog() noexcept 
	{ };

	std::string OpenFileDialog() noexcept {
		if (GetOpenFileName(&m_ofn) == TRUE) {
			return std::string(m_ofn.lpstrFile);
		}
		else {
			return std::string();
		}
	}

	FileDialog(const FileDialog& other) = delete;
	FileDialog& operator=(const FileDialog& other) = delete;
private:
	OPENFILENAME m_ofn{ 0 };
	char m_szFile[260] { 0 };
};

void render::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"API Helper",
		&render::isRunning,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse
	);

	static std::string dllPath{""};

	if (ImGui::Button("Select DLL", { 200.f, 25.f })) {
		FileDialog fileDialog;
		dllPath = fileDialog.OpenFileDialog();
	}

	ImGui::SameLine();
	ImGui::Text("%s", dllPath);

	ImGui::End();
}