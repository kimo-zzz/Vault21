#pragma once

class Capture
{
public:
	virtual ~Capture() {}

	virtual HMODULE GetCaptureModule() = 0;
	virtual HRESULT TryCapture() = 0;
	virtual HRESULT FreeCapture() = 0;

	static const std::string SYSTEM_PATH;
};