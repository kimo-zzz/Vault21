#include "stdafx.h"

static const std::string GetSystemPath()
{
	char path[MAX_PATH];
	SHGetFolderPathA(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, path);
	return std::string(path);
}

const std::string Capture::SYSTEM_PATH = GetSystemPath();