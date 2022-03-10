#include <WindowsX.h>
#include <tchar.h>
#include <crtdbg.h>
#include <time.h>
#include <iostream>

#include "../main.h"
#include "skinmanager.h"
#include "../graphic/visobj.h"

#include "../defaultskin/defaultskin.h"

texmanager texman;
objtexdata obj[30];
skinsetting skinset;

TCHAR pathbuff[256];
skinfoldnameclass *foldname;
int foldcount;

void DefaultSkinsetting() {

	skinset.packsizeborder = 250;
	skinset.mlsizeborder = 10000;

	skinset.emphasisborder1[0] = 30000;//ボーダーサイズ
	skinset.emphasisborder2[0] = 40000;//ボーダーサイズ

	skinset.emphasisborder1[1] = 30000;//ボーダーサイズ
	skinset.emphasisborder2[1] = 40000;//ボーダーサイズ

	skinset.A[0][0] = 255;
	skinset.R[0][0] = 255;
	skinset.G[0][0] = 255;
	skinset.B[0][0] = 255;

	skinset.A[0][1] = 255;
	skinset.R[0][1] = 255;
	skinset.G[0][1] = 255;
	skinset.B[0][1] = 0;

	skinset.A[0][2] = 255;
	skinset.R[0][2] = 255;
	skinset.G[0][2] = 0;
	skinset.B[0][2] = 0;

	skinset.A[1][0] = 64;
	skinset.R[1][0] = 0;
	skinset.G[1][0] = 0;
	skinset.B[1][0] = 0;

	skinset.A[1][1] = 64;
	skinset.R[1][1] = 255;
	skinset.G[1][1] = 255;
	skinset.B[1][1] = 0;

	skinset.A[1][2] = 64;
	skinset.R[1][2] = 255;
	skinset.G[1][2] = 0;
	skinset.B[1][2] = 0;

	skinset.ipview = 1;
	skinset.packetview = 1;
	skinset.packetsize = 1;
	skinset.markerview = 1;
	skinset.markercol = 1;
	skinset.lineview = 1;
	skinset.linecol = 1;

	skinset.sizeview[0] = 0;
	skinset.sizeview[1] = 0;
	skinset.sizeview[2] = 0;

	skinset.SRGB = 0x000000;

	skinset.skinid = -1;

}

void Skin_FoldNameSet() {
	foldname = (skinfoldnameclass *)malloc(sizeof(skinfoldnameclass) * 1);
	foldcount = 0;
}

void Skin_FoldNameFree() {
	if (foldname != NULL) {
		free(foldname);
		foldname = NULL;
		foldcount = 0;
	}
}

void Skin_FoldNameCheck() {
	foldcount = 0;
	setlocale(LC_CTYPE, "jpn");
	size_t leng = 0;
	char pathbasebuff[] = "texture/*";
	mbstowcs_s(&leng, pathbuff, sizeof(pathbasebuff), pathbasebuff, 260);
	
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	for (int pathcheck = 0; pathcheck < 256; pathcheck++) {
		if (pathbuff[pathcheck] == 0xfefe) {
			pathbuff[pathcheck] = 0x0000;
		}
	}

	hFind = FindFirstFile(pathbuff, &fd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
	}
	do
	{
		if (strcmp((const char*)fd.cFileName, ".") == 0
			|| strcmp((const char*)fd.cFileName, "..") == 0) {
		}
		else {
			foldname = (skinfoldnameclass *)realloc(foldname, sizeof(skinfoldnameclass) * (foldcount + 1));
			wsprintf((LPWSTR)foldname[foldcount].filenamebuff, _T("%s"), fd.cFileName);
			foldcount++;
		}

	} while (FindNextFile(hFind, &fd));
	FindClose(hFind);
}

void Skin_Reset() {
	texman.Reset();

	for (int i = 0; i < 29; i++) {
		obj[i].Setid(0);
	}
}

void Skin_TexLoad(int id, const wchar_t *path){
	texman.SetData(dxg.pDevice, id, path);
}

void Skin_Settexid(int id, int texid) {
	obj[id].Setid(texid);
}

void Skin_Free() {
	texman.Free();
}

void Skin_Load() {
	//texman.Free();
	//Skin_Reset();
	const wchar_t path[512] = _T("");

	if (skinset.skinid != -1) {
		

		wsprintf((LPWSTR)path, _T("texture\\%s\\00.png"), foldname[skinset.skinid].filenamebuff);
		Skin_TexLoad(0, path);

		wsprintf((LPWSTR)path, _T("texture\\%s\\01.png"), foldname[skinset.skinid].filenamebuff);
		Skin_TexLoad(1, path);

		wsprintf((LPWSTR)path, _T("texture\\%s\\02.png"), foldname[skinset.skinid].filenamebuff);
		Skin_TexLoad(2, path);

		wsprintf((LPWSTR)path, _T("texture\\%s\\03.png"), foldname[skinset.skinid].filenamebuff);
		Skin_TexLoad(3, path);

		wsprintf((LPWSTR)path, _T("texture\\%s\\04.png"), foldname[skinset.skinid].filenamebuff);
		Skin_TexLoad(4, path);

		wsprintf((LPWSTR)path, _T("texture\\%s\\05.png"), foldname[skinset.skinid].filenamebuff);
		Skin_TexLoad(5, path);

		wsprintf((LPWSTR)path, _T("texture\\%s\\text.png"), foldname[skinset.skinid].filenamebuff);
		Skin_TexLoad(6, path);

		Skin_Settexid(0, 6);

		Skin_Settexid(1, 0);

		Skin_Settexid(2, 0);

		Skin_Settexid(3, 0);

		Skin_Settexid(5, 1);

		Skin_Settexid(9, 1);

		Skin_Settexid(19, 3);

		Skin_Settexid(25, 3);

		Skin_Settexid(26, 5);

		Skin_Settexid(27, 3);

		Skin_Settexid(28, 4);

		Skin_Settexid(29, 2);
	}
	else {
		defaultskinload();
	}
}