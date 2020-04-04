// MathGrapher.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MathGrapher.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "Dx.h"
#include "Elements.h"
#include "Elements2D.h"
#include "UIManage.h"
#include "Studio2D.h"
#include "Studio3D.h"
#include "Animation.h"
#include "AnimationManager.h"

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")


#define MAX_LOADSTRING 100


#define ORI_X 50
#define ORI_Y 50

// 全局变量: 
HWND hWnd;
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

void MainLoop(void);
HRESULT InitD3D(HWND hWnd);

unsigned long ORI_WIDTH{ 1024 };
unsigned long ORI_HEIGHT{ 768 };

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MATHGRAPHER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MATHGRAPHER));

	//Initialize Direct3D
	g_UIManager.m_hWnd = hWnd;
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Show our window
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
		MainLoop();
	}
	else
	{
		MessageBox(0, L"渲染引擎初始化失败", 0, 0);
		exit(0);
	}

	return 0;
}

//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MATHGRAPHER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MATHGRAPHER);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
	   ORI_X, ORI_Y, 500, 500, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
float x;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CWidget* pWidget{ nullptr };
	static bool bLeftDown{ false };
	static long LastX{}, LastY{};
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		g_UIManager.m_MouseX = LOWORD(lParam);
		g_UIManager.m_MouseY = HIWORD(lParam);
		if (bLeftDown)
		{
			if (!g_dx.m_b3dStudio)
				g_studio2d.MoveCam((float)(long)g_UIManager.m_MouseX - LastX, (float)(long)g_UIManager.m_MouseY - LastY);
			else
			{
				g_dx.m_cam.RotateCamera(
					((long)g_UIManager.m_MouseY - LastY) / 100.0f,
					((long)g_UIManager.m_MouseX - LastX) / 100.0f,
					0);
				g_dx.m_cam.UpdateViewMatrix();
			}
			LastX = g_UIManager.m_MouseX;
			LastY = g_UIManager.m_MouseY;
		}
		break;
	case WM_MOUSEWHEEL:
		if (!g_dx.m_b3dStudio)
		{
			if ((short)HIWORD(wParam) > 0)
			{
				g_studio2d.Scale(1);
			}
			else
			{
				g_studio2d.Scale(-1);
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		pWidget = nullptr;
		g_UIManager.ScanTarget(&pWidget, g_UIManager.m_MouseX, g_UIManager.m_MouseY);
		if (pWidget)
		{
			pWidget->m_pEventProc(CWidget::EVENTTYPE_DBLEFTDOWN, pWidget, 0);
			break;
		}
		LastX = g_UIManager.m_MouseX;
		LastY = g_UIManager.m_MouseY;

		break;
	case WM_LBUTTONDOWN:
		pWidget = nullptr;
		g_UIManager.ScanTarget(&pWidget, g_UIManager.m_MouseX, g_UIManager.m_MouseY);
		if (pWidget)
		{
			pWidget->m_pEventProc(CWidget::EVENTTYPE_LEFTDOWN, pWidget, 0);
			break;
		}
		bLeftDown = true;
		LastX = g_UIManager.m_MouseX;
		LastY = g_UIManager.m_MouseY;
		break;
	case WM_LBUTTONUP:
		bLeftDown = false;
		LastX = 0;
		LastY = 0;
		break;
	case WM_CHAR:
		if (pWidget)
		{
			if (pWidget->m_WidgetType == CWidget::WIDGETTYPE_INPUTBOX)
			{
				((CInputbox*)pWidget)->m_newChar = wParam;
				pWidget->m_pEventProc(CWidget::EVENTTYPE_INPUT, pWidget, wParam);
			}
		}
		break;
	case WM_KEYDOWN:
		if (pWidget)
		{
			pWidget->m_pEventProc(CWidget::EVENTTYPE_KEYDOWN, pWidget, wParam);
		}
		break;
	case WM_WIDGETLOAD:
		if (wParam)
			((CWidget*)wParam)->m_pEventProc(CWidget::EVENTTYPE_LOAD, (void*)wParam, 0);
		break;
	case WM_SIZE:
		//g_dx.m_bResetFlag = true;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int OnEmptyEvent(CWidget::EVENTTYPE ulEventType, void* pWidget, unsigned int Param)
{
	return 1;
}

int OnButtonEvent(CWidget::EVENTTYPE ulEventType, void* pWidget, unsigned int Param)
{
	return 1;
}

int OnLableEvent(CWidget::EVENTTYPE ulEventType, void* pWidget, unsigned int Param)
{
	if (ulEventType == CWidget::EVENTTYPE_DBLEFTDOWN)
	{
		for (auto it = g_UIManager.m_Widgets.begin();
			it != g_UIManager.m_Widgets.end();
			it++)
		{
			if (it->second->m_WidgetType == CWidget::WIDGETTYPE_LABLE
				&& it->first > ((CLable*)pWidget)->m_ulWidgetID)
			{
				it->second->m_top -= 30;
			}
		}
		g_studio2d.DeleteGraphics(((CLable*)pWidget)->m_Text);
		g_UIManager.UnRegWidget(((CLable*)pWidget)->m_ulWidgetID);
		
	}

	return 1;
}


int OnInputboxEvent(CWidget::EVENTTYPE ulEventType, void* pWidget, unsigned int Param)
{
	CInputbox *pWg = (CInputbox*)pWidget;
	static char sLast{};
	if (ulEventType == CWidget::EVENTTYPE_INPUT)
	{
		if (isalnum(pWg->m_newChar) && !(pWg->m_newChar >= 'A' && pWg->m_newChar <= 'Z'))
		{
			sLast = pWg->m_newChar;
			pWg->ApplyInput();
		}
		else if (
			pWg->m_newChar == '=' ||
			pWg->m_newChar == '+' ||
			pWg->m_newChar == '-' ||
			pWg->m_newChar == '*' ||
			pWg->m_newChar == '/' ||
			pWg->m_newChar == '(' ||
			pWg->m_newChar == ')' ||
			pWg->m_newChar == '%' ||
			pWg->m_newChar == '^' ||
			pWg->m_newChar == '.')
		{
			pWg->ApplyInput();
		}
	}
	else if (ulEventType == CWidget::EVENTTYPE_KEYDOWN)
	{
		if (Param == 0x8)
		{
			pWg->Backspace();
		}
		else if (Param == 0xD)
		{
			CStudio2D::CGraphic* pGra;
			g_studio2d.AddGraphics(pWg->m_Text, &pGra);
			CLable* pLable = new CLable;
			pLable->SetID(1000 + g_studio2d.m_Graphics.size());
			pLable->m_WidgetType = CWidget::WIDGETTYPE_LABLE;
			pLable->m_left = (unsigned long)(g_dx.m_d3dpp.BackBufferWidth*0.8);
			pLable->m_right = g_dx.m_d3dpp.BackBufferWidth - 20;
			pLable->m_top = 40 + g_studio2d.m_Graphics.size() * 30;
			pLable->m_bottom = pLable->m_top + 30;
			pLable->m_FontColor = pGra->m_Line.GetColor();
			pLable->SetEventProc(OnLableEvent);
			pLable->SetText(pWg->m_Text);
			pLable->SetFontSize(15);
			g_UIManager.RegWidget(pLable, 1000 + g_studio2d.m_Graphics.size());
			pWg->m_Text.clear();
		}
	}
	return 1;
}

HRESULT InitD3D(HWND hWnd)
{
	// First of all, create the main D3D object. If it is created successfully we
	// should get a pointer to an IDirect3D8 interface.
	g_dx.m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_dx.m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// Get the current display mode
	if (FAILED(g_dx.m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &g_dx.m_d3ddm)))
	{
		return E_FAIL;
	}

	if (FAILED(g_dx.m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &g_dx.m_d3ddc)))
	{
		return E_FAIL;
	}

	// Create a structure to hold the settings for our device
	memset(&g_dx.m_d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));

	// Fill the structure. 
	// We want our program to be windowed, and set the back buffer to a format
	// that matches our current display mode
	g_dx.m_d3dpp.Windowed = TRUE;
	g_dx.m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_dx.m_d3dpp.BackBufferFormat = g_dx.m_d3ddm.Format;

	D3DMULTISAMPLE_TYPE multiType = D3DMULTISAMPLE_NONE;
	if (g_dx.m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, g_dx.m_d3ddm.Format, TRUE,
		D3DMULTISAMPLE_4_SAMPLES,
		NULL) == D3D_OK)
	{
		multiType = D3DMULTISAMPLE_4_SAMPLES;
	}
	g_dx.m_d3dpp.MultiSampleType = multiType;

	// Create a Direct3D device
	if (FAILED(g_dx.m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_dx.m_d3dpp, &g_dx.m_pD3DDevice)))
	{
		return E_FAIL;
	}
	
	if (g_dx.m_d3ddm.Height > g_dx.m_d3ddm.Width)
	{
		if (g_dx.m_d3ddm.Height > 1080/0.8)
			g_dx.m_d3ddm.Height = 1080;
		else
			ORI_WIDTH = (unsigned long)(g_dx.m_d3ddm.Width*0.8);
		ORI_HEIGHT = ORI_WIDTH / 16 * 9;
	}
	else
	{
		if (g_dx.m_d3ddm.Width > 1920/0.8)
			g_dx.m_d3ddm.Width = 1920;
		else
			ORI_HEIGHT = (unsigned long)(g_dx.m_d3ddm.Height*0.8);
		ORI_WIDTH = ORI_HEIGHT / 9 * 16;
	}
	
	SetWindowPos(hWnd, HWND_TOP, ORI_X, ORI_Y, ORI_WIDTH, ORI_HEIGHT, SWP_SHOWWINDOW);

	g_dx.m_d3dpp.BackBufferHeight = 0;
	g_dx.m_d3dpp.BackBufferWidth = 0;
	g_dx.m_pD3DDevice->Reset(&g_dx.m_d3dpp);
	
	// Turn on back face culling.
	// This is because we want to hide the back of our polygons
	g_dx.m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	g_dx.m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	// Turn off lighting because we are specifying that our vertices have colour
	g_dx.m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//================================================================

	//测试界面内容
	g_dx.m_cam.SetDevice(g_dx.m_pD3DDevice);
	g_dx.m_cam.UpdateViewMatrix();

	CInputbox *InputBox = new CInputbox;
	InputBox->SetID(06);
	InputBox->m_left = (unsigned long)(g_dx.m_d3dpp.BackBufferWidth*0.8);
	InputBox->m_right = g_dx.m_d3dpp.BackBufferWidth - 20;
	InputBox->m_top = 30;
	InputBox->m_bottom = 30 + 30;
	InputBox->m_WidgetType = CWidget::WIDGETTYPE_INPUTBOX;
	InputBox->SetEventProc(OnInputboxEvent);
	InputBox->SetColor(D3DCOLOR_XRGB(0, 0, 0));
	InputBox->SetLineColor(D3DCOLOR_XRGB(0, 0, 0));
	InputBox->SetFontSize(15);
	InputBox->UpdateLine();
	g_UIManager.RegWidget(InputBox, 06);

	g_studio2d.InitStudio();
	g_studio2d.UpdateSurfaceXOYLine();

	//g_studio3d.InitStudio();

	//CLineAnimation* cl = new CLineAnimation;
	//cl->SetAnimationObject(&g_studio2d.m_lCenterX);
	//cl->SetAnimationValue(300, 600);
	//cl->SetDuration(500);
	//cl->SetUpdateFunc((CAnimation::UpdateFunc)g_studio2d._InterFace_UpdateXOYLine, &g_studio2d);
	//g_AniManager.AddAnimation(cl);

	/*
	CCube* cub = new CCube;
	cub->SetDevice(g_dx.m_pD3DDevice);
	cub->InitCube(10, 10, 10, D3DCOLOR_XRGB(255, 0, 0));
	g_studio3d.RegElements(cub);
	CLable* tips = new CLable;
	tips->SetFontSize(10);
	tips->SetID(07);
	tips->SetEventProc(OnEmptyEvent);
	tips->SetText("em");
	tips->SetTextColor(D3DCOLOR_XRGB(0, 0, 0));
	tips->m_left = 0; tips->m_top = 0;
	tips->m_right = 300; tips->m_bottom = 100;
	g_UIManager.RegWidget(tips,07);
	*/
	return S_OK;
}

#include <timeapi.h>

#pragma comment (lib,"Winmm.lib")

//Render proc
void Render()
{
	if (g_dx.m_pD3DDevice == NULL)
	{
		return;
	}
	// Clear the back buffer to a specified color
	g_dx.m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	//g_AniManager.UpdateAnimation();

	// Begin the scene
	g_dx.m_pD3DDevice->BeginScene();
	/*
	CLable* p;
	g_UIManager.GetWidget((CWidget**)&p, 07);
	char str[1024];
	sprintf_s(str, "x:%f,y:%f,z:%f,d:%f",
		g_dx.m_cam.m_eye.x,
		g_dx.m_cam.m_eye.y,
		g_dx.m_cam.m_eye.z,
		D3DXVec3Length(&g_dx.m_cam.m_eye));
	p->SetText(str);
	*/
	if (!g_dx.m_b3dStudio)
		g_studio2d.Render();
	else
		g_studio3d.Render();
	g_UIManager.RenderUI();

	
	// End the scene
	g_dx.m_pD3DDevice->EndScene();
	// Filp the back and front buffers so that whatever has been rendered on the
	// back buffer will now be visible on screen (front buffer)
	g_dx.m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	if (g_dx.m_bResetFlag)
	{
		g_dx.m_d3dpp.BackBufferHeight = 0;
		g_dx.m_d3dpp.BackBufferWidth = 0;
		g_dx.m_pD3DDevice->Reset(&g_dx.m_d3dpp);
		g_studio2d.OnResetDeivce();
		g_UIManager.OnResetDevice();
		g_dx.m_bResetFlag = false;
	}
	
	return;
}

void MainLoop()
{
	// Enter the game loop
	MSG msg;
	BOOL fMessage;

	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

	while (msg.message != WM_QUIT)
	{
		fMessage = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (fMessage)
		{
			// Process message
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// No message to process, so render the current scene
			Render();
		}
	}
}