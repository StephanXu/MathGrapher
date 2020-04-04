#pragma once
#include <d3dx9.h>
#include <d3d9.h>

#include "Elements.h"
#include "Elements2D.h"
#include "Camera.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")



class CDx
{
public:
	CDx();
	~CDx();
	
	IDirect3D9* m_pD3D;
	IDirect3DDevice9* m_pD3DDevice;
	D3DPRESENT_PARAMETERS m_d3dpp;
	D3DDISPLAYMODE m_d3ddm;
	D3DCAPS9 m_d3ddc;

	CCamera m_cam;
	bool m_bResetFlag;

	bool m_b3dStudio;
};

extern CDx g_dx;