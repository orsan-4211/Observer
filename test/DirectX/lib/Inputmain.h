#ifndef INCLUDED_INPM
#define INCLUDED_INPM

#include "dinput.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define INITGUID

class DX8Input {
public:
	LPDIRECTINPUT8			pDInput;
	DIDEVCAPS				DiDev;

	BOOL Create(HWND hWnd);
	void Release();
};

class DX8Device {
public:
	LPDIRECTINPUTDEVICE8	pDIDevice;

	BOOL Create(HWND hWnd, LPDIRECTINPUT8 pDInput, const GUID guid, LPCDIDATAFORMAT format);
	void Release();

	void Set(HWND hWnd, DWORD level) {
		pDIDevice->SetCooperativeLevel(hWnd, level);
		if (pDIDevice) {
			pDIDevice->Acquire();
		}
	};

	
};

class DX8Keybord : public DX8Device {
public:

	char buffer[256];

	void Checkkey() {
		pDIDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);
	};

	int GetKeySt(DWORD keyid) {
		if (buffer[keyid] & 0x80) {
			return 1;
		}
		else
		{
			return 0;
		}
	}


};

class DX8Mouse : public DX8Device {
public:

	char buffer[256];

	POINT pos;
	DIMOUSESTATE g_zdiMouseState;
	DIPROPDWORD diprop;

	void Mousepos() {
		GetCursorPos(&pos);
	};
	void WindowMousepos(HWND hWnd, RECT &rect, RECT &rectc) {
		GetWindowRect(hWnd, &rect);
		GetClientRect(hWnd, &rectc);
		GetCursorPos(&pos);
		pos.x -= rect.left + (rect.right - rect.left - rectc.right) / 2;
		pos.y -= rect.top + (rect.bottom - rect.top - rectc.bottom) - (rect.right - rect.left - rectc.right) / 2;
	};
	void SetMousepos(int x, int y) {
		SetCursorPos(x, y);
	};

	void Set(HWND hWnd, DWORD level) {
		pDIDevice->SetDataFormat(&c_dfDIMouse);
		pDIDevice->SetCooperativeLevel(hWnd, level);
		pDIDevice->Acquire();
	};

	void Set(HWND hWnd, DWORD level, DWORD dwsize, DWORD dwhs, DWORD dwobj, DWORD dwhow, DWORD dwdata, int mode) {
		pDIDevice->SetDataFormat(&c_dfDIMouse);
		pDIDevice->SetCooperativeLevel(hWnd, level);
		diprop.diph.dwSize =/*sizeof(diprop)*/dwsize;
		diprop.diph.dwHeaderSize =/*sizeof(diprop.diph)*/dwhs;
		diprop.diph.dwObj = dwobj;
		diprop.diph.dwHow = dwhow;
		diprop.dwData = dwdata;
		switch (mode) {
		default:
			pDIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		case 0:
			pDIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		case 1:
			pDIDevice->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
		case 2:
			pDIDevice->SetProperty(DIPROP_CALIBRATIONMODE, &diprop.diph);
		case 3:
			pDIDevice->SetProperty(DIPROP_GRANULARITY, &diprop.diph);
		case 4:
			pDIDevice->SetProperty(DIPROP_FFGAIN, &diprop.diph);
		case 5:
			pDIDevice->SetProperty(DIPROP_FFLOAD, &diprop.diph);
		case 6:
			pDIDevice->SetProperty(DIPROP_AUTOCENTER, &diprop.diph);
		case 7:
			pDIDevice->SetProperty(DIPROP_RANGE, &diprop.diph);
		case 8:
			pDIDevice->SetProperty(DIPROP_DEADZONE, &diprop.diph);
		case 9:
			pDIDevice->SetProperty(DIPROP_SATURATION, &diprop.diph);
		}
		pDIDevice->Acquire();
	};

	void Checkkey() {
		pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_zdiMouseState);
	}

	int InputMouse(int keyid) {
		if (g_zdiMouseState.rgbButtons[keyid] & 0x80) {
			return 1;
		}
		else
		{
			return 0;
		}
	};

	int InputWheel() {
		return g_zdiMouseState.lZ;
	}

	void MouseDi() {
		pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_zdiMouseState);
	};

};

class DX8Pad : public DX8Device {
public:

	DIJOYSTATE js;

	void InputPads() {
		pDIDevice->Poll();
		pDIDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
	};
	int InputPad(int keyid) {
		if (js.rgbButtons[keyid] & 0x80) {
			return 1;
		}
		else
		{
			return 0;
		}
	};

};

/*
level:
DISCL_BACKGROUND バックグラウンドでも入力を受け付けます。 
DISCL_EXCLUSIVE デバイスからの入力を独占します。 
DISCL_FOREGROUND フォアグラウンド（前面）で入力を受け付けます。 
DISCL_NONEXCLUSIVE デバイスからの入力を独占しません。 
DISCL_NOWINKEY ウィンドウズキーを無効にします。 
*/


#endif