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
		MessageBox(hWnd, TEXT("DirectInput�̏������Ɏ��s���܂����B"), TEXT("Error"), MB_OK);
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
		//MessageBox(hWnd, TEXT("�f�o�C�X�쐬�Ɏ��s���܂����B"), TEXT("Error"), MB_OK);
		return FALSE;
	}
	if (pDIDevice != 0x00000000) {
		hr = pDIDevice->SetDataFormat(format);
		if (FAILED(hr)) {
			Release();
			MessageBox(hWnd, TEXT("�f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂����B"), TEXT("Error"), MB_OK);
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
