#include "render.hpp"
#include "fileDialog.hpp"
#include "injector.hpp"

#include "..\..\dependencies\imgui\imgui.h"

void render::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"Injector",
		&render::isRunning,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse
	);

	FileDialog dialog;

	static const char* status1;
	static const char* status2;
	static const char* status3;
	static const char* status4;
	static const char* status5;
	static const char* status6;
	static const char* status7;
	static const char* status8;

	if (ImGui::Button("Inject a dll", { 200.f, 25.f }))
	{
		dialog.open();

		const char* dllPath = dialog.getPath();
		const char* procName = "ac_client.exe";

		Injector injector(dllPath, procName);

		injector.injectDll();

		status1 = injector.getOpeningDLLStatus();
		status2 = injector.getPIDStatus();
		status3 = injector.getOpeningTargetStatus();
		status4 = injector.getAllocMemStatus();
		status5 = injector.getWriteMemStatus();
		status6 = injector.getCreateThreadStatus();
		status7 = injector.getThreadFinishStatus();
		status8 = injector.getSuccessfulInjectStatus();

	}

	if (status1 != nullptr)
	{
		ImGui::Text("%s", status1);
		ImGui::Text("%s", status2);
		ImGui::Text("%s", status3);
		ImGui::Text("%s", status4);
		ImGui::Text("%s", status5);
		ImGui::Text("%s", status6);
		ImGui::Text("%s", status7);
		ImGui::Text("%s", status8);
	}

	ImGui::End();
}