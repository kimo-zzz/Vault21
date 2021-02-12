#pragma once

#include "Globals.h"

namespace V21 {
	class VirtualTableManager {
	private:
		HMODULE m_hModule;
		DWORD* m_pInstance;
		WORD m_iMethodCount;
		DWORD* m_pOriginalVTable;
		DWORD* m_pNewVTable = nullptr;
		WORD CalculateMethodCount();
	public:
		VirtualTableManager(void* pInstance, HMODULE hModule = Globals::MainModule);
		~VirtualTableManager();

		DWORD HookMethod(WORD index, void* hook);
	};
}