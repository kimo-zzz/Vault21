#include "stdafx.h"
#include "NetClient.h"
#include "Functions.h"

namespace V21
{
	NetClient* NetClient::Instance;

	int NetClient::GetPing()
	{
		return Functions::GetPing(this);
	}
}