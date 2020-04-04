#include "stdafx.h"
#include "Animation.h"
#include <timeapi.h>

#pragma comment (lib,"Winmm.lib")

CAnimation::CAnimation()
	:m_ulDuration(0)
	, m_ulTimer(0)
	, m_ulOriTime(0)
	, m_pUpdateFunc(nullptr)
	, m_pUpdateObject(nullptr)
{
}


CAnimation::~CAnimation()
{
}

void CAnimation::SetUpdateFunc(UpdateFunc pFunc, void* pThis)
{
	m_pUpdateFunc = pFunc;
	m_pUpdateThis = pThis;
	return;
}

void CAnimation::SetDuration(unsigned long ulDura)
{
	m_ulDuration = ulDura;
	m_ulTimer = 0;
	m_ulOriTime = timeGetTime();
	return;
}

void CAnimation::AnimationEvent()
{
	return;
}

void CLineAnimation::AnimationEvent()
{
	m_ulTimer = timeGetTime() - m_ulOriTime;
	if (m_ulTimer > m_ulDuration)
	{
		*m_lpObj = m_ulTarValue;
		return;
	}
	*m_lpObj = (long)(m_ulOriValue + (float)((float)(m_ulTarValue - m_ulOriValue) / (float)m_ulDuration)*m_ulTimer);
	if (m_pUpdateFunc && m_pUpdateThis)
	{
		m_pUpdateFunc(m_pUpdateThis);
	}
}

void CLineAnimation::SetAnimationObject(void * pObj)
{
	m_lpObj = (long*)pObj;
	return;
}

void CLineAnimation::SetAnimationValue(unsigned long ulOriVal, unsigned long uTarVal)
{
	m_ulOriValue = ulOriVal;
	m_ulTarValue = uTarVal;
	return;
}

void CCurveAnimation::AnimationEvent()
{
	m_ulTimer = timeGetTime() - m_ulOriTime;
	if (m_ulTimer > m_ulDuration)
	{
		*m_fpObj = m_fTarValue;
		return;
	}

}

void CCurveAnimation::SetAnimationObject(void * fpObj)
{
	m_fpObj = (float*)fpObj;
	return;
}

void CCurveAnimation::SetAnimationValue(float fOriValue, float fTarValue, float fAclPer, float fDclPer)
{
	if (m_fAclPer > 1 || m_fDclPer > 1)
		return;
	m_fOriValue = fOriValue;
	m_fTarValue = fTarValue;
	m_fAclPer = fAclPer;
	m_fDclPer = fDclPer;
	return;
}
