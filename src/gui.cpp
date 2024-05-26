#include "render.hpp"
#include "fileDialog.hpp"

#include "..\..\dependencies\imgui\imgui.h"

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

	static std::string dllPath{ "" };

	if (ImGui::Button("Select DLL", { 200.f, 25.f })) {
		FileDialog fileDialog;
		dllPath = fileDialog.OpenFileDialog();
	}

	ImGui::SameLine();
	ImGui::Text("%s", dllPath);

	ImGui::End();
}