#pragma once
#include "stdafx.h"
#include "NavGridCellFlags.h"


namespace V21 {

	class NavGridCell
	{
	public:
		DWORD				m_dwReserved;
		NavGridCellFlags	m_Flags;
	};
}