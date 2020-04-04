#pragma once
#include "Dx.h"
#include "Elements.h"
#include "Elements2D.h"
#include "Calc.h"
#include <vector>
#include <map>
#include <sstream>

using namespace std;

class CStudio2D
{
public:
	CStudio2D();
	~CStudio2D();

	CDxLine m_SurfaceX1;
	CDxLine m_SurfaceY1;
	CDxLine m_SurfaceX2;
	CDxLine m_SurfaceY2;
	CDxLine m_AideX;
	CDxLine m_AideY;
	CDxFont m_Font;
	class CCali
	{
	public:
		enum NP
		{
			NP_BOTTOM,
			NP_TOP,
			NP_LEFT,
			NP_RIGHT
		};
		CCali(float unitValue, long Posx, long Posy, NP np);
		wstring m_wsText;
		long m_lPx, m_lPy;
	};
	vector<CCali> m_CaliX;
	vector<CCali> m_CaliY;

	void InitStudio();
	void Render();
	void UpdateSurfaceXOYLine();
	static void _InterFace_UpdateXOYLine(CStudio2D* pThis);
	void MoveCam(float offX, float offY);
	void Scale(float DeltaUnitSize);

	float m_dbXCamOffset;
	float m_dbYCamOffset;

	float m_dbScale;
	unsigned long m_ulNumOfPoint;
	float m_dbUnit;
	float m_fUnitSize;

	float m_fXCurrentMinValue;
	float m_fXCurrentMaxValue;
	float m_fYCurrentMinValue;
	float m_fYCurrentMaxValue;

	long m_lCenterX;
	long m_lCenterY;

	const float m_cfMaxSize;
	const float m_cfMinSize;

	class CGraphic
	{
	public:
		CGraphic();
		~CGraphic();
		void Render();
		CDxLine m_Line;
		CCalc m_calc;
		void UpdateGraph(CStudio2D* ps2d);

		string m_expression;
		bool m_ExpressionEnable;
		void SetExpression(string exp);
	};

	vector<CGraphic*> m_Graphics;	
	void AddGraphics(string exp, CGraphic** pOut = nullptr);
	void DeleteGraphics(string exp);
	const D3DXCOLOR m_StdColor[18] =
	{
		0xfff44336, //red
		0xff3f51b5, //indigo
		0xffe91e63, //pink
		0xff009688, //teal
		0xff9c27b0, //purple
		0xffcddc39, //lime
		0xff673ab7, //deep purple
		0xff4caf50, //green
		0xff03a9f4, //light blue
		0xff00bcd4, //cyan
		0xff2196f3, //blue
		0xffffeb3b, //yellow
		0xff8bc34a, //light green
		0xffffc107, //amber
		0xff795548, //brown
		0xffff9800, //orange
		0xffff5722, //deep organge
		0xff607d8b, //blue grey
	};
	unsigned long m_colCount;

	void OnResetDeivce();
};

extern CStudio2D g_studio2d;