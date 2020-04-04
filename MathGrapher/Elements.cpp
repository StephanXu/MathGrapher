#include "stdafx.h"

#include "Elements.h"
#include <d3dx9.h>
#include <d3d9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//class CElements
void CElements::Render()
{
	return;
}

void CElements::Com_Render()
{
	//Apply the translation of the elements
	D3DXMATRIX matWorld, matWorldX, matWorldY, matWorldZ;
	D3DXMATRIX matTrans;
	// Create the transformation matrices
	D3DXMatrixRotationX(&matWorldX, m_rtX);
	D3DXMatrixRotationY(&matWorldY, m_rtY);
	D3DXMatrixRotationZ(&matWorldZ, m_rtZ);
	D3DXMatrixTranslation(&matTrans, m_trX, m_trY, m_trZ);
	// Combine the transformations by multiplying them together
	D3DXMatrixMultiply(&matWorld, &matWorldX, &matWorldY);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldZ);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
	// Apply the tansformation
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	return;
}

void CElements::Translate(float x, float y, float z)
{
	m_trX = x;
	m_trY = y;
	m_trZ = z;
	return;
}

void CElements::Rotation(float x, float y, float z)
{
	m_rtX = x;
	m_rtY = y;
	m_rtZ = z;
	return;
}

void CElements::SetDevice(IDirect3DDevice9* lpDevice)
{
	m_pDevice = lpDevice;
	return;
}


//class CCube

void CCube::Render()
{
	Com_Render();
	// Rendering object
	m_pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(CUSTOMVERTEX));
	m_pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 8);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 14, 2);
	return;
}

bool CCube::InitCube(
	float Length,
	float Width,
	float Height,
	D3DXCOLOR Color
)
{
	CUSTOMVERTEX cvVertices[] = {
		// Top face
		{ -Length / 2, Height / 2, -Width / 2, Color },	// Blue
		{ -Length / 2, Height / 2, Width / 2, Color },	// Red
		{ Length / 2, Height / 2, -Width / 2, Color },	// Red
		{ Length / 2, Height / 2, Width / 2, Color },		// Green

															// Face 1
		{ -Length / 2, -Height / 2, -Width / 2, Color },	// Red
		{ -Length / 2, Height / 2, -Width / 2, Color },	// Blue
		{ Length / 2, -Height / 2, -Width / 2, Color },	// Green
		{ Length / 2, Height / 2, -Width / 2, Color },	// Red

														// Face 2
		{ Length / 2, -Height / 2, Width / 2, Color },	// Blue
		{ Length / 2, Height / 2, Width / 2, Color },		// Green

															// Face 3
		{ -Length / 2, -Height / 2, Width / 2, Color },	// Green
		{ -Length / 2, Height / 2, Width / 2, Color },	// Red

														// Face 4
		{ -Length / 2, -Height / 2, -Width / 2, Color },	// Red
		{ -Length / 2, Height / 2, -Width / 2, Color },	// Blue

														// Bottom face
		{ Length / 2, -Height / 2, -Width / 2, Color },	// Green
		{ Length / 2, -Height / 2, Width / 2, Color },	// Blue
		{ -Length / 2, -Height / 2, -Width / 2, Color },	// Red
		{ -Length / 2, -Height / 2, Width / 2, Color },	// Green
	};
	// Create the vertex buffer from our device
	if (FAILED(m_pDevice->CreateVertexBuffer(18 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertex, NULL)))
	{
		return false;
	}
	// Get a pointer to the vertex buffer vertices and lock the vertex buffer
	void *pVertices;
	if (FAILED(m_pVertex->Lock(0, sizeof(cvVertices), (void **)&pVertices, 0)))
	{
		return false;
	}
	// Copy our stored vertices values into the vertex buffer
	memcpy(pVertices, cvVertices, sizeof(cvVertices));
	// Unlock the vertex buffer
	m_pVertex->Unlock();
	return true;
}

void CArrow::Render()
{
	Com_Render();
	m_pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(CUSTOMVERTEX));
	m_pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 13);
}

bool CArrow::InitArrow(float Length, float Width, D3DXCOLOR Color)
{
	CUSTOMVERTEX cvVertices[14] =
	{
		{ 0, 0, 0, Color },

		{ 0, Length, 0, Color },
		{ -Width,Length - Width, 0, Color },

		{ 0,Length - Width,-Width,Color },
		{ 0, Length, 0, Color },
		{ 0,Length - Width,-Width,Color },

		{ Width,Length - Width,0,Color },
		{ 0, Length, 0, Color },
		{ Width,Length - Width,0,Color },

		{ 0,Length - Width,Width,Color },
		{ 0, Length, 0, Color },
		{ 0,Length - Width,Width,Color },

		{ -Width,Length - Width, 0, Color },

		{ 0, Length, 0, Color },
	};
	void *pVertices{ nullptr };
	if (FAILED(m_pDevice->CreateVertexBuffer(14 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertex, 0)))
	{
		return false;
	}
	if (FAILED(m_pVertex->Lock(0, sizeof(cvVertices), (void **)&pVertices, 0)))
	{
		return false;
	}
	memcpy(pVertices, cvVertices, sizeof(cvVertices));
	// Unlock the vertex buffer
	m_pVertex->Unlock();
	
	return true;
}

CCustomEle::CCustomEle()
	:m_Vertex(nullptr)
	, m_VertexNum(0)
{
}

void CCustomEle::Render()
{
}

bool CCustomEle::InitEle(D3DXCOLOR Color)
{
	if (m_Vertex)
		delete m_Vertex;
	m_Vertex = new CUSTOMVERTEX[2];
	m_Vertex[0] = { 0,0,0,Color };
	m_Vertex[1] = { 0,0,0,Color };
	m_VertexNum = 2;
	void *pVertices{ nullptr };
	if (FAILED(m_pDevice->CreateVertexBuffer(m_VertexNum * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertex, 0)))
	{
		return false;
	}
	if (FAILED(m_pVertex->Lock(0, m_VertexNum * sizeof(CUSTOMVERTEX), (void **)&pVertices, 0)))
	{
		return false;
	}
	memcpy(pVertices, m_Vertex, m_VertexNum * sizeof(CUSTOMVERTEX));
	m_pVertex->Unlock();
	return false;
}

void CCustomEle::AddVertex(D3DXVECTOR3 Vertex)
{
	
}
