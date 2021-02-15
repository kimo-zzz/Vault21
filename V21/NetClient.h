#pragma once

namespace V21
{
	class NetClient
	{
	public:
		static NetClient* Instance;

		int GetPing();
	};
}