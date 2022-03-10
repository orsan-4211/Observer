#include <windows.h>
#include <WindowsX.h>
#include <stdio.h>
#include <shlobj.h>
#include <tchar.h>
#include <crtdbg.h>
#include <iostream>
#include <direct.h>


#include "DX3D_11.h"
#include "Grstr.h"

void gstrclass::Stringcreate(ID3D11Device* device, const wchar_t *path, double x, double y, double texx, double texy, int xcount, double margex, double margey) {

	tex.TexLoadFromFile(device, path);

	chrsize[0] = x / texx;
	chrsize[1] = y / texy;
	chrcount = xcount;

	marge[0] = margex / texx;
	marge[1] = margey / texy;

	vert = (Vertex *)malloc(sizeof(Vertex) * (4 * 2));
	ind = (WORD *)malloc(sizeof(WORD) * (6 * 2));
	for (int strvindi = 0; strvindi < 2; strvindi++) {
		ind[0 + (6 * strvindi)] = 0 + (4 * strvindi);
		ind[1 + (6 * strvindi)] = 1 + (4 * strvindi);
		ind[2 + (6 * strvindi)] = 2 + (4 * strvindi);
		ind[3 + (6 * strvindi)] = 2 + (4 * strvindi);
		ind[4 + (6 * strvindi)] = 3 + (4 * strvindi);
		ind[5 + (6 * strvindi)] = 0 + (4 * strvindi);
	}
	vertc = 0;
	vertcmax = 0;
}

void gstrclass::Stringset(double x, double y, double sizex, double sizey, double strangl, double angl, double shorten, DWORD color, int flag, const TCHAR *FormatString/*•¶Žš—ñ*/, ...) {
	va_list valist;
	int alllen;

		static TCHAR string[8192];
		int gstrbuff, gstrbuff2;
		double xposbuf;

		va_start(valist, FormatString);

		int len = _vsctprintf(FormatString, valist);
		if (len >= 8192)
			_ASSERT(0);

		_vstprintf_s(string, FormatString, valist);
		va_end(valist);

		xposbuf = x;

		if (flag == 1) {
			xposbuf -= ((sizex - shorten) * (len - 1)) / 2.0;
		}

		if (flag == 2) {
			xposbuf -= ((sizex - shorten) * len);
		}

		for (int setsti = 0; setsti < len; setsti++) {
			if (vertc + 4 <= 65535 && indc + 6 <= 65535) {
				vertc += 4;
				indc += 6;

				if (vertc > vertcmax) {
					vertcmax = vertc;
					indcmax = indc;

					vert = (Vertex *)realloc(vert, sizeof(Vertex) * vertc);
					ind = (WORD *)realloc(ind, sizeof(WORD) * indc);

					ind[0 + (indc - 6)] = 0 + (vertc - 4);
					ind[1 + (indc - 6)] = 1 + (vertc - 4);
					ind[2 + (indc - 6)] = 2 + (vertc - 4);
					ind[3 + (indc - 6)] = 0 + (vertc - 4);
					ind[4 + (indc - 6)] = 3 + (vertc - 4);
					ind[5 + (indc - 6)] = 1 + (vertc - 4);
				}

				gstrbuff = string[setsti];
				gstrbuff = gstrbuff - 32;
				gstrbuff2 = gstrbuff / chrcount;
				gstrbuff = gstrbuff % chrcount;

				vert[(vertc - 4) + 0].x = xposbuf + ((sizex / 2) * cos((strangl - 180) * PI / 180.0)) + ((sizey / 2) * cos((strangl - 90) * PI / 180.0));
				vert[(vertc - 4) + 0].y = y + ((sizex / 2) * sin((strangl - 180) * PI / 180.0)) + ((sizey / 2) * sin((strangl - 90) * PI / 180.0));
				vert[(vertc - 4) + 0].z = 0.0f;

				vert[(vertc - 4) + 1].x = xposbuf + ((sizex / 2) * cos(strangl * PI / 180.0)) + ((sizey / 2) * cos((strangl + 90) * PI / 180.0));
				vert[(vertc - 4) + 1].y = y + ((sizex / 2) * sin(strangl * PI / 180.0)) + ((sizey / 2) * sin((strangl + 90) * PI / 180.0));
				vert[(vertc - 4) + 1].z = 0.0f;


				vert[(vertc - 4) + 2].x = xposbuf + ((sizex / 2) * cos((strangl - 180) * PI / 180.0)) + ((sizey / 2) * cos((strangl + 90) * PI / 180.0));
				vert[(vertc - 4) + 2].y = y + ((sizex / 2) * sin((strangl - 180) * PI / 180.0)) + ((sizey / 2) * sin((strangl + 90) * PI / 180.0));
				vert[(vertc - 4) + 2].z = 0.0f;


				vert[(vertc - 4) + 3].x = xposbuf + ((sizex / 2) * cos(strangl * PI / 180.0)) + ((sizey / 2) * cos((strangl - 90) * PI / 180.0));
				vert[(vertc - 4) + 3].y = y + ((sizex / 2) * sin(strangl * PI / 180.0)) + ((sizey / 2) * sin((strangl - 90) * PI / 180.0));
				vert[(vertc - 4) + 3].z = 0.0f;

				vert[(vertc - 4) + 0].tu = chrsize[0] * gstrbuff;
				vert[(vertc - 4) + 0].tv = chrsize[1] * gstrbuff2;
				vert[(vertc - 4) + 1].tu = chrsize[0] * (gstrbuff + 1);
				vert[(vertc - 4) + 1].tv = chrsize[1] * (gstrbuff2 + 1);
				vert[(vertc - 4) + 2].tu = chrsize[0] * gstrbuff;
				vert[(vertc - 4) + 2].tv = chrsize[1] * (gstrbuff2 + 1);
				vert[(vertc - 4) + 3].tu = chrsize[0] * (gstrbuff + 1);
				vert[(vertc - 4) + 3].tv = chrsize[1] * gstrbuff2;
				//0.08203125
				for (int i = 0; i < 4; i++) {
					vert[(vertc - 4) + i].tu += marge[0];
					vert[(vertc - 4) + i].tv += marge[1];
					ColorconvF(vert[(vertc - 4) + i].color, color);
				}


				xposbuf += (sizex - shorten);
				strcount++;
			}
		}

}

void gstrclass::StringCountReset() {
	vertc = 0;
	indc = 0;
	strcount = 0;

}

void gstrclass::Release() {
	free(vert);
	free(ind);
	if(tex.tex != NULL)tex.tex->Release();
}