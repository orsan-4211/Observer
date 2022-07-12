#include <WindowsX.h>
#include <tchar.h>
#include <crtdbg.h>
#include <time.h>
#include <sys/timeb.h>
#include <iostream>

#include <thread>
#include "../Include/pcap.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>

#include <Windows.h>
#include <imm.h>

#include "./DirectX/lib/DX3D_11.h"
#include "./DirectX/lib/Grstr.h"
#include "./DirectX/lib/Inputmain.h"
#include "./DirectX/lib/keycode.h"
#include "./DirectX/lib/Fps.h"


#include "main.h"

#if _DEBUG
#include "debug_code\debug.h"
#endif

#include "keyinput.h"

#include "./graphic/graphicmain.h"
#include "./pcap/packet.h"
#include "./pcap/analysis.h"

#include "./sidecharm/charmmain.h"
#include "./sidecharm/button.h"

#include "./graphic/skinmanager/skinmanager.h"
#include "./graphic/defaultskin/defaultskin.h"

#include "./setting/setting.h"

#pragma comment(lib, "iphlpapi.lib")


#define APPNAME TEXT(VERSION)
#define MENUID TEXT("MENUBAR")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND hWnd = NULL;
MSG msg;
HDC hdc;
HINSTANCE hInst;
WNDCLASSEX wc;
PAINTSTRUCT lpaint;
HRESULT      hr;
CRITICAL_SECTION cs;

short int windowsizef = 1;//-1: 3840x2100 0:1920x1080,1:1600x900,2:1280x720
RECT windowsize;
short int windowsizedx[2];
short int windowf = 1;
DEVMODE displayset, displaysettingbuff;

RECT rw, rc;
int width, height;

void Cls_OnDestroy(HWND);
void Cls_OnPaint(HWND);
BOOL Cls_OnEraseBkgnd(HWND, HDC);


short int errorf = 0;

short int checki;
int oldflame[2];

bool quitf;

bool resetf;

DX113D dxg;

DX11PShader pshader;
DX11VShader vshader;

DX11InputLayout ilayout;
D3D11_VIEWPORT view;
DX11DSBuffer dsbuf;

Vertex defaultvert[4];
WORD defaultind[6];

DX11Buffeer vbuff;
DX11Buffeer ibuff;
unsigned int vertmaxcount;
unsigned int indmaxcount;

XMMATRIX worldMatrix;

XMVECTOR eye;
XMVECTOR focus;
XMVECTOR up;
XMMATRIX viewMatrix;

float fov;
float aspect;
float nearZ;
float farZ;
XMMATRIX projMatrix;

DX11Buffeer cbuff;

D3D11_BUFFER_DESC cbDesc;

ConstantBuffer cb;

DX11SAMPLER sampler;
DX11SAMPLER samplerlin;

CD3D11_DEFAULT default_state;


CD3D11_RASTERIZER_DESC rsdesc(default_state);
CD3D11_DEPTH_STENCIL_DESC dsdesc(default_state);
CD3D11_BLEND_DESC bddesc(default_state);

DX11RSState rss;
DX11DSState dss;
DX11BDState bds;

float eyeposz;

DX8Input dip;
DX8Keybord dkb;
DX8Mouse dms;
DX8Pad dpd;

gstrclass gstr;
gstrclass gstrinner;

bool singlethredf = 0;
bool threadfinf;

double resultfps;
void limitfps();

void maingrset();
void alldr();
void maingrRelease();


int WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, int iCmdShow) {
	

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = APPNAME;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	switch (windowsizef) {
	case -1:
		windowsize.left = 0;
		windowsize.top = 0;
		windowsize.right = 3840;
		windowsize.bottom = 2100;
		break;
	case 0:
		windowsize.left = 0;
		windowsize.top = 0;
		windowsize.right = 1920;
		windowsize.bottom = 1080;
		break;
	case 1:
		windowsize.left = 0;
		windowsize.top = 0;
		windowsize.right = 1600;
		windowsize.bottom = 900;
		break;
	case 2:
		windowsize.left = 0;
		windowsize.top = 0;
		windowsize.right = 1280;
		windowsize.bottom = 720;
		break;
	default:
		windowsize.left = 0;
		windowsize.top = 0;
		windowsize.right = 1280;
		windowsize.bottom = 720;
		break;

	}

	hWnd = CreateWindow(
		APPNAME,
		APPNAME,
		WS_EX_TOOLWINDOW | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER,
		0,
		0,
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL);

	GetWindowRect(hWnd, &rw);
	GetClientRect(hWnd, &rc);


	windowsizedx[0] = windowsize.right;
	windowsizedx[1] = windowsize.bottom;



	width = (rw.right - rw.left) - (rc.right - rc.left) + windowsizedx[0];
	height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + windowsizedx[1];
	SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);

	GetWindowRect(hWnd, &rw);
	GetClientRect(hWnd, &rc);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	ZeroMemory(&msg, sizeof(msg));

	quitf = 0;


	dxg.Create(hWnd, rc.right, rc.bottom, windowf);

	vertmaxcount = 0;
	indmaxcount = 0;

	pshader.CreateShaderP(*dxg.pDevice, &g_ps_main, sizeof(g_ps_main));

	vshader.CreateShaderV(*dxg.pDevice, &g_vs_main, sizeof(g_vs_main));

	ilayout.DescSet("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	ilayout.DescSet("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	ilayout.DescSet("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	ilayout.CreateIL(dxg.pDevice, &g_vs_main, sizeof(g_vs_main));


	dsbuf.Create(dxg.pDevice, rc.right, rc.bottom);

	view.Width = (float)rc.right;
	view.Height = (float)rc.bottom;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;

	eyeposz = -5.0;

	eye = XMVectorSet(0.0f, 0.0f, eyeposz, 0.0f);
	focus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	fov = XMConvertToRadians(45.0f);
	aspect = view.Width / view.Height;
	nearZ = 1.0f;
	farZ = 100.0f;

	cbuff.CreateBuffer(dxg.pDevice, sizeof(ConstantBuffer), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	sampler.SamplerCreate(dxg.pDevice, D3D11_FILTER_MIN_MAG_MIP_LINEAR/*D3D11_FILTER_MIN_MAG_MIP_LINEAR*/, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_NEVER, 0, D3D11_FLOAT32_MAX);

	samplerlin.SamplerCreate(dxg.pDevice, D3D11_FILTER_MIN_MAG_MIP_LINEAR/*D3D11_FILTER_MIN_MAG_MIP_LINEAR*/, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_NEVER, 0, D3D11_FLOAT32_MAX);

	rsdesc.CullMode = D3D11_CULL_NONE;

	bddesc.AlphaToCoverageEnable = FALSE;
	bddesc.IndependentBlendEnable = FALSE;

	bddesc.RenderTarget[0].BlendEnable = TRUE;

	bddesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bddesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bddesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bddesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bddesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bddesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;

	bddesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	dsdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	rss.CreateState(dxg.pDevice, rsdesc);
	dss.CreateState(dxg.pDevice, dsdesc);
	bds.CreateState(dxg.pDevice, bddesc);

	dip.Create(hWnd);
	dkb.Create(hWnd, dip.pDInput, GUID_SysKeyboard, &c_dfDIKeyboard);
	dms.Create(hWnd, dip.pDInput, GUID_SysMouse, &c_dfDIMouse2);
	dpd.Create(hWnd, dip.pDInput, GUID_Joystick, &c_dfDIJoystick2);


	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	projMatrix = XMMatrixOrthographicLH(view.Width, view.Height, nearZ, farZ);


	viewMatrix = XMMatrixLookAtLH(eye, focus, up);

	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(worldMatrix));
	XMStoreFloat4x4(&cb.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(projMatrix));
	cbuff.Dataset(dxg.pImmediateContext, &cb, sizeof(ConstantBuffer));

	defaultvert[0].x = 0;
	defaultvert[0].y = 0;
	defaultvert[0].z = 0.0f;

	defaultvert[1].x = 1280;
	defaultvert[1].y = 960;
	defaultvert[1].z = 0.0f;


	defaultvert[2].x = 0;
	defaultvert[2].y = 960;
	defaultvert[2].z = 0.0f;


	defaultvert[3].x = 1280;
	defaultvert[3].y = 0;
	defaultvert[3].z = 0.0f;

	Set2DPosvert(defaultvert, view.Width, view.Height, 4, 0, cb.view, cb.projection);

	defaultvert[0].tu = 0.0f;
	defaultvert[0].tv = 0.0f;
	defaultvert[0].color[0] = 1.0f;
	defaultvert[0].color[1] = 1.0f;
	defaultvert[0].color[2] = 1.0f;
	defaultvert[0].color[3] = 1.0f;

	defaultvert[1].tu = 1.0f;
	defaultvert[1].tv = 960.0f / 1280.0f;
	defaultvert[1].color[0] = 1.0f;
	defaultvert[1].color[1] = 1.0f;
	defaultvert[1].color[2] = 1.0f;
	defaultvert[1].color[3] = 1.0f;

	defaultvert[2].tu = 0.0f;
	defaultvert[2].tv = 960.0f / 1280.0f;
	defaultvert[2].color[0] = 1.0f;
	defaultvert[2].color[1] = 1.0f;
	defaultvert[2].color[2] = 1.0f;
	defaultvert[2].color[3] = 1.0f;

	defaultvert[3].tu = 1.0f;
	defaultvert[3].tv = 0.0f;
	defaultvert[3].color[0] = 1.0f;
	defaultvert[3].color[1] = 1.0f;
	defaultvert[3].color[2] = 1.0f;
	defaultvert[3].color[3] = 1.0f;

	defaultind[0] = 0;
	defaultind[1] = 1;
	defaultind[2] = 2;
	defaultind[3] = 0;
	defaultind[4] = 3;
	defaultind[5] = 1;

	gstr.Stringcreate(dxg.pDevice, _T("defaultgraph\\textnormal.png"), 128, 128, 2048, 2048, 16, 0, 0);
	gstrinner.Stringcreate(dxg.pDevice, _T("defaultgraph\\textnormal.png"), 128, 128, 2048, 2048, 16, 0, 0);


	Skin_FoldNameSet();
	Skin_Reset();

	Ne_FileNameSet();
	Ne_FileNameCheck();

	deviceselected = -2;
	
	resetf = 0;

	PacDevSet();
	graphicinit();

	DefaultSkinsetting();
	defaultskinload();

	threadfinf = 1;

	SetAnalysis();

	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			dkb.Set(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			dms.Set(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


			Checkkeyinput();

			if (dkb.GetKeySt(KEY_F4) == 1 && dkb.GetKeySt(KEY_LALT) == 1) {
				quitf = 1;
				
			}
			gstrinner.StringCountReset();
			gstr.StringCountReset();

			if (deviceselected == -1 && modef == 0) {
				Ne_Select();

				
				for (int i = -1; i < nefilecount; i++) {
					if (i == -1) {
						gstrinner.Stringset(view.Width / 2 - 120, (view.Height / 2 - (22 * (nefilecount / 2.0))) + (22 * i), 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 0, _T("Not use Necfg file"));
					}
					else {
						gstrinner.Stringset(view.Width / 2 - 120, (view.Height / 2 - (22 * (nefilecount / 2.0))) + (22 * i), 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 0, _T("%s"), nefilename[i].filenamebuff);
					}
				}
				gstrinner.Stringset(view.Width / 2 - 130, (view.Height / 2 - (22 * (nefilecount / 2.0))) + (22 * nefileselect), 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 2, _T("%03d>"), nefileselect);

			}else
			if (deviceselected == 0 && modef == 0) {
				DevSelect(pdev.usecount);

				for (int i = -1; i < (int)pdev.usecount; i++) {
					if (i == -1) {
						gstrinner.Stringset(view.Width / 2 - 120, (view.Height / 2 - (22 * (pdev.usecount / 2.0))) + (22 * i), 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 0, _T("< Back"));
					}
					else {

						if (pdev.data[i].adapterNo == -1) {
							gstrinner.Stringset(view.Width / 2 - 120, (view.Height / 2 - (22 * (pdev.usecount / 2.0))) + (22 * i), 20, 20, 0, 0, 12, 0x40000000 + skinset.SRGB, 0, _T("%s"), pdev.data[i].FriendlyName);
						}
						else {
							gstrinner.Stringset(view.Width / 2 - 120, (view.Height / 2 - (22 * (pdev.usecount / 2.0))) + (22 * i), 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 0, _T("%s"), pdev.data[i].FriendlyName);
						}

					}
				}
				if (selecteddevice == -1) {
					gstrinner.Stringset(view.Width / 2 - 130, (view.Height / 2 - (22 * (pdev.usecount / 2.0))) + (22 * selecteddevice), 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 2, _T("   >"));
				}
				else
				{
					gstrinner.Stringset(view.Width / 2 - 130, (view.Height / 2 - (22 * (pdev.usecount / 2.0))) + (22 * selecteddevice), 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 2, _T("%3d>"), pdev.data[selecteddevice].adapterNo);
				}
				

			}

			if (deviceselected == 1) {
				DataStart();
				ReSetAnalysis();

				if (singlethredf == 0) {
					std::thread th_a(PacCapture);
					threadfinf = 0;
					th_a.detach();
				}

				deviceselected = 2;
			}

			
			if (deviceselected == 2) {
				Analysismain(modef);
			
			}

			graphicmain();

			charmmain();

			graphiccharmmain();

			gstr.Stringset(10, 10, 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 0, _T("%5.1lffps"), resultfps);

			gstr.Stringset(rc.right, rc.bottom - 12, 20, 20, 0, 0, 12, 0x40000000 + skinset.SRGB, 2, _T(VERSION));
			

			Set2DPosvert(gstr.vert, rc.right, rc.bottom, gstr.vertc, 0, cb.view, cb.projection);
			drvert(gstr.vert, gstr.vertc, gstr.ind, gstr.indc, gstr.indc, 0, 0, texman.Data(obj[0].GetId()), &view);
			
			
			alldr();


			limitfps();

			

			if (deviceselected == 2 && resetf == 1 && threadfinf == 1) {
				DataTrash();
				TrashAnalysis();
				Ne_FileNameFree();
				Ne_FileNameSet();
				Ne_FileNameCheck();
				PacTrash();
				PacDevClose();
				PacDevSet();
				deviceselected = -1;
				resetf = 0;
			}
			else if(resetf == 1 && threadfinf == 1){
				DataTrash();
				TrashAnalysis();
				Ne_FileNameFree();
				Ne_FileNameSet();
				Ne_FileNameCheck();
				PacTrash();
				PacDevClose();
				PacDevSet();
				deviceselected = -1;
				resetf = 0;
			}

			if (quitf == 1) {
				PostQuitMessage(0);
			}
		}

	}
	quitf = 1;

	graphicRelease();
	gstr.Release();
	gstrinner.Release();
	maingrRelease();

	Skin_FoldNameFree();

	Ne_FileNameFree();

	dxg.Release();
	dip.Release();
	dkb.Release();
	dms.Release();
	dpd.Release();

	while (deviceselected == 2 && threadfinf == 0) {
		
	}
	
	DataTrash();
	TrashAnalysis();
	PacTrash();
	PacDevClose();
	

	return (INT)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT, Cls_OnPaint);
		HANDLE_MSG(hWnd, WM_ERASEBKGND, Cls_OnEraseBkgnd);

	case WM_COMMAND:

		return 0;

	default:
		return (DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Cls_OnDestroy(HWND hwnd) {
	DeleteCriticalSection(&cs);
	PostQuitMessage(0);
	quitf = 1;
	return;
}

void Cls_OnPaint(HWND hwnd) {
	hdc = BeginPaint(hWnd, &lpaint);
	Rectangle(hdc, -1, -1, windowsizedx[0], windowsizedx[1]);
	EndPaint(hWnd, &lpaint);
	return;
}

BOOL Cls_OnEraseBkgnd(HWND hwnd, HDC hdc) {
	return 1;
}

void maingrset() {
	float colf[4] = { 0.5f,0.5f,0.5f,1.0f };

	view.Height = (float)rc.bottom;
	view.Width = (float)rc.right;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	fov = XMConvertToRadians(45.0f);
	aspect = view.Width / view.Height;
	nearZ = 1.0f;
	farZ = 100.0f;
	projMatrix = XMMatrixOrthographicLH(view.Width, view.Height, nearZ, farZ);

	eye = XMVectorSet(0.0f, 0.0f, eyeposz, 0.0f);
	focus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	viewMatrix = XMMatrixLookAtLH(eye, focus, up);

	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(worldMatrix));
	XMStoreFloat4x4(&cb.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(projMatrix));
	cbuff.Dataset(dxg.pImmediateContext, &cb, sizeof(ConstantBuffer));

	samplerlin.SetSampler(dxg.pImmediateContext);

	rss.SetState(dxg.pImmediateContext);
	dss.SetState(dxg.pImmediateContext);
	bds.SetState(dxg.pImmediateContext);

	SetTexnull(dxg.pImmediateContext);

	dxg.RenderTargetSet(1, dxg.pRenderTargetView, dsbuf.pDepthStencilView, colf, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f);
	
}

void alldr() {
	
	dxg.pSwapChain->Present(0, 0);

}

void drvert(Vertex *vert, unsigned int vertcount, WORD *ind, unsigned int indcount, unsigned int useind, unsigned int startind, unsigned int startvert, ID3D11ShaderResourceView* texview, D3D11_VIEWPORT *view) {

	dxg.RenderSet();

	if (vertcount > vertmaxcount) {
		if (vbuff.Buffer != NULL) vbuff.Buffer->Release();
		vbuff.CreateBuffer(dxg.pDevice, sizeof(Vertex) * vertcount, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, sizeof(Vertex));
		vertmaxcount = vertcount;
	}
	if (indcount > indmaxcount) {
		if (ibuff.Buffer != NULL) ibuff.Buffer->Release();
		ibuff.CreateBuffer(dxg.pDevice, sizeof(WORD) * indcount, D3D11_USAGE_DYNAMIC, D3D11_BIND_INDEX_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, sizeof(WORD));
		indmaxcount = indcount;
	}


	vbuff.Dataset(dxg.pImmediateContext, vert, sizeof(Vertex) * vertcount);

	ibuff.Dataset(dxg.pImmediateContext, ind, sizeof(WORD) * indcount);



	if (texview != NULL) {
		DrawSet(dxg.pImmediateContext, ilayout.Layout, vbuff.Buffer, ibuff.Buffer, vshader.Shader, pshader.Shader, cbuff.Buffer, view);
	}
	else {
		DrawSet(dxg.pImmediateContext, ilayout.Layout, vbuff.Buffer, ibuff.Buffer, vshader.Shader, dxg.nottexps, cbuff.Buffer, view);
	}

	dxg.pImmediateContext->PSSetShaderResources(0, 1, &texview);

	DrawBuffer(dxg.pImmediateContext, useind, startind, startvert);
}

void maingrRelease() {
	vbuff.ReleaseBuffer();
	ibuff.ReleaseBuffer();
	cbuff.ReleaseBuffer();

	vshader.ReleaseShader();
	pshader.ReleaseShader();
	
	dsbuf.ReleseBuffer();

	sampler.ReleaseSampler();
	samplerlin.ReleaseSampler();

	ilayout.DescDelete();

	rss.ReleaseState();
	dss.ReleaseState();
	bds.ReleaseState();

}

Fpschecker fpscheck;
Runtimechecker timecheck;

Runtimechecker oneseccheck;
int onesecflc;
int onesecflcmem;

double countresk = 0;
double countall = 0;
double reskadd = 0;
double waittime = 0;

void limitfps() {
	countresk = timecheck.Countresult();
	waittime = ((16 * FPS_LIMIT) - countresk);
	if (reskadd != 0) {
		waittime += reskadd;
	}
	if (waittime <= -10 || waittime >= 1000) {
		waittime = 0;
	}
	if (reskadd <= -10 || reskadd >= 1000) {
		reskadd = 0;
	}

	timecheck.Countstart();
	if (waittime > 1) {
		timeBeginPeriod(1);
		Sleep((int)waittime);
		timeEndPeriod(1);
	}


	countall = timecheck.Countresult();
	reskadd += (16.666666666666 * FPS_LIMIT) - (countall + countresk);

	oneseccheck.Countresult();
	resultfps = fpscheck.Fpscheck();
	onesecflc++;
	if (fpscheck.fpsflame == 60) {
		oldflame[0] = fpscheck.fpsflame;
		oldflame[1] = checki;
		checki = 0;
	}
	timecheck.Countstart();
}

