#include "render.hpp"
#include "fileDialog.hpp"
#include "injector.hpp"
#include "process.hpp"

#include "..\..\dependencies\imgui\imgui.h"

#include <iostream>

void drawMenuBar()
{
	ImGui::SetCursorPos({ 5.f, 2.f });
	ImGui::Text("Injector");

	const auto drawList = ImGui::GetWindowDrawList();
	drawList->AddLine({ 0.f, 20.f }, { 700.f, 20.f }, IM_COL32(255, 0, 0, 255), 1.f);

}

void drawInjectButton(std::vector<std::string>& progress, const std::string& procName)
{
	ImGui::SetCursorPos({ 5.f, 260.f });
	if (ImGui::Button("Inject a dll", { 390.f, 40.f }))
	{
		FileDialog dialog;

		dialog.open();

		const auto dllPath = dialog.getPath();

		Injector injector(dllPath, procName.c_str());
		injector.injectDll();

		progress = injector.getProgress();
	}
}

void drawExitButton()
{
	ImGui::SetCursorPos({ 5.f, 305 });
	if (ImGui::Button("Exit", { 390.f, 40.f }))
		render::isRunning = false;
}

void drawConsole(const std::vector<std::string>& progress, const std::string& selectedProc)
{
	ImGui::SetCursorPos({ 410.f, 30.f });
	ImGui::Text("Console");

	ImGui::SetNextWindowPos({ 400.f, 45.f });
	ImGui::BeginChild("ConsoleBackground", { 295.f, 300.f }, ImGuiChildFlags_Border);
	{
		ImGui::SetCursorPos({ 5.f, 5.f });
		if (!selectedProc.empty())
			ImGui::Text("Target: %s", selectedProc.c_str());

		if (!progress.empty()) {
			for (const auto& status : progress) {
				ImGui::SetCursorPosX(5.f);
				ImGui::Text("%s", status.c_str());
			}
		}

		ImGui::EndChild();
	}
}

[[nodiscard]] std::string drawProcessList(const std::vector<std::string>& runningProcesses)
{
	ImGui::SetCursorPos({ 10.f, 30.f });
	ImGui::Text("Process list");

	static std::string selected;

	ImGui::SetCursorPos({ 5.f, 45.f });
	ImGui::BeginChild("ProcessBackground", { 390.f, 210.f }, ImGuiChildFlags_Border);
	{
		ImGui::SetCursorPosY(5.f);

		for (const auto& process : runningProcesses)
		{
			ImGui::SetCursorPosX(5.f);
			if (ImGui::Selectable(process.c_str()))
				selected = process;
		}

		ImGui::EndChild();
	}

	return selected;
}

void render::Render() noexcept
{
#define CREATE_CONSOLE 0
#if CREATE_CONSOLE
	AllocConsole();
	FILE* f;
	(void)freopen_s(&f, "CONOUT$", "w", stdout);
#endif

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

	drawMenuBar();

	const Process proc;
	const std::vector<std::string>& runningProcesses = proc.getCurrent();
	const std::string selectedProc = drawProcessList(runningProcesses);
	static std::vector<std::string> progress{};

	drawConsole(progress, selectedProc);
	drawInjectButton(progress, selectedProc);
	drawExitButton();

	ImGui::End();
}