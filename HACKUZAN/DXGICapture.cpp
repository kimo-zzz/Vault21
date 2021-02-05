#include "stdafx.h"
#include <stdlib.h>     // for _itoa_s functions, _countof, count macro
#include "FuncHook.h"
#include "HudManager.h"
#include "EventManager.h"
#include "Menu.h"

namespace HACKUZAN {
	HRESULT Direct3D10SwapChain(HMODULE d3d10_dll, HWND hWnd, ComPtr<IDXGISwapChain>& swap_chain, ComPtr<ID3D10Device>& device);
	HRESULT Direct3D11SwapChain(HMODULE d3d11_dll, HWND hWnd, ComPtr<IDXGISwapChain>& swap_chain, ComPtr<ID3D11DeviceContext>& context);
#if (WINVER >= 0x0A00 )   
	HRESULT Direct3D12SwapChain(HMODULE d3d12_dll, HWND hWnd, ComPtr<IDXGISwapChain>& swapchain, ComPtr<ID3D12CommandQueue>& commandqueue, ComPtr<ID3D12GraphicsCommandList>& commandlist);
#endif
	LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D11Texture2D* pRenderTargetTexture = 0;
	ID3D11RenderTargetView* g_pRenderTargetView = 0;

	const auto MenuKey = VK_INSERT;

	//HWND hWnd;
	HWND window;
	RECT rect;

	FuncHook hook;

	static WNDPROC OriginalWndProcHandler = nullptr;

	DXGICapture::DXGICapture()
	{
#if (WINVER >= 0x0A00 )  
		lastCommandQueue = nullptr;
#endif
	}

	DXGICapture::~DXGICapture()
	{
		FreeCapture();
	}

	HMODULE DXGICapture::GetCaptureModule()
	{
		static const std::string d3d12_dll = Capture::SYSTEM_PATH + "\\d3d12.dll";
		static const std::string d3d11_dll = Capture::SYSTEM_PATH + "\\d3d11.dll";
		static const std::string d3d10_dll = Capture::SYSTEM_PATH + "\\d3d10.dll";

		static const std::string dlls[] = {
			d3d12_dll,
			d3d11_dll,
			d3d10_dll,
		};

		int count = _countof(dlls) - 1;
		HMODULE module;

		for (size_t i = 0; i < _countof(dlls); i++)
		{
			const auto& dll = dlls[i];
			module = GetModuleHandleA(dll.c_str());
			if (module != nullptr)
				break;
			else
				--count;
		}

		if (count < 0)
			type = DXGI_TYPE::DIRECT3D_UNKNOWN;
		else
			type = (DXGI_TYPE)count;


		int i = count;
		char buffer[20];
		_itoa_s(i, buffer, 20, 2);
		LPCSTR p = buffer;

		//MessageBoxA(0 , p, "DXGI_TYPE", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);

		return module;
	}

	LRESULT CALLBACK DXGIMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	HRESULT DXGICapture::TryCapture()
	{
		HMODULE module = GetCaptureModule();

		if (module == nullptr)
		{
			// Throw Error, Module not could, cannot capture
			//MessageBoxA(0 , "Cannot Load Module", "Fatal Error", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
			return CO_E_DLLNOTFOUND;
		}
		//MessageBoxA(0, "Module Captured. Capturing Device (DX10+)", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		HRESULT hr = 0;

		ComPtr<IDXGISwapChain> swap_chain;
#if (WINVER >= 0x0A00 )    
		ComPtr<ID3D12GraphicsCommandList> command_list;
		ComPtr<ID3D12CommandQueue> command_queue;
#endif
		// Create window
		/*
		HWND _hWnd = NULL;
		while (!_hWnd) {
			//MessageBoxA(0, "Finding LoL Window", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
			//_hWnd = FindWindowW(0, L"League of Legends (TM) Client");
			_hWnd = FindWindowW(0, L"0");
		}
		GetWindowRect(_hWnd, &rect);

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		int w = width;
		wchar_t bufferw[256];
		wsprintfW(bufferw, L"%d", w);

		int h = height;
		wchar_t bufferh[256];
		wsprintfW(bufferh, L"%d", h);

		MessageBox(0, bufferw, L"Width", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		MessageBox(0, bufferh, L"Heght", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);

		int l = rect.left;
		wchar_t bufferl[256];
		wsprintfW(bufferl, L"%d", l);

		int t = rect.top;
		wchar_t buffert[256];
		wsprintfW(buffert, L"%d", t);

		MessageBox(0, bufferl, L"Left", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		MessageBox(0, buffert, L"Right", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);

		WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
		RegisterClassExA(&wc);
		HWND hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, rect.left, rect.top, width, height, NULL, NULL, wc.hInstance, NULL);

		*/
		WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
		RegisterClassExA(&wc);
		HWND hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);
		rect.top = 0;
		rect.bottom = 2;
		rect.right = 2;
		rect.left = 0;


		/*
		wc =
		{
			sizeof(WNDCLASSEX), CS_CLASSDC, DLLWindowProc, 0L, 0L,
			g_hModule, NULL, NULL, NULL, NULL,
			TITLE, NULL
		};
		if (!RegisterClassEx(&wc))
			return E_FAIL;


		*/
		/*
		hWnd = CreateWindow(TITLE, TITLE,
			WS_OVERLAPPEDWINDOW, rect.left, rect.top, width, height,
			NULL, NULL, wc.hInstance, NULL);
			*/
			//hWnd = CreateWindow(TITLE, TITLE,
			//	WS_OVERLAPPEDWINDOW, 100, 100, 1080, 720,
			//	NULL, NULL, wc.hInstance, NULL);

		switch (type)
		{
			/*
			case DIRECT3D10:
			{
				//MessageBoxA(0, "Direct3D10 Found.", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
				ComPtr<ID3D10Device> device;
				hr =  Direct3D10SwapChain(module, hWnd, swap_chain, device);
				if (FAILED(hr))
				{
					goto destroy_window;

				}
				// May hook Device's DrawIndex here :P if you want
				break;
			}
			*/
		case DIRECT3D11:
		{
			//MessageBoxA(0, "Direct3D11 Found.", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
			ComPtr<ID3D11DeviceContext> device_context;
			hr = Direct3D11SwapChain(module, hWnd, swap_chain, device_context);
			if (FAILED(hr))
			{
				goto destroy_window;
			}

			ComPtr<ID3D11Device> device;
			swap_chain->GetDevice(IID_PPV_ARGS(&device));

			uintptr_t* pDeviceContextVTable = *(uintptr_t**)device_context.Get();
			drawIndex_hook = FuncHook(pDeviceContextVTable[12], (uintptr_t)DrawIndex);
			hook_D3D11PSSetShaderResources = FuncHook(pDeviceContextVTable[8], (uintptr_t)D3D11PSSetShaderResources);

			uintptr_t* pDeviceVTable = *(uintptr_t**)device.Get();
			hook_D3D11CreateQuery = FuncHook(pDeviceVTable[24], (uintptr_t)D3D11CreateQuery);

			drawIndex_hook.hook();
			hook_D3D11PSSetShaderResources.hook();
			hook_D3D11CreateQuery.hook();

			break;
		}
		/*
	#if (WINVER >= 0x0A00 )
		case DIRECT3D12:
		{
			//MessageBoxA(0, "Direct3D12 Found.", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
			hr = Direct3D12SwapChain(module, hWnd, swap_chain, command_queue, command_list);
			if (FAILED(hr))
			{
				goto destroy_window;
			}

			uintptr_t* vtable = *(uintptr_t**)command_queue.Get();
			executecommandlists_hook = FuncHook(vtable[10], (uintptr_t)ExecuteCommandLists);
			executecommandlists_hook.hook();
			// May hook DeviceContext's DrawIndex here :P if you want
			break;
		}
	#endif
		case DIRECT3D_UNKNOWN:
			//MessageBoxA(0, "Unkown Direct3D", "SUCCESS", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		*/
		default: {
			//MessageBoxA(0, "Unknown DXGI Device! Contact Topblast for more information.", "Fatal Error", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
			goto destroy_window;
		}
		}

		uintptr_t* vtable = *(uintptr_t**)swap_chain.Get();
		present_hook = FuncHook(vtable[8], (uintptr_t)Present);
		clearRenderTargetView_hook = FuncHook(vtable[50], (uintptr_t)ClearRenderTargetView);
		resizebuffer_hook = FuncHook(vtable[13], (uintptr_t)ResizeBuffer);
		{
			ComPtr<IDXGISwapChain1> swap_chain1;
			if (SUCCEEDED(swap_chain->QueryInterface(IID_PPV_ARGS(&swap_chain1))))
			{
				//MessageBoxA(0 , "SwapChain1", "Success", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
				vtable = *(uintptr_t**)swap_chain1.Get();
				present1_hook = FuncHook(vtable[22], (uintptr_t)Present1);
			}
		}
		{
			ComPtr<IDXGISwapChain3> swap_chain3;
			if (SUCCEEDED(swap_chain->QueryInterface(IID_PPV_ARGS(&swap_chain3))))
			{
				//MessageBoxA(0 , "SwapChain3", "Success", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
				vtable = *(uintptr_t**)swap_chain3.Get();
				resizebuffer1_hook = FuncHook(vtable[39], (uintptr_t)ResizeBuffer1);
			}
		}
		present_hook.hook();
		resizebuffer_hook.hook();
		clearRenderTargetView_hook.hook();
		//MessageBoxA(0, "Hooks Ready", "Success", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	destroy_window:
		DestroyWindow(hWnd);
		//UnregisterClass(TITLE, wc.hInstance);
		return hr;
	}

	HRESULT DXGICapture::FreeCapture()
	{
		//ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		present_hook.unhook();
		clearRenderTargetView_hook.unhook();
		drawIndex_hook.unhook();
		hook_D3D11PSSetShaderResources.unhook();
		hook_D3D11CreateQuery.unhook();

		present1_hook.unhook();
		resizebuffer_hook.unhook();
		resizebuffer1_hook.unhook();

#if (WINVER >= 0x0A00 )   
		executecommandlists_hook.unhook();
#endif
		return S_OK;
	}

	std::unique_ptr<DXGICapture> DXGICapture::capture;
	DXGICapture* DXGICapture::singleton()
	{
		if (capture == nullptr)
		{
			capture = std::unique_ptr<DXGICapture>(new DXGICapture());
		}
		return capture.get();
	}

	void DXGICapture::destroy_singleton()
	{
		DXGICapture::capture.reset();
	}

	LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_MOUSEMOVE) {
			HudManager::CursorPos2D.X = (short)lParam;
			HudManager::CursorPos2D.Y = (short)(lParam >> 16);
		}
		HACKUZAN::SDK::Menu::OnWndProc(uMsg, wParam);
		EventManager::Trigger(LeagueEvents::OnWndProc, uMsg, wParam);

		return CallWindowProc(OriginalWndProcHandler, hWnd, uMsg, wParam, lParam);
	}

	void DXGICapture::DoPresent(IDXGISwapChain* swap, UINT syncInterval, UINT flags)
	{

		static std::once_flag is_init;

		std::call_once(is_init, [&]() {
			if (SUCCEEDED(swap->GetDevice(__uuidof(pDevice), reinterpret_cast<void**>(&pDevice))))
			{
				pDevice->GetImmediateContext(&pContext);
				if (SUCCEEDED(swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pRenderTargetTexture)))
				{
					DXGI_SWAP_CHAIN_DESC sd;
					swap->GetDesc(&sd);
					window = sd.OutputWindow;

					ImGui::GetIO().ImeWindowHandle = window;
					ImGuiIO& io = ImGui::GetIO();
					io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;


					pDevice->CreateRenderTargetView(pRenderTargetTexture, NULL, &g_pRenderTargetView);
					pRenderTargetTexture->Release();

					ImGui_ImplWin32_Init(window);
					//ImGui_ImplDX11_Init(pDevice, pContext);
					ImGui::CreateContext();
					////MessageBoxA(0, "DoPresent.", "Success", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);

					//Set OriginalWndProcHandler to the Address of the Original WndProc function
					//OriginalWndProcHandler = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)hWndProc);
				}
			}
			else
			{
				////MessageBoxA(0 , "GetDevice failed.", "Fatal Error", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
			}
			});

		pContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
	}

	void DXGICapture::DoResizeBuffer(IDXGISwapChain* swap, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT giFormat, UINT flags)
	{
		//ImGui_ImplDX11_InvalidateDeviceObjects();
		if (nullptr != g_pRenderTargetView)
		{
			g_pRenderTargetView->Release();
			g_pRenderTargetView = nullptr;
		}
		if (SUCCEEDED(swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pRenderTargetTexture)))
		{
			pDevice->CreateRenderTargetView(pRenderTargetTexture, NULL, &g_pRenderTargetView);
			//ImGui_ImplDX11_CreateDeviceObjects();
			pRenderTargetTexture->Release();
		}
	}


	void DXGICapture::DoDrawIndex(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
	{

	}

	void DXGICapture::DoD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery)
	{
		if (pQueryDesc->Query == D3D11_QUERY_OCCLUSION)
		{
			D3D11_QUERY_DESC oqueryDesc = CD3D11_QUERY_DESC();
			(&oqueryDesc)->MiscFlags = pQueryDesc->MiscFlags;
			(&oqueryDesc)->Query = D3D11_QUERY_TIMESTAMP;
		}
	}

	UINT pssrStartSlot;
	D3D11_SHADER_RESOURCE_VIEW_DESC Descr;

	void DXGICapture::DoD3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews)
	{
		pssrStartSlot = StartSlot;

		for (UINT j = 0; j < NumViews; j++)
		{
			ID3D11ShaderResourceView* pShaderResView = ppShaderResourceViews[j];
			if (pShaderResView)
			{
				pShaderResView->GetDesc(&Descr);

				if ((Descr.ViewDimension == D3D11_SRV_DIMENSION_BUFFER) || (Descr.ViewDimension == D3D11_SRV_DIMENSION_BUFFEREX))
				{
					continue; //Skip buffer resources
				}
			}
		}
	}

	void DXGICapture::DoClearRenderTargetView(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4])
	{

	}

	HRESULT WINAPI DXGICapture::Present(IDXGISwapChain* swap, UINT syncInterval, UINT flags)
	{
		DXGICapture* capture = DXGICapture::singleton();

		capture->DoPresent(swap, syncInterval, flags);

		return capture->present_hook.Call<HRESULT>(swap, syncInterval, flags);
	}

	HRESULT WINAPI DXGICapture::DrawIndex(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
	{
		DXGICapture* capture = DXGICapture::singleton();

		capture->DoDrawIndex(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);

		return capture->drawIndex_hook.Call<HRESULT>(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
	}

	HRESULT WINAPI DXGICapture::D3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery)
	{
		DXGICapture* capture = DXGICapture::singleton();

		capture->DoD3D11CreateQuery(pDevice, pQueryDesc, ppQuery);

		return capture->hook_D3D11CreateQuery.Call<HRESULT>(pDevice, pQueryDesc, ppQuery);
	}

	HRESULT WINAPI DXGICapture::D3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews)
	{
		DXGICapture* capture = DXGICapture::singleton();

		capture->DoD3D11PSSetShaderResources(pContext, StartSlot, NumViews, ppShaderResourceViews);

		return capture->hook_D3D11PSSetShaderResources.Call<HRESULT>(pContext, StartSlot, NumViews, ppShaderResourceViews);
	}

	HRESULT WINAPI DXGICapture::ClearRenderTargetView(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4])
	{
		DXGICapture* capture = DXGICapture::singleton();

		capture->DoClearRenderTargetView(pContext, pRenderTargetView, ColorRGBA);

		return capture->clearRenderTargetView_hook.Call<HRESULT>(pContext, pRenderTargetView, ColorRGBA);
	}

	HRESULT WINAPI DXGICapture::Present1(IDXGISwapChain* swap, UINT syncInterval, UINT flags, const DXGI_PRESENT_PARAMETERS* pPresentParameters)
	{
		DXGICapture* capture = DXGICapture::singleton();

		capture->DoPresent(swap, syncInterval, flags);

		return capture->present1_hook.Call<HRESULT>(swap, syncInterval, flags, pPresentParameters);
	}

	HRESULT WINAPI DXGICapture::ResizeBuffer(IDXGISwapChain* swap, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT giFormat, UINT flags)
	{
		DXGICapture* capture = DXGICapture::singleton();

		capture->DoResizeBuffer(swap, bufferCount, width, height, giFormat, flags);

		return DXGICapture::singleton()->resizebuffer_hook.Call<HRESULT>(swap, bufferCount, width, height, giFormat, flags);
	}

	HRESULT WINAPI DXGICapture::ResizeBuffer1(IDXGISwapChain* swap, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT giFormat, UINT flags, const UINT* pCreationNodeMask, IUnknown* const* ppPresentQueue)
	{
		DXGICapture* capture = DXGICapture::singleton();

		capture->DoResizeBuffer(swap, bufferCount, width, height, giFormat, flags);

		return DXGICapture::singleton()->resizebuffer1_hook.Call<HRESULT>(swap, bufferCount, width, height, giFormat, flags, pCreationNodeMask, ppPresentQueue);
	}

#if (WINVER >= 0x0A00 )   
	HRESULT WINAPI DXGICapture::ExecuteCommandLists(ID3D12CommandQueue* _this, UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists)
	{
		DXGICapture* capture = DXGICapture::singleton();
		/*
			D3D12_COMMAND_LIST_TYPE_DIRECT	= 0,
			D3D12_COMMAND_LIST_TYPE_BUNDLE	= 1,
			D3D12_COMMAND_LIST_TYPE_COMPUTE	= 2,
			D3D12_COMMAND_LIST_TYPE_COPY	= 3,
			D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE	= 4,
			D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS	= 5
			*/
		if (_this->GetDesc().Type == D3D12_COMMAND_LIST_TYPE_DIRECT)
			capture->lastCommandQueue = _this;
		HRESULT hr = capture->executecommandlists_hook.Call<HRESULT>(_this, NumCommandLists, ppCommandLists);

		return hr;
	}
#endif


	HRESULT Direct3D10SwapChain(HMODULE d3d10_dll, HWND hWnd, ComPtr<IDXGISwapChain>& swap_chain, ComPtr<ID3D10Device>& device)
	{
		if (d3d10_dll == nullptr)
		{
			throw std::runtime_error("Invalid 'd3d10.dll' module!");
		}

		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(swapDesc));
		swapDesc.BufferCount = 2;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.Width = 2;
		swapDesc.BufferDesc.Height = 2;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = hWnd;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.Windowed = TRUE;

		HRESULT hr = ModuleCall<HRESULT>(d3d10_dll, "D3D10CreateDeviceAndSwapChain", nullptr, D3D10_DRIVER_TYPE_NULL, nullptr, 0, D3D10_SDK_VERSION, &swapDesc, &swap_chain, &device);
		if (FAILED(hr))
		{
			//MessageBoxA(0 , "Failed to create device and swapchain.", "Fatal Error", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
		else if (SUCCEEDED(hr)) {
			//MessageBoxA(0 , "Device and Swapchain Created", "Success", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
		return hr;
	}

	HRESULT Direct3D11SwapChain(HMODULE d3d11_dll, HWND hWnd, ComPtr<IDXGISwapChain>& swap_chain, ComPtr<ID3D11DeviceContext>& context)
	{
		if (d3d11_dll == nullptr)
		{
			//MessageBoxA(0 , "Invalid 'd3d11.dll' module!", "Fatal Error", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
		/*
		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(swapDesc));
		swapDesc.BufferCount = 1;
		//swapDesc.BufferCount = 2;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.Width = rect.right - rect.left;
		swapDesc.BufferDesc.Height = rect.bottom - rect.top;
		//swapDesc.BufferDesc.Width = 1;
		//swapDesc.BufferDesc.Height = 1;
		//swapDesc.BufferDesc.Width = 2;
		//swapDesc.BufferDesc.Height = 2;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = hWnd;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.Windowed = TRUE;

		D3D_FEATURE_LEVEL desiredLevels[7] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};
		D3D_FEATURE_LEVEL receivedLevel;

		ComPtr<ID3D11Device> device;

		HRESULT hr = ModuleCall<HRESULT>(d3d11_dll, "D3D11CreateDeviceAndSwapChain", nullptr, D3D_DRIVER_TYPE_NULL, nullptr, 0, desiredLevels, 7, D3D11_SDK_VERSION, &swapDesc, &swap_chain, &device, &receivedLevel, &context);
		*/
		/*
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

		DXGI_RATIONAL refreshRate;
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = 100;
		bufferDesc.Height = 100;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ID3D11Device* device;
		HRESULT hr = ModuleCall<HRESULT>(d3d11_dll, "D3D11CreateDeviceAndSwapChain", NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swap_chain, &device, &featureLevel, &context);
		*/
		// define all the properties of our DXGI_SWAP_CHAIN_DESC:
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		//sd.BufferDesc.Width = 2;
		//sd.BufferDesc.Height = 2;
		sd.BufferDesc.Width = rect.right - rect.left;
		sd.BufferDesc.Height = rect.bottom - rect.top;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// It is important to define D3D_FEATURE_LEVEL with the value D3D_FEATURE_LEVEL_11_0.
		// This will define our DirectX target that we are going to hook. 
		// Everything is ready to call to D3D11CreateDeviceAndSwapChain, remember that we will need to manage the return value
		// to validate if everything worked and decide if we have to call our backup method or not:
		D3D_FEATURE_LEVEL FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
		UINT numFeatureLevelsRequested = 1;
		D3D_FEATURE_LEVEL FeatureLevelsSupported;

		HRESULT hr = ModuleCall<HRESULT>(d3d11_dll, "D3D11CreateDeviceAndSwapChain", NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevelsRequested, numFeatureLevelsRequested, D3D11_SDK_VERSION, &sd, &swap_chain, &pDevice, &FeatureLevelsSupported, &context);

		if (FAILED(hr))
		{
			//MessageBoxA(0 , "D3D11CreateDeviceAndSwapChain Failed.", "Fatal Error", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
		else {
			//MessageBoxA(0, "D3D11CreateDeviceAndSwapChain Success.", "Success", MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
		}
		return hr;
	}

#if (WINVER >= 0x0A00 )   
	/*HRESULT Direct3D12SwapChain(HMODULE d3d12_dll, HWND hWnd, ComPtr<IDXGISwapChain>& swapchain, ComPtr<ID3D12CommandQueue>& commandqueue, ComPtr<ID3D12GraphicsCommandList>& commandlist)
	{
		if (d3d12_dll == nullptr)
		{
			throw std::runtime_error("Invalid 'd3d12.dll' module!");
		}
		HRESULT hr;

		ComPtr<IDXGIFactory4> factory;
		hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
		if (FAILED(hr))
			return hr;

		ComPtr<IDXGIAdapter3> warpAdapter;
		hr = factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
		if (FAILED(hr))
			return hr;

		ComPtr<ID3D12Device> device;
		PFN_D3D12_CREATE_DEVICE pcreate = (PFN_D3D12_CREATE_DEVICE)GetProcAddress(d3d12_dll, "D3D12CreateDevice");
		if (pcreate == nullptr)
			throw Instalog::SystemFacades::Win32Exception::FromLastError();
		hr = pcreate(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
		if (FAILED(hr))
			return hr;

		ComPtr<ID3D12CommandQueue> command_queue;
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&command_queue));
		if (FAILED(hr))
			return hr;

		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(swapDesc));
		swapDesc.BufferCount = 2;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.Width = 2;
		swapDesc.BufferDesc.Height = 2;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapDesc.OutputWindow = hWnd;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.Windowed = TRUE;

		ComPtr<IDXGISwapChain> swap_chain;
		ComPtr<ID3D12GraphicsCommandList> command_list;
		ComPtr<ID3D12CommandAllocator> command_allocator;

		hr = factory->CreateSwapChain(command_queue.Get(), &swapDesc, &swap_chain);
		if (FAILED(hr))
			return hr;

		hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator));
		if (FAILED(hr))
			return hr;

		hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator.Get(), 0, IID_PPV_ARGS(&command_list));
		if (FAILED(hr))
			return hr;

		swapchain = swap_chain.Detach();
		commandlist = command_list.Detach();
		commandqueue = command_queue.Detach();

		return hr;
	}*/
#endif
	LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		return 0;
	}
}