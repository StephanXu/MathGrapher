#pragma once
#include "Dx.h"
#include "Elements2D.h"
#include <string>
#include <sstream>

using namespace std;

class CWidget
{
public:
	CWidget();
	~CWidget();
	
	unsigned long m_ulWidgetID;
	void SetID(unsigned long id);

	enum WIDGETTYPE
	{
		WIDGETTYPE_UNKNOWN,
		WIDGETTYPE_BUTTON,
		WIDGETTYPE_INPUTBOX,
		WIDGETTYPE_LABLE
	};
	WIDGETTYPE m_WidgetType;

	unsigned long m_left, m_top, m_right, m_bottom;

	enum EVENTTYPE
	{
		EVENTTYPE_LOAD,
		EVENTTYPE_MOUSE_COME,
		EVENTTYPE_MOUSE_LEAVE,
		EVENTTYPE_LEFTDOWN,
		EVENTTYPE_DBLEFTDOWN,
		EVENTTYPE_RIGHTDOWN,
		EVENTTYPE_DBRIGHTDOWN,
		EVENTTYPE_INPUT,
		EVENTTYPE_KEYDOWN
	};
	typedef int(*WidgetEvent)(EVENTTYPE ulEventType, void*, unsigned int);
	WidgetEvent m_pEventProc;
	void SetEventProc(WidgetEvent lpProc);
	
	virtual void Render();
	virtual void OnResetDevice();
};

class CButton : public CWidget
{
public:
	CButton();
	~CButton();
	CDxLine m_Line;
	void Render();
	void UpdateRect();
	void SetColor(D3DXCOLOR col);

	void OnResetDevice();
};

class CInputbox : public CWidget
{
public:
	CInputbox();
	~CInputbox();
	bool m_InputEnable;
	string m_Text;
	char m_newChar;
	CDxLine m_Line;
	CDxFont m_Font;
	unsigned long m_FontSize;
	D3DXCOLOR m_FontColor;

	void Render();
	void UpdateLine();
	void SetColor(D3DXCOLOR col);
	void SetLineColor(D3DXCOLOR col);
	void ApplyInput();
	void Backspace();
	bool SetFontSize(unsigned long FontSize);

	void OnResetDevice();
};

class CLable : public CWidget
{
public:
	CLable();
	~CLable();

	string m_Text;
	CDxFont m_Font;
	unsigned long m_FontSize;
	D3DXCOLOR m_FontColor;

	void Render();
	void SetTextColor(D3DXCOLOR col);
	void SetText(string text);
	bool SetFontSize(unsigned long FontSize);

	void OnResetDevice();

};