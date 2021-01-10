#pragma once

#include <d3d9.h>

namespace DirectX9 {
	namespace TypeDefs {
		typedef HRESULT(WINAPI* Prototype_Present)(LPDIRECT3DDEVICE9, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
		typedef HRESULT(WINAPI* Prototype_Reset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
	}
	namespace Functions {
		TypeDefs::Prototype_Present Original_Present;
		TypeDefs::Prototype_Reset Original_Reset;
	}
	namespace Offsets {
		LPDIRECT3DDEVICE9 pDevice;
	}
}