#include "stdafx.h"
#include "Studio3D.h"

CStudio3D g_studio3d;

CStudio3D::CStudio3D()
{
}


CStudio3D::~CStudio3D()
{
}

void CStudio3D::InitStudio()
{
	m_arrX.SetDevice(g_dx.m_pD3DDevice);
	m_arrY.SetDevice(g_dx.m_pD3DDevice);
	m_arrZ.SetDevice(g_dx.m_pD3DDevice);
	m_arrX.InitArrow(10, 0.2, D3DCOLOR_XRGB(255, 0, 0));
	m_arrY.InitArrow(10, 0.2, D3DCOLOR_XRGB(0, 255, 0));
	m_arrZ.InitArrow(10, 0.2, D3DCOLOR_XRGB(0, 0, 255));
	m_arrX.Rotation(0, 0, -D3DXToRadian(90));
	m_arrZ.Rotation(D3DXToRadian(90), 0, 0);

	return;
}

void CStudio3D::Render()
{
	m_arrX.Render();
	m_arrY.Render();
	m_arrZ.Render();
	for (auto it = m_Elements.begin();
		it!=m_Elements.end();
		it++)
	{
		(*it)->Render();
	}
	return;
}

void CStudio3D::RegElements(CElements * pEle)
{
	if (pEle)
		m_Elements.push_back(pEle);
}
