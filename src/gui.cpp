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

		ImGui::SameLine(60.f);
		ImGui::TextColored(ImVec4(ImColor(119, 99, 191, 255)), "v1.0");

		const auto drawList = ImGui::GetWindowDrawList();
		drawList->AddLine({ 0.f, 20.f }, { 700.f, 20.f }, IM_COL32(22, 22, 26, 255), 1.f);
	}

	void InjectButton(std::vector<std::string>& progress)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(21, 21, 23, 255)));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(39, 39, 43, 255)));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(46, 46, 55, 255)));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(ImColor(30, 30, 32, 255)));

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

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(4);
	}

	void ExitButton()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(21, 21, 23, 150)));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(21, 21, 23, 200)));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(21, 21, 23, 255)));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(ImColor(30, 30, 32, 255)));

		ImGui::SetCursorPos({ 5.f, 305.f });
		if (ImGui::Button("Exit", { 190.f, 40.f })) {
			render::isRunning = false;
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(4);
	}

	void Console(const std::vector<std::string>& progress, const std::string& selectedProc)
	{
		ImGui::SetCursorPos({ 405.f, 28.f });
		ImGui::Text("Console");

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(162, 141, 238, 255)));

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.f);

		ImGui::SetCursorPos({ 400.f, 45.f });
		ImGui::BeginChild("ConsoleBg", { 295.f, 210.f });
		{
			ImGui::PopStyleVar();

			const std::string truncatedName = (strlen(selectedProc.c_str()) > 30) ? std::string(selectedProc.c_str()).substr(0, 30 - 3) + "..."
			                                                                      : std::string(selectedProc.c_str());
			ImGui::SetCursorPos({ 5.f, 5.f });
			if (!selectedProc.empty()) {
				ImGui::Text("Target - %s", truncatedName.c_str());
			}
			else {
				ImGui::Text("Target");
			}

			const auto drawList = ImGui::GetWindowDrawList();
			drawList->AddLine({ 400.f, 69.f }, { 700.f, 69.f }, IM_COL32(97, 82, 115, 255), 1.f);

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(17, 17, 17, 255)));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(ImColor(42, 39, 46, 255)));

			ImGui::SetCursorPos({ 0.f, 25.f });
			ImGui::BeginChild("progressBg", { 295.f, 185.f }, ImGuiChildFlags_Border);
			{
				ImGui::SetCursorPosY(3.f);
				if (!progress.empty()) {
					for (const auto& status : progress)
					{
						ImGui::SetCursorPosX(5.f);
						ImGui::Text("%s", status.c_str());
					}
				}

				ImGui::PopStyleColor(3);
				ImGui::EndChild();
			}

			ImGui::PopStyleColor();
			ImGui::EndChild();
		}
	}

	[[nodiscard]] std::string ProcessList(const std::set<Process::info>& processInfo)
	{
		static std::string selectedProc;

		ImGui::SetCursorPos({ 10.f, 28.f });
		ImGui::Text("Process List");

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.f);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(162, 141, 238, 255)));

		ImGui::SetCursorPos({ 5.f, 45.f });
		ImGui::BeginChild("ProcessBg", { 390.f, 210.f });
		{
			ImGui::PopStyleVar();

			ImGui::SetCursorPos({ 5.f, 5.f });
			ImGui::Text("Name");

			ImGui::SetCursorPos({ 200.f, 5.f });
			ImGui::Text("PID");

			ImGui::SetCursorPos({ 300.f, 5.f });
			ImGui::Text("Type");

			const auto drawList = ImGui::GetWindowDrawList();
			drawList->AddLine({ 5.f, 69.f }, { 400.f, 69.f }, IM_COL32(97, 82, 115, 255), 1.f);

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImColor(17, 17, 17, 255)));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(ImColor(42, 39, 46, 255)));

			ImGui::SetCursorPos({ 0.f, 25.f });
			ImGui::BeginChild("runningProcBg", { 390.f, 185.f }, ImGuiChildFlags_Border);
			{
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(ImColor(0, 85, 89, 200)));
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(ImColor(0, 85, 89, 255)));

				ImGui::SetCursorPosY(3.f);
				for (const auto& proc : processInfo)
				{
					const std::string truncatedName = (strlen(proc.name) > 20) ? std::string(proc.name).substr(0, 20 - 3) + "..."
					                                                           : std::string(proc.name);
					ImGui::SetCursorPosX(5.f);
					if (ImGui::Selectable(truncatedName.c_str())) {
						selectedProc = proc.name;
					}
					ImGui::SameLine(200);
					ImGui::Text("%d", proc.pid);

					ImGui::SameLine(300);

					proc.is64Bit ? ImGui::Text("x64") : ImGui::Text("x86");
				}

				ImGui::PopStyleColor();
				ImGui::EndChild();
			}

			ImGui::PopStyleColor(3);
			ImGui::EndChild();
		}

		return selectedProc;
	}

	void RefreshButton(std::set<Process::info>& processInfo)
	{
		const Process proc;
		static auto refresh{ false };

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(21, 21, 23, 150)));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(21, 21, 23, 200)));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(21, 21, 23, 255)));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(ImColor(30, 30, 32, 255)));

		ImGui::SetCursorPos({ 200.f, 305.f });
		if (ImGui::Button("Refresh", { 195.f, 40.f })) {
			refresh = true;
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(4);

		if (refresh)
		{
			processInfo = proc.processInformation();
			refresh = false;
		}
	}
} // draw

void render::Render() noexcept
{
#define CREATE_CONSOLE 0
#if CREATE_CONSOLE
	AllocConsole();
	FILE* f;
	(void)freopen_s(&f, "CONOUT$", "w", stdout);
#endif // CREATE_CONSOLE

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
	static std::set<Process::info> processInfo = proc.processInformation();

	draw::RefreshButton(processInfo);
	const auto selectedProc = draw::ProcessList(processInfo);

	static std::vector<std::string> progress{};
	draw::MenuBar();
	draw::Console(progress, selectedProc);
	draw::InjectButton(progress);
	draw::ExitButton();

	ImGui::SetCursorPos({ 500, 295 });
	ImGui::Text("Place holder");

	ImGui::End();
}