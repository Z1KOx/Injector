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

	static std::vector<std::string> progress;

	if (ImGui::Button("Inject a dll", { 200.f, 25.f }))
	{
		dialog.open();

		auto dllPath = dialog.getPath();
		auto procName = "ac_client.exe";

		Injector injector(dllPath, procName);
		injector.injectDll();

		progress = injector.getProgress();
	}

	if (!progress.empty()) {
		for (const auto& status : progress) {
			ImGui::Text("%s", status.c_str());
		}
	}

	ImGui::End();
}