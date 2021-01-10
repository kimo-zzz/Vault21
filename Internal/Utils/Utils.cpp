#include "Utils.h"
namespace Utils {
	DWORD GetDeviceAddress(int VTableIndex) {
		PDWORD VTable = nullptr;
		DWORD dwObjBase = NULL;

		dwObjBase = (DWORD)LoadLibrary("d3d9.dll");
		while (dwObjBase++ < dwObjBase + 0x128000)
		{
			if ((*(WORD*)(dwObjBase + 0x00)) == 0x06C7
				&& (*(WORD*)(dwObjBase + 0x06)) == 0x8689
				&& (*(WORD*)(dwObjBase + 0x0C)) == 0x8689
				) {
				dwObjBase += 2; break;
			}
		}

		*(DWORD*)&VTable = *(DWORD*)dwObjBase;
		return VTable[VTableIndex];
	}
}