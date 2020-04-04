#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

class CCamera
{
public:
	CCamera();
	~CCamera();

	void SetDevice(IDirect3DDevice9* pDevice);
	void UpdateViewMatrix();
	void SetEye(D3DXVECTOR3 eye);
	void SetLookAt(D3DXVECTOR3 lookat);
	void SetUp(D3DXVECTOR3 up);

	void MoveCamera(float x, float y, float z);
	void RotateCamera(float x, float y, float z);
	
	float RotationX, RotationY, RotationZ;
	D3DXVECTOR3 m_eye;
private:
	
	D3DXVECTOR3 m_lookat;
	D3DXVECTOR3 m_up;

	IDirect3DDevice9* m_pDevice;
};

