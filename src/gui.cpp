#include "render.hpp"
#include "fileDialog.hpp"
#include "injector.hpp"
#include "process.hpp"

#include "..\..\dependencies\imgui\imgui.h"

namespace draw
{
	void MenuBar()
{
	ImGui::SetCursorPos({ 5.f, 2.f });
	ImGui::Text("Injector");

	const auto drawList = ImGui::GetWindowDrawList();
	drawList->AddLine({ 0.f, 20.f }, { 700.f, 20.f }, IM_COL32(22, 22, 26, 255), 1.f);
}

void InjectButton(std::vector<std::string>& progress)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.f);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(6, 89, 107, 255)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(34, 122, 136, 255)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(68, 155, 165, 255)));

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

	ImGui::PopStyleColor(3);
}

void ExitButton()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.f);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(6, 89, 107, 150)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(6, 89, 107, 200)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(6, 89, 107, 255)));

	ImGui::SetCursorPos({ 5.f, 305.f });
	if (ImGui::Button("Exit", { 190.f, 40.f })) {
		render::isRunning = false;
	}
	ImGui::PopStyleVar();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(3);
}

void Console(const std::vector<std::string>& progress, const std::string& selectedProc)
{
	ImGui::SetCursorPos({ 405.f, 28.f });
	ImGui::Text("Console");

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(17, 19, 23)));

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.f);

	ImGui::SetCursorPos({ 400.f, 45.f });
	ImGui::BeginChild("ConsoleBg", { 295.f, 300.f });
	{
		ImGui::PopStyleVar();

		ImGui::SetCursorPos({ 5.f, 5.f });
		if (!selectedProc.empty()) {
			ImGui::Text("Target - %s", selectedProc.c_str());
		}
		else {
			ImGui::Text("Target");
		}

		const auto drawList = ImGui::GetWindowDrawList();
		drawList->AddLine({ 400.f, 68.f }, { 700.f, 68.f }, IM_COL32(33, 33, 36, 255), 1.f);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(11, 12, 14)));

		ImGui::SetCursorPos({ 0.f, 25.f });
		ImGui::BeginChild("progressBg", { 400.f, 275.f });
		{
			ImGui::SetCursorPosY(3.f);
			if (!progress.empty()) {
				for (const auto& status : progress)
				{
					ImGui::SetCursorPosX(5.f);
					ImGui::Text("%s", status.c_str());
				}
			}

			ImGui::PopStyleColor();
			ImGui::EndChild();
		}

		ImGui::PopStyleColor();
		ImGui::EndChild();
	}
}

[[nodiscard]] std::string ProcessList(const std::set<Process::ProcessInfo>& processInfo)
{
	static std::string selectedProc;

	ImGui::SetCursorPos({ 10.f, 28.f });
	ImGui::Text("Process List");

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.f);

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(17, 19, 23)));

	ImGui::SetCursorPos({ 5.f, 45.f });
	ImGui::BeginChild("ProcessBg", { 390.f, 210.f });
	{
		ImGui::PopStyleVar();

		ImGui::SetCursorPos({ 5.f, 7.f });
		ImGui::Text("Name");

		ImGui::SetCursorPos({ 200.f, 7.f });
		ImGui::Text("PID");

		const auto drawList = ImGui::GetWindowDrawList();
		drawList->AddLine({ 5.f, 68.f }, { 400.f, 68.f }, IM_COL32(33, 33, 36, 255), 1.f);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(11, 12, 14, 255)));

		ImGui::SetCursorPos({ 0.f, 25.f });
		ImGui::BeginChild("runningProcBg", { 400.f, 185.f });
		{
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(ImColor(0, 85, 89, 200)));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(ImColor(0, 85, 89, 255)));

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

			ImGui::PopStyleColor();
			ImGui::EndChild();
		}

		ImGui::PopStyleColor(3);
		ImGui::EndChild();
	}

	return selectedProc;
}

void RefreshButton(std::set<Process::ProcessInfo>& processInfo)
{
	const Process proc;
	static auto refresh{ false };

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.f);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(6, 89, 107, 150)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(6, 89, 107, 200)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(6, 89, 107, 255)));

	ImGui::SetCursorPos({ 200.f, 305.f });
	if (ImGui::Button("Refresh", { 195.f, 40.f })) {
		refresh = true;
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleColor(3);

	if (refresh) 
	{
		processInfo = proc.processInformation();
		refresh = false;
	}
}}

void render::Render() noexcept
{
#define CREATE_CONSOLE 0
#if CREATE_CONSOLE
	AllocConsole();
	FILE* f;
	(void)freopen_s(&f, "CONOUT$", "w", stdout);
#endif // CREATE_CONSOLE

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(ImColor(9, 10, 11, 255)));
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

	const Process proc;
	static std::set<Process::ProcessInfo> processInfo = proc.processInformation();

	draw::RefreshButton(processInfo);
	const auto selectedProc = draw::ProcessList(processInfo);

	static std::vector<std::string> progress{};
	draw::MenuBar();
	draw::Console(progress, selectedProc);
	draw::InjectButton(progress);
	draw::ExitButton();

	ImGui::End();
}