#include "process.hpp"

#include <TlHelp32.h>

[[nodiscard]] std::set<Process::ProcessInfo> Process::processInformation() const
{
	std::set<ProcessInfo> processes;

	const auto hSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

	std::set<const char*> sysProc{ "injector.exe", "Registry", "smss.exe", "csrss.exe", "wininit.exe", "services.exe", "System", "[System Process]",
		"lsass.exe", "winlogon.exe", "svchost.exe", "fontdrvhost.exe", "WUDFHost.exe", "dwm.exe", "Memory Compression", "WmiPrvSE.exe", "audiodg.exe",
		"spoolsv.exe", "MpDefenderCoreService.exe", "pservice.exe", "logi_lamparray_service.exe", "vmnetdhcp.exe", "MsMpEng.exe", "plasticchangetrackerservice.exe",
		"plasticd.exe", "OfficeClickToRun.exe", "dasHost.exe", "gamingservices.exe", "gamingservicesnet.exe", "rundll32.exe", "AggregatorHost.exe",
		"sihost.exe", "taskhostw.exe", "explorer.exe", "nvcontainer.exe", "NVDisplay.Container.exe", "SearchHost.exe", "StartMenuExperienceHost.exe",
		"Widgets.exe", "RuntimeBroker.exe", "RuntimeBroker.exe", "dllhost.exe", "SearchIndexer.exe", "ctfmon.exe", "NisSrv.exe", "nvsphelper64.exe",
		"NVIDIA Share.exe", "conhost.exe", "TextInputHost.exe", "RuntimeBroker.exe", "unsecapp.exe", "SystemSettings.exe", "WidgetService.exe",
		"ApplicationFrameHost.exe", "ShellExperienceHost.exe", "SDXHelper.exe", "XboxGameBarWidgets.exe", "XboxPcAppFT.exe", "Microsoft.Photos.exe",
		"msedge.exe", "devenv.exe", "PerfWatson2.exe", "Microsoft.ServiceHub.Controller.exe", "ServiceHub.VSDetouredHost.exe", "ServiceHub.IdentityHost.exe",
		"vsls-agent.exe", "ServiceHub.SettingsHost.exe", "ServiceHub.IndexingService.exe", "ServiceHub.IntellicodeModelService.exe", "cmd.exe",
		"ServiceHub.RoslynCodeAnalysisService.exe", "vshost.exe", "ServiceHub.Host.AnyCPU.exe", "ServiceHub.TestWindowStoreHost.exe", "vctip.exe",
		"UserOOBEBroker.exe", "smartscreen.exe", "vcpkgsrv.exe", "backgroundTaskHost.exe", "clang-tidy.exe", "NVIDIA Web Helper.exe", "nordvpn-service.exe",
		"NordUpdateService.exe", "vmware-authd.exe", "vmnat.exe", "lghub_updater.exe", "vmware-usbarbitrator64.exe", "BraveCrashHandler.exe",
		"BraveCrashHandler64.exe", "webwallpaper32.exe", "java.exe", "Taskmgr.exe", "steamwebhelper.exe", "lghub_system_tray.exe", "lghub_agent.exe", "uhssvc.exe",
		"steamservice.exe", "cl.exe", "PowerToys.AdvancedPaste.exe", "PowerToys.AlwaysOnTop.exe", "PowerToys.Awake.exe", "PowerToys.ColorPickerUI.exe",
		"PhoneExperienceHost.exe", "PowerToys.CropAndLock.exe", "PowerToys.FancyZones.exe", "PowerToys.Peek.UI.exe", "PowerToys.PowerLauncher.exe",
		"PowerToys.PowerOCR.exe", "sppsvc.exe", "OpenConsole.exe", "SppExtComObj.Exe", "WindowsTerminal.exe", "GameBar.exe", "GameBarFTServer.exe",
		"SecurityHealthService.exe", "Video.UI.exe", "SystemSettingsBroker.exe", "SearchProtocolHost.exe", "SearchFilterHost.exe", "DataExchangeHost.exe",
		"LockApp.exe"
	};

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		ProcessInfo procInfo;

		PROCESSENTRY32 lppe;
		lppe.dwSize = sizeof(lppe);

		for (bool proc = Process32First(hSnapshot, &lppe); proc; proc = Process32Next(hSnapshot, &lppe))
		{
			if (std::ranges::find_if(sysProc, [&](const char* procName) -> bool {
				return strcmp(procName, lppe.szExeFile) == 0;
				}) != sysProc.end()) {
				continue;
			}

			procInfo.pid = lppe.th32ProcessID;
			strcpy_s(procInfo.name, lppe.szExeFile);

			processes.insert(procInfo);
		}

		CloseHandle(hSnapshot);
	}


	return processes;
}
