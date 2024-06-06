#include "render.hpp"
#include "fileDialog.hpp"
#include "injector.hpp"
#include "process.hpp"

#include "..\..\dependencies\imgui\imgui.h"

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
	ImGui::SetCursorPos({ 5.f, 305.f });
	if (ImGui::Button("Exit", { 190.f, 40.f })) {
		render::isRunning = false;
	}
}

void drawConsole(const std::vector<std::string>& progress, const std::string& selectedProc)
{
	ImGui::SetCursorPos({ 400.f, 28.f });
	ImGui::Text("Console");

	ImGui::SetNextWindowPos({ 400.f, 45.f });
	ImGui::BeginChild("ConsoleBackground", { 295.f, 300.f }, ImGuiChildFlags_Border);
	{
		ImGui::SetCursorPos({ 5.f, 5.f });
		if (!selectedProc.empty()) {
			ImGui::Text("Target: %s", selectedProc.c_str());
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

[[nodiscard]] std::string drawProcessList(const std::set<Process::ProcessInfo>& processInfo)
{
	static std::string selectedProc;

	ImGui::SetCursorPos({ 5.f, 28.f });
	ImGui::Text("Process List");

	ImGui::SetCursorPos({ 5.f, 45.f });
	ImGui::BeginChild("ProcessBg", { 390.f, 210.f }, ImGuiChildFlags_Border);
	{
		ImGui::SetCursorPos({ 5.f, 7.f });
		ImGui::Text("Name");

		ImGui::SetCursorPos({ 200.f, 7.f });
		ImGui::Text("PID");

		const auto drawList = ImGui::GetWindowDrawList();
		drawList->AddLine({5.f, 68.f }, { 400.f, 68.f }, IM_COL32(62, 62, 71, 255), 1.f);

		ImGui::SetCursorPos({ 0.f, 25.f });
		ImGui::BeginChild("runningProcBg", { 400.f, 180.f });
		{
			ImGui::SetCursorPosY(3.f);

			for (const auto& proc : processInfo)
			{
				ImGui::SetCursorPosX(5.f);
				if (ImGui::Selectable(proc.name)) {
					selectedProc = proc.name;
				}
				ImGui::SameLine(200);
				ImGui::Text("%d", proc.pid);
			}

			ImGui::EndChild();
		}

		ImGui::EndChild();
	}

	return selectedProc;
}

void drawRefreshButton(std::set<Process::ProcessInfo>& processInfo)
{
	constexpr Process proc;
	static auto refresh{ false };

	ImGui::SetCursorPos({ 200.f, 305.f });
	if (ImGui::Button("Refresh", {195.f, 40.f})) {
		refresh = true;
	}

	if (refresh) {
		processInfo = proc.processInformation();
		refresh = false;
	}
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

	constexpr Process proc;
	static std::set<Process::ProcessInfo> processInfo = proc.processInformation();

	drawRefreshButton(processInfo);
	const auto selectedProc = drawProcessList(processInfo);

	static std::vector<std::string> progress{};
	drawMenuBar();
	drawConsole(progress, selectedProc);
	drawInjectButton(progress);
	drawExitButton();

	ImGui::End();
}