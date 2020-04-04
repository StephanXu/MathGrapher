#include "stdafx.h"
#include "Studio2D.h"
#include <process.h>

CStudio2D g_studio2d;

CStudio2D::CStudio2D()
	:m_dbScale(0)
	,m_ulNumOfPoint(0)
	,m_dbUnit(1)
	,m_fUnitSize(50)
	,m_cfMaxSize(100)
	,m_cfMinSize(50)
	,m_dbXCamOffset(0)
	,m_dbYCamOffset(0)
	,m_lCenterX(0)
	,m_lCenterY(0)
	,m_fXCurrentMaxValue(0)
	,m_fXCurrentMinValue(0)
	,m_fYCurrentMaxValue(0)
	,m_fYCurrentMinValue(0)
	,m_colCount(0)
{
}


CStudio2D::~CStudio2D()
{
}

void CStudio2D::InitStudio()
{
	m_SurfaceX1.SetDevice(g_dx.m_pD3DDevice);
	m_SurfaceY1.SetDevice(g_dx.m_pD3DDevice);
	m_SurfaceX2.SetDevice(g_dx.m_pD3DDevice);
	m_SurfaceY2.SetDevice(g_dx.m_pD3DDevice);
	m_AideX.SetDevice(g_dx.m_pD3DDevice);
	m_AideY.SetDevice(g_dx.m_pD3DDevice);
	m_SurfaceX1.InitializeLine();
	m_SurfaceY1.InitializeLine();
	m_SurfaceX2.InitializeLine();
	m_SurfaceY2.InitializeLine();
	m_AideX.InitializeLine();
	m_AideY.InitializeLine();
	m_SurfaceX1.SetColor(D3DCOLOR_XRGB(0, 0, 0));
	m_SurfaceY1.SetColor(D3DCOLOR_XRGB(0, 0, 0));
	m_SurfaceX2.SetColor(D3DCOLOR_XRGB(0, 0, 0));
	m_SurfaceY2.SetColor(D3DCOLOR_XRGB(0, 0, 0));
	m_AideX.SetColor(D3DCOLOR_XRGB(0xd9, 0xd9, 0xd9));
	m_AideY.SetColor(D3DCOLOR_XRGB(0xd9, 0xd9, 0xd9));
	m_lCenterX = g_dx.m_d3dpp.BackBufferWidth / 2;
	m_lCenterY = g_dx.m_d3dpp.BackBufferHeight / 2;

	m_Font.SetDevice(g_dx.m_pD3DDevice);
	m_Font.InitializeFont(9, L"Î¢ÈíÑÅºÚ");
	return;
}

void CStudio2D::Render()
{
	m_AideX.Render();
	m_AideY.Render();
	m_SurfaceX1.Render();
	m_SurfaceY1.Render();
	m_SurfaceX2.Render();
	m_SurfaceY2.Render();
	for (auto it = m_CaliX.begin();
		it != m_CaliX.end();
		it++)
	{
		m_Font.PrintText(it->m_lPx, it->m_lPy, 100, 100, (wchar_t*)it->m_wsText.c_str(), D3DCOLOR_XRGB(0, 0, 0));
	}
	for (auto it = m_CaliY.begin();
		it != m_CaliY.end();
		it++)
	{
		m_Font.PrintText(it->m_lPx, it->m_lPy, 100, 100, (wchar_t*)it->m_wsText.c_str(), D3DCOLOR_XRGB(0, 0, 0));
	}
	m_Font.PrintText(m_lCenterX - 10, m_lCenterY, 100, 100, L"0", D3DCOLOR_XRGB(0, 0, 0));

	for (vector<CGraphic*>::iterator it = m_Graphics.begin();
		it != m_Graphics.end();
		it++)
	{
		(*it)->Render();
	}
}

void CStudio2D::UpdateSurfaceXOYLine()
{
	static D3DXVECTOR2 SurfaceX1Vertex[1024]{};
	static D3DXVECTOR2 SurfaceX2Vertex[1024]{};
	static D3DXVECTOR2 SurfaceY1Vertex[1024]{};
	static D3DXVECTOR2 SurfaceY2Vertex[1024]{};
	static D3DXVECTOR2 AideX[1024]{};
	static D3DXVECTOR2 AideY[1024]{};
	unsigned long ulNumOfXAideVertex{};
	unsigned long ulNumOfYAideVertex{};
	unsigned long ulNumOfX1Vertex{};
	unsigned long ulNumOfX2Vertex{};
	unsigned long ulNumOfY1Vertex{};
	unsigned long ulNumOfY2Vertex{};
	unsigned long ulNumOfX1Skip{};
	unsigned long ulNumOfX2Skip{};
	unsigned long ulNumOfY1Skip{};
	unsigned long ulNumOfY2Skip{};

	m_CaliX.clear();
	m_CaliY.clear();

	for (long iVertex = m_lCenterX;
		iVertex <= (long)g_dx.m_d3dpp.BackBufferWidth;
		iVertex += (long)m_fUnitSize)
	{
		if (iVertex > 0 - m_fUnitSize && iVertex <= (long)g_dx.m_d3dpp.BackBufferWidth + m_fUnitSize)
		{
			SurfaceX1Vertex[ulNumOfX1Vertex++] = D3DXVECTOR2((float)iVertex, m_lCenterY + m_dbXCamOffset);
			SurfaceX1Vertex[ulNumOfX1Vertex++] = D3DXVECTOR2((float)iVertex, m_lCenterY + m_dbXCamOffset - 2);
			SurfaceX1Vertex[ulNumOfX1Vertex++] = D3DXVECTOR2((float)iVertex, m_lCenterY + m_dbXCamOffset + 2);
			SurfaceX1Vertex[ulNumOfX1Vertex++] = D3DXVECTOR2((float)iVertex, m_lCenterY + m_dbXCamOffset);
			if (ulNumOfX1Vertex / 4 - 1 > 0)
			{
				if (m_dbXCamOffset >= 0)
					m_CaliX.push_back(CCali((ulNumOfX1Vertex / 4 - 1 + ulNumOfX1Skip)*m_dbUnit, iVertex, m_lCenterY + (long)m_dbXCamOffset, CCali::NP_BOTTOM));
				else
					m_CaliX.push_back(CCali((ulNumOfX1Vertex / 4 - 1 + ulNumOfX1Skip)*m_dbUnit, iVertex, m_lCenterY + (long)m_dbXCamOffset, CCali::NP_TOP));
			}
			AideX[ulNumOfXAideVertex++] = D3DXVECTOR2((float)iVertex, -1);
			AideX[ulNumOfXAideVertex++] = D3DXVECTOR2((float)iVertex, (float)g_dx.m_d3dpp.BackBufferHeight);
			AideX[ulNumOfXAideVertex++] = D3DXVECTOR2((float)iVertex, -1);
		}
		else
			ulNumOfX1Skip++;
	}
	SurfaceX1Vertex[ulNumOfX1Vertex] = D3DXVECTOR2((float)g_dx.m_d3dpp.BackBufferWidth, m_lCenterY + m_dbXCamOffset);
	ulNumOfX1Vertex++;
	
	//x1

	for (long iVertex = m_lCenterY;
		iVertex <= (long)g_dx.m_d3dpp.BackBufferHeight;
		iVertex += (long)m_fUnitSize)
	{
		if (iVertex > 0 - m_fUnitSize && iVertex <= (long)g_dx.m_d3dpp.BackBufferHeight + m_fUnitSize)
		{
			SurfaceY2Vertex[ulNumOfY2Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset, (float)iVertex);
			SurfaceY2Vertex[ulNumOfY2Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset + 2, (float)iVertex);
			SurfaceY2Vertex[ulNumOfY2Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset - 2, (float)iVertex);
			SurfaceY2Vertex[ulNumOfY2Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset, (float)iVertex);
			if (ulNumOfY2Vertex / 4 - 1 > 0)
			{
				if (m_dbYCamOffset <= 0)
					m_CaliY.push_back(CCali(0 - (ulNumOfY2Vertex / 4 - 1 + ulNumOfY2Skip)*m_dbUnit, m_lCenterX + (long)m_dbYCamOffset, iVertex, CCali::NP_LEFT));
				else
					m_CaliY.push_back(CCali(0 - (ulNumOfY2Vertex / 4 - 1 + ulNumOfY2Skip)*m_dbUnit, m_lCenterX + (long)m_dbYCamOffset, iVertex, CCali::NP_RIGHT));
			}
			AideY[ulNumOfYAideVertex++] = D3DXVECTOR2((float)-1, (float)iVertex);
			AideY[ulNumOfYAideVertex++] = D3DXVECTOR2((float)g_dx.m_d3dpp.BackBufferWidth, (float)iVertex);
			AideY[ulNumOfYAideVertex++] = D3DXVECTOR2((float)-1, (float)iVertex);
		}
		else
			ulNumOfY2Skip++;
	}
	SurfaceY2Vertex[ulNumOfY2Vertex] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset, (float)g_dx.m_d3dpp.BackBufferHeight);
	ulNumOfY2Vertex++;
	//y2

	for (long iVertex = m_lCenterX;
		iVertex >= 0;
		iVertex -= (long)m_fUnitSize)
	{
		if (iVertex > 0 - m_fUnitSize && iVertex <= (long)g_dx.m_d3dpp.BackBufferWidth + m_fUnitSize)
		{
			SurfaceX2Vertex[ulNumOfX2Vertex++] = D3DXVECTOR2((float)iVertex, m_lCenterY + m_dbXCamOffset);
			SurfaceX2Vertex[ulNumOfX2Vertex++] = D3DXVECTOR2((float)iVertex, m_lCenterY + m_dbXCamOffset - 2);
			SurfaceX2Vertex[ulNumOfX2Vertex++] = D3DXVECTOR2((float)iVertex, m_lCenterY + m_dbXCamOffset + 2);
			SurfaceX2Vertex[ulNumOfX2Vertex++] = D3DXVECTOR2((float)iVertex, m_lCenterY + m_dbXCamOffset);
			if (ulNumOfX2Vertex / 4 - 1 > 0)
			{
				if (m_dbXCamOffset >= 0)
					m_CaliX.push_back(CCali(0 - (ulNumOfX2Vertex / 4 - 1 + ulNumOfX2Skip)*m_dbUnit, iVertex, m_lCenterY + (long)m_dbXCamOffset, CCali::NP_BOTTOM));
				else
					m_CaliX.push_back(CCali(0 - (ulNumOfX2Vertex / 4 - 1 + ulNumOfX2Skip)*m_dbUnit, iVertex, m_lCenterY + (long)m_dbXCamOffset, CCali::NP_TOP));
			}
			AideX[ulNumOfXAideVertex++] = D3DXVECTOR2((float)iVertex, -1);
			AideX[ulNumOfXAideVertex++] = D3DXVECTOR2((float)iVertex, (float)g_dx.m_d3dpp.BackBufferHeight);
			AideX[ulNumOfXAideVertex++] = D3DXVECTOR2((float)iVertex, -1);
		}
		else
			ulNumOfX2Skip++;
	}
	SurfaceX2Vertex[ulNumOfX2Vertex] = D3DXVECTOR2((float)0, m_lCenterY + m_dbXCamOffset);
	ulNumOfX2Vertex++;
	//x2

	for (long iVertex = m_lCenterY;
		iVertex >= 0;
		iVertex -= (long)m_fUnitSize)
	{
		if (iVertex > 0 - m_fUnitSize && iVertex <= (long)g_dx.m_d3dpp.BackBufferHeight + m_fUnitSize)
		{
			SurfaceY1Vertex[ulNumOfY1Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset, (float)iVertex);
			SurfaceY1Vertex[ulNumOfY1Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset + 2, (float)iVertex);
			SurfaceY1Vertex[ulNumOfY1Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset - 2, (float)iVertex);
			SurfaceY1Vertex[ulNumOfY1Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset, (float)iVertex);
			if (ulNumOfY1Vertex / 4 - 1 > 0)
			{
				if (m_dbYCamOffset <= 0)
					m_CaliY.push_back(CCali((ulNumOfY1Vertex / 4 - 1 + ulNumOfY1Skip)*m_dbUnit, m_lCenterX + (long)m_dbYCamOffset, iVertex, CCali::NP_LEFT));
				else
					m_CaliY.push_back(CCali((ulNumOfY1Vertex / 4 - 1 + ulNumOfY1Skip)*m_dbUnit, m_lCenterX + (long)m_dbYCamOffset, iVertex, CCali::NP_RIGHT));
			}
			AideY[ulNumOfYAideVertex++] = D3DXVECTOR2((float)-1, (float)iVertex);
			AideY[ulNumOfYAideVertex++] = D3DXVECTOR2((float)g_dx.m_d3dpp.BackBufferWidth, (float)iVertex);
			AideY[ulNumOfYAideVertex++] = D3DXVECTOR2((float)-1, (float)iVertex);
		}
		else
			ulNumOfY1Skip++;
	}
	SurfaceY1Vertex[ulNumOfY1Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset, 0);
	//y1

	SurfaceX1Vertex[ulNumOfX1Vertex++] = D3DXVECTOR2((float)g_dx.m_d3dpp.BackBufferWidth, m_lCenterY + m_dbXCamOffset);
	SurfaceX1Vertex[ulNumOfX1Vertex++] = D3DXVECTOR2((float)g_dx.m_d3dpp.BackBufferWidth - 5, m_lCenterY + m_dbXCamOffset - 5);
	SurfaceX1Vertex[ulNumOfX1Vertex++] = D3DXVECTOR2((float)g_dx.m_d3dpp.BackBufferWidth - 5, m_lCenterY + m_dbXCamOffset + 5);
	SurfaceX1Vertex[ulNumOfX1Vertex++] = D3DXVECTOR2((float)g_dx.m_d3dpp.BackBufferWidth, m_lCenterY + m_dbXCamOffset);
	
	SurfaceY1Vertex[ulNumOfY1Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset, 0);
	SurfaceY1Vertex[ulNumOfY1Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset - 5, 0 + 5);
	SurfaceY1Vertex[ulNumOfY1Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset + 5, 0 + 5);
	SurfaceY1Vertex[ulNumOfY1Vertex++] = D3DXVECTOR2((float)m_lCenterX + m_dbYCamOffset, 0);


	m_SurfaceX1.SetVertex(SurfaceX1Vertex, ulNumOfX1Vertex);
	m_SurfaceX2.SetVertex(SurfaceX2Vertex, ulNumOfX2Vertex);
	m_SurfaceY1.SetVertex(SurfaceY1Vertex, ulNumOfY1Vertex);
	m_SurfaceY2.SetVertex(SurfaceY2Vertex, ulNumOfY2Vertex);

	m_AideX.SetVertex(AideX, ulNumOfXAideVertex);
	m_AideY.SetVertex(AideY, ulNumOfYAideVertex);
	
	m_fXCurrentMinValue = (float)(0 - m_lCenterX) / m_fUnitSize*m_dbUnit;
	m_fXCurrentMaxValue = m_fXCurrentMinValue + (float)g_dx.m_d3dpp.BackBufferWidth / m_fUnitSize*m_dbUnit;
	m_fYCurrentMinValue = (float)(m_lCenterY - (float)g_dx.m_d3dpp.BackBufferHeight) / m_fUnitSize*m_dbUnit;
	m_fYCurrentMaxValue = m_fYCurrentMinValue + (float)g_dx.m_d3dpp.BackBufferHeight / m_fUnitSize*m_dbUnit;

	for (vector<CGraphic*>::iterator it = m_Graphics.begin();
		it != m_Graphics.end();
		it++)
	{
		(*it)->UpdateGraph(this);
	}
	return;
}

void CStudio2D::_InterFace_UpdateXOYLine(CStudio2D * pThis)
{
	if (pThis)
		pThis->UpdateSurfaceXOYLine();
}

void CStudio2D::MoveCam(float offX, float offY)
{
	m_lCenterX += (long)offX;
	m_lCenterY += (long)offY;
	if (m_lCenterY < 0)
		m_dbXCamOffset = 0 - (float)m_lCenterY;
	else if (m_lCenterY > (long)g_dx.m_d3dpp.BackBufferHeight)
		m_dbXCamOffset = (float)((long)g_dx.m_d3dpp.BackBufferHeight - m_lCenterY);
	else
		m_dbXCamOffset = 0;

	if (m_lCenterX < 0)
		m_dbYCamOffset = 0 - (float)m_lCenterX;
	else if (m_lCenterX > (long)g_dx.m_d3dpp.BackBufferWidth)
		m_dbYCamOffset = (float)((long)g_dx.m_d3dpp.BackBufferWidth - m_lCenterX);
	else
		m_dbYCamOffset = 0;

	UpdateSurfaceXOYLine();
	
}

void CStudio2D::Scale(float DeltaUnitSize)
{
	m_fUnitSize += DeltaUnitSize;
	if (m_fUnitSize > m_cfMaxSize)
	{
		m_fUnitSize = m_cfMinSize;
		m_dbUnit /= 2;
	}
	if (m_fUnitSize < m_cfMinSize)
	{
		m_fUnitSize = m_cfMaxSize;
		m_dbUnit *= 2;
	}
	UpdateSurfaceXOYLine();
}

void CStudio2D::AddGraphics(string exp, CGraphic** pOut)
{
	CGraphic* pGra = new CGraphic;
	pGra->UpdateGraph(this);
	pGra->SetExpression(exp.c_str());
	pGra->UpdateGraph(this);
	pGra->m_Line.SetColor(m_StdColor[m_colCount++]);
	if (m_colCount >= 18)
		m_colCount = 0;
	m_Graphics.push_back(pGra);
	if (pOut)
		*pOut = pGra;
	return;
}

void CStudio2D::DeleteGraphics(string exp)
{
	for (auto it = m_Graphics.begin();
		it != m_Graphics.end();
		it++)
	{
		if (!exp.compare((*it)->m_expression))
		{
			delete *it;
			m_Graphics.erase(it);
			break;
		}
	}
	return;
}

void CStudio2D::OnResetDeivce()
{
	m_SurfaceX1.OnResetDevice();
	m_SurfaceX2.OnResetDevice();
	m_SurfaceY1.OnResetDevice();
	m_SurfaceY2.OnResetDevice();
	m_AideX.OnResetDevice();
	m_AideY.OnResetDevice();
	m_Font.OnResetDevice();
	return;
}

CStudio2D::CCali::CCali(float unitValue, long Posx, long Posy, NP np)
{
	wstringstream oss;
	oss << unitValue;
	oss >> m_wsText;
	if (np == NP_BOTTOM)
	{
		m_lPx = (long)(Posx - m_wsText.length() / 2 * 7.5);
		m_lPy = Posy + 2;
	}
	if (np == NP_LEFT)
	{
		m_lPx = (long)(Posx - m_wsText.length() * 7.5 - 2);
		m_lPy = (long)(Posy - 7.5);
	}
	if (np == NP_RIGHT)
	{
		m_lPx = (long)(Posx + m_wsText.length() * 7.5);
		m_lPy = (long)(Posy - 7.5);
	}
	if (np == NP_TOP)
	{
		m_lPx = (long)(Posx - m_wsText.length() / 2 * 7.5);
		m_lPy = Posy - 15 - 2;
	}
	return;
}

CStudio2D::CGraphic::CGraphic()
{
	m_Line.SetDevice(g_dx.m_pD3DDevice);
	m_Line.InitializeLine();
	m_Line.SetColor(D3DCOLOR_XRGB(0, 0, 0));
	m_Line.SetWidth(2);
}

CStudio2D::CGraphic::~CGraphic()
{
	m_Line.ReleaseLine();
}

void CStudio2D::CGraphic::Render()
{
	m_Line.Render();
}

void CStudio2D::CGraphic::UpdateGraph(CStudio2D* ps2d)
{
	if (!m_ExpressionEnable)
		return;
	D3DXVECTOR2* pBuffer = new D3DXVECTOR2[g_dx.m_d3dpp.BackBufferWidth]{};
	long VertexNum{};
	float Value{};
	for (long i = 0; i < (long)g_dx.m_d3dpp.BackBufferWidth; i++)
	{
		m_calc.SetUnknownValue('x', (ps2d->m_fXCurrentMinValue + i / ps2d->m_fUnitSize*ps2d->m_dbUnit));
		Value = (float)m_calc.GetValue();
		pBuffer[VertexNum++] = D3DXVECTOR2((float)i, (ps2d->m_fYCurrentMaxValue - Value) / ps2d->m_dbUnit*ps2d->m_fUnitSize);
	}
	m_Line.SetVertex(pBuffer, VertexNum);
	delete[] pBuffer;
	return;
}

void CStudio2D::CGraphic::SetExpression(string exp)
{
	m_expression = exp;
	m_calc.m_UnknownVar.clear();
	m_calc.SetExpression(m_expression.substr(m_expression.find("=") + 1));	
	m_calc.AddUnknownVar('x', 1);
	bool ret{ false };
	m_calc.GetValue(&ret);
	if (ret)
		m_ExpressionEnable = false;
	else
		m_ExpressionEnable = true;

	return;
}
