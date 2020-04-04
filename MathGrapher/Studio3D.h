#pragma once

#include "Dx.h"
#include "Elements.h"
#include "Camera.h"
#include <vector>

using namespace std;

class CStudio3D
{
public:
	CStudio3D();
	~CStudio3D();

	vector<CElements*> m_Elements;

	CArrow m_arrX, m_arrY, m_arrZ;

	void InitStudio();

	void Render();
	void RegElements(CElements* pEle);
	
};

extern CStudio3D g_studio3d;