#ifndef INCLUDED_MAIN
#define INCLUDED_MAIN

#include "./pcap/packet.h"
#include "./pcap/analysis.h"
#include "./graphic/skinmanager/skinmanager.h"
#include "./DirectX/lib/DX3D_11.h"
#include "./DirectX/lib/Grstr.h"
#include "./DirectX/lib/Inputmain.h"
#include "./DirectX/lib/keycode.h"
#include "./DirectX/lib/Fps.h"

#if _DEBUG
#include "debug_code\debug.h"
#endif

#include "keyinput.h"

#define VERSION "Observer 2021.09.0003"

#define FPS_LIMIT 60.0 / 60.0


extern HWND hWnd;
extern MSG msg;
extern HDC hdc;
extern HINSTANCE hInst;
extern WNDCLASSEX wc;
extern HRESULT hr;

extern RECT rw, rc;

extern DX113D dxg;

extern DX11PShader pshader;
extern DX11VShader vshader;

extern DX11InputLayout ilayout;
extern D3D11_VIEWPORT view;
extern DX11DSBuffer dsbuf;

extern Vertex defaultvert[4];
extern WORD defaultind[6];

extern DX11Buffeer vbuff;
extern DX11Buffeer ibuff;
extern int vertmaxcount;
extern int indmaxcount;

extern XMMATRIX worldMatrix;

extern XMVECTOR eye;
extern XMVECTOR focus;
extern XMVECTOR up;
extern XMMATRIX viewMatrix;

extern float fov;
extern float aspect;
extern float nearZ;
extern float farZ;
extern XMMATRIX projMatrix;

extern DX11Buffeer cbuff;

extern D3D11_BUFFER_DESC cbDesc;

extern ConstantBuffer cb;

extern DX11SAMPLER sampler;
extern DX11SAMPLER samplerlin;

extern DX11RSState rss;
extern DX11DSState dss;
extern DX11BDState bds;

extern float eyeposz;

extern DX8Input dip;
extern DX8Keybord dkb;
extern DX8Mouse dms;
extern DX8Pad dpd;

extern gstrclass gstr;
extern gstrclass gstrinner;

extern bool quitf;
extern bool resetf;

extern bool threadfinf;

extern DX113D dxg;

extern void maingrset();
extern void drvert(Vertex *vert, int vertcount, WORD *ind, int indcount, int useind, int startind, int startvert, ID3D11ShaderResourceView* texview, D3D11_VIEWPORT *view);

#endif