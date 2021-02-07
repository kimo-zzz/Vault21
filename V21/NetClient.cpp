#include "stdafx.h"
#include "NetClient.h"
#include "Functions.h"

namespace HACKUZAN
{
	NetClient* NetClient::Instance;

	int NetClient::GetPing()
	{
		return Functions::GetPing(this);
	}
}