#pragma once
#include "stdafx.h"
#include "NavGridCellFlags.h"


namespace HACKUZAN {

	class NavGridCell
	{
	public:
		DWORD				m_dwReserved;
		NavGridCellFlags	m_Flags;
	};
}