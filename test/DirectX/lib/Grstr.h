#ifndef INCLUDED_GSTR
#define INCLUDED_GSTR

#include "DX3D_11.h"

class gstrclass : public vertclass {
public:
	int strcount;
	DX11Tex tex;
	double chrsize[2];//�e�N�X�`�����̏c���̕����T�C�Y(px�P��)
	int chrcount;//�e�N�X�`�����̉��̕�����
	double marge[2];//�e�N�X�`�����̕����̊J�n�ʒu
	void Stringcreate(ID3D11Device* device, const wchar_t *path, double x, double y, double texx, double texy, int xcount, double margex, double margey);
	void Stringset(double x, double y, double sizex, double sizey, double strangl, double angl, double shorten, DWORD color, int flag, const TCHAR *FormatString/*������*/, ...);
	void StringCountReset();
	void Release();
};


#endif