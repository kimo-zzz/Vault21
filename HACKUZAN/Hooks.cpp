#include "stdafx.h"
#include "Hooks.h"
#include "Globals.h"
#include "DeviceHandler.h"
#include "Instances.h"
#include "Renderer.h"
#include "RenderLayer.h"
#include "HudManager.h"
#include "EventManager.h"
#include "Menu.h"
#include "TargetSelector.h"
#include "Orbwalker.h"
#include "Plugins.h"
#include "MenuSettings.h"

using namespace HACKUZAN::SDK;

namespace HACKUZAN {
	WNDPROC Hooks::WndProc;
	VirtualTableManager* Hooks::Direct3DDevice9VMT;
	fnReset Hooks::Reset;
	fnPresent Hooks::Present;

	void Hooks::Initialize() {
		WndProc = (WNDPROC)SetWindowLong(Globals::MainWindow, GWL_WNDPROC, (LONG)HookWndProc);

		Direct3DDevice9VMT = new VirtualTableManager(DeviceHandler::Instance->Deviceptr->Direct3DDevice, Globals::D3D9Module);
		Reset = (fnReset)Direct3DDevice9VMT->HookMethod(16, HookReset);
		Present = (fnPresent)Direct3DDevice9VMT->HookMethod(17, HookPresent);
	}

	void Hooks::Dispose() {
		SetWindowLong(Globals::MainWindow, GWL_WNDPROC, (LONG)WndProc);
		delete Direct3DDevice9VMT;
	}

	LRESULT CALLBACK Hooks::HookWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg == WM_MOUSEMOVE) {
			HudManager::CursorPos2D.X = (short)lparam;
			HudManager::CursorPos2D.Y = (short)(lparam >> 16);
		}
		Menu::OnWndProc(msg, wparam);
		EventManager::Trigger(LeagueEvents::OnWndProc, msg, wparam);
		return CallWindowProc(WndProc, hwnd, msg, wparam, lparam);
	}

	HRESULT STDMETHODCALLTYPE Hooks::HookReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) {
		Renderer::PreReset();
		auto result = Reset(device, params);
		Renderer::PostReset();

		return result;
	}

	HRESULT STDMETHODCALLTYPE Hooks::HookPresent(IDirect3DDevice9* device, RECT* src_rect, RECT* dst_rect, HWND override_window, RGNDATA* dirty_region) {
		Renderer::NewFrame();
		EventManager::Trigger(LeagueEvents::OnPresent);
		Menu::OnDraw();
		Renderer::AddText("HACKUZAN", 16.0f, Rect(0.0f, 0.0f, (float)RenderLayer::Instance->ClientWidth, 0.0f), DT_CENTER, IM_COL32(255, 24, 24, 255));
		Renderer::RenderFrame();
		return Present(device, src_rect, dst_rect, override_window, dirty_region);
	}
}