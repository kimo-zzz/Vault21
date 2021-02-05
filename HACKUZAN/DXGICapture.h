#pragma once
#include "stdafx.h"

namespace HACKUZAN {

	enum DXGI_TYPE
	{
		DIRECT3D10 = 0,
		DIRECT3D11,
		DIRECT3D12,
		DIRECT3D_UNKNOWN,
	};
	//class Renderer;
	class DXGICapture : public Capture
	{
		static HRESULT WINAPI Present(IDXGISwapChain* swap, UINT syncInterval, UINT flags);
		static HRESULT WINAPI DrawIndex(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
		static HRESULT WINAPI D3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);
		static HRESULT WINAPI D3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews);
		static HRESULT WINAPI ClearRenderTargetView(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

		static HRESULT WINAPI Present1(IDXGISwapChain* swap, UINT syncInterval, UINT flags, const DXGI_PRESENT_PARAMETERS* pPresentParameters);
		static HRESULT WINAPI ResizeBuffer(IDXGISwapChain* swap, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT giFormat, UINT flags);
		static HRESULT WINAPI ResizeBuffer1(IDXGISwapChain* swap, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT giFormat, UINT flags, const UINT* pCreationNodeMask, IUnknown* const* ppPresentQueue);
#if (WINVER >= 0x0A00 )   // DirectX 12 
		static HRESULT WINAPI ExecuteCommandLists(ID3D12CommandQueue* _this, UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists);
#endif
	protected:

		FuncHook present_hook;
		FuncHook drawIndex_hook;
		FuncHook hook_D3D11CreateQuery;
		FuncHook hook_D3D11PSSetShaderResources;
		FuncHook clearRenderTargetView_hook;

		FuncHook present1_hook;
		FuncHook resizebuffer_hook;
		FuncHook resizebuffer1_hook;

#if (WINVER >= 0x0A00 )   
		ID3D12CommandQueue* lastCommandQueue;
		FuncHook executecommandlists_hook;
#endif

		DXGI_TYPE type;

		void DoPresent(IDXGISwapChain* swap, UINT syncInterval, UINT flags);

		void DoDrawIndex(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
		void DoD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);
		void DoD3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews);
		void DoClearRenderTargetView(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

		void DoResizeBuffer(IDXGISwapChain* swap, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT giFormat, UINT flags);

		static std::unique_ptr<DXGICapture> capture;

		DXGICapture();

	public:
		virtual ~DXGICapture();
		virtual HMODULE GetCaptureModule();
		virtual HRESULT TryCapture();
		virtual HRESULT FreeCapture();

		static DXGICapture* singleton();
		static void destroy_singleton();
	};

}