#include "stdafx.h"

//#if (WINVER >= 0x0A00 )     

HRESULT Direct3D12SwapChain(HMODULE d3d12_dll, HWND hWnd, ComPtr<IDXGISwapChain>& swapchain, ComPtr<ID3D12CommandQueue>& commandqueue, ComPtr<ID3D12GraphicsCommandList>& commandlist);

Direct3D12Capture::Direct3D12Capture()
{
	lastCommandQueue = nullptr;
}

Direct3D12Capture::~Direct3D12Capture()
{
	FreeCapture();
}

HMODULE Direct3D12Capture::GetCaptureModule()
{
	static const std::string d3d12_dll = Capture::SYSTEM_PATH + "\\d3d12.dll";

	return GetModuleHandleA(d3d12_dll.c_str());
}

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

HRESULT Direct3D12Capture::TryCapture()
{
	HMODULE module = GetCaptureModule();

	if (module == nullptr)
	{
		return CO_E_DLLNOTFOUND;
	}
	HRESULT hr;

	ComPtr<IDXGISwapChain> swap_chain;
	ComPtr<ID3D12GraphicsCommandList> command_list;
	ComPtr<ID3D12CommandQueue> command_queue;
	
	static const LPTSTR TITLE = TEXT("BlastMenuV3Hook");
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, DLLWindowProc, 0L, 0L,
		g_hModule, NULL, NULL, NULL, NULL,
		TITLE, NULL
	};
	if (!RegisterClassEx(&wc))
		return E_FAIL;
	HWND hWnd = CreateWindow(TITLE, TITLE,
		WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
		NULL, NULL, wc.hInstance, NULL);

	hr = Direct3D12SwapChain(module, hWnd, swap_chain, command_queue, command_list);
	if (FAILED(hr))
		return hr;

	// May hook CommandList's DrawIndexInstanced here :P if you want

	uintptr_t *vtable = *(uintptr_t**)swap_chain.Get();
	present_hook = FuncHook(vtable[8], (uintptr_t)Present);
	resizebuffer_hook = FuncHook(vtable[13], (uintptr_t)ResizeBuffer);

	vtable = *(uintptr_t**)command_queue.Get();
	executecommandlists_hook = FuncHook(vtable[10], (uintptr_t)ExecuteCommandLists);

	present_hook.hook();
	resizebuffer_hook.hook();
	executecommandlists_hook.hook();

	command_list.Reset();
	command_queue.Reset();
	swap_chain.Reset();
	UnregisterClass(TITLE, wc.hInstance);

	return S_OK;
}

HRESULT Direct3D12Capture::FreeCapture()
{
	present_hook.unhook();
	resizebuffer_hook.unhook();
	executecommandlists_hook.unhook();
	return S_OK;
}

Direct3D12Capture* Direct3D12Capture::singleton()
{
	static std::unique_ptr<Direct3D12Capture> capture(new Direct3D12Capture());
	return capture.get();
}

HRESULT WINAPI Direct3D12Capture::Present(IDXGISwapChain *swap, UINT syncInterval, UINT flags)
{
	Direct3D12Capture* capture = Direct3D12Capture::singleton();

	D3D10_RECT rkt = { 0, 0, 100, 1000 };

	static Font fnt;

	static ComPtr<ID3D12Device> device;
	static ComPtr<IDXGISwapChain3> swap_chain;
	static ComPtr<ID3D12CommandQueue> command_queue;
	static bool santa = false;
	static std::unique_ptr<Renderer> renderer;

	if (!santa && capture->lastCommandQueue != nullptr)
	{
		Instalog::SystemFacades::ThrowIfFailed(swap->QueryInterface(IID_PPV_ARGS(&swap_chain)));
		Instalog::SystemFacades::ThrowIfFailed(swap->GetDevice(IID_PPV_ARGS(&device)));
		command_queue.Attach(capture->lastCommandQueue);
		renderer = std::make_unique<Direct3D12Renderer>(std::move(swap_chain), std::move(command_queue));

		santa = true;
	}
	FPSCheck(framerate);

	if (renderer)
	{
		if (renderer->Begin())
		{
			g_menu.Render(renderer.get());

			renderer->End();
		}
		renderer->Present();
	}
	HRESULT hr = capture->present_hook.Call<HRESULT>(swap, syncInterval, flags);

	return hr;
}

HRESULT WINAPI Direct3D12Capture::ResizeBuffer(IDXGISwapChain *swap, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT giFormat, UINT flags)
{
	HRESULT hr = Direct3D12Capture::singleton()->resizebuffer_hook.Call<HRESULT>(swap, bufferCount, width, height, giFormat, flags);

	return hr;
}

HRESULT WINAPI Direct3D12Capture::ExecuteCommandLists(ID3D12CommandQueue *_this, UINT NumCommandLists, ID3D12CommandList *const *ppCommandLists)
{
	Direct3D12Capture* capture = Direct3D12Capture::singleton();
	capture->lastCommandQueue = _this;
	HRESULT hr = capture->executecommandlists_hook.Call<HRESULT>(_this, NumCommandLists, ppCommandLists);

	return hr;
}

typedef struct
{
	DWORD process_id;
	HWND handle_window;
	BOOL success;
} EnumData;

BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam)
{
	EnumData& ed = *reinterpret_cast<EnumData*>(lParam);

	DWORD procId;
	GetWindowThreadProcessId(hWnd, &procId);

	if (ed.process_id == procId)
	{
		ed.handle_window = hWnd;
		ed.success =  TRUE;
		return FALSE;
	}
	ed.success = FALSE;
	return TRUE;
}

HWND FindHWND()
{
	EnumData ed;
	ed.success = FALSE;
	ed.process_id = GetCurrentProcessId();
	if (!EnumWindows(EnumProc, (LPARAM)&ed)
		&& ed.success == TRUE)
		return ed.handle_window;

	return GetForegroundWindow();
}

HRESULT Direct3D12SwapChain(HMODULE d3d12_dll, HWND hWnd, ComPtr<IDXGISwapChain>& swapchain, ComPtr<ID3D12CommandQueue>& commandqueue, ComPtr<ID3D12GraphicsCommandList>& commandlist)
{
	if (d3d12_dll == nullptr)
	{
		throw std::runtime_error("Invalid 'd3d12.dll' module!");
	}
	HRESULT hr;

#ifdef _DEBUG
	{/*
		try
		{
			ComPtr<ID3D12Debug> debug;
			hr = ModuleCall<HRESULT>(d3d12_dll, "D3D12GetDebugInterface", IID_PPV_ARGS(&debug));
			if (SUCCEEDED(hr))
				debug->EnableDebugLayer();
		}
		catch (std::exception&) {}*/
	}
#endif
	ComPtr<IDXGIFactory4> factory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
		return hr;

	ComPtr<IDXGIAdapter3> warpAdapter;
	hr = factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
	if (FAILED(hr))
		return hr;

	ComPtr<ID3D12Device> device;
	hr = ModuleCall<HRESULT>(d3d12_dll, "D3D12CreateDevice", warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
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
}

//#endif