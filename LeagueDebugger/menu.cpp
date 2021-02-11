#include "stdafx.h"

#include "ExampleAppLog.h"
#include "LeagueHooks.h"
#include "UltimateHooks.h"

#define oLocalPlayer 0x2F4F764 //			A1 ?? ?? ?? ?? 85 C0 74 07 05 ?? ?? ?? ?? EB 02 33 C0 56 // dword_[offset]


ExampleAppLog AppLog;

std::vector<watchListEntry> readWatchlist;
std::vector<watchListEntry> writeWatchlist;
std::vector<watchListEntry> executeWatchlist;

LeagueHooksVEH _LeagueHooksVEH;

namespace DX11
{
	bool Menu::bIsOpen = true;
	static bool onInit = false;


	static void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	char str_buffer[256];
	void Menu::Content()
	{
		static DWORD meBase = 0x0;
		static DWORD Base = 0x0;
		static DWORD HiddenBase = 0x0;

		static std::string meBase_debug = "0x0";
		static std::string base_debug = "0x0";
		static std::string hiddenBase_debug = "0x0";

		static char newInputName[128] = "";
		static char newInputAddress[128] = "";
		static char newInputSize[128] = "";

		const char* items[] = { "Read", "Write" , "Execute" };
		static int item_current_3 = 0; // If the selection isn't within 0..count, Combo won't display a preview

		static char newInputAddressVirtualQuery[128] = "";
		static std::string virtualQueryResult = "";
		static DWORD virtualQueryResultVal = 0x0;
		static std::string virtualQueryResultVal_debug = "0x0";

		if ((isMainThreadAllow && !onInit) || !meBase)
		{

			HiddenBase = FindHiddenModule();
			hiddenBase_debug = hexify<DWORD>(HiddenBase);

			Base = baseAddr;
			base_debug = hexify<DWORD>(Base);

			meBase = (DWORD) (*(DWORD*)(baseAddr + oLocalPlayer));
			meBase_debug = hexify<DWORD>(meBase);
			AppLog.AddLog("Init Success\n");
			onInit = true;
		}

		if (Menu::bIsOpen)
		{
			//ImGui::ShowDemoWindow(&bShowDemo);

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize;

			ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(800, 800));
			ImGui::Begin("SH3N##1", &Menu::bIsOpen, window_flags);

			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("Tabs", tab_bar_flags))
			{
				if (ImGui::BeginTabItem("Main"))
				{
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Add address to watchlist");
					ImGui::Separator();
					ImGui::InputText("Name", newInputName, IM_ARRAYSIZE(newInputName));
					ImGui::InputText("Address", newInputAddress, IM_ARRAYSIZE(newInputAddress));
					ImGui::InputText("Func Size (Execute)", newInputSize, IM_ARRAYSIZE(newInputSize));
					ImGui::Combo("Watch Type", &item_current_3, items, IM_ARRAYSIZE(items));
					if (ImGui::Button("Add Entry")) {
						watchListEntry wl;
						std::vector<WatchData> wd;
						wl.isStarted = false;
						wl.name = string(newInputName);
						wl.address = std::strtoul(newInputAddress, NULL, 16);
						wl.funcSize = std::strtoul(newInputSize, NULL, 16);
						wl.watchData = wd;
						if (item_current_3 == 0) {
							readWatchlist.push_back(wl);
						}else if (item_current_3 == 1) {
							writeWatchlist.push_back(wl);
						}
						else if (item_current_3 == 2) {
							executeWatchlist.push_back(wl);
						}
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Read"))
				{
					std::vector<watchListEntry> _readWatchlist = readWatchlist;
					std::vector<watchListEntry> __readWatchlist;
					if (ImGui::BeginTabBar("ReadTabs", tab_bar_flags))
					{
						int i = 0;
						for (watchListEntry wl : _readWatchlist) {
							i++;
							if (ImGui::BeginTabItem((to_string(i) + "_r_" + wl.name).c_str()))
							{
								ImGui::Text(("Address: " + hexify<DWORD>(wl.address)).c_str());
								if (wl.isStarted) {
									if (ImGui::Button("Stop")) {
										_LeagueHooksVEH.removeHook(wl.address);
										wl.isStarted = !wl.isStarted;
									}
								}
								else {
									if (ImGui::Button("Start")) {
										_LeagueHooksVEH.addHook(wl.address);
										wl.isStarted = !wl.isStarted;
									}
								}
								for (WatchData wd : wl.watchData) {
									Separator();
									ImGui::Text(("Hit Count: " + to_string(wd.hitCount)).c_str());
									ImGui::Text(("Exception Address: " + hexify<DWORD>(wd.exceptionAddress)).c_str());
									ImGui::Text(("Instruction: " + wd.disassemblerInstruction).c_str());
									Separator();
									Columns(2, 0, true); // 2-ways, with border
									ImGui::Text(("Eax: " + hexify<DWORD>(wd.cr.Eax)).c_str());
									ImGui::Text(("Ebp: " + hexify<DWORD>(wd.cr.Ebp)).c_str());
									ImGui::Text(("Ebx: " + hexify<DWORD>(wd.cr.Ebx)).c_str());
									ImGui::Text(("Ecx: " + hexify<DWORD>(wd.cr.Ecx)).c_str());
									ImGui::Text(("Edi: " + hexify<DWORD>(wd.cr.Edi)).c_str());
									NextColumn();
									ImGui::Text(("Edx: " + hexify<DWORD>(wd.cr.Edx)).c_str());
									ImGui::Text(("Eip: " + hexify<DWORD>(wd.cr.Eip)).c_str());
									ImGui::Text(("Esi: " + hexify<DWORD>(wd.cr.Esi)).c_str());
									ImGui::Text(("Esp: " + hexify<DWORD>(wd.cr.Esp)).c_str());
									Columns(1);
									Spacing();
								}
								ImGui::EndTabItem();
							}
							__readWatchlist.push_back(wl);
						}
						ImGui::EndTabBar();
					}
					readWatchlist = __readWatchlist;
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Write"))
				{
					std::vector<watchListEntry> _writeWatchlist = writeWatchlist;
					std::vector<watchListEntry> __writeWatchlist;
					if (ImGui::BeginTabBar("WriteTabs", tab_bar_flags))
					{
						int i = 0;
						for (watchListEntry wl : _writeWatchlist) {
							i++;
							if (ImGui::BeginTabItem((to_string(i) + "_w_" + wl.name).c_str()))
							{
								ImGui::Text(("Address: " + hexify<DWORD>(wl.address)).c_str());
								if (wl.isStarted) {
									if (ImGui::Button("Stop")) {
										_LeagueHooksVEH.removeHook(wl.address);
										wl.isStarted = !wl.isStarted;
									}
								}
								else {
									if (ImGui::Button("Start")) {
										_LeagueHooksVEH.addHook(wl.address);
										wl.isStarted = !wl.isStarted;
									}
								}
								for (WatchData wd : wl.watchData) {
									Separator();
									ImGui::Text(("Hit Count: " + to_string(wd.hitCount)).c_str());
									ImGui::Text(("Exception Address: " + hexify<DWORD>(wd.exceptionAddress)).c_str());
									ImGui::Text(("Instruction: " + wd.disassemblerInstruction).c_str());
									Separator();
									Columns(2, 0, true); // 2-ways, with border
									ImGui::Text(("Eax: " + hexify<DWORD>(wd.cr.Eax)).c_str());
									ImGui::Text(("Ebp: " + hexify<DWORD>(wd.cr.Ebp)).c_str());
									ImGui::Text(("Ebx: " + hexify<DWORD>(wd.cr.Ebx)).c_str());
									ImGui::Text(("Ecx: " + hexify<DWORD>(wd.cr.Ecx)).c_str());
									ImGui::Text(("Edi: " + hexify<DWORD>(wd.cr.Edi)).c_str());
									NextColumn();
									ImGui::Text(("Edx: " + hexify<DWORD>(wd.cr.Edx)).c_str());
									ImGui::Text(("Eip: " + hexify<DWORD>(wd.cr.Eip)).c_str());
									ImGui::Text(("Esi: " + hexify<DWORD>(wd.cr.Esi)).c_str());
									ImGui::Text(("Esp: " + hexify<DWORD>(wd.cr.Esp)).c_str());
									Columns(1);
									Spacing();
								}
								ImGui::EndTabItem();
							}
							__writeWatchlist.push_back(wl);
						}
						ImGui::EndTabBar();
					}
					writeWatchlist = __writeWatchlist;
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Execute"))
				{
					std::vector<watchListEntry> _executeWatchlist = executeWatchlist;
					std::vector<watchListEntry> __executeWatchlist;
					if (ImGui::BeginTabBar("ExecuteTabs", tab_bar_flags))
					{
						int i = 0;
						for (watchListEntry wl : _executeWatchlist) {
							i++;
							if (ImGui::BeginTabItem((to_string(i) + "_e_" + wl.name).c_str()))
							{
								ImGui::Text(("Address: " + hexify<DWORD>(wl.address)).c_str());
								if (wl.isStarted) {
									if (ImGui::Button("Stop")) {
										UltimateHooks::removeHook(wl.address);
										wl.isStarted = !wl.isStarted;
									}
								}
								else {
									if (ImGui::Button("Start")) {
										UltimateHooks::addHook(wl.address, wl.funcSize);
										wl.isStarted = !wl.isStarted;
									}
								}
								for (WatchData wd : wl.watchData) {
									Separator();
									ImGui::Text(("Hit Count: " + to_string(wd.hitCount)).c_str());
									ImGui::Text(("Exception Address: " + hexify<DWORD>(wd.exceptionAddress)).c_str());
									ImGui::Text(("Instruction: " + wd.disassemblerInstruction).c_str());
									Separator();
									Columns(2, 0, true); // 2-ways, with border
									ImGui::Text(("Eax: " + hexify<DWORD>(wd.cr.Eax)).c_str());
									ImGui::Text(("Ebp: " + hexify<DWORD>(wd.cr.Ebp)).c_str());
									ImGui::Text(("Ebx: " + hexify<DWORD>(wd.cr.Ebx)).c_str());
									ImGui::Text(("Ecx: " + hexify<DWORD>(wd.cr.Ecx)).c_str());
									ImGui::Text(("Edi: " + hexify<DWORD>(wd.cr.Edi)).c_str());
									NextColumn();
									ImGui::Text(("Edx: " + hexify<DWORD>(wd.cr.Edx)).c_str());
									ImGui::Text(("Eip: " + hexify<DWORD>(wd.cr.Eip)).c_str());
									ImGui::Text(("Esi: " + hexify<DWORD>(wd.cr.Esi)).c_str());
									ImGui::Text(("Esp: " + hexify<DWORD>(wd.cr.Esp)).c_str());
									Columns(1);
									Spacing();
								}
								ImGui::EndTabItem();
							}
							__executeWatchlist.push_back(wl);
						}
						ImGui::EndTabBar();
					}
					executeWatchlist = __executeWatchlist;
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("VirtualQuery"))
				{
					ImGui::InputText("Address", newInputAddressVirtualQuery, IM_ARRAYSIZE(newInputAddressVirtualQuery));
					if (ImGui::Button("Check Protection")) {
						DWORD addr = std::strtoul(newInputAddressVirtualQuery, NULL, 16);
						auto mbi = MEMORY_BASIC_INFORMATION{ 0 };
						if (!VirtualQuery(reinterpret_cast<void*>(addr), &mbi, sizeof(mbi))) {
							virtualQueryResult = "Result: VirtualQuery failed";
						}

						virtualQueryResultVal = mbi.Protect;
						virtualQueryResultVal_debug = hexify<DWORD>(virtualQueryResultVal);

						string stringOutputProtect = "";

						if (mbi.Protect & PAGE_NOACCESS) (stringOutputProtect += " | PAGE_NOACCESS");
						if (mbi.Protect & PAGE_READONLY) (stringOutputProtect += "| PAGE_READONLY");
						if (mbi.Protect & PAGE_READWRITE) (stringOutputProtect += " | PAGE_READWRITE");
						if (mbi.Protect & PAGE_WRITECOPY) (stringOutputProtect += "  | PAGE_WRITECOPY");
						if (mbi.Protect & PAGE_EXECUTE) (stringOutputProtect += " | PAGE_EXECUTE");
						if (mbi.Protect & PAGE_EXECUTE_READ) (stringOutputProtect += " | PAGE_EXECUTE_READ");
						if (mbi.Protect & PAGE_EXECUTE_READWRITE) (stringOutputProtect += " | PAGE_EXECUTE_READWRITE");
						if (mbi.Protect & PAGE_EXECUTE_WRITECOPY) (stringOutputProtect += "  | PAGE_EXECUTE_WRITECOPY");
						if (mbi.Protect & PAGE_GUARD) (stringOutputProtect += " | PAGE_GUARD");
						if (mbi.Protect & PAGE_NOCACHE) (stringOutputProtect += " | PAGE_NOCACHE");

						string stringOutputMem = "";

						if (mbi.State & MEM_COMMIT) (stringOutputMem += " | MEM_COMMIT");
						if (mbi.State & MEM_FREE) (stringOutputMem += "| MEM_FREE");
						if (mbi.State & MEM_RESERVE) (stringOutputMem += " | MEM_RESERVE");

						string stringOutputType = "";

						if (mbi.Type & MEM_IMAGE) (stringOutputMem += " | MEM_COMMIT");
						if (mbi.Type & MEM_MAPPED) (stringOutputMem += " | MEM_FREE");
						if (mbi.Type & MEM_PRIVATE) (stringOutputMem += "  | MEM_RESERVE");

						virtualQueryResult = stringOutputProtect + stringOutputMem + stringOutputType;
						virtualQueryResult = removeZero(virtualQueryResult, ' ');
						virtualQueryResult = removeZero(virtualQueryResult, '|');
						virtualQueryResult = removeZero(virtualQueryResult, ' ');
						virtualQueryResult = "Result: " + virtualQueryResult;
						
					}
					ImGui::Text(("Protect:"+virtualQueryResultVal_debug).c_str());
					ImGui::Text((virtualQueryResult).c_str());
							
					ImGui::EndTabItem();
				}
				if (BeginTabItem("Debug"))
				{
					TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "===DEBUG===");
					Separator();
					ImGui::Text("Hidden Module %s", &hiddenBase_debug);
					ImGui::Text("Module %s", &base_debug);
					ImGui::Text("Me %s", &meBase_debug);
					EndTabItem();
				}
				
				ImGui::EndTabBar();
			}

			ImGui::End();
		}


	}

	void Menu::Render11()
	{
		Menu::Content();
	}

	void Menu::Render9()
	{
		Menu::Content();
	}
}

void SetupGameHooks() {

}

void MainLoop() {

}

void RemoveGameHooks() {

}