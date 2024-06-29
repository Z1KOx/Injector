#include "render.hpp"
#include "fileDialog.hpp"
#include "imgui_internal.h"
#include "injector.hpp"
#include "process.hpp"

#include "..\..\dependencies\imgui\imgui.h"

namespace util
{
	void gradientRect( ImVec2 p1, ImVec2 p2, ImU32 col_top, ImU32 col_bottom )
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		if ( (col_top >> 24) == 0 && (col_bottom >> 24) == 0 ) {
			return;
		}

		draw_list->AddRectFilledMultiColor( p1, p2, col_top, col_top, col_bottom, col_bottom );
	}
} // util

namespace draw
{
	void menuBar()
	{
		util::gradientRect( { 0.f, 0.f }, { 700.f, 23.f }, IM_COL32(20, 20, 25, 255), IM_COL32(31, 33, 43, 255) );

		ImGui::SetCursorPos( { 5.f, 3.f } );
		ImGui::Text( "Injector" );

		ImGui::SameLine( 660.f );
		ImGui::TextColored( ImVec4(ImColor(119, 99, 191, 255)), "v1.2.0" );
	}

	void injectButton( std::vector<std::string>& progress, const std::string& selectedProc, bool& bInjectClicked, int& counter )
	{
		ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding,   7.f );
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.f );


		static auto lastInjectClicked{ false };

		if ( !selectedProc.empty() )
		{
			ImGui::PushStyleColor( ImGuiCol_Button,        IM_COL32(23, 27, 46, 255) );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, IM_COL32(35, 39, 59, 200) );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive,  IM_COL32(73, 78, 102, 255) );
			ImGui::PushStyleColor( ImGuiCol_Border,        IM_COL32(134, 109, 255, 255) );

			ImGui::SetCursorPos(         { 190.f, 260.f } );
			if (ImGui::Button( "Inject", { 205.f, 85.f }) )
			{
				FileDialog dialog;
				dialog.open();

				const auto dllPath = dialog.getPath();

				Injector injector( dllPath, "ac_client.exe" );
				injector.injectDll();

				progress = injector.getProgress();

				bInjectClicked = true;
				counter = 11000;
				lastInjectClicked = true;
			}
			else
			{
				if ( lastInjectClicked ) {
					lastInjectClicked = false;
				}
				bInjectClicked = false;
			}
		}
		else
		{
			ImGui::PushStyleColor( ImGuiCol_Border,        IM_COL32(255, 13, 50, 255) );
			ImGui::PushStyleColor( ImGuiCol_Button,        IM_COL32(48, 14, 20, 255) );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, IM_COL32(71, 26, 34, 255) );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive,  IM_COL32(77, 26, 27, 255) );

			ImGui::SetCursorPos(     { 190.f, 260.f } );
			ImGui::Button( "Inject", { 205.f, 85.f } );

			bInjectClicked = false;
		}

		ImGui::PopStyleVar( 2 );
		ImGui::PopStyleColor( 4 );
	}

	void exitButton()
	{
		ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding,   7.f );
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.f );

		ImGui::PushStyleColor( ImGuiCol_Button,        IM_COL32(23, 27, 46, 255) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, IM_COL32(35, 39, 59, 200) );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive,  IM_COL32(73, 78, 102, 255) );
		ImGui::PushStyleColor( ImGuiCol_Border,        IM_COL32(134, 109, 255, 255) );

		ImGui::SetCursorPos(       { 5.f, 305.f } );
		if ( ImGui::Button("Exit", { 180.f, 40.f }) ) {
			render::isRunning = false;
		}

		ImGui::PopStyleVar( 2 );
		ImGui::PopStyleColor( 4 );
	}

	void console( const std::vector<std::string>& progress, const std::string& selectedProc, bool& bInjectClicked, int& counter )
	{
		// Seperator
		util::gradientRect( { 395.f, 25.f }, { 695.f, 50.f }, IM_COL32(30, 32, 42, 255), IM_COL32(100, 52, 123, 255) );
		util::gradientRect( { 395.f, 50.f }, { 695.f, 65.f }, IM_COL32(98, 51, 121, 255), IM_COL32(28, 30, 40, 255) );

		util::gradientRect( { 400.f, 21.f }, { 695.f, 45.f }, IM_COL32(30, 32, 42, 255), IM_COL32(100, 52, 123, 255) );

		ImGui::SetCursorPos( { 405.f, 25.f } );
		ImGui::TextColored( ImVec4(ImColor(227, 225, 227, 255)), "Console" );

		util::gradientRect( { 400.f, 45.f }, { 695.f, 75.f }, IM_COL32(155, 94, 196, 255), IM_COL32(44, 26, 56, 255) );

		ImGui::PushStyleColor( ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 50) );

		ImGui::SetCursorPos(            { 400.f, 45.f } );
		ImGui::BeginChild( "ConsoleBg", { 295.f, 300.f } );
		{
			ImGui::PopStyleVar();

			const auto truncatedName = (selectedProc.length() > 30) ? selectedProc.substr(0, 27) + "..."
			                                                                 : selectedProc;
			ImGui::SetCursorPos( { 5.f, 5.f } );
			if ( !selectedProc.empty() ) {
				ImGui::Text( "Target - %s", truncatedName.c_str() );
			}
			else {
				ImGui::Text( "Target" );
			}

			util::gradientRect( { 400.f, 75.f }, { 695.f, 350.f }, IM_COL32(30, 30, 30, 255), IM_COL32(10, 10, 10, 255) );

			ImGui::PushStyleColor( ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0) );

			ImGui::SetCursorPos(             { 0.f, 30.f } );
			ImGui::BeginChild( "progressBg", { 295.f, 270.f } );
			{
				static auto bPrint{ false };
				if ( bInjectClicked ) {
					bPrint = true;
				}

				if ( counter == 1000 )
				{
					bPrint = false;
					bInjectClicked = false;
				}

				ImGui::SetCursorPosY( 3.f );
				if ( !progress.empty() && bPrint )
				{
					for ( const auto& status : progress )
					{
						ImGui::SetCursorPosX( 5.f );
						ImGui::Text( "%s", status.c_str() );

						--counter;
					}

					ImGui::SetCursorPos( { 150.f, 240.f } );
					ImGui::TextColored( ImVec4(ImColor(100, 50, 150, 255)), "Clearing in %d seconds", counter / 1000 );
				}

				ImGui::PopStyleColor( 2 );
				ImGui::EndChild();
			}

			ImGui::PopStyleColor();
			ImGui::EndChild();
		}
	}

	[[nodiscard]] std::string processList( const std::set<Process::info>& processInfo )
	{
		util::gradientRect( { 5.f, 23.f }, { 395.f, 45.f }, IM_COL32(30, 32, 42, 255), IM_COL32(100, 52, 123, 255) );


		ImGui::SetCursorPos(                 { 10.f, 25.f } );
		ImGui::TextColored( ImVec4(ImColor(227, 225, 227, 255)), "Process List" );
		if ( ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip) )
		{
			ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 10.0f );

			ImGui::SetNextWindowSize( { 259.f, 74.f } );

			ImGui::BeginTooltip();

			ImGui::SetCursorPos( { 10.f, 10.f } );
			ImGui::TextWrapped( "If you can't find your process, click the Refresh button." );

			const auto processNum{ processInfo.size() };
			ImGui::SetCursorPos( { 10.f, 50.f } );
			ImGui::TextWrapped( "Current running processes: %d", processNum );

			ImGui::PopStyleVar();
			ImGui::EndTooltip();
		}

		static std::string selectedProc;

		util::gradientRect( { 5.f, 45.f }, { 395.f, 75.f }, IM_COL32(155, 94, 196, 255), IM_COL32(44, 26, 56, 255) );

		ImGui::PushStyleColor( ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 50) );

		ImGui::SetCursorPos(            { 5.f, 45.f }) ;
		ImGui::BeginChild( "ProcessBg", { 390.f, 210.f } );
		{
			ImGui::PopStyleVar();

			ImGui::SetCursorPos( { 5.f, 5.f }) ;
			ImGui::Text( "Name" );

			ImGui::SetCursorPos( { 200.f, 5.f } );
			ImGui::Text( "PID" );

			ImGui::SetCursorPos( { 300.f, 5.f } );
			ImGui::Text( "Type" );

			const auto drawList = ImGui::GetWindowDrawList();
			drawList->AddLine( { 5.f, 77.f }, { 400.f, 77.f }, IM_COL32(97, 82, 115, 255), 1.f );

			util::gradientRect( { 0.f, 75.f }, { 400.f, 260.f }, IM_COL32(30, 30, 30, 255), IM_COL32(10, 10, 10, 255) );

			ImGui::PushStyleColor( ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0) );

			ImGui::SetCursorPos( { 0.f, 30.f } );
			ImGui::BeginChild( "runningProcBg", { 390.f, 180.f });
			{
				ImGui::PushStyleColor( ImGuiCol_HeaderHovered, IM_COL32(0, 85, 89, 200) );
				ImGui::PushStyleColor( ImGuiCol_HeaderActive,  IM_COL32(0, 85, 89, 255) );

				ImGui::SetCursorPosY( 3.f );
				for ( const auto& proc : processInfo )
				{
					const auto truncatedName = (strlen(proc.name) > 20) ? std::string(proc.name).substr(0, 20 - 3) + "..."
					                                                          : std::string(proc.name);

					ImGui::SetCursorPosX( 5.f );
					if ( ImGui::Selectable(truncatedName.c_str()) ) {
						selectedProc = proc.name;
					}
					ImGui::SameLine( 200.f );
					ImGui::Text( "%d", proc.pid );

					ImGui::SameLine( 300.f );

					proc.is64Bit ? ImGui::Text("x64") : ImGui::Text("x86");
				}

				ImGui::PopStyleColor();
				ImGui::EndChild();
			}

			ImGui::PopStyleColor( 2 );
			ImGui::EndChild();
		}

		return selectedProc;
	}

	void refreshButton( std::set<Process::info>& processInfo )
	{
		static auto refresh{ false };

		ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding,   7.f );
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.f );

		ImGui::PushStyleColor( ImGuiCol_Button,        IM_COL32(23, 27, 46, 255) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, IM_COL32(35, 39, 59, 200) );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive,  IM_COL32(73, 78, 102, 255) );
		ImGui::PushStyleColor( ImGuiCol_Border,        IM_COL32(134, 109, 255, 255) );

		ImGui::SetCursorPos(           { 5.f, 260.f } );
		if ( ImGui::Button( "Refresh", { 180.f, 40.f }) ) {
			refresh = true;
		}

		ImGui::PopStyleVar( 2 );
		ImGui::PopStyleColor( 4 );

		if ( refresh )
		{
			const Process proc;
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
	(void)freopen_s( &f, "CONOUT$", "w", stdout );
#endif // CREATE_CONSOLE

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f) );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );

	ImGui::SetNextWindowPos( { 0, 0 } );
	ImGui::SetNextWindowSize( { WIDTH, HEIGHT } );
	ImGui::Begin( "Injector",
	              &render::isRunning,
		          ImGuiWindowFlags_NoResize |
		          ImGuiWindowFlags_NoSavedSettings |
		          ImGuiWindowFlags_NoCollapse |
		          ImGuiWindowFlags_NoTitleBar );

	util::gradientRect( { 0.f, 0.f }, { 700.f, 350.f }, IM_COL32(32, 34, 44, 255), IM_COL32(27, 29, 39, 255) );

	const Process proc;
	static std::set<Process::info> processInfo = proc.processInformation();

	draw::menuBar();

	ImGui::SetCursorPos( { 0.f, 20.f } );
	draw::refreshButton( processInfo );
	const auto selectedProc = draw::processList( processInfo );
	static std::vector<std::string> progress;

	static auto bInjectClicked{ false };
	static auto counter{ 11000 }; // 10 seconds

	draw::injectButton( progress, selectedProc, bInjectClicked, counter );
	draw::console( progress, selectedProc, bInjectClicked, counter );

	draw::exitButton();

	ImGui::End();
}