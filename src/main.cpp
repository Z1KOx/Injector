#include "include/render.hpp"

#include <thread>

int __stdcall wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR lpCmdLine,
	int nShowCmd)
{
	// create gui
	render::CreateHWindow("Injector");
	render::CreateDevice();
	render::CreateImGui();

	while (render::isRunning)
	{
		render::BeginRender();
		render::Render();
		render::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// destroy gui
	render::DestroyImGui();
	render::DestroyDevice();
	render::DestroyHWindow();

	return EXIT_SUCCESS;
}