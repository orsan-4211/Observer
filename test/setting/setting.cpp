#include <WindowsX.h>
#include <tchar.h>
#include <crtdbg.h>
#include <sys/timeb.h>
#include <iostream>

#include "../main.h"

#include "../graphic/graphicmain.h"
#include "../graphic/visobj.h"

#include "setting.h"

TCHAR nepathbuff[256];
necfgfilenameclass *nefilename;
int nefilecount;

NecfgDataMaster necfgdata;
int nefileselect;

void Ne_FileNameSet() {
	nefilename = (necfgfilenameclass *)malloc(sizeof(necfgfilenameclass) * 1);
	nefilecount = 0;
	nefileselect = -1;
}

void Ne_FileNameFree() {
	if (nefilename != NULL) {
		free(nefilename);
		nefilename = NULL;
		nefilecount = 0;
	}
}

void Ne_FileNameCheck() {
	nefilecount = 0;
	setlocale(LC_CTYPE, "jpn");
	size_t leng = 0;
	char pathbasebuff[] = "config/*.necfg";
	mbstowcs_s(&leng, nepathbuff, sizeof(pathbasebuff), pathbasebuff, 260);

	HANDLE hFind;
	WIN32_FIND_DATA fd;

	for (int pathcheck = 0; pathcheck < 256; pathcheck++) {
		if (nepathbuff[pathcheck] == 0xfefe) {
			nepathbuff[pathcheck] = 0x0000;
		}
	}

	hFind = FindFirstFile(nepathbuff, &fd);

	if (hFind == INVALID_HANDLE_VALUE){
		nefilecount = -1;
	}
	
	if (nefilecount != -1) {
		do {
			if (strcmp((const char*)fd.cFileName, ".") == 0
				|| strcmp((const char*)fd.cFileName, "..") == 0) {
			}
			else {
				nefilename = (necfgfilenameclass *)realloc(nefilename, sizeof(necfgfilenameclass) * (nefilecount + 1));
				wsprintf((LPWSTR)nefilename[nefilecount].filenamebuff, _T("%s"), fd.cFileName);
				nefilecount++;
			}

		} while (FindNextFile(hFind, &fd));
	}
	else {
		nefilecount = 0;
	}

	FindClose(hFind);
}

void Ne_FileCheck(int id) {
	FILE *fp;
	char readdata[1024];
	
	char ipstrdatabuff[1024];
	char cfgpathbuff[260];
	char cfgopenpathbuff[260];
	int count;
	int ipstrcount;

	int ipbuff[2][8];
	int ipbuffcount;

	size_t leng = 0;
	int verflag[2] = { 0,0 };
	int flagcount = 0;
	int flagcheckrestf = 0;
	//mbstowcs_s(&leng, nefilename[id].filenamebuff, sizeof(nefilename[id].filenamebuff), cfgpathbuff, 260);
	
	WideCharToMultiByte(CP_ACP, 0, nefilename[id].filenamebuff, -1, cfgpathbuff, sizeof(cfgpathbuff), NULL, NULL);

	sprintf_s(cfgopenpathbuff, "config/%s", cfgpathbuff);
	fopen_s(&fp, cfgopenpathbuff, "r");

	necfgdata.Set();

	ipbuffcount = 0;

	if (fp == NULL) return;

	while (fgets(readdata, 1024, fp) != NULL) {

		count = 0;
		ipstrcount = 0;
		ipbuffcount = 0;
		flagcheckrestf = 0;
		flagcount = 0;
		do {

			//ipv6‚ÍŒ»ó‚Í–³Ž‹
			if (flagcheckrestf == 0) {
				if (readdata[count] == '-') {
					verflag[flagcount] = -1;
					flagcount++;
					flagcheckrestf = 1;
				}else
				if (readdata[count] == '.') {
					verflag[flagcount] = 4;
					flagcount++;
					flagcheckrestf = 1;
				}else
				if (readdata[count] == ':') {
					verflag[flagcount] = 6;
					flagcount++;
					flagcheckrestf = 1;
				}
				if (readdata[count] != ' ' && readdata[count] != '\t') {
					ipstrdatabuff[ipstrcount] = readdata[count];
					ipstrcount++;
				}
			}
			else {
				if (readdata[count] == '-' || readdata[count] == '\0' || readdata[count] == '\n') {
					flagcheckrestf = 2;
				}else
				if (readdata[count] != ' ' && readdata[count] != '\t') {
					ipstrdatabuff[ipstrcount] = readdata[count];
					ipstrcount++;
				}
				
			}
			if (flagcheckrestf == 2) {
				for (int i = 0; i < 8; i++) {
					ipbuff[ipbuffcount][i] = 0;
				}
				ipbuff[ipbuffcount][0] = -1;
				if (verflag[flagcount - 1] == 4) {
					ipstrdatabuff[ipstrcount] = '\0';
					
					sscanf_s(ipstrdatabuff, "%d.%d.%d.%d", &ipbuff[ipbuffcount][0], &ipbuff[ipbuffcount][1], &ipbuff[ipbuffcount][2], &ipbuff[ipbuffcount][3]);
				}
				flagcheckrestf = 0;
				ipstrcount = 0;
				ipbuffcount++;
			}

			count++;
		} while (count < 1024 && readdata[count - 1] != '\n');

		if (ipbuffcount == 2) {
			necfgdata.DataSet(ipbuff[0], verflag[0], ipbuff[1], verflag[1]);
		}
			
	}

#if _DEBUG
	char debptextc[8192];
	if (ORDER_LOG == 1 && ORDER_WORD_LOG == 1) {
		
	}
#endif

	fclose(fp);
}

void Ne_Select() {

	for (int i = -1; i < nefilecount; i++) {
		if ((view.Height / 2 - (22 * (nefilecount / 2.0))) + (22 * i) - 11 <= dms.pos.y
			&& (view.Height / 2 - (22 * (nefilecount / 2.0))) + (22 * i) + 11 >= dms.pos.y) {
			nefileselect = i;
		}
	}

	if (mouseleft == 1
		&& (view.Width / 2) - 128 <= dms.pos.x
		&& (view.Width / 2) + 128 >= dms.pos.x
		&& (view.Height / 2 - (22 * (nefilecount / 2.0))) + (22 * nefileselect) - 11 <= dms.pos.y
		&& (view.Height / 2 - (22 * (nefilecount / 2.0))) + (22 * nefileselect) + 11 >= dms.pos.y) {
		deviceselected = 0;
		if (deviceselected >= 0 && nefileselect >= 0) {
			Ne_FileCheck(nefileselect);
		}
	}

}