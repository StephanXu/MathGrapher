#include "stdafx.h"
#include "Dx.h"

CDx g_dx;


CDx::CDx()
	:m_pD3D(nullptr)
	,m_pD3DDevice(nullptr)
	,m_bResetFlag(false)
	,m_b3dStudio(false)
{
}


CDx::~CDx()
{
}

