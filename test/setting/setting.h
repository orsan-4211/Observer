#ifndef INCLUDED_SETTING
#define INCLUDED_SETTING

#include "../DirectX/lib/DX3D_11.h"

class necfgfilenameclass {
public:
	TCHAR filenamebuff[256];

};

class NecfgData {
public:
	int ip[8];
	int version;

	int connectip[8];
	int connectversion;
};

class NecfgDataMaster {
private:
	NecfgData *data;
	int count;

public:
	void Set() {
		data = (NecfgData *)malloc(sizeof(NecfgData) * 1);

		count = 0;
	}

	void DataSet(int *ip1, int ver1, int *ip2, int ver2) {
		data = (NecfgData *)realloc(data, sizeof(NecfgData) * (count + 1));
		
		data[count].ip[0] = ip1[0];
		data[count].ip[1] = ip1[1];
		data[count].ip[2] = ip1[2];
		data[count].ip[3] = ip1[3];
		data[count].ip[4] = ip1[4];
		data[count].ip[5] = ip1[5];
		data[count].ip[6] = ip1[6];
		data[count].ip[7] = ip1[7];
		data[count].version = ver1;

		data[count].connectip[0] = ip2[0];
		data[count].connectip[1] = ip2[1];
		data[count].connectip[2] = ip2[2];
		data[count].connectip[3] = ip2[3];
		data[count].connectip[4] = ip2[4];
		data[count].connectip[5] = ip2[5];
		data[count].connectip[6] = ip2[6];
		data[count].connectip[7] = ip2[7];
		data[count].connectversion = ver2;
		
		count++;
	}

	void datafree() {
		if (data != NULL) {
			free(data);
		}
		count = 0;
	}

	NecfgData Data(int id) {
		return data[id];
	}

	int Count() {
		return count;
	}

};

extern void Ne_FileNameSet();
extern void Ne_FileNameFree();
extern void Ne_FileNameCheck();
extern void Ne_Select();

extern necfgfilenameclass *nefilename;
extern int nefilecount;

extern NecfgDataMaster necfgdata;
extern int nefileselect;

#endif