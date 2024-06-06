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

void drawInjectButton(std::vector<std::string>& progress)
{
	ImGui::SetCursorPos({ 5.f, 260.f });
	if (ImGui::Button("Inject", { 390.f, 40.f }))
	{
		FileDialog dialog;

		dialog.open();

		const auto dllPath = dialog.getPath();

		Injector injector(dllPath, "ac_client.exe");
		injector.injectDll();

		progress = injector.getProgress();
	}
}

void drawExitButton()
{
	ImGui::SetCursorPos({ 5.f, 305 });
	if (ImGui::Button("Exit", { 395.f, 40.f }))
		render::isRunning = false;
}

void drawConsole(const std::vector<std::string>& progress, const std::string& selected)
{
	ImGui::SetCursorPos({ 410.f, 30.f });
	ImGui::Text("Console");

	ImGui::SetNextWindowPos({ 400.f, 45.f });
	ImGui::BeginChild("ConsoleBackground", { 295.f, 300.f }, ImGuiChildFlags_Border);
	{
		ImGui::SetCursorPos({ 5.f, 5.f });
		if (!selected.empty()) {
			ImGui::Text("Target: %s", selected.c_str());
		}

		if (!progress.empty()) {
			for (const auto& status : progress) 
			{
				ImGui::SetCursorPosX(5.f);
				ImGui::Text("%s", status.c_str());
			}
		}

		ImGui::EndChild();
	}
}

[[nodiscard]] std::string drawProcessList(const std::set<Process::ProcessInfo>& processInfos)
{
	ImGui::SetCursorPos({ 10.f, 30.f });
	ImGui::Text("Process List");

	static std::string selected;

	ImGui::SetCursorPos({ 5.f, 45.f });
	ImGui::BeginChild("ProcessBg", { 390.f, 210.f }, ImGuiChildFlags_Border);
	{
		ImGui::SetCursorPosY(5.f);

		for (const auto& proc : processInfos)
		{
			ImGui::SetCursorPosX(5.f);
			if (ImGui::Selectable(proc.name))
				selected = proc.name;
		}

		ImGui::EndChild();
	}

	return selected;
}

void render::Render() noexcept
{
#define CREATE_CONSOLE 1
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

	static std::vector<std::string> progress{};

	constexpr Process proc;
	const auto processInfos = proc.processInformation();

	auto selected = drawProcessList(processInfos);

	drawConsole(progress, selected);
	drawInjectButton(progress);
	drawExitButton();

	ImGui::End();
}