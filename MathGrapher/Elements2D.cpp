#include "stdafx.h"

#include "Elements2D.h"
#include <d3dx9.h>
#include <d3d9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


//class CElements2D
void CElements2D::SetDevice(IDirect3DDevice9* lpDevice)
{
	m_pDevice = lpDevice;
	return;
}

//class CDxFont
bool CDxFont::InitializeFont(int PointSize, LPCTSTR lpFontFace)
{
	if (!m_pDevice)
		return false;
	if (m_Font)
		return false;
	if (FAILED(D3DXCreateFont(
		m_pDevice,
		-MulDiv(PointSize, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72), 0,   // �����ַ��Ŀ��
		0,       // �Ƿ�Ӵ�
		1,       // Mipmap����
		0,       // �Ƿ�Ϊб��
		DEFAULT_CHARSET,        // ����Ĭ���ַ���
		OUT_DEFAULT_PRECIS,     // ������ȣ�ʹ��Ĭ��ֵ
		PROOF_QUALITY,        // �ı�����
		DEFAULT_PITCH | FF_DONTCARE,
		lpFontFace,   // ������������
		&m_Font     // Ҫ����LPD3DXFONT�������
	)))
		return false;
	return true;
}

void CDxFont::ReleaseFont()
{
	if (!m_Font)
		return;
	m_Font->Release();
}

bool CDxFont::PrintTextW(unsigned int x, unsigned int y,unsigned int Width, unsigned int Height, wchar_t * lpText, D3DXCOLOR Color)
{
	if (!m_Font)
		return false;
	RECT Position;
	Position.left = x;
	Position.top = y;
	Position.right = x + Width;
	Position.bottom = y + Height;
	m_Font->DrawTextW(NULL, lpText,
		-1, &Position, DT_LEFT,
		Color);
	return true;
}

bool CDxFont::PrintTextA(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height, char * lpText, D3DXCOLOR Color)
{
	if (!m_Font)
		return false;
	RECT Position;
	Position.left = x;
	Position.top = y;
	Position.right = x + Width;
	Position.bottom = y + Height;
	m_Font->DrawTextA(NULL, lpText,
		-1, &Position, DT_LEFT,
		Color);
	return true;
}

bool CDxFont::ChangeFont(int FontSize, LPCTSTR lpFontFace)
{
	LPD3DXFONT pOldFont{ m_Font };
	if (FAILED(D3DXCreateFont(
		m_pDevice,
		-MulDiv(FontSize, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72), 0,   // �����ַ��Ŀ��
		0,       // �Ƿ�Ӵ�
		1,       // Mipmap����
		0,       // �Ƿ�Ϊб��
		DEFAULT_CHARSET,        // ����Ĭ���ַ���
		OUT_DEFAULT_PRECIS,     // ������ȣ�ʹ��Ĭ��ֵ
		DEFAULT_QUALITY,        // �ı�����
		DEFAULT_PITCH | FF_DONTCARE,
		lpFontFace,   // ������������
		&m_Font     // Ҫ����LPD3DXFONT�������
	)))
		return false;
	pOldFont->Release();
	return true;
}

void CDxFont::OnResetDevice()
{
	m_Font->OnLostDevice();
	m_Font->OnResetDevice();
	return;
}

//class CDXLine

bool CDxLine::InitializeLine()
{
	if (!m_pDevice)
		return false;
	if (m_Line)
		ReleaseLine();
	if (FAILED(D3DXCreateLine(m_pDevice, &m_Line)))
	{
		return false;
	}
	m_Line->SetAntialias(TRUE);
	return true;
}

void CDxLine::ReleaseLine()
{
	if (!m_Line)
		return;
	m_Line->Release();
}

bool CDxLine::SetVertex(D3DXVECTOR2 * lpVertex, unsigned int VertexNum)
{
	if (m_Vertex)
		delete[] m_Vertex;
	m_Vertex = new D3DXVECTOR2[VertexNum];
	memcpy_s(m_Vertex, sizeof(D3DXVECTOR2)*VertexNum, lpVertex, sizeof(D3DXVECTOR2)*VertexNum);
	m_VertexNum = VertexNum;
	return true;
}

bool CDxLine::SetColor(D3DXCOLOR Color)
{
	m_Color = Color;
	return true;
}

void CDxLine::SetWidth(float width)
{
	m_Line->SetWidth(width);
	return;
}

bool CDxLine::Render()
{
	if (!m_pDevice)
		return false;
	if (!m_Vertex)
		return false;
	if (!m_Line)
		return false;

	m_Line->Begin();
	m_Line->Draw(m_Vertex, m_VertexNum, m_Color);
	m_Line->End();
	return false;
}

void CDxLine::OnResetDevice()
{
	m_Line->OnLostDevice();
	m_Line->OnResetDevice();
	return;
}

D3DXCOLOR CDxLine::GetColor()
{

	return m_Color;
}
