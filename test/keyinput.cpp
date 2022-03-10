#include <WindowsX.h>
#include <tchar.h>
#include <crtdbg.h>
#include <sys/timeb.h>
#include <iostream>

#include <thread>
#include "../Include/pcap.h"
#include <WinSock2.h>


#include "../main.h"


int mainkeyup;
int mainkeydown;
int mainkeyleft;
int mainkeyright;

int mainkeyq;
int mainkeyw;
int mainkeyr;
int mainkey01;
int mainkey02;
int mainkey03;
int mainkeyesc;
int mainkeyctrl;
int mainkeyenter;

//int stickvec;

int keynumkey[10];
int keybackspace;

int mouseleft;
int mouseright;

int mousewheel;
POINT oldpos;


void Checkkeyinput() {
	
	dkb.Checkkey();
	dms.Checkkey();

	if (dkb.GetKeySt(KEY_UP)) {
		if (mainkeyup >= 1) mainkeyup++;
		else mainkeyup = 1;
	}
	else {
		mainkeyup = 0;
	}

	if (dkb.GetKeySt(KEY_DOWN)) {
		if (mainkeydown >= 1) mainkeydown++;
		else mainkeydown = 1;
	}
	else {
		mainkeydown = 0;
	}

	if (dkb.GetKeySt(KEY_LEFT)) {
		if (mainkeyleft >= 1) mainkeyleft++;
		else mainkeyleft = 1;
	}
	else {
		mainkeyleft = 0;
	}

	if (dkb.GetKeySt(KEY_RIGHT)) {
		if (mainkeyright >= 1) mainkeyright++;
		else mainkeyright = 1;
	}
	else {
		mainkeyright = 0;
	}

	if (dkb.GetKeySt(KEY_0) || dkb.GetKeySt(DIK_NUMPAD0)) {
		if (keynumkey[0] >= 1) keynumkey[0]++;
		else keynumkey[0] = 1;
	}
	else {
		keynumkey[0] = 0;
	}

	if (dkb.GetKeySt(KEY_1) || dkb.GetKeySt(DIK_NUMPAD1)) {
		if (keynumkey[1] >= 1) keynumkey[1]++;
		else keynumkey[1] = 1;
	}
	else {
		keynumkey[1] = 0;
	}

	if (dkb.GetKeySt(KEY_2) || dkb.GetKeySt(DIK_NUMPAD2)) {
		if (keynumkey[2] >= 1) keynumkey[2]++;
		else keynumkey[2] = 1;
	}
	else {
		keynumkey[2] = 0;
	}

	if (dkb.GetKeySt(KEY_3) || dkb.GetKeySt(DIK_NUMPAD3)) {
		if (keynumkey[3] >= 1) keynumkey[3]++;
		else keynumkey[3] = 1;
	}
	else {
		keynumkey[3] = 0;
	}

	if (dkb.GetKeySt(KEY_4) || dkb.GetKeySt(DIK_NUMPAD4)) {
		if (keynumkey[4] >= 1) keynumkey[4]++;
		else keynumkey[4] = 1;
	}
	else {
		keynumkey[4] = 0;
	}

	if (dkb.GetKeySt(KEY_5) || dkb.GetKeySt(DIK_NUMPAD5)) {
		if (keynumkey[5] >= 1) keynumkey[5]++;
		else keynumkey[5] = 1;
	}
	else {
		keynumkey[5] = 0;
	}

	if (dkb.GetKeySt(KEY_6) || dkb.GetKeySt(DIK_NUMPAD6)) {
		if (keynumkey[6] >= 1) keynumkey[6]++;
		else keynumkey[6] = 1;
	}
	else {
		keynumkey[6] = 0;
	}

	if (dkb.GetKeySt(KEY_7) || dkb.GetKeySt(DIK_NUMPAD7)) {
		if (keynumkey[7] >= 1) keynumkey[7]++;
		else keynumkey[7] = 1;
	}
	else {
		keynumkey[7] = 0;
	}

	if (dkb.GetKeySt(KEY_8) || dkb.GetKeySt(DIK_NUMPAD8)) {
		if (keynumkey[8] >= 1) keynumkey[8]++;
		else keynumkey[8] = 1;
	}
	else {
		keynumkey[8] = 0;
	}

	if (dkb.GetKeySt(KEY_9) || dkb.GetKeySt(DIK_NUMPAD9)) {
		if (keynumkey[9] >= 1) keynumkey[9]++;
		else keynumkey[9] = 1;
	}
	else {
		keynumkey[9] = 0;
	}

	if (dkb.GetKeySt(KEY_BACK)) {
		if (keybackspace >= 1) keybackspace++;
		else keybackspace = 1;
	}
	else {
		keybackspace = 0;
	}

	if (dms.InputMouse(0) == 1) {
		if (mouseleft >= 1) mouseleft = 2;
		else mouseleft = 1;
	}else {
		mouseleft = 0;
	}

	if (dms.InputMouse(2) == 1) {
		if (mouseright >= 1) mouseright = 2;
		else mouseright = 1;
	}
	else {
		mouseright = 0;
	}


	RECT inputrect, inputrectc;

	oldpos = dms.pos;

	dms.WindowMousepos(hWnd, inputrect, inputrectc);
	mousewheel = dms.InputWheel();

#if _DEBUG
	if (MOUSEPOS_LOG == 1) {
		char mtext[4096];
		sprintf_s(mtext, "MousePos X:%d Y:%d Z:%d \n", dms.pos.x, dms.pos.y, mousewheel);
		OutputDebugStringA(mtext);
	}
#endif

}
