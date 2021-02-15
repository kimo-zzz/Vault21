#include "stdafx.h"
#include "VirtualTableManager.h"

namespace V21 {
	WORD VirtualTableManager::CalculateMethodCount() {
		IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)this->m_hModule;
		IMAGE_NT_HEADERS* pNtHeaders = (IMAGE_NT_HEADERS*)((DWORD)pDosHeader + pDosHeader->e_lfanew);
		IMAGE_SECTION_HEADER * pSectionHeader = (IMAGE_SECTION_HEADER*)((DWORD)pNtHeaders + sizeof(IMAGE_NT_HEADERS));

		DWORD dwTextStart = (DWORD)pDosHeader + (DWORD)pSectionHeader[0].VirtualAddress;
		DWORD dwTextEnd = dwTextStart + pSectionHeader[0].Misc.VirtualSize;

		WORD dwCount = 0;
		for (;; dwCount++) {
			auto ptr = this->m_pOriginalVTable[dwCount];
			if (ptr < dwTextStart || ptr >= dwTextEnd)
				break;
		}

		return dwCount;
	}

	VirtualTableManager::VirtualTableManager(void* pInstance, HMODULE hModule) {
		this->m_hModule = hModule;
		this->m_pInstance = (DWORD*)pInstance;
		this->m_pOriginalVTable = *(DWORD**)pInstance;
		this->m_iMethodCount = this->CalculateMethodCount();
		this->m_pNewVTable = new DWORD[this->m_iMethodCount];
		memcpy(this->m_pNewVTable, this->m_pOriginalVTable, this->m_iMethodCount * sizeof(DWORD));
		*(DWORD**)pInstance = this->m_pNewVTable;
	}

	VirtualTableManager::~VirtualTableManager() {
		*(DWORD**)this->m_pInstance = this->m_pOriginalVTable;
		delete[] this->m_pNewVTable;
	}

	DWORD VirtualTableManager::HookMethod(WORD index, void* hook) {
		this->m_pNewVTable[index] = (DWORD)hook;
		return this->m_pOriginalVTable[index];
	}
}