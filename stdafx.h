#pragma once
#include <Windows.h>
#include <memory>
#include <thread>
#include <atomic>
#include <array>
#include <string>
#include <locale>
#include <codecvt>
#include <chrono>
#include <future>
#include <unordered_map>
#include <stdint.h>
#include <stdexcept>
#include <map>
#include <list>
#include <math.h>
#include <Shlobj.h>
#include <Windows.h>
#include <gdiplus.h>
#include <stdint.h>
#include <cinttypes>
#include <mutex>
#include <fstream>

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <stdio.h>

#pragma comment (lib, "gdiplus.lib")
#include <ntverp.h>
#if (WINVER >= 0x0A00 )
#include <d3d12.h>
#include <dxgi1_4.h>
#include "d3dx12.h"
#endif
#include <d3d11.h>
#include <d3d10.h>
#include <d3d9.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;
#include "Win32Exception.h"
#include "Capture.h"
#include "FuncHook.h"
#include "DXGICapture.h"
#include "Direct3D9Capture.h"
//#include "Renderer.h"
//#include "Direct3DRenderer.h"
//#include "Direct3D9Renderer.h"
//#include "Direct3D10Renderer.h"
//#include "Direct3D11Renderer.h"
#if (WINVER >= 0x0A00 )
//#include "Direct3D12Renderer.h"
#endif


#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_dx11.h"
#include "inputhook.h"
using namespace ImGui;

#include "menu.h"
using namespace DX11;
/*

#include "Utils.h"
#include "Vector.h"
#include "VectorLong.h"

#include "Offsets.h"
#include "BuffManager.h"
#include "BuffEntry.h"

#include "SpellData.h"
#include "SpellInfo.h"
#include "SpellSlot.h"

#include "Hooks.h"
#include "CObject.h"
#include "CObjectManager.h"
#include "Engine.h"
*/

static bool isMainThreadAllow = true;
const auto MenuKey = VK_INSERT;
