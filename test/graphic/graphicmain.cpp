#include <WindowsX.h>

#include <tchar.h>
#include <crtdbg.h>
#include <time.h>
#include <iostream>

#include "../main.h"

#include "visobj.h"
#include "../pcap/packet.h"
#include "graphicmain.h"
#include "../sidecharm/charmmain.h"

DX11RendTex screentex;
int screentexsize;
int frontx, fronty;
Vertex screenvert[20];
WORD screenind[30];
DX11RendTex areatex;
vertclass areav;
int areadrcount;

MarkerMasterclass marker;
int markerdrcount;

LineMasterclass line;
int linedrcount;

PacketMasterclass packet;
int packetdrcount;

RoutingMasterclass route;

MenuAreaMasterclass area;

//gstrclass gmainstr;

//DX11Tex objtex;

Pos bgpos;
double bgtu[2], bgtv[2];
Vertex bgvert[4];
WORD bgind[6];

vertclass markerv;
vertclass linev;
vertclass packetv;

vertclass sizegraphv;
vertclass sizegrapbghv;
vertclass protocolgraphv;
vertclass systemtxtv;

vertclass buttonv;
int buttondrcount;

vertclass selectboxv;
int selectboxdrcount;

vertclass selectboxtxtv;
int selectboxtxtdrcount;

vertclass textboxv;
int textboxdrcount;

Pos frontpos;


double viewx;
double viewxresult;
double vxmaxlength;
double viewxx;

int skinid;

void hostmarkerset();

void screenblur();
void areavset(Vertex vert[]);

void bgvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv, DWORD col);

bool hostcheckedf;

void graphicinit() {

	screentexsize = rc.right;
	frontx = screentexsize / 3;
	fronty = screentexsize / 3.5;
	areatex.CreateTex(dxg.pDevice, D3D11_USAGE_DEFAULT, DXGI_FORMAT_B8G8R8X8_TYPELESS, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		screentexsize, screentexsize, 0, 1, 1, 1, 0, DXGI_FORMAT_B8G8R8X8_UNORM);
	screentex.CreateTex(dxg.pDevice, D3D11_USAGE_DEFAULT, DXGI_FORMAT_B8G8R8X8_TYPELESS, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		screentexsize, screentexsize, 0, 1, 1, 1, 0, DXGI_FORMAT_B8G8R8X8_UNORM);

	//gmainstr.Stringcreate(dxg.pDevice, _T("defaultgraph\\textlight.png"), 102, 224, 2048, 2048, 20, 0, 0);

	//objtex.TexLoadFromFile(dxg.pDevice, _T("defaultgraph\\defaulttex.png"));

	
	skinid = 0;

	markerv.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	markerv.vertc = 0;
	markerv.vertcmax = 0;
	markerv.ind = (WORD *)malloc(sizeof(WORD) * 6);
	markerv.indc = 0;
	markerv.indcmax = 0;

	linev.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	linev.vertc = 0;
	linev.vertcmax = 0;
	linev.ind = (WORD *)malloc(sizeof(WORD) * 6);
	linev.indc = 0;
	linev.indcmax = 0;

	packetv.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	packetv.vertc = 0;
	packetv.vertcmax = 0;
	packetv.ind = (WORD *)malloc(sizeof(WORD) * 6);
	packetv.indc = 0;
	packetv.indcmax = 0;

	sizegraphv.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	sizegraphv.vertc = 0;
	sizegraphv.vertcmax = 0;
	sizegraphv.ind = (WORD *)malloc(sizeof(WORD) * 6);
	sizegraphv.indc = 0;
	sizegraphv.indcmax = 0;

	sizegrapbghv.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	sizegrapbghv.vertc = 0;
	sizegrapbghv.vertcmax = 0;
	sizegrapbghv.ind = (WORD *)malloc(sizeof(WORD) * 6);
	sizegrapbghv.indc = 0;
	sizegrapbghv.indcmax = 0;

	protocolgraphv.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	protocolgraphv.vertc = 0;
	protocolgraphv.vertcmax = 0;
	protocolgraphv.ind = (WORD *)malloc(sizeof(WORD) * 6);
	protocolgraphv.indc = 0;
	protocolgraphv.indcmax = 0;


	markerdrcount = 0;
	linedrcount = 0;
	packetdrcount = 0;


	buttonv.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	buttonv.vertc = 0;
	buttonv.vertcmax = 0;
	buttonv.ind = (WORD *)malloc(sizeof(WORD) * 6);
	buttonv.indc = 0;
	buttonv.indcmax = 0;

	selectboxv.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	selectboxv.vertc = 0;
	selectboxv.vertcmax = 0;
	selectboxv.ind = (WORD *)malloc(sizeof(WORD) * 6);
	selectboxv.indc = 0;
	selectboxv.indcmax = 0;

	selectboxtxtv.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	selectboxtxtv.vertc = 0;
	selectboxtxtv.vertcmax = 0;
	selectboxtxtv.ind = (WORD *)malloc(sizeof(WORD) * 6);
	selectboxtxtv.indc = 0;
	selectboxtxtv.indcmax = 0;

	textboxv.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	textboxv.vertc = 0;
	textboxv.vertcmax = 0;
	textboxv.ind = (WORD *)malloc(sizeof(WORD) * 6);
	textboxv.indc = 0;
	textboxv.indcmax = 0;

	systemtxtv.vert = (Vertex *)malloc(sizeof(Vertex) * 4);
	systemtxtv.vertc = 0;
	systemtxtv.vertcmax = 0;
	systemtxtv.ind = (WORD *)malloc(sizeof(WORD) * 6);
	systemtxtv.indc = 0;
	systemtxtv.indcmax = 0;


	buttondrcount = 0;
	selectboxdrcount = 0;
	selectboxtxtdrcount = 0;
	textboxdrcount = 0;


	viewx = 1.0;
	viewxresult = 1.0;
	vxmaxlength = 0.0;
	viewxx = 1.0;

	hostcheckedf = 0;

	frontpos.x = 0;
	frontpos.y = 0;


	



	screenvert[0].x = 0;
	screenvert[0].y = 0;
	screenvert[0].z = 0.0f;

	screenvert[1].x = view.Width;
	screenvert[1].y = view.Height;
	screenvert[1].z = 0.0f;

	screenvert[2].x = 0;
	screenvert[2].y = view.Height;
	screenvert[2].z = 0.0f;

	screenvert[3].x = view.Width;
	screenvert[3].y = 0;
	screenvert[3].z = 0.0f;

	Set2DPosvert(screenvert, view.Width, view.Height, 4, 0, cb.view, cb.projection);

	screenvert[0].tu = 0.0f;
	screenvert[0].tv = 0.0f;
	screenvert[0].color[0] = 1.0f;
	screenvert[0].color[1] = 1.0f;
	screenvert[0].color[2] = 1.0f;
	screenvert[0].color[3] = 1.0f;

	screenvert[1].tu = view.Width / screentexsize;
	screenvert[1].tv = view.Height / screentexsize;
	screenvert[1].color[0] = 1.0f;
	screenvert[1].color[1] = 1.0f;
	screenvert[1].color[2] = 1.0f;
	screenvert[1].color[3] = 1.0f;

	screenvert[2].tu = 0.0f;
	screenvert[2].tv = view.Height / screentexsize;
	screenvert[2].color[0] = 1.0f;
	screenvert[2].color[1] = 1.0f;
	screenvert[2].color[2] = 1.0f;
	screenvert[2].color[3] = 1.0f;

	screenvert[3].tu = view.Width / screentexsize;
	screenvert[3].tv = 0.0f;
	screenvert[3].color[0] = 1.0f;
	screenvert[3].color[1] = 1.0f;
	screenvert[3].color[2] = 1.0f;
	screenvert[3].color[3] = 1.0f;

	screenind[0] = 0;
	screenind[1] = 1;
	screenind[2] = 2;
	screenind[3] = 0;
	screenind[4] = 3;
	screenind[5] = 1;


	view.Height = (float)screentexsize;
	view.Width = (float)screentexsize;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	fov = XMConvertToRadians(45.0f);
	aspect = screentexsize / screentexsize;
	nearZ = 1.0f;
	farZ = 100.0f;
	projMatrix = XMMatrixOrthographicLH(screentexsize, screentexsize, nearZ, farZ);

	eye = XMVectorSet(0.0f, 0.0f, eyeposz, 0.0f);
	focus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	viewMatrix = XMMatrixLookAtLH(eye, focus, up);

	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(worldMatrix));
	XMStoreFloat4x4(&cb.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(projMatrix));
	cbuff.Dataset(dxg.pImmediateContext, &cb, sizeof(ConstantBuffer));

	rss.SetState(dxg.pImmediateContext);
	dss.SetState(dxg.pImmediateContext);
	bds.SetState(dxg.pImmediateContext);

	screenvert[4].x = 0 - BLUR_MARGE;
	screenvert[4].y = 0;
	screenvert[4].z = 0.0f;

	screenvert[5].x = screentexsize - BLUR_MARGE;
	screenvert[5].y = screentexsize;
	screenvert[5].z = 0.0f;

	screenvert[6].x = 0 - BLUR_MARGE;
	screenvert[6].y = screentexsize;
	screenvert[6].z = 0.0f;

	screenvert[7].x = screentexsize - BLUR_MARGE;
	screenvert[7].y = 0;
	screenvert[7].z = 0.0f;


	screenvert[8].x = 0 + BLUR_MARGE;
	screenvert[8].y = 0;
	screenvert[8].z = 0.0f;

	screenvert[9].x = screentexsize + BLUR_MARGE;
	screenvert[9].y = screentexsize;
	screenvert[9].z = 0.0f;

	screenvert[10].x = 0 + BLUR_MARGE;
	screenvert[10].y = screentexsize;
	screenvert[10].z = 0.0f;

	screenvert[11].x = screentexsize + BLUR_MARGE;
	screenvert[11].y = 0;
	screenvert[11].z = 0.0f;


	screenvert[12].x = 0;
	screenvert[12].y = 0 - BLUR_MARGE;
	screenvert[12].z = 0.0f;

	screenvert[13].x = screentexsize;
	screenvert[13].y = screentexsize - BLUR_MARGE;
	screenvert[13].z = 0.0f;

	screenvert[14].x = 0;
	screenvert[14].y = screentexsize - BLUR_MARGE;
	screenvert[14].z = 0.0f;

	screenvert[15].x = screentexsize;
	screenvert[15].y = 0 - BLUR_MARGE;
	screenvert[15].z = 0.0f;
	

	screenvert[16].x = 0;
	screenvert[16].y = 0 + BLUR_MARGE;
	screenvert[16].z = 0.0f;

	screenvert[17].x = screentexsize;
	screenvert[17].y = screentexsize + BLUR_MARGE;
	screenvert[17].z = 0.0f;

	screenvert[18].x = 0;
	screenvert[18].y = screentexsize + BLUR_MARGE;
	screenvert[18].z = 0.0f;

	screenvert[19].x = screentexsize;
	screenvert[19].y = 0 + BLUR_MARGE;
	screenvert[19].z = 0.0f;

	Set2DPosvert(screenvert, screentexsize, screentexsize, 20, 4, cb.view, cb.projection);

	for (int i = 1; i < 5; i++) {

		screenvert[(i * 4) + 0].tu = 0.0f;
		screenvert[(i * 4) + 0].tv = 0.0f;
		screenvert[(i * 4) + 0].color[0] = 1.0f;
		screenvert[(i * 4) + 0].color[1] = 1.0f;
		screenvert[(i * 4) + 0].color[2] = 1.0f;
		screenvert[(i * 4) + 0].color[3] = 0.3f;

		screenvert[(i * 4) + 1].tu = 1.0f;
		screenvert[(i * 4) + 1].tv = 1.0f;
		screenvert[(i * 4) + 1].color[0] = 1.0f;
		screenvert[(i * 4) + 1].color[1] = 1.0f;
		screenvert[(i * 4) + 1].color[2] = 1.0f;
		screenvert[(i * 4) + 1].color[3] = 0.3f;

		screenvert[(i * 4) + 2].tu = 0.0f;
		screenvert[(i * 4) + 2].tv = 1.0f;
		screenvert[(i * 4) + 2].color[0] = 1.0f;
		screenvert[(i * 4) + 2].color[1] = 1.0f;
		screenvert[(i * 4) + 2].color[2] = 1.0f;
		screenvert[(i * 4) + 2].color[3] = 0.3f;

		screenvert[(i * 4) + 3].tu = 1.0f;
		screenvert[(i * 4) + 3].tv = 0.0f;
		screenvert[(i * 4) + 3].color[0] = 1.0f;
		screenvert[(i * 4) + 3].color[1] = 1.0f;
		screenvert[(i * 4) + 3].color[2] = 1.0f;
		screenvert[(i * 4) + 3].color[3] = 0.3f;

		screenind[(i * 6) + 0] = (i * 4) + 0;
		screenind[(i * 6) + 1] = (i * 4) + 1;
		screenind[(i * 6) + 2] = (i * 4) + 2;
		screenind[(i * 6) + 3] = (i * 4) + 0;
		screenind[(i * 6) + 4] = (i * 4) + 3;
		screenind[(i * 6) + 5] = (i * 4) + 1;
		
	}


	bgvert[0].x = 0;
	bgvert[0].y = 0;
	bgvert[0].z = 0.0f;

	bgvert[1].x = screentexsize;
	bgvert[1].y = screentexsize;
	bgvert[1].z = 0.0f;

	bgvert[2].x = 0;
	bgvert[2].y = screentexsize;
	bgvert[2].z = 0.0f;

	bgvert[3].x = screentexsize;
	bgvert[3].y = 0;
	bgvert[3].z = 0.0f;

	Set2DPosvert(bgvert, screentexsize, screentexsize, 4, 0, cb.view, cb.projection);

	bgvert[0].tu = 0.0f;
	bgvert[0].tv = 0.0f;
	bgvert[0].color[0] = 1.0f;
	bgvert[0].color[1] = 1.0f;
	bgvert[0].color[2] = 1.0f;
	bgvert[0].color[3] = 1.0f;

	bgvert[1].tu = view.Width / 1280;
	bgvert[1].tv = view.Height / 1280;
	bgvert[1].color[0] = 1.0f;
	bgvert[1].color[1] = 1.0f;
	bgvert[1].color[2] = 1.0f;
	bgvert[1].color[3] = 1.0f;

	bgvert[2].tu = 0.0f;
	bgvert[2].tv = view.Height / 1280;
	bgvert[2].color[0] = 1.0f;
	bgvert[2].color[1] = 1.0f;
	bgvert[2].color[2] = 1.0f;
	bgvert[2].color[3] = 1.0f;

	bgvert[3].tu = view.Width / 1280;
	bgvert[3].tv = 0.0f;
	bgvert[3].color[0] = 1.0f;
	bgvert[3].color[1] = 1.0f;
	bgvert[3].color[2] = 1.0f;
	bgvert[3].color[3] = 1.0f;

	bgind[0] = 0;
	bgind[1] = 1;
	bgind[2] = 2;
	bgind[3] = 0;
	bgind[4] = 3;
	bgind[5] = 1;



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


	area.reset();
	Pos posbuff;
	
	posbuff.x = view.Width;
	posbuff.y = 0;
	posbuff.z = 0;

	areav.vertcmax = 0;
	areav.vertc = 0;
	areav.indcmax = 0;
	areav.indc = 0;

	area.jenerate(posbuff, 0, 0, 0, 0, view.Height);
}



void changeViewx() {
	double vxbuff;

	if (dms.InputMouse(2) == 1) {
		frontpos.x += (dms.pos.x - oldpos.x) * (screentexsize / rc.right) / viewx;
		frontpos.y += (dms.pos.y - oldpos.y) * (screentexsize / rc.bottom) / viewx;
	}
	if (dkb.GetKeySt(KEY_R) == 1) {
		frontpos.x = 0;
		frontpos.y = 0;
	}

	

	viewxx += mousewheel / 2000.0;
	

	/*if (dkb.GetKeySt(KEY_LSHIFT) == 1) {
		if (viewxx > 0.001 && dkb.GetKeySt(KEY_DOWN) == 1) {
			viewxx -= 0.005;
		}
		if (dkb.GetKeySt(KEY_UP) == 1) {
			viewxx += 0.005;
		}

		if (viewxx > 0.001 && dkb.GetKeySt(KEY_LEFT) == 1) {
			viewxx -= 0.025;
		}
		if (dkb.GetKeySt(KEY_RIGHT) == 1) {
			viewxx += 0.025;
		}
	}
	else {
		if (dkb.GetKeySt(KEY_UP) == 1) {
			frontpos.y -= 25;
		}
		if (dkb.GetKeySt(KEY_DOWN) == 1) {
			frontpos.y += 25;
		}

		if (dkb.GetKeySt(KEY_LEFT) == 1) {
			frontpos.x -= 25;
		}
		if (dkb.GetKeySt(KEY_RIGHT) == 1) {
			frontpos.x += 25;
		}
	}*/


	/*if (markerdrcount < 9) {
		vxbuff = 1.0;
	}
	else {
		vxbuff = 1.0 / ((markerdrcount - 2) / 8) * (1.0 + (((markerdrcount - 2) / 8) / 3.5));
		if (vxbuff > 1.0) vxbuff = 1.0;
	}*/
	if (vxmaxlength > 0) {
		vxbuff = fronty / vxmaxlength;
		if (vxbuff > 1) vxbuff = 1.0;
		if (vxbuff < 0.01) vxbuff = 0.01;
	}
	else {
		vxbuff = 1.0;
	}
	vxmaxlength = 0;

	if (viewxresult > vxbuff) {

		double vxmovebuff = viewxresult - vxbuff;
		if (viewxresult - vxbuff > 0.0001) vxmovebuff /= 50.0;

		viewxresult -= vxmovebuff;

	}
	else if (viewxresult < vxbuff) {
		double vxmovebuff = vxbuff - viewxresult;
		if (vxbuff - viewxresult > 0.0001) vxmovebuff /= 50.0;

		viewxresult += vxmovebuff;
	}

	if (viewxresult * viewxx < 0.01) {
		viewxx = 0.01 / viewxresult;
	}

	viewx = viewxresult * viewxx;

	if (viewx < 0.01) {
		viewx = 0.01;
	}

#if _DEBUG
	gstr.Stringset(20, 60, 18, 24, 0, 0, 6, 0xFF000000 + skinset.SRGB, 0, _T("viewx : %5.3lf"), viewx);
	gstr.Stringset(20, 85, 18, 24, 0, 0, 6, 0xFF000000 + skinset.SRGB, 0, _T("vbuffx : %5.3lf"), vxbuff);
	gstr.Stringset(20, 110, 18, 24, 0, 0, 6, 0xFF000000 + skinset.SRGB, 0, _T("newcount : %d"), pdata.newdatacount);
	gstr.Stringset(20, 135, 18, 24, 0, 0, 6, 0xFF000000 + skinset.SRGB, 0, _T("front x : %5.3lf y : %5.3lf"), frontpos.x, frontpos.y);
#endif
}

void setscreentex() {
	float colf[4] = { 0.0f,0.0f,0.0f,0.0f };

	view.Height = (float)screentexsize;
	view.Width = (float)screentexsize;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	fov = XMConvertToRadians(45.0f);
	aspect = screentexsize / screentexsize;
	nearZ = 1.0f;
	farZ = 100.0f;
	projMatrix = XMMatrixOrthographicLH(screentexsize, screentexsize, nearZ, farZ);

	eye = XMVectorSet(0.0f, 0.0f, eyeposz, 0.0f);
	focus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	viewMatrix = XMMatrixLookAtLH(eye, focus, up);

	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(worldMatrix));
	XMStoreFloat4x4(&cb.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(projMatrix));
	cbuff.Dataset(dxg.pImmediateContext, &cb, sizeof(ConstantBuffer));

	rss.SetState(dxg.pImmediateContext);
	dss.SetState(dxg.pImmediateContext);
	bds.SetState(dxg.pImmediateContext);

	dxg.RenderSet();

	SetTexnull(dxg.pImmediateContext);
	dxg.RenderTargetSet(1, screentex.rtview, screentex.dsview, colf, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f);

	
	 
}

void setareatex() {
	float colf[4] = { 0.5f,0.5f,0.5f,1.0f };

	view.Height = (float)screentexsize;
	view.Width = (float)screentexsize;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	fov = XMConvertToRadians(45.0f);
	aspect = screentexsize / screentexsize;
	nearZ = 1.0f;
	farZ = 100.0f;
	projMatrix = XMMatrixOrthographicLH(screentexsize, screentexsize, nearZ, farZ);

	eye = XMVectorSet(0.0f, 0.0f, eyeposz, 0.0f);
	focus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	viewMatrix = XMMatrixLookAtLH(eye, focus, up);

	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(worldMatrix));
	XMStoreFloat4x4(&cb.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(projMatrix));
	cbuff.Dataset(dxg.pImmediateContext, &cb, sizeof(ConstantBuffer));

	rss.SetState(dxg.pImmediateContext);
	dss.SetState(dxg.pImmediateContext);
	bds.SetState(dxg.pImmediateContext);

	dxg.RenderSet();

	SetTexnull(dxg.pImmediateContext);
	dxg.RenderTargetSet(1, areatex.rtview, areatex.dsview, colf, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f);

}

void setmaincharmpos(double leftpos) {
	int pxybuff[4];

	for (int areai = 0; areai < area.usecount; areai++) {
		if (area.data[areai].GetId() == 0) {
			for (int i = 0; i < 4; i++) {
				pxybuff[i] = area.data[areai].GetPosxy(i);
			}

			if (view.Width - pxybuff[0] > leftpos) {
				if ((view.Width - pxybuff[0]) - leftpos >= 0.05) {
					double posbuff = (view.Width - pxybuff[0]) - leftpos;
					if (posbuff > 0.05) posbuff /= 20.0;
					pxybuff[0] += posbuff;
				}
			}
			else if (view.Width - pxybuff[0] < leftpos) {
				if (leftpos - (view.Width - pxybuff[0]) >= 0.05) {

					double posbuff = leftpos - (view.Width - pxybuff[0]);
					if (posbuff > 0.05) posbuff /= 20.0;
					pxybuff[0] -= posbuff;
				}
			}

			area.data[areai].SetPos(area.data[areai].GetPos(), 0, pxybuff[0], pxybuff[1], pxybuff[2], pxybuff[3]);
			break;
		}
	}
}

void areaposcheck() {
	areadrcount = 0;
	for (int i = 0; i < area.usecount; i++) {
		area.data[i].CheckPos(cb.view, cb.projection, view.Width, view.Height);
		areavset(area.data[i].vert);
	}

	area.remove();
}

//main
void graphicmain() {
	setscreentex();
	if (deviceselected == 2) {
		changeViewx();
	}
	else {
		viewx = 1.0;
		viewxresult = 1.0;
		vxmaxlength = 0.0;
		viewxx = 1.0;

		hostcheckedf = 0;

		frontpos.x = 0;
		frontpos.y = 0;

		for (int i = 0; i < area.usecount; i++) {
			if (area.data[i].GetId() != 0) {
				area.data[i].Delete();
			}
		}
	}


	hostmarkerset();

	markerdrcount = 0;
	linedrcount = 0;
	packetdrcount = 0;


	VisobjtimeCheck();

	if (deviceselected == 2) {
		marker.move();
		line.move(marker);
		packet.move(marker);
	}

	bgpos.x = frontpos.x;
	bgpos.y = frontpos.y;
	bgpos.z = 0;

	bgtu[0] = 0.0;
	bgtu[1] = 1000.0;

	bgtv[0] = 0.0;
	bgtv[1] = 1000.0;

	bgvset(bgpos, screentexsize * 1000, screentexsize * 1000, 1.0, 0, bgtu, bgtv, 0xFFFFFFFF);

	Set2DPosvert(bgvert, screentexsize, screentexsize, 4, 0, cb.view, cb.projection);

	drvert(bgvert, 4, bgind, 6, 6, 0, 0, texman.Data(obj[29].GetId()), &view);


	if (line.getUsecount() > 0) {
		Set2DPosvert(linev.vert, screentexsize, screentexsize, linev.vertc, 0, cb.view, cb.projection);
		drvert(linev.vert, linev.vertc, linev.ind, linev.indc, linev.indc, 0, 0, texman.Data(obj[2].GetId()), &view);
		/*for (int i = 0; i < linev.vertc; i++) {
			gstr.Stringset(20, 120 + (24 * i), 18, 24, 0, 0, 5, 0xFFFFFFFF, 0, _T("%3d :x<%9.3f>y<%9.3f>z<%9.3f>"), i, linev.vert[i].x, linev.vert[i].y, linev.vert[i].z);
		}*/

		linev.vertc = 0;
		linev.indc = 0;
	}

	if (packet.getUsecount() > 0) {
		Set2DPosvert(packetv.vert, screentexsize, screentexsize, packetv.vertc, 0, cb.view, cb.projection);
		drvert(packetv.vert, packetv.vertc, packetv.ind, packetv.indc, packetv.indc, 0, 0, texman.Data(obj[3].GetId()), &view);
		/*for (int i = 0; i < packetv.vertc; i++) {
			gstr.Stringset(20, 120 + (24 * i), 18, 24, 0, 0, 5, 0xFFFFFFFF, 0, _T("%3d :x<%9.3f>y<%9.3f>z<%9.3f>"), i, packetv.vert[i].x, packetv.vert[i].y, packetv.vert[i].z);
		}*/
#if _DEBUG
		gstr.Stringset(20, 160, 18, 24, 0, 0, 5, 0xFF000000, 0, _T("count: %10d"), packet.getUsecount());
		gstr.Stringset(20, 180, 18, 24, 0, 0, 5, 0xFF000000, 0, _T("vert : %10d ( %10u )"), packetv.vertc, sizeof(Vertex) * packetv.vertc);
		gstr.Stringset(20, 200, 18, 24, 0, 0, 5, 0xFF000000, 0, _T("ind  : %10d ( %10u )"), packetv.indc, sizeof(DWORD) * packetv.indc);
#endif

		packetv.vertc = 0;
		packetv.indc = 0;
	}

	if (marker.getUsecount() > 0) {
		Set2DPosvert(markerv.vert, screentexsize, screentexsize, markerv.vertc, 0, cb.view, cb.projection);
		drvert(markerv.vert, markerv.vertc, markerv.ind, markerv.indc, markerv.indc, 0, 0, texman.Data(obj[1].GetId()), &view);
		/*for (int i = 0; i < markerv.vertc; i++) {
			gstr.Stringset(20, 120 + (24 * i), 18, 24, 0, 0, 5, 0xFFFFFFFF, 0, _T("%3d :x<%9.3f>y<%9.3f>z<%9.3f>"), i, markerv.vert[i].x, markerv.vert[i].y, markerv.vert[i].z);
		}*/
		
		markerv.vertc = 0;
		markerv.indc = 0;
	}

	Set2DPosvert(gstrinner.vert, screentexsize, screentexsize, gstrinner.vertc, 0, cb.view, cb.projection);
	drvert(gstrinner.vert, gstrinner.vertc, gstrinner.ind, gstrinner.indc, gstrinner.indc, 0, 0, texman.Data(obj[0].GetId()), &view);

	
	setareatex();

	drvert(bgvert, 4, bgind, 6, 6, 0, 0, texman.Data(obj[29].GetId()), &view);

	screenblur();

	maingrset();

	setmaincharmpos(charmleftpos);
	areaposcheck();

	drvert(screenvert, 20, screenind, 30, 6, 0, 0, screentex.view, &view);

}

void graphiccharmmain() {

	if (area.usecount > 0) {
		drvert(areav.vert, areav.vertc, areav.ind, areav.indc, areav.indc, 0, 0, areatex.view, &view);
		areav.vertc = 0;
		areav.indc = 0;
	}

	charmdrmain();

	if (protocolgraphv.vertc > 0 && protocolgraphv.indc > 0) {

		Set2DPosvert(protocolgraphv.vert, view.Width, view.Height, protocolgraphv.vertc, 0, cb.view, cb.projection);
		drvert(protocolgraphv.vert, protocolgraphv.vertc, protocolgraphv.ind, protocolgraphv.indc, protocolgraphv.indc, 0, 0, texman.Data(obj[9].GetId()), &view);

		protocolgraphv.vertc = 0;
		protocolgraphv.indc = 0;
	}


	if (sizegrapbghv.vertc > 0 && sizegrapbghv.indc > 0) {

		Set2DPosvert(sizegrapbghv.vert, view.Width, view.Height, sizegrapbghv.vertc, 0, cb.view, cb.projection);
		drvert(sizegrapbghv.vert, sizegrapbghv.vertc, sizegrapbghv.ind, sizegrapbghv.indc, sizegrapbghv.indc, 0, 0, texman.Data(obj[5].GetId()), &view);

		sizegrapbghv.vertc = 0;
		sizegrapbghv.indc = 0;
	}

	if (sizegraphv.vertc > 0 && sizegraphv.indc > 0) {

		Set2DPosvert(sizegraphv.vert, view.Width, view.Height, sizegraphv.vertc, 0, cb.view, cb.projection);
		drvert(sizegraphv.vert, sizegraphv.vertc, sizegraphv.ind, sizegraphv.indc, sizegraphv.indc, 0, 0, texman.Data(obj[5].GetId()), &view);

		/*gstr.Stringset(20, 120, 18, 24, 0, 0, 5, 0xFFFFFFFF, 0, _T("%d, %d"), sizegraphv.vertc, sizegraphv.indc);
		for (int i = 0; i < sizegraphv.indc; i++) {
		gstr.Stringset(20, 135 + (15 * i) , 15, 15, 0, 0, 5, 0xFFFFFFFF, 0, _T("[%2d] %2d"), i, sizegraphv.ind[i]);
		}*/

		sizegraphv.vertc = 0;
		sizegraphv.indc = 0;
	}

	if (deviceselected >= 0 && selecteddevice >= 0) {
		gstr.Stringset(view.Width / 2, view.Height - 20, 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 1, _T("%s[%d / %d]"), pdev.data[selecteddevice].FriendlyName, selecteddevice + 1, pdev.usecount);

		for (int i = 0; i < pdev.data[selecteddevice].hostipmax; i++) {
			if (hdata.data[pdev.data[selecteddevice].hostipid[i]].version == 4) {
				gstr.Stringset(view.Width / 2, view.Height - 20 - (20 * pdev.data[selecteddevice].hostipmax) + (20 * i), 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 1, _T("%d.%d.%d.%d"), hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[0], hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[1], hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[2], hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[3]);
			}
			else {
				gstr.Stringset(view.Width / 2, view.Height - 20 - (20 * pdev.data[selecteddevice].hostipmax) + (20 * i), 20, 20, 0, 0, 12, 0xFF000000 + skinset.SRGB, 1, _T("%x:%x:%x:%x:%x:%x:%x:%x"),
					hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[0], hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[1], hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[2], hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[3],
					hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[4], hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[5], hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[6], hdata.data[pdev.data[selecteddevice].hostipid[i]].ip[7]);
			}
		}
	}

	if (systemtxtv.vertc > 0 && systemtxtv.indc > 0) {

		Set2DPosvert(systemtxtv.vert, view.Width, view.Height, systemtxtv.vertc, 0, cb.view, cb.projection);
		drvert(systemtxtv.vert, systemtxtv.vertc, systemtxtv.ind, systemtxtv.indc, systemtxtv.indc, 0, 0, texman.Data(obj[28].GetId()), &view);

		systemtxtv.vertc = 0;
		systemtxtv.indc = 0;
	}


	if (button.GetUsecount() > 0) {
		Set2DPosvert(buttonv.vert, view.Width, view.Height, buttonv.vertc, 0, cb.view, cb.projection);
		drvert(buttonv.vert, buttonv.vertc, buttonv.ind, buttonv.indc, buttonv.indc, 0, 0, texman.Data(obj[19].GetId()), &view);

		buttonv.vertc = 0;
		buttonv.indc = 0;
	}

	if (selectbox.GetUsecount() > 0) {
		Set2DPosvert(selectboxv.vert, view.Width, view.Height, selectboxv.vertc, 0, cb.view, cb.projection);
		drvert(selectboxv.vert, selectboxv.vertc, selectboxv.ind, selectboxv.indc, selectboxv.indc, 0, 0, texman.Data(obj[25].GetId()), &view);

		selectboxv.vertc = 0;
		selectboxv.indc = 0;

		Set2DPosvert(selectboxtxtv.vert, view.Width, view.Height, selectboxtxtv.vertc, 0, cb.view, cb.projection);
		drvert(selectboxtxtv.vert, selectboxtxtv.vertc, selectboxtxtv.ind, selectboxtxtv.indc, selectboxtxtv.indc, 0, 0, texman.Data(obj[26].GetId()), &view);

		selectboxtxtv.vertc = 0;
		selectboxtxtv.indc = 0;
	}


	if (textbox.GetUsecount() > 0) {
		Set2DPosvert(textboxv.vert, view.Width, view.Height, textboxv.vertc, 0, cb.view, cb.projection);
		drvert(textboxv.vert, textboxv.vertc, textboxv.ind, textboxv.indc, textboxv.indc, 0, 0, texman.Data(obj[27].GetId()), &view);

		textboxv.vertc = 0;
		textboxv.indc = 0;
	}

	Set2DPosvert(gstr.vert, view.Width, view.Height, gstr.vertc, 0, cb.view, cb.projection);
	drvert(gstr.vert, gstr.vertc, gstr.ind, gstr.indc, gstr.indc, 0, 0, texman.Data(obj[0].GetId()), &view);
	gstr.StringCountReset();

	buttondrcount = 0;
	selectboxdrcount = 0;
	textboxdrcount = 0;
	selectboxtxtdrcount = 0;

}


void hostmarkerset() {
	int dummyip[8] = { 0,0,0,0,0,0,0,0 };
	if ((pdatacheckf & 0b00010) == 0b00000) {
		if (pdrdata.newdatacount >= 1) {
			for (int i = 0; i < hdata.usecount; i++) {
				if (hdata.data[i].adapterNo == pdev.data[selecteddevice].adapterNo) {
					for (int j = 0; j < pdrdata.newdatacount; j++) {
						if ((pdrdata.newdata[j].checked & 0b00010) == 0b00000) {
							if (!IpMatchCheck(pdrdata.newdata[j].fromip, hdata.data[i].ip)) {
								marker.jenerate(hdata.data[i].ip, hdata.data[i].version, dummyip);
							}

							if (!IpMatchCheck(pdrdata.newdata[j].atip, hdata.data[i].ip)) {
								marker.jenerate(hdata.data[i].ip, hdata.data[i].version, dummyip);
							}

						}
						pdrdata.newdata[j].checked |= 0b00010;
					}
				}
			}
		}
		pdatacheckf |= 0b00010;
	}
}


void bgvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv, DWORD col) {

	bgvert[0].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	bgvert[0].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	bgvert[0].z = 0.0f;

	bgvert[1].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	bgvert[1].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	bgvert[1].z = 0.0f;


	bgvert[2].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	bgvert[2].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	bgvert[2].z = 0.0f;


	bgvert[3].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	bgvert[3].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	bgvert[3].z = 0.0f;



	bgvert[0].tu = tu[0];
	bgvert[0].tv = tv[0];
	bgvert[1].tu = tu[1];
	bgvert[1].tv = tv[1];
	bgvert[2].tu = tu[0];
	bgvert[2].tv = tv[1];
	bgvert[3].tu = tu[1];
	bgvert[3].tv = tv[0];

	for (int i = 0; i < 4; i++) {
		bgvert[i].x *= viewx;
		bgvert[i].y *= viewx;

		bgvert[i].x += frontx % (screentexsize);
		bgvert[i].y += fronty % (screentexsize);

		ColorconvF(bgvert[i].color, col);
	}

}

void markervset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv, DWORD col) {
	if (markerv.vertc + 4 <= 65535 && markerv.indc + 6 <= 65535) {
		markerv.vertc += 4;
		markerv.indc += 6;
	}

	if (markerv.vertc > markerv.vertcmax) {
		markerv.vertcmax = markerv.vertc;
		markerv.indcmax = markerv.indc;

		markerv.vert = (Vertex *)realloc(markerv.vert, sizeof(Vertex) * markerv.vertc);
		markerv.ind = (WORD *)realloc(markerv.ind, sizeof(WORD) * markerv.indc);

		markerv.ind[0 + (markerv.indc - 6)] = 0 + (markerv.vertc - 4);
		markerv.ind[1 + (markerv.indc - 6)] = 1 + (markerv.vertc - 4);
		markerv.ind[2 + (markerv.indc - 6)] = 2 + (markerv.vertc - 4);
		markerv.ind[3 + (markerv.indc - 6)] = 0 + (markerv.vertc - 4);
		markerv.ind[4 + (markerv.indc - 6)] = 3 + (markerv.vertc - 4);
		markerv.ind[5 + (markerv.indc - 6)] = 1 + (markerv.vertc - 4);
	}

	markerv.vert[(markerv.vertc - 4) + 0].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	markerv.vert[(markerv.vertc - 4) + 0].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	markerv.vert[(markerv.vertc - 4) + 0].z = 0.0f;

	markerv.vert[(markerv.vertc - 4) + 1].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	markerv.vert[(markerv.vertc - 4) + 1].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	markerv.vert[(markerv.vertc - 4) + 1].z = 0.0f;


	markerv.vert[(markerv.vertc - 4) + 2].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	markerv.vert[(markerv.vertc - 4) + 2].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	markerv.vert[(markerv.vertc - 4) + 2].z = 0.0f;


	markerv.vert[(markerv.vertc - 4) + 3].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	markerv.vert[(markerv.vertc - 4) + 3].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	markerv.vert[(markerv.vertc - 4) + 3].z = 0.0f;



	markerv.vert[(markerv.vertc - 4) + 0].tu = tu[0];
	markerv.vert[(markerv.vertc - 4) + 0].tv = tv[0];
	markerv.vert[(markerv.vertc - 4) + 1].tu = tu[1];
	markerv.vert[(markerv.vertc - 4) + 1].tv = tv[1];
	markerv.vert[(markerv.vertc - 4) + 2].tu = tu[0];
	markerv.vert[(markerv.vertc - 4) + 2].tv = tv[1];
	markerv.vert[(markerv.vertc - 4) + 3].tu = tu[1];
	markerv.vert[(markerv.vertc - 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {
		markerv.vert[(markerv.vertc - 4) + i].x *= viewx;
		markerv.vert[(markerv.vertc - 4) + i].y *= viewx;

		markerv.vert[(markerv.vertc - 4) + i].x += frontx;
		markerv.vert[(markerv.vertc - 4) + i].y += fronty;

		ColorconvF(markerv.vert[(markerv.vertc - 4) + i].color, col);
	}

	markerdrcount++;
}

void linevset(Pos pos1, Pos pos2, double sx, double size, double angl, double *tu, double *tv, DWORD col) {
	if (linev.vertc + 4 <= 65535 && linev.indc + 6 <= 65535) {
		linev.vertc += 4;
		linev.indc += 6;
	}
	if (linev.vertc > linev.vertcmax) {
		linev.vertcmax = linev.vertc;
		linev.indcmax = linev.indc;

		linev.vert = (Vertex *)realloc(linev.vert, sizeof(Vertex) * linev.vertc);
		linev.ind = (WORD *)realloc(linev.ind, sizeof(WORD) * linev.indc);

		linev.ind[0 + (linev.indc - 6)] = 0 + (linev.vertc - 4);
		linev.ind[1 + (linev.indc - 6)] = 1 + (linev.vertc - 4);
		linev.ind[2 + (linev.indc - 6)] = 2 + (linev.vertc - 4);
		linev.ind[3 + (linev.indc - 6)] = 0 + (linev.vertc - 4);
		linev.ind[4 + (linev.indc - 6)] = 3 + (linev.vertc - 4);
		linev.ind[5 + (linev.indc - 6)] = 1 + (linev.vertc - 4);
	}

	linev.vert[(linev.vertc - 4) + 0].x = pos1.x + ((sx / 2) * cos((angl - 90) * PI / 180.0) * size);
	linev.vert[(linev.vertc - 4) + 0].y = pos1.y + ((sx / 2) * sin((angl - 90) * PI / 180.0) * size);
	linev.vert[(linev.vertc - 4) + 0].z = 0.0f;

	linev.vert[(linev.vertc - 4) + 1].x = pos2.x + ((sx / 2) * cos((angl + 90) * PI / 180.0) * size);
	linev.vert[(linev.vertc - 4) + 1].y = pos2.y + ((sx / 2) * sin((angl + 90) * PI / 180.0) * size);
	linev.vert[(linev.vertc - 4) + 1].z = 0.0f;


	linev.vert[(linev.vertc - 4) + 2].x = pos1.x + ((sx / 2) * cos((angl + 90) * PI / 180.0) * size);
	linev.vert[(linev.vertc - 4) + 2].y = pos1.y + ((sx / 2) * sin((angl + 90) * PI / 180.0) * size);
	linev.vert[(linev.vertc - 4) + 2].z = 0.0f;


	linev.vert[(linev.vertc - 4) + 3].x = pos2.x + ((sx / 2) * cos((angl - 90) * PI / 180.0) * size);
	linev.vert[(linev.vertc - 4) + 3].y = pos2.y + ((sx / 2) * sin((angl - 90) * PI / 180.0) * size);
	linev.vert[(linev.vertc - 4) + 3].z = 0.0f;

	double uvbufx, uvbufy, uvbuflen;

	uvbufx = (pos1.x - pos2.x) * (pos1.x - pos2.x);
	uvbufy = (pos1.y - pos2.y) * (pos1.y - pos2.y);
	uvbuflen = sqrt(uvbufx + uvbufy);


	linev.vert[(linev.vertc - 4) + 0].tu = tu[0];
	linev.vert[(linev.vertc - 4) + 0].tv = tv[0];
	linev.vert[(linev.vertc - 4) + 1].tu = tu[1] * (uvbuflen / (64 / (8 / sx)));
	linev.vert[(linev.vertc - 4) + 1].tv = tv[1];
	linev.vert[(linev.vertc - 4) + 2].tu = tu[0];
	linev.vert[(linev.vertc - 4) + 2].tv = tv[1];
	linev.vert[(linev.vertc - 4) + 3].tu = tu[1] * (uvbuflen / (64 / (8 / sx)));
	linev.vert[(linev.vertc - 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {
		linev.vert[(linev.vertc - 4) + i].x *= viewx;
		linev.vert[(linev.vertc - 4) + i].y *= viewx;

		linev.vert[(linev.vertc - 4) + i].x += frontx;
		linev.vert[(linev.vertc - 4) + i].y += fronty;

		ColorconvF(linev.vert[(linev.vertc - 4) + i].color, col);
	}

	linedrcount++;
}

void packetvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv, DWORD col) {
	if (packetv.vertc + 4 <= 4294967295 && packetv.indc + 6 <= 4294967295) {
		packetv.vertc += 4;
		packetv.indc += 6;
	}

	if (packetv.vertc > packetv.vertcmax) {
		packetv.vertcmax = packetv.vertc;
		packetv.indcmax = packetv.indc;

		packetv.vert = (Vertex *)realloc(packetv.vert, sizeof(Vertex) * packetv.vertc);
		packetv.ind = (WORD *)realloc(packetv.ind, sizeof(WORD) * packetv.indc);

		packetv.ind[0 + (packetv.indc - 6)] = 0 + (packetv.vertc - 4);
		packetv.ind[1 + (packetv.indc - 6)] = 1 + (packetv.vertc - 4);
		packetv.ind[2 + (packetv.indc - 6)] = 2 + (packetv.vertc - 4);
		packetv.ind[3 + (packetv.indc - 6)] = 0 + (packetv.vertc - 4);
		packetv.ind[4 + (packetv.indc - 6)] = 3 + (packetv.vertc - 4);
		packetv.ind[5 + (packetv.indc - 6)] = 1 + (packetv.vertc - 4);
	}

	packetv.vert[(packetv.vertc - 4) + 0].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	packetv.vert[(packetv.vertc - 4) + 0].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	packetv.vert[(packetv.vertc - 4) + 0].z = 0.0f;

	packetv.vert[(packetv.vertc - 4) + 1].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	packetv.vert[(packetv.vertc - 4) + 1].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	packetv.vert[(packetv.vertc - 4) + 1].z = 0.0f;


	packetv.vert[(packetv.vertc - 4) + 2].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	packetv.vert[(packetv.vertc - 4) + 2].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	packetv.vert[(packetv.vertc - 4) + 2].z = 0.0f;


	packetv.vert[(packetv.vertc - 4) + 3].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	packetv.vert[(packetv.vertc - 4) + 3].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	packetv.vert[(packetv.vertc - 4) + 3].z = 0.0f;



	packetv.vert[(packetv.vertc - 4) + 0].tu = tu[0];
	packetv.vert[(packetv.vertc - 4) + 0].tv = tv[0];
	packetv.vert[(packetv.vertc - 4) + 1].tu = tu[1];
	packetv.vert[(packetv.vertc - 4) + 1].tv = tv[1];
	packetv.vert[(packetv.vertc - 4) + 2].tu = tu[0];
	packetv.vert[(packetv.vertc - 4) + 2].tv = tv[1];
	packetv.vert[(packetv.vertc - 4) + 3].tu = tu[1];
	packetv.vert[(packetv.vertc - 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {

		
		packetv.vert[(packetv.vertc - 4) + i].x *= viewx;
		packetv.vert[(packetv.vertc - 4) + i].y *= viewx;

		packetv.vert[(packetv.vertc - 4) + i].x += frontx;
		packetv.vert[(packetv.vertc - 4) + i].y += fronty;

		ColorconvF(packetv.vert[(packetv.vertc - 4) + i].color, col);
	}

	packetdrcount++;
}

void sizegraphvset(Pos pos, double height, double tu, double *tv) {
	if (sizegraphv.vertc + 2 <= 65535) {
		sizegraphv.vertc += 2;
	}
	if (sizegraphv.vertc > sizegraphv.vertcmax) {
		sizegraphv.vertcmax = sizegraphv.vertc;

		sizegraphv.vert = (Vertex *)realloc(sizegraphv.vert, sizeof(Vertex) * sizegraphv.vertc);

	}

	sizegraphv.vert[(sizegraphv.vertc - 2) + 0].x = pos.x;
	sizegraphv.vert[(sizegraphv.vertc - 2) + 0].y = pos.y - height;
	sizegraphv.vert[(sizegraphv.vertc - 2) + 0].z = 0.0f;

	sizegraphv.vert[(sizegraphv.vertc - 2) + 1].x = pos.x;
	sizegraphv.vert[(sizegraphv.vertc - 2) + 1].y = pos.y;
	sizegraphv.vert[(sizegraphv.vertc - 2) + 1].z = 0.0f;

	sizegraphv.vert[(sizegraphv.vertc - 2) + 0].tu = tu;
	sizegraphv.vert[(sizegraphv.vertc - 2) + 0].tv = tv[0];

	sizegraphv.vert[(sizegraphv.vertc - 2) + 1].tu = tu;
	sizegraphv.vert[(sizegraphv.vertc - 2) + 1].tv = tv[1];

	ColorconvF(sizegraphv.vert[(sizegraphv.vertc - 2) + 0].color, 0xffffffff);
	ColorconvF(sizegraphv.vert[(sizegraphv.vertc - 2) + 1].color, 0xffffffff);
}

void sizegraphiset() {
	if (sizegraphv.indc + 6 <= 65535) {
		sizegraphv.indc += 6;
	}
	if (sizegraphv.indc > sizegraphv.indcmax) {
		sizegraphv.indcmax = sizegraphv.indc;

		sizegraphv.ind = (WORD *)realloc(sizegraphv.ind, sizeof(WORD) * sizegraphv.indc);

		
	}
	sizegraphv.ind[0 + (sizegraphv.indc - 6)] = -2 + (sizegraphv.vertc - 2);
	sizegraphv.ind[1 + (sizegraphv.indc - 6)] = 0 + (sizegraphv.vertc - 2);
	sizegraphv.ind[2 + (sizegraphv.indc - 6)] = -1 + (sizegraphv.vertc - 2);
	sizegraphv.ind[3 + (sizegraphv.indc - 6)] = 0 + (sizegraphv.vertc - 2);
	sizegraphv.ind[4 + (sizegraphv.indc - 6)] = 1 + (sizegraphv.vertc - 2);
	sizegraphv.ind[5 + (sizegraphv.indc - 6)] = -1 + (sizegraphv.vertc - 2);

}

void sizegraphvsetsq(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv, DWORD col) {
	if (sizegraphv.vertc + 4 <= 65535 && sizegraphv.indc + 6 <= 65535) {
		sizegraphv.vertc += 4;
		sizegraphv.indc += 6;
	}
	if (sizegraphv.vertc > sizegraphv.vertcmax) {
		sizegraphv.vertcmax = sizegraphv.vertc;
		sizegraphv.indcmax = sizegraphv.indc;

		sizegraphv.vert = (Vertex *)realloc(sizegraphv.vert, sizeof(Vertex) * sizegraphv.vertc);
		sizegraphv.ind = (WORD *)realloc(sizegraphv.ind, sizeof(WORD) * sizegraphv.indc);

	}

	sizegraphv.ind[0 + (sizegraphv.indc - 6)] = 0 + (sizegraphv.vertc - 4);
	sizegraphv.ind[1 + (sizegraphv.indc - 6)] = 1 + (sizegraphv.vertc - 4);
	sizegraphv.ind[2 + (sizegraphv.indc - 6)] = 2 + (sizegraphv.vertc - 4);
	sizegraphv.ind[3 + (sizegraphv.indc - 6)] = 0 + (sizegraphv.vertc - 4);
	sizegraphv.ind[4 + (sizegraphv.indc - 6)] = 3 + (sizegraphv.vertc - 4);
	sizegraphv.ind[5 + (sizegraphv.indc - 6)] = 1 + (sizegraphv.vertc - 4);

	sizegraphv.vert[(sizegraphv.vertc - 4) + 0].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	sizegraphv.vert[(sizegraphv.vertc - 4) + 0].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	sizegraphv.vert[(sizegraphv.vertc - 4) + 0].z = 0.0f;

	sizegraphv.vert[(sizegraphv.vertc - 4) + 1].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	sizegraphv.vert[(sizegraphv.vertc - 4) + 1].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	sizegraphv.vert[(sizegraphv.vertc - 4) + 1].z = 0.0f;


	sizegraphv.vert[(sizegraphv.vertc - 4) + 2].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	sizegraphv.vert[(sizegraphv.vertc - 4) + 2].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	sizegraphv.vert[(sizegraphv.vertc - 4) + 2].z = 0.0f;


	sizegraphv.vert[(sizegraphv.vertc - 4) + 3].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	sizegraphv.vert[(sizegraphv.vertc - 4) + 3].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	sizegraphv.vert[(sizegraphv.vertc - 4) + 3].z = 0.0f;



	sizegraphv.vert[(sizegraphv.vertc - 4) + 0].tu = tu[0];
	sizegraphv.vert[(sizegraphv.vertc - 4) + 0].tv = tv[0];
	sizegraphv.vert[(sizegraphv.vertc - 4) + 1].tu = tu[1];
	sizegraphv.vert[(sizegraphv.vertc - 4) + 1].tv = tv[1];
	sizegraphv.vert[(sizegraphv.vertc - 4) + 2].tu = tu[0];
	sizegraphv.vert[(sizegraphv.vertc - 4) + 2].tv = tv[1];
	sizegraphv.vert[(sizegraphv.vertc - 4) + 3].tu = tu[1];
	sizegraphv.vert[(sizegraphv.vertc - 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {

		ColorconvF(sizegraphv.vert[(sizegraphv.vertc - 4) + i].color, col);
	}

}

void sizegraphbgvsetsq(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv, DWORD col) {
	if (sizegrapbghv.vertc + 4 <= 65535 && sizegrapbghv.indc + 6 <= 65535) {
		sizegrapbghv.vertc += 4;
		sizegrapbghv.indc += 6;
	}
	if (sizegrapbghv.vertc > sizegrapbghv.vertcmax) {
		sizegrapbghv.vertcmax = sizegrapbghv.vertc;
		sizegrapbghv.indcmax = sizegrapbghv.indc;

		sizegrapbghv.vert = (Vertex *)realloc(sizegrapbghv.vert, sizeof(Vertex) * sizegrapbghv.vertc);
		sizegrapbghv.ind = (WORD *)realloc(sizegrapbghv.ind, sizeof(WORD) * sizegrapbghv.indc);

		sizegrapbghv.ind[0 + (sizegrapbghv.indc - 6)] = 0 + (sizegrapbghv.vertc - 4);
		sizegrapbghv.ind[1 + (sizegrapbghv.indc - 6)] = 1 + (sizegrapbghv.vertc - 4);
		sizegrapbghv.ind[2 + (sizegrapbghv.indc - 6)] = 2 + (sizegrapbghv.vertc - 4);
		sizegrapbghv.ind[3 + (sizegrapbghv.indc - 6)] = 0 + (sizegrapbghv.vertc - 4);
		sizegrapbghv.ind[4 + (sizegrapbghv.indc - 6)] = 3 + (sizegrapbghv.vertc - 4);
		sizegrapbghv.ind[5 + (sizegrapbghv.indc - 6)] = 1 + (sizegrapbghv.vertc - 4);
	}

	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 0].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 0].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 0].z = 0.0f;

	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 1].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 1].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 1].z = 0.0f;


	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 2].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 2].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 2].z = 0.0f;


	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 3].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 3].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 3].z = 0.0f;



	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 0].tu = tu[0];
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 0].tv = tv[0];
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 1].tu = tu[1];
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 1].tv = tv[1];
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 2].tu = tu[0];
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 2].tv = tv[1];
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 3].tu = tu[1];
	sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {

		ColorconvF(sizegrapbghv.vert[(sizegrapbghv.vertc - 4) + i].color, col);
	}

}

void protocolgraphvset(Pos pos, double rad, double angl, double startangl, double *tu, double *tv, DWORD col) {
	for (int i = 0; i < ((int)(angl / 2) > 2 ? (int)(angl / 2) : 2); i++) {
		if (protocolgraphv.vertc + 4 <= 65535) {
			protocolgraphv.vertc += 2;
		}
		if (protocolgraphv.vertc > protocolgraphv.vertcmax) {
			protocolgraphv.vertcmax = protocolgraphv.vertc;

			protocolgraphv.vert = (Vertex *)realloc(protocolgraphv.vert, sizeof(Vertex) * protocolgraphv.vertc);

		}
		if (i >= 1) {
			if (protocolgraphv.indc + 6 <= 65535) {
				protocolgraphv.indc += 6;
			}
			if (protocolgraphv.indc > protocolgraphv.indcmax) {
				protocolgraphv.indcmax = protocolgraphv.indc;

				protocolgraphv.ind = (WORD *)realloc(protocolgraphv.ind, sizeof(WORD) * protocolgraphv.indc);

			}

			protocolgraphv.ind[0 + (protocolgraphv.indc - 6)] = -2 + (protocolgraphv.vertc - 2);
			protocolgraphv.ind[1 + (protocolgraphv.indc - 6)] = 0 + (protocolgraphv.vertc - 2);
			protocolgraphv.ind[2 + (protocolgraphv.indc - 6)] = -1 + (protocolgraphv.vertc - 2);
			protocolgraphv.ind[3 + (protocolgraphv.indc - 6)] = 0 + (protocolgraphv.vertc - 2);
			protocolgraphv.ind[4 + (protocolgraphv.indc - 6)] = 1 + (protocolgraphv.vertc - 2);
			protocolgraphv.ind[5 + (protocolgraphv.indc - 6)] = -1 + (protocolgraphv.vertc - 2);
		}

		if (i == 0) {
			protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].x = pos.x + (rad * cos((startangl - 90) * PI / 180.0));
			protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].y = pos.y + (rad * sin((startangl - 90) * PI / 180.0));
			protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].z = 0.0f;
		}
		else if(i == ((int)(angl / 2) > 2 ? (int)(angl / 2) : 2) - 1){
			protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].x = pos.x + (rad * cos((angl + startangl - 90) * PI / 180.0));
			protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].y = pos.y + (rad * sin((angl + startangl - 90) * PI / 180.0));
			protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].z = 0.0f;
		}
		else {
			protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].x = pos.x + (rad * cos((((i * 2) - 90) + startangl) * PI / 180.0));
			protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].y = pos.y + (rad * sin((((i * 2) - 90) + startangl) * PI / 180.0));
			protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].z = 0.0f;
		}

		protocolgraphv.vert[(protocolgraphv.vertc - 2) + 1].x = pos.x;
		protocolgraphv.vert[(protocolgraphv.vertc - 2) + 1].y = pos.y;
		protocolgraphv.vert[(protocolgraphv.vertc - 2) + 1].z = 0.0f;

		protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].tu = tu[0];
		protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].tv = tv[0];

		protocolgraphv.vert[(protocolgraphv.vertc - 2) + 1].tu = tu[1];
		protocolgraphv.vert[(protocolgraphv.vertc - 2) + 1].tv = tv[1];

		ColorconvF(protocolgraphv.vert[(protocolgraphv.vertc - 2) + 0].color, col);
		ColorconvF(protocolgraphv.vert[(protocolgraphv.vertc - 2) + 1].color, col);
	}
}

void protocolgraphsqvset(Pos pos, double sx, double sy, double *tu, double *tv, DWORD col) {
	if (protocolgraphv.vertc + 4 <= 65535 && protocolgraphv.indc + 6 <= 65535) {
		protocolgraphv.vertc += 4;
		protocolgraphv.indc += 6;
	}

	if (protocolgraphv.vertc > protocolgraphv.vertcmax) {
		protocolgraphv.vertcmax = protocolgraphv.vertc;

		protocolgraphv.vert = (Vertex *)realloc(protocolgraphv.vert, sizeof(Vertex) * protocolgraphv.vertc);

	}

	if (protocolgraphv.indc > protocolgraphv.indcmax) {
		protocolgraphv.indcmax = protocolgraphv.indc;

		protocolgraphv.ind = (WORD *)realloc(protocolgraphv.ind, sizeof(WORD) * protocolgraphv.indc);

	}

	protocolgraphv.ind[0 + (protocolgraphv.indc - 6)] = 0 + (protocolgraphv.vertc - 4);
	protocolgraphv.ind[1 + (protocolgraphv.indc - 6)] = 1 + (protocolgraphv.vertc - 4);
	protocolgraphv.ind[2 + (protocolgraphv.indc - 6)] = 2 + (protocolgraphv.vertc - 4);
	protocolgraphv.ind[3 + (protocolgraphv.indc - 6)] = 0 + (protocolgraphv.vertc - 4);
	protocolgraphv.ind[4 + (protocolgraphv.indc - 6)] = 3 + (protocolgraphv.vertc - 4);
	protocolgraphv.ind[5 + (protocolgraphv.indc - 6)] = 1 + (protocolgraphv.vertc - 4);


	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 0].x = pos.x - (sx / 2);
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 0].y = pos.y - (sy / 2);
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 0].z = 0.0f;

	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 1].x = pos.x + (sx / 2);
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 1].y = pos.y + (sy / 2);
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 1].z = 0.0f;


	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 2].x = pos.x - (sx / 2);
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 2].y = pos.y + (sy / 2);
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 2].z = 0.0f;


	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 3].x = pos.x + (sx / 2);
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 3].y = pos.y - (sy / 2);
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 3].z = 0.0f;



	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 0].tu = tu[0];
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 0].tv = tv[0];
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 1].tu = tu[1];
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 1].tv = tv[1];
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 2].tu = tu[0];
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 2].tv = tv[1];
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 3].tu = tu[1];
	protocolgraphv.vert[(protocolgraphv.vertc - 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {

		ColorconvF(protocolgraphv.vert[(protocolgraphv.vertc - 4) + i].color, col);
	}

}

void buttonvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv) {
	if (buttonv.vertc + 4 <= 65535 && buttonv.indc + 6 <= 65535) {
		buttonv.vertc += 4;
		buttonv.indc += 6;
	}
	if (buttonv.vertc > buttonv.vertcmax) {
		buttonv.vertcmax = buttonv.vertc;
		buttonv.indcmax = buttonv.indc;

		buttonv.vert = (Vertex *)realloc(buttonv.vert, sizeof(Vertex) * buttonv.vertc);
		buttonv.ind = (WORD *)realloc(buttonv.ind, sizeof(WORD) * buttonv.indc);

		buttonv.ind[0 + (buttonv.indc - 6)] = 0 + (buttonv.vertc - 4);
		buttonv.ind[1 + (buttonv.indc - 6)] = 1 + (buttonv.vertc - 4);
		buttonv.ind[2 + (buttonv.indc - 6)] = 2 + (buttonv.vertc - 4);
		buttonv.ind[3 + (buttonv.indc - 6)] = 0 + (buttonv.vertc - 4);
		buttonv.ind[4 + (buttonv.indc - 6)] = 3 + (buttonv.vertc - 4);
		buttonv.ind[5 + (buttonv.indc - 6)] = 1 + (buttonv.vertc - 4);
	}

	buttonv.vert[(buttondrcount * 4) + 0].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	buttonv.vert[(buttondrcount * 4) + 0].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	buttonv.vert[(buttondrcount * 4) + 0].z = 0.0f;

	buttonv.vert[(buttondrcount * 4) + 1].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	buttonv.vert[(buttondrcount * 4) + 1].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	buttonv.vert[(buttondrcount * 4) + 1].z = 0.0f;


	buttonv.vert[(buttondrcount * 4) + 2].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	buttonv.vert[(buttondrcount * 4) + 2].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	buttonv.vert[(buttondrcount * 4) + 2].z = 0.0f;


	buttonv.vert[(buttondrcount * 4) + 3].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	buttonv.vert[(buttondrcount * 4) + 3].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	buttonv.vert[(buttondrcount * 4) + 3].z = 0.0f;



	buttonv.vert[(buttondrcount * 4) + 0].tu = tu[0];
	buttonv.vert[(buttondrcount * 4) + 0].tv = tv[0];
	buttonv.vert[(buttondrcount * 4) + 1].tu = tu[1];
	buttonv.vert[(buttondrcount * 4) + 1].tv = tv[1];
	buttonv.vert[(buttondrcount * 4) + 2].tu = tu[0];
	buttonv.vert[(buttondrcount * 4) + 2].tv = tv[1];
	buttonv.vert[(buttondrcount * 4) + 3].tu = tu[1];
	buttonv.vert[(buttondrcount * 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {

		ColorconvF(buttonv.vert[(buttondrcount * 4) + i].color, 0xffffffff);
	}

	buttondrcount++;
}

void selectboxvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv) {
	if (selectboxv.vertc + 4 <= 65535 && selectboxv.indc + 6 <= 65535) {
		selectboxv.vertc += 4;
		selectboxv.indc += 6;
	}
	if (selectboxv.vertc > selectboxv.vertcmax) {
		selectboxv.vertcmax = selectboxv.vertc;
		selectboxv.indcmax = selectboxv.indc;

		selectboxv.vert = (Vertex *)realloc(selectboxv.vert, sizeof(Vertex) * selectboxv.vertc);
		selectboxv.ind = (WORD *)realloc(selectboxv.ind, sizeof(WORD) * selectboxv.indc);

		selectboxv.ind[0 + (selectboxv.indc - 6)] = 0 + (selectboxv.vertc - 4);
		selectboxv.ind[1 + (selectboxv.indc - 6)] = 1 + (selectboxv.vertc - 4);
		selectboxv.ind[2 + (selectboxv.indc - 6)] = 2 + (selectboxv.vertc - 4);
		selectboxv.ind[3 + (selectboxv.indc - 6)] = 0 + (selectboxv.vertc - 4);
		selectboxv.ind[4 + (selectboxv.indc - 6)] = 3 + (selectboxv.vertc - 4);
		selectboxv.ind[5 + (selectboxv.indc - 6)] = 1 + (selectboxv.vertc - 4);
	}

	selectboxv.vert[(selectboxdrcount * 4) + 0].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	selectboxv.vert[(selectboxdrcount * 4) + 0].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	selectboxv.vert[(selectboxdrcount * 4) + 0].z = 0.0f;

	selectboxv.vert[(selectboxdrcount * 4) + 1].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	selectboxv.vert[(selectboxdrcount * 4) + 1].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	selectboxv.vert[(selectboxdrcount * 4) + 1].z = 0.0f;


	selectboxv.vert[(selectboxdrcount * 4) + 2].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	selectboxv.vert[(selectboxdrcount * 4) + 2].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	selectboxv.vert[(selectboxdrcount * 4) + 2].z = 0.0f;


	selectboxv.vert[(selectboxdrcount * 4) + 3].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	selectboxv.vert[(selectboxdrcount * 4) + 3].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	selectboxv.vert[(selectboxdrcount * 4) + 3].z = 0.0f;



	selectboxv.vert[(selectboxdrcount * 4) + 0].tu = tu[0];
	selectboxv.vert[(selectboxdrcount * 4) + 0].tv = tv[0];
	selectboxv.vert[(selectboxdrcount * 4) + 1].tu = tu[1];
	selectboxv.vert[(selectboxdrcount * 4) + 1].tv = tv[1];
	selectboxv.vert[(selectboxdrcount * 4) + 2].tu = tu[0];
	selectboxv.vert[(selectboxdrcount * 4) + 2].tv = tv[1];
	selectboxv.vert[(selectboxdrcount * 4) + 3].tu = tu[1];
	selectboxv.vert[(selectboxdrcount * 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {

		ColorconvF(selectboxv.vert[(selectboxdrcount * 4) + i].color, 0xffffffff);
	}

	selectboxdrcount++;
}

void selectboxtxtvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv) {
	if (selectboxtxtv.vertc + 4 <= 65535 && selectboxtxtv.indc + 6 <= 65535) {
		selectboxtxtv.vertc += 4;
		selectboxtxtv.indc += 6;
	}
	if (selectboxtxtv.vertc > selectboxtxtv.vertcmax) {
		selectboxtxtv.vertcmax = selectboxtxtv.vertc;
		selectboxtxtv.indcmax = selectboxtxtv.indc;

		selectboxtxtv.vert = (Vertex *)realloc(selectboxtxtv.vert, sizeof(Vertex) * selectboxtxtv.vertc);
		selectboxtxtv.ind = (WORD *)realloc(selectboxtxtv.ind, sizeof(WORD) * selectboxtxtv.indc);

		selectboxtxtv.ind[0 + (selectboxtxtv.indc - 6)] = 0 + (selectboxtxtv.vertc - 4);
		selectboxtxtv.ind[1 + (selectboxtxtv.indc - 6)] = 1 + (selectboxtxtv.vertc - 4);
		selectboxtxtv.ind[2 + (selectboxtxtv.indc - 6)] = 2 + (selectboxtxtv.vertc - 4);
		selectboxtxtv.ind[3 + (selectboxtxtv.indc - 6)] = 0 + (selectboxtxtv.vertc - 4);
		selectboxtxtv.ind[4 + (selectboxtxtv.indc - 6)] = 3 + (selectboxtxtv.vertc - 4);
		selectboxtxtv.ind[5 + (selectboxtxtv.indc - 6)] = 1 + (selectboxtxtv.vertc - 4);
	}

	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 0].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 0].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 0].z = 0.0f;

	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 1].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 1].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 1].z = 0.0f;


	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 2].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 2].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 2].z = 0.0f;


	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 3].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 3].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 3].z = 0.0f;



	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 0].tu = tu[0];
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 0].tv = tv[0];
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 1].tu = tu[1];
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 1].tv = tv[1];
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 2].tu = tu[0];
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 2].tv = tv[1];
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 3].tu = tu[1];
	selectboxtxtv.vert[(selectboxtxtdrcount * 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {

		ColorconvF(selectboxtxtv.vert[(selectboxtxtdrcount * 4) + i].color, 0xffffffff);
	}

	selectboxtxtdrcount++;
}

void textboxvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv) {
	if (textboxv.vertc + 4 <= 65535 && textboxv.indc + 6 <= 65535) {
		textboxv.vertc += 4;
		textboxv.indc += 6;
	}
	if (textboxv.vertc > textboxv.vertcmax) {
		textboxv.vertcmax = textboxv.vertc;
		textboxv.indcmax = textboxv.indc;

		textboxv.vert = (Vertex *)realloc(textboxv.vert, sizeof(Vertex) * textboxv.vertc);
		textboxv.ind = (WORD *)realloc(textboxv.ind, sizeof(WORD) * textboxv.indc);

		textboxv.ind[0 + (textboxv.indc - 6)] = 0 + (textboxv.vertc - 4);
		textboxv.ind[1 + (textboxv.indc - 6)] = 1 + (textboxv.vertc - 4);
		textboxv.ind[2 + (textboxv.indc - 6)] = 2 + (textboxv.vertc - 4);
		textboxv.ind[3 + (textboxv.indc - 6)] = 0 + (textboxv.vertc - 4);
		textboxv.ind[4 + (textboxv.indc - 6)] = 3 + (textboxv.vertc - 4);
		textboxv.ind[5 + (textboxv.indc - 6)] = 1 + (textboxv.vertc - 4);
	}

	textboxv.vert[(textboxdrcount * 4) + 0].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	textboxv.vert[(textboxdrcount * 4) + 0].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	textboxv.vert[(textboxdrcount * 4) + 0].z = 0.0f;

	textboxv.vert[(textboxdrcount * 4) + 1].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	textboxv.vert[(textboxdrcount * 4) + 1].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	textboxv.vert[(textboxdrcount * 4) + 1].z = 0.0f;


	textboxv.vert[(textboxdrcount * 4) + 2].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	textboxv.vert[(textboxdrcount * 4) + 2].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	textboxv.vert[(textboxdrcount * 4) + 2].z = 0.0f;


	textboxv.vert[(textboxdrcount * 4) + 3].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	textboxv.vert[(textboxdrcount * 4) + 3].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	textboxv.vert[(textboxdrcount * 4) + 3].z = 0.0f;



	textboxv.vert[(textboxdrcount * 4) + 0].tu = tu[0];
	textboxv.vert[(textboxdrcount * 4) + 0].tv = tv[0];
	textboxv.vert[(textboxdrcount * 4) + 1].tu = tu[1];
	textboxv.vert[(textboxdrcount * 4) + 1].tv = tv[1];
	textboxv.vert[(textboxdrcount * 4) + 2].tu = tu[0];
	textboxv.vert[(textboxdrcount * 4) + 2].tv = tv[1];
	textboxv.vert[(textboxdrcount * 4) + 3].tu = tu[1];
	textboxv.vert[(textboxdrcount * 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {

		ColorconvF(textboxv.vert[(textboxdrcount * 4) + i].color, 0xffffffff);
	}

	textboxdrcount++;
}

void systemtxtvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv) {
	if (systemtxtv.vertc + 4 <= 65535 && systemtxtv.indc + 6 <= 65535) {
		systemtxtv.vertc += 4;
		systemtxtv.indc += 6;
	}
	if (systemtxtv.vertc > systemtxtv.vertcmax) {
		systemtxtv.vertcmax = systemtxtv.vertc;
		systemtxtv.indcmax = systemtxtv.indc;

		systemtxtv.vert = (Vertex *)realloc(systemtxtv.vert, sizeof(Vertex) * systemtxtv.vertc);
		systemtxtv.ind = (WORD *)realloc(systemtxtv.ind, sizeof(WORD) * systemtxtv.indc);

		systemtxtv.ind[0 + (systemtxtv.indc - 6)] = 0 + (systemtxtv.vertc - 4);
		systemtxtv.ind[1 + (systemtxtv.indc - 6)] = 1 + (systemtxtv.vertc - 4);
		systemtxtv.ind[2 + (systemtxtv.indc - 6)] = 2 + (systemtxtv.vertc - 4);
		systemtxtv.ind[3 + (systemtxtv.indc - 6)] = 0 + (systemtxtv.vertc - 4);
		systemtxtv.ind[4 + (systemtxtv.indc - 6)] = 3 + (systemtxtv.vertc - 4);
		systemtxtv.ind[5 + (systemtxtv.indc - 6)] = 1 + (systemtxtv.vertc - 4);
	}

	systemtxtv.vert[(systemtxtv.vertc - 4) + 0].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	systemtxtv.vert[(systemtxtv.vertc - 4) + 0].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	systemtxtv.vert[(systemtxtv.vertc - 4) + 0].z = 0.0f;

	systemtxtv.vert[(systemtxtv.vertc - 4) + 1].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	systemtxtv.vert[(systemtxtv.vertc - 4) + 1].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	systemtxtv.vert[(systemtxtv.vertc - 4) + 1].z = 0.0f;


	systemtxtv.vert[(systemtxtv.vertc - 4) + 2].x = pos.x + ((sx / 2) * cos((angl - 180) * PI / 180.0) * size) + ((sy / 2) * cos((angl + 90) * PI / 180.0)* size);
	systemtxtv.vert[(systemtxtv.vertc - 4) + 2].y = pos.y + ((sx / 2) * sin((angl - 180) * PI / 180.0) * size) + ((sy / 2) * sin((angl + 90) * PI / 180.0)* size);
	systemtxtv.vert[(systemtxtv.vertc - 4) + 2].z = 0.0f;


	systemtxtv.vert[(systemtxtv.vertc - 4) + 3].x = pos.x + ((sx / 2) * cos(angl * PI / 180.0) * size) + ((sy / 2) * cos((angl - 90) * PI / 180.0)* size);
	systemtxtv.vert[(systemtxtv.vertc - 4) + 3].y = pos.y + ((sx / 2) * sin(angl * PI / 180.0) * size) + ((sy / 2) * sin((angl - 90) * PI / 180.0)* size);
	systemtxtv.vert[(systemtxtv.vertc - 4) + 3].z = 0.0f;



	systemtxtv.vert[(systemtxtv.vertc - 4) + 0].tu = tu[0];
	systemtxtv.vert[(systemtxtv.vertc - 4) + 0].tv = tv[0];
	systemtxtv.vert[(systemtxtv.vertc - 4) + 1].tu = tu[1];
	systemtxtv.vert[(systemtxtv.vertc - 4) + 1].tv = tv[1];
	systemtxtv.vert[(systemtxtv.vertc - 4) + 2].tu = tu[0];
	systemtxtv.vert[(systemtxtv.vertc - 4) + 2].tv = tv[1];
	systemtxtv.vert[(systemtxtv.vertc - 4) + 3].tu = tu[1];
	systemtxtv.vert[(systemtxtv.vertc - 4) + 3].tv = tv[0];

	for (int i = 0; i < 4; i++) {

		ColorconvF(systemtxtv.vert[(systemtxtv.vertc - 4) + i].color, 0xffffffff);
	}

}

void areavset(Vertex vert[]) {
	if (areav.vertc + 4 <= 65535 && areav.indc + 6 <= 65535) {
		areav.vertc += 4;
		areav.indc += 6;
	}
	if (areav.vertc > areav.vertcmax) {
		areav.vertcmax = areav.vertc;
		areav.indcmax = areav.indc;

		areav.vert = (Vertex *)realloc(areav.vert, sizeof(Vertex) * areav.vertc);
		areav.ind = (WORD *)realloc(areav.ind, sizeof(WORD) * areav.indc);

		areav.ind[0 + (areav.indc - 6)] = 0 + (areav.vertc - 4);
		areav.ind[1 + (areav.indc - 6)] = 1 + (areav.vertc - 4);
		areav.ind[2 + (areav.indc - 6)] = 2 + (areav.vertc - 4);
		areav.ind[3 + (areav.indc - 6)] = 0 + (areav.vertc - 4);
		areav.ind[4 + (areav.indc - 6)] = 3 + (areav.vertc - 4);
		areav.ind[5 + (areav.indc - 6)] = 1 + (areav.vertc - 4);
	}

	areav.vert[(areav.vertc - 4) + 0].x = vert[0].x;
	areav.vert[(areav.vertc - 4) + 0].y = vert[0].y;
	areav.vert[(areav.vertc - 4) + 0].z = 0.0f;

	areav.vert[(areav.vertc - 4) + 1].x = vert[1].x;
	areav.vert[(areav.vertc - 4) + 1].y = vert[1].y;
	areav.vert[(areav.vertc - 4) + 1].z = 0.0f;


	areav.vert[(areav.vertc - 4) + 2].x = vert[2].x;
	areav.vert[(areav.vertc - 4) + 2].y = vert[2].y;
	areav.vert[(areav.vertc - 4) + 2].z = 0.0f;


	areav.vert[(areav.vertc - 4) + 3].x = vert[3].x;
	areav.vert[(areav.vertc - 4) + 3].y = vert[3].y;
	areav.vert[(areav.vertc - 4) + 3].z = 0.0f;



	areav.vert[(areav.vertc - 4) + 0].tu = vert[0].tu;
	areav.vert[(areav.vertc - 4) + 0].tv = vert[0].tv;
	areav.vert[(areav.vertc - 4) + 1].tu = vert[1].tu;
	areav.vert[(areav.vertc - 4) + 1].tv = vert[1].tv;
	areav.vert[(areav.vertc - 4) + 2].tu = vert[2].tu;
	areav.vert[(areav.vertc - 4) + 2].tv = vert[2].tv;
	areav.vert[(areav.vertc - 4) + 3].tu = vert[3].tu;
	areav.vert[(areav.vertc - 4) + 3].tv = vert[3].tv;

	for (int i = 0; i < 4; i++) {
		areav.vert[(areav.vertc - 4) + i].color[0] = vert[i].color[0];
		areav.vert[(areav.vertc - 4) + i].color[1] = vert[i].color[1];
		areav.vert[(areav.vertc - 4) + i].color[2] = vert[i].color[2];
		areav.vert[(areav.vertc - 4) + i].color[3] = vert[i].color[3];
		//ColorconvF(, 0xffffffff);
	}

	areadrcount++;
}

void screenblur() {
	drvert(screenvert, 20, screenind, 30, 24, 6, 0, screentex.view, &view);

}

void graphicRelease() {

	marker.datafree();
	line.datafree();
	packet.datafree();
	route.datafree();

	free(markerv.vert);
	free(markerv.ind);

	free(linev.vert);
	free(linev.ind);

	free(packetv.vert);
	free(packetv.ind);

	free(areav.vert);
	free(areav.ind);

	//menutex.tex->Release();

}
