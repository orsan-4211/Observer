#ifndef INCLUDED_GSTR
#define INCLUDED_GSTR

#include "DX3D_11.h"

class gstrclass : public vertclass {
public:
	int strcount;
	DX11Tex tex;
	double chrsize[2];//テクスチャ内の縦横の文字サイズ(px単位)
	int chrcount;//テクスチャ内の横の文字個数
	double marge[2];//テクスチャ内の文字の開始位置
	void Stringcreate(ID3D11Device* device, const wchar_t *path, double x, double y, double texx, double texy, int xcount, double margex, double margey);
	void Stringset(double x, double y, double sizex, double sizey, double strangl, double angl, double shorten, DWORD color, int flag, const TCHAR *FormatString/*文字列*/, ...);
	void StringCountReset();
	void Release();
};


#endif