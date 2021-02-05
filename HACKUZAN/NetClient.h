#pragma once

namespace HACKUZAN
{
	class NetClient
	{
	public:
		static NetClient* Instance;

		int GetPing();
	};
}