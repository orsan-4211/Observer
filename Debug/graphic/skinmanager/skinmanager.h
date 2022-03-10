#ifndef INCLUDED_SKINMANA
#define INCLUDED_SKINMANA

#include "../DirectX/lib/DX3D_11.h"

class skinfoldnameclass {
public:
	TCHAR filenamebuff[256];

};

class skinsetting {
public:
	unsigned long long packsizeborder;//パケットサイズの基準値
	unsigned long long mlsizeborder;//マーカーとラインのサイズ基準値

	int A[2][3];//0:マーカー, 1:ライン
	int R[2][3];
	int G[2][3];
	int B[2][3];
	unsigned long long emphasisborder1[2];//ボーダーサイズ
	unsigned long long emphasisborder2[2];//ボーダーサイズ
	int ipview;
	int sizeview[3];//0:マーカー, 1:ライン, 2:パケット
	int packetview;
	int packetsize;
	int markerview;
	int markercol;
	int lineview;
	int linecol;

	DWORD SRGB;//文字色

	int skinid;

};

class texmanager {
private:
	DX11Tex *data;
	int texcount;
public:
	void Reset() {
		data = (DX11Tex*)malloc(sizeof(DX11Tex) * 1);
		texcount = 0;
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

	void SetData(ID3D11Device* dev,int id, const wchar_t *path) {
		if ((id + 1) > texcount) {
			data = (DX11Tex*)realloc(data, sizeof(DX11Tex) * (id + 1));
			texcount = (id + 1);
		}
		data[id].TexLoadFromFile(dev, path);
	}

	ID3D11ShaderResourceView *Data(int id) {
		if (id < texcount) {
			return data[id].view;
		}
		else {
			return (ID3D11ShaderResourceView *)NULL;
		}
	}

	void Free() {
		for (int i = 0; i < texcount; i++) {
			if (data[i].tex != NULL) {
				data[i].tex->Release();
				data[i].tex = NULL;
			}
			if (data[i].view != NULL) {
				data[i].view->Release();
				data[i].view = NULL;
			}
		}
		if (data != NULL)free(data);
		data = NULL;
	}

};

/*

0	text
1	marker
2	line
3	packet
5	sizegraph_fill
6	sizegraph_bg
7	sizegraph_line
8	sizegraph_boderline
9	circlegraph_0
10	circlegraph_1
11	circlegraph_2
12	circlegraph_3
13	circlegraph_4
14	circlegraph_5
15	circlegraph_6
16	circlegraph_7
17	circlegraph_8
18	circlegraph_9
19	button_setting
20	button_analyze_simple
21	button_analyze_detailed
22	button_pause
23  button_trash
24	button_close
25	selectbox
26	selboxtxt
27	textbox
28	systemtext
29	bg


各グラフ,ボタンのテクスチャIDはオブジェクトIDの若いデータの物が適応される
*/

class objtexdata {
private:

	int id;

public:

	void Setid(int id){
		this->id = id;
	}

	int GetId() {
		return id;
	}

};

extern texmanager texman;
extern objtexdata obj[30];
extern skinsetting skinset;

extern skinfoldnameclass *foldname;
extern int foldcount;

extern void Skin_FoldNameSet();
extern void Skin_FoldNameFree();
extern void Skin_FoldNameCheck();

extern void Skin_Reset();
extern void Skin_TexLoad(int id, const wchar_t *path);
extern void Skin_Settexid(int id, int texid);
extern void Skin_Free();

extern void Skin_Load();

extern void DefaultSkinsetting();

#endif