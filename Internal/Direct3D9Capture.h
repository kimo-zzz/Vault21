#pragma once

class Direct3D9Capture : public Capture
{
	static HRESULT WINAPI EndScene(IDirect3DDevice9* device);
	static HRESULT WINAPI Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
protected:

	FuncHook endscene_hook;
	FuncHook reset_hook;

	void DoPresent(IDirect3DDevice9* device);
	void DoReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);

	static std::unique_ptr<Direct3D9Capture> capture;

	Direct3D9Capture();
public:
	virtual ~Direct3D9Capture();
	HMODULE GetCaptureModule() override;
	HRESULT TryCapture() override;
	HRESULT FreeCapture() override;

	static Direct3D9Capture* singleton();
	static void destroy_singleton();
};
