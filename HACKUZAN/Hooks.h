#pragma once

#include "stdafx.h"
#include "VirtualTableManager.h"

namespace HACKUZAN {
	typedef HRESULT(STDMETHODCALLTYPE* fnPresent)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
	typedef HRESULT(STDMETHODCALLTYPE* fnReset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

	class Hooks {
	public:
		static WNDPROC WndProc;
		static VirtualTableManager* Direct3DDevice9VMT;
		static fnPresent Present;
		static fnReset Reset;

		static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static HRESULT STDMETHODCALLTYPE HookPresent(IDirect3DDevice9* device, RECT* src_rect, RECT* dst_rect, HWND override_window, RGNDATA* dirty_region);
		static HRESULT STDMETHODCALLTYPE HookReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);

		static void Initialize();
		static void Dispose();
	};
}