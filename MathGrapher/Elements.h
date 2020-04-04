#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//typedef void(*ElementAnimation)();

struct CUSTOMVERTEX
{
	float x, y, z;		// The transformed position for the vertex
	DWORD color;		// The vertex colour
};

class CElements
{
public:
	virtual void Render();
	virtual void Com_Render();
	virtual void Translate(float x, float y, float z);
	virtual void Rotation(float x, float y, float z);
	void SetDevice(IDirect3DDevice9* lpDevice);
	//void SetAppearAnimation(ElementAnimation lpAnimationProc);
	//void SetDisappearAnimation(ElementAnimation lpAnimationProc);
	//void SetAlwaysAnimation(ElementAnimation lpAnimationProc);

protected:
	//Translate coor
	float m_trX, m_trY, m_trZ;
	//Rotation coor
	float m_rtX, m_rtY, m_rtZ;
	//Vertex buffer
	IDirect3DVertexBuffer9* m_pVertex;
	IDirect3DDevice9* m_pDevice;
};

class CCube : public CElements
{
public:
	void Render();
	bool InitCube(
		float Length,
		float Width,
		float Height,
		D3DXCOLOR Color);
};

class CArrow : public CElements
{
public:
	void Render();
	bool InitArrow(
		float Length,
		float Width,
		D3DXCOLOR Color
	);

};

class CCustomEle : public CElements
{
public:
	CCustomEle();
	void Render();
	bool InitEle(D3DXCOLOR Color);
	void AddVertex(D3DXVECTOR3 Vertex);
	void RemoveVertex(D3DXVECTOR3 Vertex);

private:
	CUSTOMVERTEX* m_Vertex;
	unsigned long m_VertexNum;
};