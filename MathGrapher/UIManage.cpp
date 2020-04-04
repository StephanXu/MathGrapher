#include "stdafx.h"
#include "UIManage.h"


CUIManage g_UIManager;

CUIManage::CUIManage()
	:m_MouseX(0)
	,m_MouseY(0)
	,m_hWnd(nullptr)
{
}


CUIManage::~CUIManage()
{
}

int CUIManage::RegWidget(CWidget* wid, unsigned long id)
{
	if (!m_hWnd)
		return -2;//无窗口
	if (m_Widgets.end() != m_Widgets.find(id))
	{
		return -1;//有相同id
	}
	m_Widgets.insert(pair<unsigned long, CWidget*>(id, wid));
	wid->SetID(id);
	SendMessage(m_hWnd, WM_WIDGETLOAD, (WPARAM)wid, 0);
	return 1;
}

int CUIManage::UnRegWidget(unsigned long id)
{
	map<unsigned long, CWidget*>::iterator SearchResult;
	SearchResult = m_Widgets.find(id);
	CWidget* pWidget = SearchResult->second;
	if (m_Widgets.end() == SearchResult)
	{
		return -1;//找不到id
	}
	m_Widgets.erase(id);
	if (pWidget->m_ulWidgetID != 0)
		delete pWidget;
	return 1;
}

int CUIManage::GetWidget(CWidget** ppWidget, unsigned long id)
{
	map<unsigned long, CWidget*>::iterator SearchResult;
	SearchResult = m_Widgets.find(id);
	if (SearchResult == m_Widgets.end())
		return -1;//找不到id
	*ppWidget = SearchResult->second;
	return 1;
}

bool CUIManage::IsInRect(CWidget * pWidget, unsigned long x, unsigned long y)
{
	if (x > pWidget->m_left && x < pWidget->m_right)
	{
		if (y > pWidget->m_top && y < pWidget->m_bottom)
		{
			return true;
		}
	}
	return false;
}

int CUIManage::ScanTarget(CWidget** ppWidget, unsigned long x, unsigned long y)
{
	map<unsigned long, CWidget*>::iterator it;
	for (it = m_Widgets.begin(); it != m_Widgets.end(); it++)
	{
		if (IsInRect(it->second, x, y))
		{
			*ppWidget = it->second;
			return 1;
		}
	}
	return 0;
}

void CUIManage::RenderUI()
{
	map<unsigned long, CWidget*>::iterator it;
	for (it = m_Widgets.begin(); it != m_Widgets.end(); it++)
	{
		it->second->Render();
	}
	return;
}

void CUIManage::OnResetDevice()
{
	for (auto it = m_Widgets.begin();
		it != m_Widgets.end();
		it++)
	{
		it->second->OnResetDevice();
	}
}
