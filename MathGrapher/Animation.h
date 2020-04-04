#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
class CAnimation
{
public:
	CAnimation();
	~CAnimation();

	unsigned long m_ulDuration;
	unsigned long m_ulTimer;
	unsigned long m_ulOriTime;

	typedef void (*UpdateFunc)(void*);
	UpdateFunc m_pUpdateFunc;
	void* m_pUpdateThis;
	virtual void SetUpdateFunc(UpdateFunc pFunc, void* pThis);
	void* m_pUpdateObject;

	virtual void SetDuration(unsigned long ulDura);
	virtual void AnimationEvent();
};

class CLineAnimation : public CAnimation
{
public:

	unsigned long m_ulOriValue;
	unsigned long m_ulTarValue;
	long* m_lpObj;

	void AnimationEvent();
	void SetAnimationObject(void* pObj);
	void SetAnimationValue(unsigned long ulOriVal, unsigned long uTarVal);
};

class CCurveAnimation : public CAnimation
{
public:
	
	float m_fOriValue;
	float m_fTarValue;

	//Accelerate time
	float m_fAclPer;
	//Decelerate time
	float m_fDclPer;

	float* m_fpObj;

	void AnimationEvent();
	void SetAnimationObject(void* fpObj);
	void SetAnimationValue(
		float fOriValue,
		float fTarValue,
		float fAclPer,
		float fDclPer
	);

};