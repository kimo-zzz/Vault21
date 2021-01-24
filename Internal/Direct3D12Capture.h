#pragma once

class Direct3D12Capture : public Capture
{
	static HRESULT WINAPI Present(IDXGISwapChain *swap, UINT syncInterval, UINT flags);
	static HRESULT WINAPI ResizeBuffer(IDXGISwapChain *swap, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT giFormat, UINT flags);
	static HRESULT WINAPI ExecuteCommandLists(ID3D12CommandQueue *_this, UINT NumCommandLists, ID3D12CommandList *const *ppCommandLists);
protected:
	FuncHook present_hook;
	FuncHook resizebuffer_hook;
	FuncHook executecommandlists_hook;
	ID3D12CommandQueue* lastCommandQueue;


	Direct3D12Capture();
public:
	virtual ~Direct3D12Capture();
	virtual HMODULE GetCaptureModule();
	virtual HRESULT TryCapture();
	virtual HRESULT FreeCapture();

	static Direct3D12Capture* singleton();
};
