#pragma once

#define WIN32_LEAN_AND_MEAN

#include <sdkddkver.h>
#include <Windows.h>

// C++ standard library
#include <iostream>
#include <algorithm>
#include <fstream>
#include <functional>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <chrono>

// DirectX 9
#include <d3d9.h>

using namespace std;

#include <memory>
#include <thread>
#include <atomic>
#include <array>
#include <locale>
#include <codecvt>
#include <future>
#include <stdint.h>
#include <stdexcept>
#include <map>
#include <list> 
#include <Shlobj.h>

#include <stdint.h>
#include <cinttypes>
#include <mutex>

#include <iomanip>
#include <sstream>
#include <cstdlib>

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

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_dx11.h"


using namespace ImGui;

extern HMODULE g_hModule;


// json
#include "json.hpp"
using json = nlohmann::json;
#define baseAddr (DWORD)GetModuleHandleA(NULL) //GetModuleHandle

#include "GameClient.h"
//Debugger
#include "Console.h"

