#include <windows.h>
#include <WindowsX.h>
#include <stdio.h>
#include <tchar.h>
#include <crtdbg.h>

#include "../lib/Inputmain.h"

BOOL DX8Input::Create(HWND hWnd) {
	HINSTANCE hInst;
	HRESULT   hr;

	hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&pDInput, NULL);
	if (FAILED(hr)) {
		MessageBox(hWnd, TEXT("DirectInputの初期化に失敗しました。"), TEXT("Error"), MB_OK);
		return FALSE;
	}

	return TRUE;
}

void DX8Input::Release() {
	if (pDInput != NULL) {
		pDInput->Release();
	}
}

BOOL DX8Device::Create(HWND hWnd, LPDIRECTINPUT8 pDInput,const GUID guid, LPCDIDATAFORMAT format) {
	HINSTANCE hInst;
	HRESULT   hr;


	hr = pDInput->CreateDevice(guid, &pDIDevice, NULL);
	if (FAILED(hr)) {
		//MessageBox(hWnd, TEXT("デバイス作成に失敗しました。"), TEXT("Error"), MB_OK);
		return FALSE;
	}
	if (pDIDevice != 0x00000000) {
		hr = pDIDevice->SetDataFormat(format);
		if (FAILED(hr)) {
			Release();
			MessageBox(hWnd, TEXT("データフォーマットの設定に失敗しました。"), TEXT("Error"), MB_OK);
			return FALSE;
		}
	}

	return TRUE;
}

void DX8Device::Release() {
	if (pDIDevice != NULL) {
		pDIDevice->Unacquire();
		pDIDevice->Release();
	}
}
