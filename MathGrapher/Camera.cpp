#include "stdafx.h"
#include "Camera.h"
#include "Dx.h"
#include <d3dx9.h>
#include <d3d9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

CCamera::CCamera()
	:m_eye(0.0f, 0.0f, -30.0f)
	,m_lookat(0.0f, 0.0f, 0.0f)
	,m_up(0.0f, 1.0f, 0.0f)
	,m_pDevice(nullptr)
	,RotationX(0)
	,RotationY(0)
	,RotationZ(0)
{
	
}


CCamera::~CCamera()
{
}

void CCamera::SetDevice(IDirect3DDevice9* pDevice)
{
	m_pDevice = pDevice;
	return;
}

void CCamera::UpdateViewMatrix()
{
	//Set up camera matrix
	D3DXMATRIX matView;

	D3DXMatrixLookAtLH(&matView, &m_eye, &m_lookat, &m_up);
	m_pDevice->SetTransform(D3DTS_VIEW, &matView);

	//Set project matrix
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 3, (float)g_dx.m_d3dpp.BackBufferWidth/g_dx.m_d3dpp.BackBufferHeight, 1.0f, 500.0f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	return;
}

void CCamera::SetEye(D3DXVECTOR3 eye)
{
	m_eye = eye;
	return;
}

void CCamera::SetLookAt(D3DXVECTOR3 lookat)
{
	m_lookat = lookat;
	return;
}

void CCamera::SetUp(D3DXVECTOR3 up)
{
	m_up = up;
	return;
}

void CCamera::MoveCamera(float x, float y, float z)
{
	m_eye.x += x;
	m_eye.y += y;
	m_eye.z += z;
	return;
}

void CCamera::RotateCamera(float x, float y, float z)
{
	RotationX += x;
	RotationY += y;
	RotationZ += z;

	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, z);
	D3DXVec3TransformNormal(&m_eye, &m_eye, &mat);
	if (RotationX >= D3DXToRadian(89.9))
		RotationX = D3DXToRadian(89.9);
	else if (RotationX <= -D3DXToRadian(89.9))
		RotationX = -D3DXToRadian(89.9);
	else
	{
		D3DXVECTOR3 Axis;
		D3DXVec3Cross(&Axis, &m_eye, &D3DXVECTOR3(0, 1, 0));
		D3DXMatrixRotationAxis(&mat, &Axis, x);
		D3DXVec3TransformNormal(&m_eye, &m_eye, &mat);
	}
	D3DXMatrixRotationY(&mat, y);
	D3DXVec3TransformNormal(&m_eye, &m_eye, &mat);

	return;
}

