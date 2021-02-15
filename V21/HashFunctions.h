#pragma once

namespace V21
{
	namespace HashFunctions
	{
		unsigned int HashStringCaseInsensitiveFNV1a(const char* str, unsigned int seed = 0x811C9DC5);
		unsigned int HashStringSDBM(const char* str);
		const char* TranslateString(const char* str);
	};
}