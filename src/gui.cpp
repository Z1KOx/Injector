#include "render.hpp"
#include "fileDialog.hpp"
#include "injector.hpp"

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

    FileDialog dialog;

    if (ImGui::Button("Inject BTN", { 200.f, 25.f }))
    {
        dialog.open();

        const char* dllPath = dialog.getPath();
        const char* procName = "ac_client.exe";

        Injector injector(dllPath, procName);
        injector.injectDll();
    }

    ImGui::End();
}