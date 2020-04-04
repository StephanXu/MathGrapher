#include "stdafx.h"
#include "Widget.h"


CWidget::CWidget()
	:m_WidgetType(WIDGETTYPE_UNKNOWN)
	,m_left(0)
	,m_top(0)
	,m_right(0)
	,m_bottom(0)
	,m_pEventProc(nullptr)
{
}


CWidget::~CWidget()
{
}

void CWidget::SetID(unsigned long id)
{
	m_ulWidgetID = id;
}

void CWidget::SetEventProc(WidgetEvent lpProc)
{
	m_pEventProc = lpProc;
}

void CWidget::Render()
{
	return;
}

void CWidget::OnResetDevice()
{
	return;
}

CButton::CButton()
{
	m_Line.InitializeLine();
	m_WidgetType = WIDGETTYPE_BUTTON;
}

CButton::~CButton()
{
	m_Line.ReleaseLine();
}

void CButton::Render()
{
	m_Line.Render();
}

void CButton::UpdateRect()
{
	m_Line.SetDevice(g_dx.m_pD3DDevice);
	m_Line.InitializeLine();
	D3DXVECTOR2 vec[5] =
	{
		D3DXVECTOR2((float)m_left,(float)m_top),
		D3DXVECTOR2((float)m_right,(float)m_top),
		D3DXVECTOR2((float)m_right,(float)m_bottom),
		D3DXVECTOR2((float)m_left,(float)m_bottom),
		D3DXVECTOR2((float)m_left,(float)m_top)
	};
	m_Line.SetVertex(vec, 5);
	return;
}

void CButton::SetColor(D3DXCOLOR col)
{
	m_Line.SetColor(col);
}

void CButton::OnResetDevice()
{
	m_Line.OnResetDevice();
}

CInputbox::CInputbox()
	:m_InputEnable(false)
	,m_FontSize(0)
	,m_newChar(0)

{
	m_Line.SetDevice(g_dx.m_pD3DDevice);
	m_Font.SetDevice(g_dx.m_pD3DDevice);
	m_Line.InitializeLine();
	m_Font.InitializeFont(m_FontSize, L"Î¢ÈíÑÅºÚ");
	m_Line.SetColor(D3DCOLOR_XRGB(0, 0, 0));
}

CInputbox::~CInputbox()
{
	m_Line.ReleaseLine();
	m_Font.ReleaseFont();
}

void CInputbox::Render()
{
	m_Line.Render();
	m_Font.PrintTextA(m_left, m_top, m_right - m_left, m_bottom - m_top, (char*)m_Text.c_str(), m_FontColor);
	return;
}

void CInputbox::UpdateLine()
{
	D3DXVECTOR2 vec[2];
	vec[0] = D3DXVECTOR2((float)m_left, (float)m_bottom);
	vec[1] = D3DXVECTOR2((float)m_right, (float)m_bottom);
	m_Line.SetVertex(vec, 2);
}

void CInputbox::SetColor(D3DXCOLOR col)
{
	m_FontColor = col;
}

void CInputbox::SetLineColor(D3DXCOLOR col)
{
	m_Line.SetColor(col);
}

void CInputbox::ApplyInput()
{
	m_Text += m_newChar;
}

void CInputbox::Backspace()
{
	m_Text = m_Text.substr(0, m_Text.length() - 1);
}

bool CInputbox::SetFontSize(unsigned long FontSize)
{
	if (m_Font.ChangeFont(FontSize, L"Î¢ÈíÑÅºÚ"))
	{
		m_FontSize = FontSize;
		return true;
	}
	return false;
}

void CInputbox::OnResetDevice()
{
	m_Line.OnResetDevice();
	m_Font.OnResetDevice();
}

CLable::CLable()
{
	m_Font.SetDevice(g_dx.m_pD3DDevice);
	m_Font.InitializeFont(
		m_FontSize,
		L"Î¢ÈíÑÅºÚ"
	);
}

CLable::~CLable()
{
	m_Font.ReleaseFont();
}

void CLable::Render()
{
	m_Font.PrintTextA(m_left, m_top, m_right - m_left, m_bottom - m_top, (char*)m_Text.c_str(), m_FontColor);
}

void CLable::SetTextColor(D3DXCOLOR col)
{
	m_FontColor = col;
}

void CLable::SetText(string text)
{
	m_Text = text;
}

bool CLable::SetFontSize(unsigned long FontSize)
{
	if (m_Font.ChangeFont(FontSize, L"Î¢ÈíÑÅºÚ"))
	{
		m_FontSize = FontSize;
		return true;
	}
	return false;
}

void CLable::OnResetDevice()
{
	m_Font.OnResetDevice();
	return;
}
