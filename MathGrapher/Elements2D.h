#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")

#ifdef _UNICODE
#define PrintText PrintTextW
#else
#define PrintText PrintTextA
#endif 

class CElements2D
{
public:
	void SetDevice(IDirect3DDevice9* lpDevice);
protected:
	IDirect3DDevice9* m_pDevice;
};

class CDxFont : public CElements2D
{
public:
	//Initialize the font object.
	//Height: The size of the font
	//Width: keep zero or set the font's width.
	//lpFontFace: Font name. e.g:Consolas
	bool InitializeFont(int PointSize, LPCTSTR lpFontFace);
	
	//Release font object
	void ReleaseFont();

	//Print text
	bool PrintTextW(
		unsigned int x,
		unsigned int y,
		unsigned int Width,
		unsigned int Height,
		wchar_t* lpText,
		D3DXCOLOR Color);
	bool PrintTextA(
		unsigned int x,
		unsigned int y,
		unsigned int Width,
		unsigned int Height,
		char* lpText,
		D3DXCOLOR Color);
	bool ChangeFont(int FontSize, LPCTSTR lpFontFace);

	void OnResetDevice();
private:
	LPD3DXFONT m_Font;

};

class CDxLine : public CElements2D
{
public:
	//Init line object
	bool InitializeLine();
	//Release line object
	void ReleaseLine();
	//set vertex
	//lpVertex: a point to vertex array.
	//VertexNum: Vertex num
	bool SetVertex(D3DXVECTOR2* lpVertex, unsigned int VertexNum);
	//set line's color
	bool SetColor(D3DXCOLOR Color);
	//set line's weight
	void SetWidth(float width);

	bool Render();

	void OnResetDevice();

	D3DXCOLOR GetColor();

private:
	LPD3DXLINE m_Line;
	D3DXVECTOR2* m_Vertex;
	unsigned int m_VertexNum;
	D3DXCOLOR m_Color;
};