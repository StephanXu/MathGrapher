#include "stdafx.h"
#include "AnimationManager.h"

CAnimationManage g_AniManager;

CAnimationManage::CAnimationManage()
{
}


CAnimationManage::~CAnimationManage()
{
}

int CAnimationManage::AddAnimation(CAnimation * pAni)
{
	m_AnimationList.push_back(pAni);
	return 0;
}

void CAnimationManage::UpdateAnimation()
{
	for (vector<CAnimation*>::iterator it = m_AnimationList.begin();
		it != m_AnimationList.end();
		it++)
	{
		(*it)->AnimationEvent();
		if ((*it)->m_ulTimer > (*it)->m_ulDuration)
		{
			delete (*it);
			m_AnimationList.erase(it);
			break;
		}
	}
	return;
}

