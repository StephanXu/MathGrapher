#pragma once

#include "Widget.h"
#include <map>

using namespace std;

#define WM_WIDGETLOAD 0x5001

class CUIManage
{
public:
	CUIManage();
	~CUIManage();

	unsigned long m_MouseX, m_MouseY;
	HWND m_hWnd;

	map<unsigned long, CWidget*> m_Widgets;
	int RegWidget(CWidget* wid, unsigned long id);
	int UnRegWidget(unsigned long id);
	int GetWidget(CWidget** ppWidget, unsigned long id);
	bool IsInRect(CWidget* pWidget, unsigned long x, unsigned long y);
	int ScanTarget(CWidget** ppWidget, unsigned long x, unsigned long y);

	void RenderUI();

	void OnResetDevice();
};

extern CUIManage g_UIManager;