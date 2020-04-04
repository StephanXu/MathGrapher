#pragma once
#include <vector>
#include "Animation.h"

using namespace std;

class CAnimationManage
{
public:
	CAnimationManage();
	~CAnimationManage();

	vector<CAnimation*> m_AnimationList;
	int AddAnimation(CAnimation* pAni);
	void UpdateAnimation();
};

extern CAnimationManage g_AniManager;