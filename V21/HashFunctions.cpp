#include "stdafx.h"
#include "HashFunctions.h"
#include "Functions.h"

namespace V21
{
	unsigned int HashFunctions::HashStringCaseInsensitiveFNV1a(const char* str, unsigned int seed)
	{
		for (; *str; ++str)
		{
			seed = (tolower(*str) ^ seed) * 0x1000193;
		}
		return seed;
	}

	unsigned int HashFunctions::HashStringSDBM(const char* str)
	{
		unsigned int hash = 0;
		for (; *str; ++str)
		{
			hash = tolower(*str) + (hash << 6) + (hash << 16) - hash;
		}
		return hash;
	}

	const char* HashFunctions::TranslateString(const char* str)
	{
		return Functions::TranslateString(str);
	}

}