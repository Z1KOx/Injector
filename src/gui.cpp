#include "render.hpp"
#include "fileDialog.hpp"
#include "injector.hpp"

#include "..\..\dependencies\imgui\imgui.h"



void render::Render() noexcept
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"Injector",
		&render::isRunning,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar
	);

	const auto drawList = ImGui::GetWindowDrawList();
	drawList->AddLine({ 0.f, 20.f }, {400.f, 20.f }, IM_COL32(62, 62, 71, 255) ,1.f);

	static std::vector<std::string> progress;

	ImGui::SetCursorPos({ 5.f, 260.f });
	if (ImGui::Button("Inject a dll", { 390.f, 40.f }))
	{
		FileDialog dialog;

		dialog.open();

		const auto dllPath = dialog.getPath();
		const auto procName = "ac_client.exe";

		Injector injector(dllPath, procName);
		injector.injectDll();

		progress = injector.getProgress();
	}

	ImGui::SetCursorPos({ 5.f, 305 });
	if(ImGui::Button("Exit", {390.f, 40.f}))
		PostQuitMessage(0);

	ImGui::SetCursorPos({ 405.f, 25.f });
	ImGui::Text("Console");

	ImGui::SetNextWindowPos({ 400.f, 20.f });
	ImGui::BeginChild("ConsoleBackground", { 295.f, 325.f }, ImGuiChildFlags_Border);
	{
		ImGui::SetCursorPos({ 5.f, 25 });
		if (!progress.empty()) {
			for (const auto& status : progress) {
				ImGui::SetCursorPosX(5.f);
				ImGui::Text("%s", status.c_str());
			}
		}

		ImGui::EndChild();
	}

	ImGui::End();
}