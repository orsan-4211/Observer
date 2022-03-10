#include <WindowsX.h>
#include <tchar.h>
#include <crtdbg.h>
#include <sys/timeb.h>
#include <iostream>

#include "../Include/pcap.h"

#include "../main.h"
#include "../graphic/graphicmain.h"
#include "charmmain.h"
#include "../sidecharm/button.h"

#include "../keyinput.h"

int modef = 0;
bool modeswitchf = 1;
bool closef = 0;

double charmleftpos;

void Timetxtcheck(textboxclass &txtbx, unsigned long long time);
void Timetxtinput(textboxclass &txtbx, Pos posbuff);
void Timetxtdraw(textboxclass &txtbx, Pos posbuff, Pos mouseposbuff);
unsigned long long TimetxtReturn(textboxclass &txtbx);

void Numtxtcheck(textboxclass &txtbx, unsigned long long data, int maxselid);
void Numtxtinput(textboxclass &txtbx, Pos posbuff, int maxselid);
void Numtxtdraw(textboxclass &txtbx, Pos posbuff, Pos mouseposbuff, int maxselid);
unsigned long long NumtxtReturn(textboxclass &txtbx);

void Charm_set_Mode0();
void Charm_main_Mode0(Pos mouseposbuff);

void Charm_set_Mode1();
void Charm_main_Mode1(Pos mouseposbuff);

void Charm_set_Mode2();
void Charm_main_Mode2(Pos mouseposbuff);

void Charm_set_Mode3();
void Charm_main_Mode3(Pos mouseposbuff);

buttonmaster button;
selectboxmaster selectbox;
textboxmaster textbox;


void buttonreset() {
	button.Set();
	selectbox.Set();
	textbox.Set();
}

void resetbutton() {
	resetf = 1;

	modef = 0;
	modeswitchf = 1;
}

void charmmain() {

	Pos mouseposbuff;

	mouseposbuff.x = dms.pos.x;
	mouseposbuff.y = dms.pos.y;

	switch (modef) {
	case 0:
		Charm_set_Mode0();
		Charm_main_Mode0(mouseposbuff);
		break;
	case 1:
		Charm_set_Mode1();
		Charm_main_Mode1(mouseposbuff);
		break;
	case 2:
		Charm_set_Mode2();
		Charm_main_Mode2(mouseposbuff);
		break;
	case 3:
		Charm_set_Mode3();
		Charm_main_Mode3(mouseposbuff);
		break;

	default:
		break;
	}

	button.Checkdel();
	selectbox.Checkdel();
	textbox.Checkdel();
}

void charmdrmain() {
	Analysisdrmain(modef);
}

void Charm_set_Mode0() {
	if(modeswitchf == 1){

		for (int i = 0; i < button.GetUsecount(); i++) {
			button.Get(i).SetFlag(-2);
		}

		for (int i = 0; i < selectbox.GetUsecount(); i++) {
			selectbox.Get(i).SetFlag(-2);
		}

		for (int i = 0; i < textbox.GetUsecount(); i++) {
			textbox.Get(i).SetFlag(-2);
		}

		Pos posbuff;
		Pos sizebuff;
		double tubuff[2], tvbuff[2];

		//閉じるタブ
		posbuff.x = -128;
		posbuff.y = rc.bottom - (12 + TAB_SIZE_Y);
		posbuff.z = 0;

		sizebuff.x = TAB_SIZE_X;
		sizebuff.y = TAB_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 0, 0, tubuff, tvbuff);
		
		posbuff.x = -128;
		posbuff.y = rc.bottom - (12 + SQBUTTON_SIZE);
		posbuff.z = 0;

		sizebuff.x = SQBUTTON_SIZE;
		sizebuff.y = SQBUTTON_SIZE;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 1, 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = rc.bottom - (12 + SQBUTTON_SIZE);
		posbuff.z = 0;

		sizebuff.x = SQBUTTON_SIZE;
		sizebuff.y = SQBUTTON_SIZE;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 2, 0, tubuff, tvbuff);

		modeswitchf = 0;
	}
}

void Charm_main_Mode0(Pos mouseposbuff) {
	Pos posbuff;
	Pos sizebuff;
	double tubuff[2], tvbuff[2];

	double charmleftposbuf;
	for (int j = 0; j < area.usecount; j++) {
		if (area.data[j].GetId() == 0) {
			charmleftposbuf = area.data[j].GetPos().x - area.data[j].GetPosxy(0);
			break;
		}
	}

	for (int i = 0; i < button.GetUsecount(); i++) {
		if (button.Get(i).GetFlag() != -2) {

			button.Get(i).Check(mouseposbuff, mouseleft);

			switch (button.Get(i).GetId()) {
			case 0:
				if (closef) {
					tubuff[0] = 0.25;
					tubuff[1] = 0.2;
					tvbuff[0] = 0.1;
					tvbuff[1] = 0.2;


					if (button.Get(i).GetFlag() == 1) {
						closef = 0;

						tubuff[0] = 0.3;
						tubuff[1] = 0.25;
						tvbuff[0] = 0.1;
						tvbuff[1] = 0.2;
					}
				}
				else {

					tubuff[0] = 0.2;
					tubuff[1] = 0.25;
					tvbuff[0] = 0.1;
					tvbuff[1] = 0.2;


					if (button.Get(i).GetFlag() == 1) {
						closef = 1;

						tubuff[0] = 0.25;
						tubuff[1] = 0.3;
						tvbuff[0] = 0.1;
						tvbuff[1] = 0.2;
					}

				}
				button.Get(i).SetTuTv(tubuff, tvbuff);


				posbuff.x = charmleftposbuf - (TAB_SIZE_X / 2);
				posbuff.y = rc.bottom - (12 + TAB_SIZE_Y);
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;

			case 1:

				tubuff[0] = 0.4;
				tubuff[1] = 0.5;
				tvbuff[0] = 0.0;
				tvbuff[1] = 0.1;

				if (button.Get(i).GetFlag() == 1) {
					modef = 1;
					modeswitchf = 1;

					tubuff[0] = 0.5;
					tubuff[1] = 0.6;
					tvbuff[0] = 0.0;
					tvbuff[1] = 0.1;

				}
				
				button.Get(i).SetTuTv(tubuff, tvbuff);
				
				posbuff.x = charmleftposbuf + 450;
				posbuff.y = rc.bottom - (12 + SQBUTTON_SIZE);
				posbuff.z = 0;

				

				button.Get(i).SetPos(posbuff);


				break;
			case 2:

				tubuff[0] = 0.0;
				tubuff[1] = 0.1;
				tvbuff[0] = 0.0;
				tvbuff[1] = 0.1;


				if (button.Get(i).GetFlag() == 1) {
					modef = 2;
					modeswitchf = 1;

					tubuff[0] = 0.1;
					tubuff[1] = 0.2;
					tvbuff[0] = 0.0;
					tvbuff[1] = 0.1;
				}
				
				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf + 450 - (SQBUTTON_SIZE + 10);
				posbuff.y = rc.bottom - (12 + SQBUTTON_SIZE);
				posbuff.z = 0;

				

				button.Get(i).SetPos(posbuff);
				break;

			default:
				break;
			}

			buttonvset(button.Get(i).GetPos(), button.Get(i).GetSize().x, button.Get(i).GetSize().y, 1.0, 0, button.Get(i).GetTuTv(0), button.Get(i).GetTuTv(1));
		}
	}
	if (closef) {
		charmleftpos = view.Width;
		if (frontx < view.Width / 2) {
			if ((view.Width / 2) - frontx >= 0.05) {
				double posbuff = ((view.Width / 2) - frontx);
				if (posbuff > 0.05) posbuff /= 20.0;
				frontx += posbuff;
			}
		}
	}
	else {
		charmleftpos = view.Width - 512;
		if (frontx > 533) {
			if (frontx - 533 >= 0.05) {
				double posbuff = frontx - 533;
				if (posbuff > 0.05) posbuff /= 20.0;
				frontx -= posbuff;
			}
		}
	}
}

void Charm_set_Mode1_Txtbox(int id, double y, double sizex, double sizey, unsigned long long data, int maxselid) {
	Pos posbuff;
	Pos sizebuff;
	double tubuff[2], tvbuff[2];

	posbuff.x = -128;
	posbuff.y = y;
	posbuff.z = 0;

	sizebuff.x = sizex;
	sizebuff.y = sizey;
	sizebuff.z = 0;

	tubuff[0] = 0.0;
	tubuff[1] = 1.0;

	tvbuff[0] = 0.0;
	tvbuff[1] = 1.0;

	modeswitchf = 0;

	int *databuff;
	int databuffcount = 0;
	int datacalledbuff;
	DWORD datacalbuff;
	databuff = (int *)malloc(sizeof(int) * 15);

	datacalbuff = data;
	for (int i = 0; i < 15; i++) {
		datacalledbuff = datacalbuff % 10;
		//datacalbuff -= datacalbuff % 10;
		datacalbuff = datacalbuff / 10;
		//databuff = (int *)realloc(databuff, sizeof(int) * (databuffcount + 1));

		databuff[databuffcount] = datacalledbuff;

		databuffcount++;

	}

	textbox.Add(posbuff, sizebuff, id, databuff, maxselid, 0, tubuff, tvbuff);
}

void Charm_set_Mode1() {
	if (modeswitchf == 1) {

		for (int i = 0; i < button.GetUsecount(); i++) {
			button.Get(i).SetFlag(-2);
		}

		for (int i = 0; i < selectbox.GetUsecount(); i++) {
			selectbox.Get(i).SetFlag(-2);
		}

		for (int i = 0; i < textbox.GetUsecount(); i++) {
			textbox.Get(i).SetFlag(-2);
		}

		Pos posbuff;
		Pos sizebuff;
		double tubuff[2], tvbuff[2];
		
		//閉じるタブ
		posbuff.x = -128;
		posbuff.y = rc.bottom - (12 + TAB_SIZE_Y);
		posbuff.z = 0;

		sizebuff.x = TAB_SIZE_X;
		sizebuff.y = TAB_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 0, 0, tubuff, tvbuff);


		//基準値
		Charm_set_Mode1_Txtbox(0, -12 + (8 + SELECTBOX_SIZE_Y) * 2, SELECTBOX_SIZE_X, SELECTBOX_SIZE_Y, skinset.mlsizeborder, 15);
		/*posbuff.x = -128;
		posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 2;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		modeswitchf = 0;

		int *databuff;
		int databuffcount = 0;
		int datacalledbuff;
		DWORD datacalbuff;
		databuff = (int *)malloc(sizeof(int) * 1);

		datacalbuff = setting.stime;
		for (int i = 0; ; i++) {
			datacalledbuff = datacalbuff % 10;
			datacalbuff -= datacalbuff % 10;
			databuff = (int *)realloc(databuff, sizeof(int) * (databuffcount + 1));

			databuff[databuffcount] = datacalledbuff;

			databuffcount++;
		}

		textbox.Add(posbuff, sizebuff, 0, databuff, databuffcount, 0, tubuff, tvbuff);*/

		//マーカーサイズ
		posbuff.x = -128;
		posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 2;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 0, 2, skinset.markerview, 0, tubuff, tvbuff);

		//マーカーカラー
		posbuff.x = -128;
		posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 2;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 1, 2, skinset.markercol, 0, tubuff, tvbuff);


		//A
		Charm_set_Mode1_Txtbox(1, -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.A[0][0], 3);
		//R
		Charm_set_Mode1_Txtbox(2, -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.R[0][0], 3);
		//G
		Charm_set_Mode1_Txtbox(3, -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.G[0][0], 3);
		//B
		Charm_set_Mode1_Txtbox(4, -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.B[0][0], 3);

		for (int i = 0; i < 2; i++) {
			//A
			Charm_set_Mode1_Txtbox(5 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.A[0][1 + i], 3);
			//R
			Charm_set_Mode1_Txtbox(6 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.R[0][1 + i], 3);
			//G
			Charm_set_Mode1_Txtbox(7 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.G[0][1 + i], 3);
			//B
			Charm_set_Mode1_Txtbox(8 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.B[0][1 + i], 3);
			//基準値
			if(i == 0) Charm_set_Mode1_Txtbox(9 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 2, SELECTBOX_SIZE_X, SELECTBOX_SIZE_Y, skinset.emphasisborder1[0], 15);
			else if(i == 1) Charm_set_Mode1_Txtbox(9 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 2, SELECTBOX_SIZE_X, SELECTBOX_SIZE_Y, skinset.emphasisborder2[0], 15);
		}



		//ラインサイズ
		posbuff.x = -128;
		posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 9;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 2, 2, skinset.lineview, 0, tubuff, tvbuff);

		//ラインカラー
		posbuff.x = -128;
		posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 9;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 3, 2, skinset.linecol, 0, tubuff, tvbuff);

		//A
		Charm_set_Mode1_Txtbox(15, -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.A[1][0], 3);
		//R
		Charm_set_Mode1_Txtbox(16, -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.R[1][0], 3);
		//G
		Charm_set_Mode1_Txtbox(17, -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.G[1][0], 3);
		//B
		Charm_set_Mode1_Txtbox(18, -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.B[1][0], 3);

		for (int i = 0; i < 2; i++) {
			//A
			Charm_set_Mode1_Txtbox(19 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.A[1][1 + i], 3);
			//R
			Charm_set_Mode1_Txtbox(20 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.R[1][1 + i], 3);
			//G
			Charm_set_Mode1_Txtbox(21 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.G[1][1 + i], 3);
			//B
			Charm_set_Mode1_Txtbox(22 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 3, 60, SELECTBOX_SIZE_Y, skinset.B[1][1 + i], 3);
			//基準値
			if (i == 0) Charm_set_Mode1_Txtbox(23 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 2, SELECTBOX_SIZE_X, SELECTBOX_SIZE_Y, skinset.emphasisborder1[1], 15);
			else if (i == 1) Charm_set_Mode1_Txtbox(23 + (5 * i), -12 + (8 + SELECTBOX_SIZE_Y) * 2, SELECTBOX_SIZE_X, SELECTBOX_SIZE_Y, skinset.emphasisborder2[1], 15);
		}

		//パケット表示
		posbuff.x = -128;
		posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 14;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 4, 2, skinset.packetview, 0, tubuff, tvbuff);

		//パケットサイズ
		posbuff.x = -128;
		posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 14;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 5, 2, skinset.packetsize, 0, tubuff, tvbuff);

		modeswitchf = 0;

		Charm_set_Mode1_Txtbox(29, -12 + (8 + SELECTBOX_SIZE_Y) * 14, SELECTBOX_SIZE_X, SELECTBOX_SIZE_Y, skinset.packsizeborder, 15);

		//R
		Charm_set_Mode1_Txtbox(31, -12 + (8 + SELECTBOX_SIZE_Y) * 2, 60, SELECTBOX_SIZE_Y, (skinset.SRGB % 0x01000000) / 0x00010000, 3);
		//G
		Charm_set_Mode1_Txtbox(32, -12 + (8 + SELECTBOX_SIZE_Y) * 2, 60, SELECTBOX_SIZE_Y, (skinset.SRGB % 0x00010000) / 0x00000100, 3);
		//B
		Charm_set_Mode1_Txtbox(33, -12 + (8 + SELECTBOX_SIZE_Y) * 2, 60, SELECTBOX_SIZE_Y, (skinset.SRGB % 0x00000100) / 0x00000001, 3);
		

		//スキンセレクター
		posbuff.x = -128;
		posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 1;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 6, foldcount + 1, skinset.skinid + 1, 0, tubuff, tvbuff);

		modeswitchf = 0;
	}
}

void Charm_move_Mode1_Txtbox(int i, int id, double x, double y, unsigned long long &data, int maxselid, Pos mouseposbuff) {
	
	Pos posbuff;
	Pos sizebuff;
	double tubuff[2], tvbuff[2];
	
	posbuff.x = x;
	posbuff.y = y;
	posbuff.z = 0;

	tubuff[0] = 0.0;
	tubuff[1] = 0.7;
	tvbuff[0] = 0.2;
	tvbuff[1] = 0.3;

	if (textbox.Get(i).GetFlag() == 1) {

		tubuff[0] = 0.0;
		tubuff[1] = 0.7;
		tvbuff[0] = 0.3;
		tvbuff[1] = 0.4;
	}

	textbox.Get(i).SetTuTv(tubuff, tvbuff);


	textbox.Get(i).SetPos(posbuff);
	Numtxtcheck(textbox.Get(i), data, maxselid);

	Numtxtdraw(textbox.Get(i), posbuff, mouseposbuff, maxselid);

	if (textbox.Get(i).GetFlag() == 2) {
		Numtxtinput(textbox.Get(i), posbuff, maxselid);
	}
	data = NumtxtReturn(textbox.Get(i));
}

void Charm_move_Mode1_Txtbox(int i, int id, double x, double y, int &data, int maxselid, Pos mouseposbuff) {

	Pos posbuff;
	Pos sizebuff;
	double tubuff[2], tvbuff[2];

	posbuff.x = x;
	posbuff.y = y;
	posbuff.z = 0;

	tubuff[0] = 0.0;
	tubuff[1] = 0.7;
	tvbuff[0] = 0.2;
	tvbuff[1] = 0.3;

	if (textbox.Get(i).GetFlag() == 1) {

		tubuff[0] = 0.0;
		tubuff[1] = 0.7;
		tvbuff[0] = 0.3;
		tvbuff[1] = 0.4;
	}

	textbox.Get(i).SetTuTv(tubuff, tvbuff);


	textbox.Get(i).SetPos(posbuff);
	Numtxtcheck(textbox.Get(i), data, maxselid);

	Numtxtdraw(textbox.Get(i), posbuff, mouseposbuff, maxselid);

	if (textbox.Get(i).GetFlag() == 2) {
		Numtxtinput(textbox.Get(i), posbuff, maxselid);
	}
	data = NumtxtReturn(textbox.Get(i)) < 255 ? NumtxtReturn(textbox.Get(i)) : 255;
}

void Charm_main_Mode1(Pos mouseposbuff) {
	Pos posbuff;
	Pos sizebuff;
	double tubuff[2], tvbuff[2];

	int strcolbuff[3];

	double charmleftposbuf;
	for (int j = 0; j < area.usecount; j++) {
		if (area.data[j].GetId() == 0) {
			charmleftposbuf = area.data[j].GetPos().x - area.data[j].GetPosxy(0);
			break;
		}
	}

	for (int i = 0; i < button.GetUsecount(); i++) {
		if (button.Get(i).GetFlag() != -2) {

			button.Get(i).Check(mouseposbuff, mouseleft);

			switch (button.Get(i).GetId()) {
			case 0:

				tubuff[0] = 0.2;
				tubuff[1] = 0.25;
				tvbuff[0] = 0.1;
				tvbuff[1] = 0.2;


				if (button.Get(i).GetFlag() == 1) {
					modef = 0;
					modeswitchf = 1;

					tubuff[0] = 0.25;
					tubuff[1] = 0.3;
					tvbuff[0] = 0.1;
					tvbuff[1] = 0.2;
				}

				button.Get(i).SetTuTv(tubuff, tvbuff);


				posbuff.x = charmleftposbuf - (TAB_SIZE_X / 2);
				posbuff.y = rc.bottom - (12 + TAB_SIZE_Y);
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;
				/*case 1:

					posbuff.x = charmleftposbuf + 450 - (SQBUTTON_SIZE + 10);
					posbuff.y = rc.bottom - (12 + SQBUTTON_SIZE);
					posbuff.z = 0;

					button.Get(i).SetPos(posbuff);
					break;*/

			default:
				break;
			}

			buttonvset(button.Get(i).GetPos(), button.Get(i).GetSize().x, button.Get(i).GetSize().y, 1.0, 0, button.Get(i).GetTuTv(0), button.Get(i).GetTuTv(1));
		}
	}

	for (int i = 0; i < selectbox.GetUsecount(); i++) {
		if (selectbox.Get(i).GetFlag() != -2) {

			selectbox.Get(i).Check(mouseposbuff, mouseleft);

			switch (selectbox.Get(i).GetId()) {
			case 0:
				skinset.markerview = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 420;
				posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 4;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;
				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.2 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.2 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;

			case 1:
				skinset.markercol = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 1020;
				posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 4;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;
				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.2 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.2 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;

			case 2:
				skinset.lineview = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 420;
				posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 9;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;
				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.2 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.2 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;

			case 3:
				skinset.linecol = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 1020;
				posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 9;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;
				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.2 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.2 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;

			case 4:
				skinset.packetview = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 420;
				posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 14;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;
				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.0 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.0 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;

			case 5:
				skinset.packetsize = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 1020;
				posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 14;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;
				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.2 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.2 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;

			case 6:
				skinset.skinid = selectbox.Get(i).GetSelectid() - 1;
				posbuff.x = charmleftposbuf + 1020;
				posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 1;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {
					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;
					
					selectbox.Get(i).SetSelidcount(foldcount + 1);
					Skin_FoldNameCheck();
					Skin_Load();
					
				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0;

				tvbuff[0] = 0.0;
				tvbuff[1] = 0.0;

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;
				

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}

				if (skinset.skinid != -1) {
					gstr.Stringset(posbuff.x, posbuff.y, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, CHARM_MIDTXT_MARG, 0xFF000000 + skinset.SRGB, 1, _T("%s"), foldname[skinset.skinid].filenamebuff);
				}
				else {
					gstr.Stringset(posbuff.x, posbuff.y, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, CHARM_MIDTXT_MARG, 0xFF000000 + skinset.SRGB, 1, _T("default"));
				}

				break;
			default:
				break;
			}

			selectboxvset(selectbox.Get(i).GetPos(), selectbox.Get(i).GetSize().x, selectbox.Get(i).GetSize().y, 1.0, 0, selectbox.Get(i).GetTuTv(0), selectbox.Get(i).GetTuTv(1));
			selectboxtxtvset(selectbox.Get(i).GetPos(), sizebuff.x, sizebuff.y, 1.0, 0, selectbox.Get(i).GetTxtTuTv(0), selectbox.Get(i).GetTxtTuTv(1));
		}
	}

	for (int i = 0; i < textbox.GetUsecount(); i++) {
		if (textbox.Get(i).GetFlag() != -2) {

			textbox.Get(i).Check(mouseposbuff, mouseleft);

			switch (textbox.Get(i).GetId()) {
			case 0:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 420, -12 + (8 + SELECTBOX_SIZE_Y) * 2, skinset.mlsizeborder, 15, mouseposbuff);
				
				break;
			case 1:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 220, -12 + (8 + SELECTBOX_SIZE_Y) * 5, skinset.A[0][0], 3, mouseposbuff);
				
				break;
			case 2:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 320, -12 + (8 + SELECTBOX_SIZE_Y) * 5, skinset.R[0][0], 3, mouseposbuff);

				break;
			case 3:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 420, -12 + (8 + SELECTBOX_SIZE_Y) * 5, skinset.G[0][0], 3, mouseposbuff);

				break;
			case 4:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 520, -12 + (8 + SELECTBOX_SIZE_Y) * 5, skinset.B[0][0], 3, mouseposbuff);

				break;
			case 5:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 220, -12 + (8 + SELECTBOX_SIZE_Y) * 6, skinset.A[0][1], 3, mouseposbuff);

				break;
			case 6:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 320, -12 + (8 + SELECTBOX_SIZE_Y) * 6, skinset.R[0][1], 3, mouseposbuff);

				break;
			case 7:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 420, -12 + (8 + SELECTBOX_SIZE_Y) * 6, skinset.G[0][1], 3, mouseposbuff);

				break;
			case 8:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 520, -12 + (8 + SELECTBOX_SIZE_Y) * 6, skinset.B[0][1], 3, mouseposbuff);

				break;
			case 9:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 880, -12 + (8 + SELECTBOX_SIZE_Y) * 6, skinset.emphasisborder1[0], 15, mouseposbuff);

				break;
			case 10:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 220, -12 + (8 + SELECTBOX_SIZE_Y) * 7, skinset.A[0][2], 3, mouseposbuff);

				break;
			case 11:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 320, -12 + (8 + SELECTBOX_SIZE_Y) * 7, skinset.R[0][2], 3, mouseposbuff);

				break;
			case 12:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 420, -12 + (8 + SELECTBOX_SIZE_Y) * 7, skinset.G[0][2], 3, mouseposbuff);

				break;
			case 13:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 520, -12 + (8 + SELECTBOX_SIZE_Y) * 7, skinset.B[0][2], 3, mouseposbuff);

				break;
			case 14:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 880, -12 + (8 + SELECTBOX_SIZE_Y) * 7, skinset.emphasisborder2[0], 15, mouseposbuff);

				break;

			case 15:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 220, -12 + (8 + SELECTBOX_SIZE_Y) * 10, skinset.A[1][0], 3, mouseposbuff);

				break;
			case 16:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 320, -12 + (8 + SELECTBOX_SIZE_Y) * 10, skinset.R[1][0], 3, mouseposbuff);

				break;
			case 17:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 420, -12 + (8 + SELECTBOX_SIZE_Y) * 10, skinset.G[1][0], 3, mouseposbuff);

				break;
			case 18:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 520, -12 + (8 + SELECTBOX_SIZE_Y) * 10, skinset.B[1][0], 3, mouseposbuff);

				break;
			case 19:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 220, -12 + (8 + SELECTBOX_SIZE_Y) * 11, skinset.A[1][1], 3, mouseposbuff);

				break;
			case 20:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 320, -12 + (8 + SELECTBOX_SIZE_Y) * 11, skinset.R[1][1], 3, mouseposbuff);

				break;
			case 21:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 420, -12 + (8 + SELECTBOX_SIZE_Y) * 11, skinset.G[1][1], 3, mouseposbuff);

				break;
			case 22:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 520, -12 + (8 + SELECTBOX_SIZE_Y) * 11, skinset.B[1][1], 3, mouseposbuff);

				break;
			case 23:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 880, -12 + (8 + SELECTBOX_SIZE_Y) * 11, skinset.emphasisborder1[1], 15, mouseposbuff);

				break;
			case 24:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 220, -12 + (8 + SELECTBOX_SIZE_Y) * 12, skinset.A[1][2], 3, mouseposbuff);

				break;
			case 25:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 320, -12 + (8 + SELECTBOX_SIZE_Y) * 12, skinset.R[1][2], 3, mouseposbuff);

				break;
			case 26:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 420, -12 + (8 + SELECTBOX_SIZE_Y) * 12, skinset.G[1][2], 3, mouseposbuff);

				break;
			case 27:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 520, -12 + (8 + SELECTBOX_SIZE_Y) * 12, skinset.B[1][2], 3, mouseposbuff);

				break;
			case 28:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 880, -12 + (8 + SELECTBOX_SIZE_Y) * 12, skinset.emphasisborder2[1], 15, mouseposbuff);

				break;
			case 29:
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 880, -12 + (8 + SELECTBOX_SIZE_Y) * 15, skinset.packsizeborder, 15, mouseposbuff);

				break;

			case 31:
				//textbox.Get(i).SetData((textbox.Get(i).GetData(i) % 0x01000000) / 0x00010000);
				strcolbuff[0] = (skinset.SRGB % 0x01000000) / 0x00010000;
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 820, -12 + (8 + SELECTBOX_SIZE_Y) * 2, strcolbuff[0], 3, mouseposbuff);

				break;
			case 32:
				//textbox.Get(i).SetData((textbox.Get(i).GetData(i) % 0x00010000) / 0x00000100);
				strcolbuff[1] = (skinset.SRGB % 0x00010000) / 0x00000100;
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 920, -12 + (8 + SELECTBOX_SIZE_Y) * 2, strcolbuff[1], 3, mouseposbuff);

				break;
			case 33:
				//textbox.Get(i).SetData((textbox.Get(i).GetData(i) % 0x00000100) / 0x00000001);
				strcolbuff[2] = (skinset.SRGB % 0x00000100) / 0x00000001;
				Charm_move_Mode1_Txtbox(i, textbox.Get(i).GetId(), charmleftposbuf + 1020, -12 + (8 + SELECTBOX_SIZE_Y) * 2, strcolbuff[2], 3, mouseposbuff);
			
			default:
				break;
			}

			textboxvset(textbox.Get(i).GetPos(), textbox.Get(i).GetSize().x, textbox.Get(i).GetSize().y, 1.0, 0, textbox.Get(i).GetTuTv(0), textbox.Get(i).GetTuTv(1));
		}

	}
	skinset.SRGB =  (int)(strcolbuff[0] * 0x00010000) + (int)(strcolbuff[1] * 0x00000100) + (int)(strcolbuff[2] * 0x00000001);

	//オプション

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.2;
	tvbuff[1] = 0.3;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 1;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.4, 0, tubuff, tvbuff);

	//マーカー

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.5;
	tvbuff[1] = 0.6;

	posbuff.x = charmleftposbuf + 92;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 1.6;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.2, 0, tubuff, tvbuff);

	//ライン

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.6;
	tvbuff[1] = 0.7;

	posbuff.x = charmleftposbuf + 92;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 1.9;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.2, 0, tubuff, tvbuff);

	//基準値

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 105;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 2.3;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);



	//マーカー////////////////////////////////////////////////////////////////////////

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.5;
	tvbuff[1] = 0.6;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 3;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.4, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 4;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//カラー

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.9;
	tvbuff[1] = 1.0;

	posbuff.x = charmleftposbuf + 720;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 4;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//通常

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.3;
	tvbuff[1] = 0.4;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 5;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//A

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.5;
	tvbuff[1] = 0.6;

	posbuff.x = charmleftposbuf + 250;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 5;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//R

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.6;
	tvbuff[1] = 0.7;

	posbuff.x = charmleftposbuf + 350;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 5;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//G

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 450;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 5;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//B

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 550;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 5;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);


	//第一段階

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.1;
	tvbuff[1] = 0.2;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 6;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//A

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.5;
	tvbuff[1] = 0.6;

	posbuff.x = charmleftposbuf + 250;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 6;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//R

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.6;
	tvbuff[1] = 0.7;

	posbuff.x = charmleftposbuf + 350;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 6;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//G

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 450;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 6;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//B

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 550;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 6;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 680;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 5.8;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);
	//基準値

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 680;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 6.2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);



	//第二段階

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.2;
	tvbuff[1] = 0.3;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 7;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//A

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.5;
	tvbuff[1] = 0.6;

	posbuff.x = charmleftposbuf + 250;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 7;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//R

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.6;
	tvbuff[1] = 0.7;

	posbuff.x = charmleftposbuf + 350;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 7;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//G

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 450;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 7;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//B

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 550;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 7;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 680;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 6.8;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);
	//基準値

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 680;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 7.2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);



	//ライン////////////////////////////////////////////////////////////////////////

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.6;
	tvbuff[1] = 0.7;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 8;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.4, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 9;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//カラー

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.9;
	tvbuff[1] = 1.0;

	posbuff.x = charmleftposbuf + 720;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 9;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//通常

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.3;
	tvbuff[1] = 0.4;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 10;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//A

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.5;
	tvbuff[1] = 0.6;

	posbuff.x = charmleftposbuf + 250;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 10;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//R

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.6;
	tvbuff[1] = 0.7;

	posbuff.x = charmleftposbuf + 350;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 10;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//G

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 450;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 10;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//B

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 550;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 10;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);


	//第一段階

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.1;
	tvbuff[1] = 0.2;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 11;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//A

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.5;
	tvbuff[1] = 0.6;

	posbuff.x = charmleftposbuf + 250;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 11;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//R

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.6;
	tvbuff[1] = 0.7;

	posbuff.x = charmleftposbuf + 350;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 11;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//G

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 450;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 11;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//B

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 550;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 11;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 680;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 10.8;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);
	//基準値

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 680;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 11.2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);


	//第二段階

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.2;
	tvbuff[1] = 0.3;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 12;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//A

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.5;
	tvbuff[1] = 0.6;

	posbuff.x = charmleftposbuf + 250;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 12;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//R

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.6;
	tvbuff[1] = 0.7;

	posbuff.x = charmleftposbuf + 350;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 12;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//G

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 450;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 12;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//B

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 550;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 12;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 680;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 11.8;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);
	//基準値

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 680;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 12.2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);

	//パケット////////////////////////////////////////////////////////////////////////

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.4;
	tvbuff[1] = 0.5;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 13;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.4, 0, tubuff, tvbuff);

	//表示

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.0;
	tvbuff[1] = 0.1;

	posbuff.x = charmleftposbuf + 120;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 14;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 720;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 14;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 680;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 14.8;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);

	//基準値

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 680;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 15.2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);
	
	//文字色////////////////////////////////////////////////////////////////////////

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.4;
	tvbuff[1] = 0.5;

	posbuff.x = charmleftposbuf + 720;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);


	//R

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.6;
	tvbuff[1] = 0.7;

	posbuff.x = charmleftposbuf + 850;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//G

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.7;
	tvbuff[1] = 0.8;

	posbuff.x = charmleftposbuf + 950;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//B

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 1050;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	//スキン設定

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.1;
	tvbuff[1] = 0.2;

	posbuff.x = charmleftposbuf + 720;
	posbuff.y = -12 + (8 + SELECTBOX_SIZE_Y) * 1;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.3, 0, tubuff, tvbuff);

	charmleftpos = view.Width - 1280;
}

void Charm_set_Mode2() {
	if (modeswitchf == 1) {

		for (int i = 0; i < button.GetUsecount(); i++) {
			button.Get(i).SetFlag(-2);
		}

		for (int i = 0; i < selectbox.GetUsecount(); i++) {
			selectbox.Get(i).SetFlag(-2);
		}

		for (int i = 0; i < textbox.GetUsecount(); i++) {
			textbox.Get(i).SetFlag(-2);
		}

		Pos posbuff;
		Pos sizebuff;
		double tubuff[2], tvbuff[2];

		posbuff.x = -128;
		posbuff.y = rc.bottom - (12 + TAB_SIZE_Y);
		posbuff.z = 0;

		sizebuff.x = TAB_SIZE_X;
		sizebuff.y = TAB_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 0, 0, tubuff, tvbuff);


		//sidecharm
		posbuff.x = -128;
		posbuff.y = (12 + SQBUTTON_SIZE);
		posbuff.z = 0;

		sizebuff.x = SQBUTTON_SIZE;
		sizebuff.y = SQBUTTON_SIZE;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 1, 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = (12 + SQBUTTON_SIZE) * 2;
		posbuff.z = 0;

		sizebuff.x = SQBUTTON_SIZE;
		sizebuff.y = SQBUTTON_SIZE;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 2, 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = (12 + SQBUTTON_SIZE) * 3;
		posbuff.z = 0;

		sizebuff.x = SQBUTTON_SIZE;
		sizebuff.y = SQBUTTON_SIZE;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 3, 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = (12 + SELECTBOX_SIZE_Y) * 2;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 0, 2, skinset.ipview, 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = (12 + SELECTBOX_SIZE_Y) * 3;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 1, 2, skinset.sizeview[0], 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = (12 + SELECTBOX_SIZE_Y) * 4;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 2, 2, skinset.sizeview[1], 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = (12 + SELECTBOX_SIZE_Y) * 5;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 3, 2, skinset.sizeview[2], 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = (12 + SELECTBOX_SIZE_Y) * 6;
		posbuff.z = 0;

		sizebuff.x = SELECTBOX_SIZE_X;
		sizebuff.y = SELECTBOX_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		selectbox.Add(posbuff, sizebuff, 4, 5, setting.viewdatalange, 0, tubuff, tvbuff);

		modeswitchf = 0;
	}
}

void Charm_main_Mode2(Pos mouseposbuff) {
	Pos posbuff;
	Pos sizebuff;
	double tubuff[2], tvbuff[2];

	double charmleftposbuf;
	for (int j = 0; j < area.usecount; j++) {
		if (area.data[j].GetId() == 0) {
			charmleftposbuf = area.data[j].GetPos().x - area.data[j].GetPosxy(0);
			break;
		}
	}


	for (int i = 0; i < button.GetUsecount(); i++) {
		if (button.Get(i).GetFlag() != -2) {

			button.Get(i).Check(mouseposbuff, mouseleft);

			switch (button.Get(i).GetId()) {
			case 0:
				tubuff[0] = 0.2;
				tubuff[1] = 0.25;
				tvbuff[0] = 0.1;
				tvbuff[1] = 0.2;


				if (button.Get(i).GetFlag() == 1) {
					modef = 0;
					modeswitchf = 1;

					tubuff[0] = 0.25;
					tubuff[1] = 0.3;
					tvbuff[0] = 0.1;
					tvbuff[1] = 0.2;
				}

				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf - (TAB_SIZE_X / 2);
				posbuff.y = rc.bottom - (12 + TAB_SIZE_Y);
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;


			case 1:

				tubuff[0] = 0.6;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.0;
				tvbuff[1] = 0.1;

				if (setting.stopf == 0) {
					tubuff[0] = 0.6;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.0;
					tvbuff[1] = 0.1;
				}
				else {
					tubuff[0] = 0.8;
					tubuff[1] = 0.9;
					tvbuff[0] = 0.0;
					tvbuff[1] = 0.1;
				}

				if (button.Get(i).GetFlag() == 1) {
					setting.stopf ^= 1;
					if (setting.stopf == 0) {
						tubuff[0] = 0.7;
						tubuff[1] = 0.8;
						tvbuff[0] = 0.0;
						tvbuff[1] = 0.1;
					}
					else {
						tubuff[0] = 0.9;
						tubuff[1] = 1.0;
						tvbuff[0] = 0.0;
						tvbuff[1] = 0.1;
					}
				}

				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf + 1280 - (SQBUTTON_SIZE + 10);
				posbuff.y = (12 + SQBUTTON_SIZE);
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;

			case 2:

				tubuff[0] = 0.0;
				tubuff[1] = 0.1;
				tvbuff[0] = 0.1;
				tvbuff[1] = 0.2;

				if (button.Get(i).GetFlag() == 1) {
					resetbutton();

					tubuff[0] = 0.1;
					tubuff[1] = 0.2;
					tvbuff[0] = 0.1;
					tvbuff[1] = 0.2;

				}

				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf + 1280 - (SQBUTTON_SIZE + 10);
				posbuff.y = (12 + SQBUTTON_SIZE) * 2;
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;

			case 3:

				tubuff[0] = 0.2;
				tubuff[1] = 0.3;
				tvbuff[0] = 0.0;
				tvbuff[1] = 0.1;

				if (button.Get(i).GetFlag() == 1) {
					modef = 3;
					modeswitchf = 1;

					tubuff[0] = 0.3;
					tubuff[1] = 0.4;
					tvbuff[0] = 0.0;
					tvbuff[1] = 0.1;

				}

				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf + 1280 - (SQBUTTON_SIZE + 10);
				posbuff.y = (12 + SQBUTTON_SIZE) * 3;
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;

			default:
				break;
			}

			buttonvset(button.Get(i).GetPos(), button.Get(i).GetSize().x, button.Get(i).GetSize().y, 1.0, 0, button.Get(i).GetTuTv(0), button.Get(i).GetTuTv(1));
		}
	}

	for (int i = 0; i < selectbox.GetUsecount(); i++) {
		if (selectbox.Get(i).GetFlag() != -2) {

			selectbox.Get(i).Check(mouseposbuff, mouseleft);

			switch (selectbox.Get(i).GetId()) {
			case 0:
				skinset.ipview = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 950;
				posbuff.y = (12 + SELECTBOX_SIZE_Y) * 2;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;
				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.0 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.0 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;


			case 1:
				skinset.sizeview[0] = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 950;
				posbuff.y = (12 + SELECTBOX_SIZE_Y) * 3;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;

				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.0 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.0 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;

			case 2:
				skinset.sizeview[1] = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 950;
				posbuff.y = (12 + SELECTBOX_SIZE_Y) * 4;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;

				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.0 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.0 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;

			case 3:
				skinset.sizeview[2] = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 950;
				posbuff.y = (12 + SELECTBOX_SIZE_Y) * 5;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;

				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.0 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.0 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;

			case 4:
				setting.viewdatalange = selectbox.Get(i).GetSelectid();
				posbuff.x = charmleftposbuf + 950;
				posbuff.y = (12 + SELECTBOX_SIZE_Y) * 6;
				posbuff.z = 0;

				selectbox.Get(i).SetPos(posbuff);

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (selectbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;

				}

				selectbox.Get(i).SetTuTv(tubuff, tvbuff);

				tubuff[0] = 0;
				tubuff[1] = 0.5;

				tvbuff[0] = 0.4 + (0.1 * selectbox.Get(i).GetSelectid());
				tvbuff[1] = 0.4 + (0.1 * (selectbox.Get(i).GetSelectid() + 1));

				selectbox.Get(i).SetTxtTuTv(tubuff, tvbuff);

				sizebuff.x = 640 * (selectbox.Get(i).GetSize().y / 128);
				sizebuff.y = selectbox.Get(i).GetSize().y;

				if (selectbox.Get(i).GetFlag() == 2) {
					gstr.Stringset(posbuff.x, posbuff.y, SELECTBOX_SIZE_Y, SELECTBOX_SIZE_Y, 0, 0, 6, 0xFF000000 + skinset.SRGB, 1, _T("<      >"));
				}
				break;

			default:
				break;
			}

			selectboxvset(selectbox.Get(i).GetPos(), selectbox.Get(i).GetSize().x, selectbox.Get(i).GetSize().y, 1.0, 0, selectbox.Get(i).GetTuTv(0), selectbox.Get(i).GetTuTv(1));
			selectboxtxtvset(selectbox.Get(i).GetPos(), sizebuff.x, sizebuff.y, 1.0, 0, selectbox.Get(i).GetTxtTuTv(0), selectbox.Get(i).GetTxtTuTv(1));
		}
	}

	//表示設定

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.0;
	tvbuff[1] = 0.1;

	posbuff.x = charmleftposbuf + 750;
	posbuff.y = (12 + SELECTBOX_SIZE_Y) * 1;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.4, 0, tubuff, tvbuff);

	//IPアドレス

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.3;
	tvbuff[1] = 0.4;

	posbuff.x = charmleftposbuf + 725;
	posbuff.y = (12 + SELECTBOX_SIZE_Y) * 2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);


	//マーカー

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.5;
	tvbuff[1] = 0.6;

	posbuff.x = charmleftposbuf + 725;
	posbuff.y = (12 + SELECTBOX_SIZE_Y) * 2.8;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 725;
	posbuff.y = (12 + SELECTBOX_SIZE_Y) * 3.2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);

	//ライン

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.6;
	tvbuff[1] = 0.7;

	posbuff.x = charmleftposbuf + 725;
	posbuff.y = (12 + SELECTBOX_SIZE_Y) * 3.8;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 725;
	posbuff.y = (12 + SELECTBOX_SIZE_Y) * 4.2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);

	//パケット

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.4;
	tvbuff[1] = 0.5;

	posbuff.x = charmleftposbuf + 725;
	posbuff.y = (12 + SELECTBOX_SIZE_Y) * 4.8;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);

	//サイズ

	tubuff[0] = 0.0;
	tubuff[1] = 0.5;
	tvbuff[0] = 0.8;
	tvbuff[1] = 0.9;

	posbuff.x = charmleftposbuf + 725;
	posbuff.y = (12 + SELECTBOX_SIZE_Y) * 5.2;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);

	//表示範囲

	tubuff[0] = 0.5;
	tubuff[1] = 1.0;
	tvbuff[0] = 0.9;
	tvbuff[1] = 1.0;

	posbuff.x = charmleftposbuf + 725;
	posbuff.y = (12 + SELECTBOX_SIZE_Y) * 6;
	posbuff.z = 0;

	systemtxtvset(posbuff, 512, 128, 0.25, 0, tubuff, tvbuff);

	charmleftpos = view.Width - 1280;
}

void Charm_set_Mode3() {
	if (modeswitchf == 1) {

		for (int i = 0; i < button.GetUsecount(); i++) {
			button.Get(i).SetFlag(-2);
		}

		for (int i = 0; i < selectbox.GetUsecount(); i++) {
			selectbox.Get(i).SetFlag(-2);
		}

		for (int i = 0; i < textbox.GetUsecount(); i++) {
			textbox.Get(i).SetFlag(-2);
		}

		Pos posbuff;
		Pos sizebuff;
		double tubuff[2], tvbuff[2];


		posbuff.x = -128;
		posbuff.y = rc.bottom - (12 + TAB_SIZE_Y);
		posbuff.z = 0;

		sizebuff.x = TAB_SIZE_X;
		sizebuff.y = TAB_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 0, 0, tubuff, tvbuff);


		//sidecharm
		posbuff.x = -128;
		posbuff.y = (12 + SQBUTTON_SIZE);
		posbuff.z = 0;

		sizebuff.x = SQBUTTON_SIZE;
		sizebuff.y = SQBUTTON_SIZE;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 1, 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = (12 + SQBUTTON_SIZE) * 2;
		posbuff.z = 0;

		sizebuff.x = SQBUTTON_SIZE;
		sizebuff.y = SQBUTTON_SIZE;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 2, 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = (12 + SQBUTTON_SIZE) * 3;
		posbuff.z = 0;

		sizebuff.x = SQBUTTON_SIZE;
		sizebuff.y = SQBUTTON_SIZE;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 3, 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = rc.bottom - (12 + TAB_SIZE_Y);
		posbuff.z = 0;

		sizebuff.x = TAB_SIZE_X;
		sizebuff.y = TAB_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 4, 0, tubuff, tvbuff);

		posbuff.x = -128;
		posbuff.y = rc.bottom - (12 + TAB_SIZE_Y);
		posbuff.z = 0;

		sizebuff.x = TAB_SIZE_X;
		sizebuff.y = TAB_SIZE_Y;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		button.Add(posbuff, sizebuff, 5, 0, tubuff, tvbuff);

		//Textbox

		posbuff.x = -128;
		posbuff.y = (12 + SQBUTTON_SIZE) * 3;
		posbuff.z = 0;

		sizebuff.x = 168;
		sizebuff.y = CHARM_MIDTXT_SIZE + 6;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		modeswitchf = 0;

		int *databuff;
		int databuffcount = 0;
		int datacalledbuff;
		DWORD datacalbuff;
		databuff = (int *)malloc(sizeof(int) * 1);

		datacalbuff = setting.stime;
		for (int i = 0; ; i++) {
			switch (i) {
			case 0:
				datacalledbuff = datacalbuff % 10;
				datacalbuff -= datacalbuff % 10;
				break;
			case 1:
				datacalledbuff = (datacalbuff % 60) / 10;
				datacalbuff -= datacalbuff % 60;
				break;
			case 2:
				datacalledbuff = (datacalbuff % 600) / 60;
				datacalbuff -= datacalbuff % 600;
				break;
			case 3:
				datacalledbuff = (datacalbuff % 3600) / 600;
				datacalbuff -= datacalbuff % 3600;
				break;
			default:
				int xbuff = 10;
				int xbuffb = 1;
				for (int j = 0; j < i - 4; j++) {
					xbuff *= 10;
					xbuffb *= 10;
				}
				datacalledbuff = (datacalbuff % (3600 * xbuff)) / (3600 * xbuffb);
				datacalbuff -= datacalbuff % (3600 * xbuff);
				break;
			}
			if (databuffcount >= 5 && datacalbuff <= 0) break;
			databuff = (int *)realloc(databuff, sizeof(int) * (databuffcount + 1));

			databuff[databuffcount] = datacalledbuff;

			databuffcount++;
		}

		textbox.Add(posbuff, sizebuff, 0, databuff, databuffcount, 0, tubuff, tvbuff);


		posbuff.x = -128;
		posbuff.y = (12 + SQBUTTON_SIZE) * 3;
		posbuff.z = 0;

		sizebuff.x = 168;
		sizebuff.y = CHARM_MIDTXT_SIZE + 6;
		sizebuff.z = 0;

		tubuff[0] = 0.0;
		tubuff[1] = 1.0;

		tvbuff[0] = 0.0;
		tvbuff[1] = 1.0;

		modeswitchf = 0;

		free(databuff);

		databuff = (int *)malloc(sizeof(int) * 1);

		databuffcount = 0;

		datacalbuff = setting.etime;
		for (int i = 0; ; i++) {
			switch (i) {
			case 0:
				datacalledbuff = datacalbuff % 10;
				datacalbuff -= datacalbuff % 10;
				break;
			case 1:
				datacalledbuff = (datacalbuff % 60) / 10;
				datacalbuff -= datacalbuff % 60;
				break;
			case 2:
				datacalledbuff = (datacalbuff % 600) / 60;
				datacalbuff -= datacalbuff % 600;
				break;
			case 3:
				datacalledbuff = (datacalbuff % 3600) / 600;
				datacalbuff -= datacalbuff % 3600;
				break;
			default:
				int xbuff = 10;
				int xbuffb = 1;
				for (int j = 0; j < i - 4; j++) {
					xbuff *= 10;
					xbuffb *= 10;
				}
				datacalledbuff = (datacalbuff % (3600 * xbuff)) / (3600 * xbuffb);
				datacalbuff -= datacalbuff % (3600 * xbuff);
				break;
			}
			if (databuffcount >= 5 && datacalbuff <= 0) break;
			databuff = (int *)realloc(databuff, sizeof(int) * (databuffcount + 1));

			databuff[databuffcount] = datacalledbuff;

			databuffcount++;
		}

		textbox.Add(posbuff, sizebuff, 1, databuff, databuffcount, 0, tubuff, tvbuff);

		free(databuff);

	}
}

void Charm_main_Mode3(Pos mouseposbuff) {
	Pos posbuff;
	Pos sizebuff;
	double tubuff[2], tvbuff[2];
	double strposbuff;

	int datacalledbuff;
	int databuffcount = 0;
	unsigned long datacalbuff;
	int selidbuff;

	double charmleftposbuf;
	for (int j = 0; j < area.usecount; j++) {
		if (area.data[j].GetId() == 0) {
			charmleftposbuf = area.data[j].GetPos().x - area.data[j].GetPosxy(0);
			break;
		}
	}


	for (int i = 0; i < button.GetUsecount(); i++) {
		if (button.Get(i).GetFlag() != -2) {

			button.Get(i).Check(mouseposbuff, mouseleft);

			switch (button.Get(i).GetId()) {
			case 0:
				tubuff[0] = 0.2;
				tubuff[1] = 0.25;
				tvbuff[0] = 0.1;
				tvbuff[1] = 0.2;


				if (button.Get(i).GetFlag() == 1) {
					modef = 0;
					modeswitchf = 1;

					tubuff[0] = 0.25;
					tubuff[1] = 0.3;
					tvbuff[0] = 0.1;
					tvbuff[1] = 0.2;
				}

				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf - (TAB_SIZE_X / 2);
				posbuff.y = rc.bottom - (12 + TAB_SIZE_Y);
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;


			case 1:

				tubuff[0] = 0.6;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.0;
				tvbuff[1] = 0.1;

				if (setting.stopf == 0) {
					tubuff[0] = 0.6;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.0;
					tvbuff[1] = 0.1;
				}
				else {
					tubuff[0] = 0.8;
					tubuff[1] = 0.9;
					tvbuff[0] = 0.0;
					tvbuff[1] = 0.1;
				}

				if (button.Get(i).GetFlag() == 1) {
					setting.stopf ^= 1;
					if (setting.stopf == 0) {
						tubuff[0] = 0.7;
						tubuff[1] = 0.8;
						tvbuff[0] = 0.0;
						tvbuff[1] = 0.1;
					}
					else {
						tubuff[0] = 0.9;
						tubuff[1] = 1.0;
						tvbuff[0] = 0.0;
						tvbuff[1] = 0.1;
					}
				}

				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf + 1280 - (SQBUTTON_SIZE + 10);
				posbuff.y = (12 + SQBUTTON_SIZE);
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;

			case 2:
				tubuff[0] = 0.0;
				tubuff[1] = 0.1;
				tvbuff[0] = 0.1;
				tvbuff[1] = 0.2;

				if (button.Get(i).GetFlag() == 1) {
					resetbutton();

					tubuff[0] = 0.1;
					tubuff[1] = 0.2;
					tvbuff[0] = 0.1;
					tvbuff[1] = 0.2;

				}

				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf + 1280 - (SQBUTTON_SIZE + 10);
				posbuff.y = (12 + SQBUTTON_SIZE) * 2;
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;

			case 3:


				tubuff[0] = 0.0;
				tubuff[1] = 0.1;
				tvbuff[0] = 0.0;
				tvbuff[1] = 0.1;

				if (button.Get(i).GetFlag() == 1) {
					modef = 2;
					modeswitchf = 1;

					tubuff[0] = 0.1;
					tubuff[1] = 0.2;
					tvbuff[0] = 0.0;
					tvbuff[1] = 0.1;
				}
				
				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf + 1280 - (SQBUTTON_SIZE + 10);
				posbuff.y = (12 + SQBUTTON_SIZE) * 3;
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;

			case 4:
				tubuff[0] = 0.2;
				tubuff[1] = 0.25;
				tvbuff[0] = 0.1;
				tvbuff[1] = 0.2;


				if (button.Get(i).GetFlag() == 1) {
					setting.endtimechase = 1;

					tubuff[0] = 0.25;
					tubuff[1] = 0.3;
					tvbuff[0] = 0.1;
					tvbuff[1] = 0.2;
				}

				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf + 785;
				posbuff.y = 40;
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;

			case 5:
				tubuff[0] = 0.25;
				tubuff[1] = 0.2;
				tvbuff[0] = 0.1;
				tvbuff[1] = 0.2;


				if (button.Get(i).GetFlag() == 1) {
					setting.stime = 0;

					tubuff[0] = 0.3;
					tubuff[1] = 0.25;
					tvbuff[0] = 0.1;
					tvbuff[1] = 0.2;
				}

				button.Get(i).SetTuTv(tubuff, tvbuff);

				posbuff.x = charmleftposbuf + 361;
				posbuff.y = 40;
				posbuff.z = 0;

				button.Get(i).SetPos(posbuff);
				break;

			default:
				break;
			}

			buttonvset(button.Get(i).GetPos(), button.Get(i).GetSize().x, button.Get(i).GetSize().y, 1.0, 0, button.Get(i).GetTuTv(0), button.Get(i).GetTuTv(1));
		}
	}

	for (int i = 0; i < textbox.GetUsecount(); i++) {
		if (textbox.Get(i).GetFlag() != -2) {

			textbox.Get(i).Check(mouseposbuff, mouseleft);

			switch (textbox.Get(i).GetId()) {
			case 0:

				posbuff.x = charmleftposbuf + 481;
				posbuff.y = 40;
				posbuff.z = 0;

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (textbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;
				}

				textbox.Get(i).SetTuTv(tubuff, tvbuff);

				
				textbox.Get(i).SetPos(posbuff);

				Timetxtcheck(textbox.Get(i), setting.stime);

				Timetxtdraw(textbox.Get(i), posbuff, mouseposbuff);

				if (textbox.Get(i).GetFlag() == 2) {
					Timetxtinput(textbox.Get(i), posbuff);
				}

				for (int j = 0; j < textbox.GetUsecount(); j++) {
					if (textbox.Get(j).GetFlag() != -2) {
						if (textbox.Get(j).GetId() == 1) {
							int timebuff[2] = {TimetxtReturn(textbox.Get(i)), TimetxtReturn(textbox.Get(j)) };
							if (timebuff[0] > timebuff[1]) {
								Timetxtcheck(textbox.Get(i), timebuff[1]);
							}
						}
					}
				}


				setting.stime = TimetxtReturn(textbox.Get(i));

				gstr.Stringset(posbuff.x + 4, 43, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE / 1.5, 0, 0, CHARM_MIDTXT_MARG, 0x80000000 + skinset.SRGB, 1, _T("   h  m  s"));
				break;

			case 1:

				posbuff.x = charmleftposbuf + 665;
				posbuff.y = 40;
				posbuff.z = 0;

				tubuff[0] = 0.0;
				tubuff[1] = 0.7;
				tvbuff[0] = 0.2;
				tvbuff[1] = 0.3;

				if (textbox.Get(i).GetFlag() == 1) {

					tubuff[0] = 0.0;
					tubuff[1] = 0.7;
					tvbuff[0] = 0.3;
					tvbuff[1] = 0.4;
				}

				textbox.Get(i).SetTuTv(tubuff, tvbuff);
				
				textbox.Get(i).SetPos(posbuff);

				Timetxtcheck(textbox.Get(i), setting.etime);

				Timetxtdraw(textbox.Get(i), posbuff, mouseposbuff);

				if (textbox.Get(i).GetFlag() == 2) {
					Timetxtinput(textbox.Get(i), posbuff);
				}

				setting.etime = TimetxtReturn(textbox.Get(i));

				gstr.Stringset(posbuff.x + 4, 43, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE / 1.5, 0, 0, CHARM_MIDTXT_MARG, 0x80000000 + skinset.SRGB, 1, _T("   h  m  s"));
				break;
			default:
				break;
			}

			textboxvset(textbox.Get(i).GetPos(), textbox.Get(i).GetSize().x, textbox.Get(i).GetSize().y, 1.0, 0, textbox.Get(i).GetTuTv(0), textbox.Get(i).GetTuTv(1));
		}

		gstr.Stringset(charmleftposbuf + 573, 40, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, CHARM_MIDTXT_MARG, 0xFF000000 + skinset.SRGB, 1, _T("~"));
	}
	

	charmleftpos = view.Width - 1280;
}

void Timetxtcheck(textboxclass &txtbx, unsigned long long time) {

	int datacalledbuff;
	int databuffcount = 0;
	unsigned long long datacalbuff;
	int selidbuff;

	datacalbuff = time;
	databuffcount = 0;
	selidbuff = txtbx.GetSelectid();

	for (int j = 0; j < 7; j++) {
		switch (j) {
		case 0:
			if (setting.endtimechase) {
				datacalledbuff = datacalbuff % 10;
				datacalbuff -= datacalbuff % 10;
			}
			else {
				datacalledbuff = 0;
			}
			break;
		case 1:
			datacalledbuff = (datacalbuff % 60) / 10;
			datacalbuff -= datacalbuff % 60;
			break;
		case 2:
			datacalledbuff = (datacalbuff % 600) / 60;
			datacalbuff -= datacalbuff % 600;
			break;
		case 3:
			datacalledbuff = (datacalbuff % 3600) / 600;
			datacalbuff -= datacalbuff % 3600;
			break;
		default:
			int xbuff = 10;
			int xbuffb = 1;
			for (int k = 0; k < j - 4; k++) {
				xbuff *= 10;
				xbuffb *= 10;
			}
			datacalledbuff = (datacalbuff % (3600 * xbuff)) / (3600 * xbuffb);
			datacalbuff -= datacalbuff % (3600 * xbuff);
			break;
		}
		txtbx.SetSelectid(j);
		txtbx.SetData(datacalledbuff);
		databuffcount++;
	}

	txtbx.SetSelectid(selidbuff);
}

void Timetxtinput(textboxclass &txtbx, Pos posbuff) {
	double strposbuff;

	int maxbuff = analysis.time.GetCaptureTime(0);
	int timebuff;

	strposbuff = (CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) * txtbx.GetSelectid() + (txtbx.GetSelectid() < 5 ? (CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) * (txtbx.GetSelectid() / 2) : (CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) * 2);
	gstr.Stringset(posbuff.x + 60 - strposbuff, posbuff.y, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, 0, 0xFF000000 + skinset.SRGB, 1, _T("_"));

	for (int k = 0; k < 10; k++) {
		if (keynumkey[k] == 1) {
			if (k > 5 && (txtbx.GetSelectid() == 1 || txtbx.GetSelectid() == 3)) {
				txtbx.SetData(5);
			}
			else {
				txtbx.SetData(k);
			}

			if (txtbx.GetSelectid() < 6) {
				txtbx.SetSelectid(txtbx.GetSelectid() + 1);
			}

			if (txtbx.GetId() == 1) {
				setting.endtimechase = 0;
			}

			break;
		}

	}

	if (keybackspace == 1) {
		txtbx.SetData(0);
		if (txtbx.GetSelectid() > 1) {
			txtbx.SetSelectid(txtbx.GetSelectid() - 1);
		}
		if (txtbx.GetId() == 1) {
			setting.endtimechase = 0;
		}
	}

	timebuff = TimetxtReturn(txtbx);

	if (maxbuff < timebuff) {
		Timetxtcheck(txtbx, maxbuff);
		if (txtbx.GetId() == 1) {
			setting.endtimechase = 1;
		}
	}
}

void Timetxtdraw(textboxclass &txtbx, Pos posbuff, Pos mouseposbuff) {
	double strposbuff = 0;

	int datacalledbuff;
	int databuffcount = 0;
	unsigned long datacalbuff;
	int selidbuff;

	double charmleftposbuf;

	for (int j = 0; j < txtbx.GetSelidcount(); j++) {
		if (j < 5 || (txtbx.GetData(j) > 0 && j == 6) || (txtbx.GetData(j) > 0 || txtbx.GetData(j + 1) > 0)) {
			gstr.Stringset(posbuff.x + 60 - strposbuff, posbuff.y, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, 0, 0xFF000000 + skinset.SRGB, 1, _T("%d"), txtbx.GetData(j));
		}

		if (txtbx.GetFlag() == 1) {
			if (mouseposbuff.y >= posbuff.y - (txtbx.GetSize().y / 2) && mouseposbuff.y <= posbuff.y + (txtbx.GetSize().y / 2)) {
				if (mouseposbuff.x >= posbuff.x + 60 - (strposbuff + ((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) / 2))
					&& mouseposbuff.x <= posbuff.x + 60 - (strposbuff - ((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) / 2))) {
					txtbx.SetSelectid(j);
				}
			}
		}

		strposbuff += (CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG);
		if (j == 1 || j == 3) strposbuff += (CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG);

		
	}

	if (mainkeyleft == 1 || (mainkeyleft > 15 && mainkeyleft % 10 == 0)) {
		txtbx.SetSelectid(txtbx.GetSelectid() + 1);
	}
	if (mainkeyright == 1 || (mainkeyright > 15 && mainkeyright % 10 == 0)) {
		txtbx.SetSelectid(txtbx.GetSelectid() - 1);
	}

	if (txtbx.GetSelectid() < 1) {
		txtbx.SetSelectid(1);
	}
	if (txtbx.GetSelectid() >= 7) {
		txtbx.SetSelectid(6);
	}

	

}

unsigned long long TimetxtReturn(textboxclass &txtbx) {

	unsigned long long datacalledbuff = 0;

	for (int i = 0; i < txtbx.GetSelidcount(); i++) {
		switch (i) {
		case 0:
			datacalledbuff += txtbx.GetData(i);
			break;
		case 1:
			datacalledbuff += txtbx.GetData(i) * 10;
			break;
		case 2:
			datacalledbuff += txtbx.GetData(i) * 60;
			break;
		case 3:
			datacalledbuff += txtbx.GetData(i) * 600;
			break;
		default:
			int xbuffb = 1;
			for (int j = 0; j < i - 4; j++) {
				xbuffb *= 10;
			}
			datacalledbuff += txtbx.GetData(i) * (3600 * xbuffb);
			break;
		}
	}

	return datacalledbuff;
}


void Numtxtcheck(textboxclass &txtbx, unsigned long long data, int maxselid) {
	unsigned long long datacalledbuff;
	int databuffcount = 0;
	unsigned long long datacalbuff;
	int selidbuff;

	datacalbuff = data;
	databuffcount = 0;
	selidbuff = txtbx.GetSelectid();

	for (int j = 0; j < maxselid; j++) {
		datacalledbuff = datacalbuff % 10;
		//datacalbuff -= datacalbuff % 10;
		datacalbuff = datacalbuff / 10;
		txtbx.SetSelectid(j);
		txtbx.SetData(datacalledbuff);
		databuffcount++;
		
		if (datacalbuff <= 0) break;
	}

	txtbx.SetSelidcount(databuffcount);
	txtbx.SetSelectid(selidbuff);
}

void Numtxtinput(textboxclass &txtbx, Pos posbuff, int maxselid) {
	double strposbuff;
	int selidbuff = txtbx.GetSelectid();


	strposbuff = (CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) * txtbx.GetSelectid();
	
	if (maxselid > selidbuff) {
		for (int k = 0; k < 10; k++) {
			if (keynumkey[k] == 1 || (keynumkey[k] > 15 && keynumkey[k] % 10 == 0)) {
				txtbx.SetSelidcount(txtbx.GetSelidcount() + 1);
				for (int l = txtbx.GetSelidcount() - 2; l >= selidbuff; l--) {
					txtbx.SetSelectid(l + 1);
					txtbx.SetData(txtbx.GetData(l));
				}
				txtbx.SetSelectid(selidbuff);
				txtbx.SetData(k);
				break;
			}

		}
	}
	if (txtbx.GetSelidcount() > selidbuff) {
		if (keybackspace == 1 || (keybackspace > 15 && keybackspace % 10 == 0)) {
			selidbuff = txtbx.GetSelectid();
			for (int l = txtbx.GetSelectid(); l < txtbx.GetSelidcount() - 1; l++) {
				txtbx.SetSelectid(l);
				txtbx.SetData(txtbx.GetData(l + 1));
				txtbx.SetSelectid(l + 1);
				txtbx.SetData(0);
			}

			txtbx.SetSelectid(selidbuff);

			txtbx.SetSelidcount(txtbx.GetSelidcount() - 1);
			if (txtbx.GetSelidcount() == 0) {
				txtbx.SetSelidcount(1);
				txtbx.SetSelectid(0);
				txtbx.SetData(0);
			}

		}
	}

}

void Numtxtdraw(textboxclass &txtbx, Pos posbuff, Pos mouseposbuff, int maxselid) {

	double strposbuff = 0;

	int datacalledbuff;
	int databuffcount = 0;
	unsigned long long datacalbuff;
	int selidbuff;

	double charmleftposbuf;
	bool showflag = 0;

	strposbuff = -((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) * (maxselid - 1)) / 2;

	for (int j = txtbx.GetSelidcount() - 1; j >= 0; j--) {
		if (j == 0 || txtbx.GetData(j) > 0 || showflag == 1) {
			gstr.Stringset(posbuff.x  + strposbuff, posbuff.y, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, 0, 0xFF000000 + skinset.SRGB, 1, _T("%d"), txtbx.GetData(j));
			showflag = 1;
		}
		
		if (txtbx.GetFlag() == 1) {
			if (mouseposbuff.y >= posbuff.y - (txtbx.GetSize().y / 2) && mouseposbuff.y <= posbuff.y + (txtbx.GetSize().y / 2)) {
				if (mouseposbuff.x <= posbuff.x + (strposbuff + ((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) / 2))
					&& mouseposbuff.x >= posbuff.x + (strposbuff - ((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) / 2))) {
					txtbx.SetSelectid(j);
				}
			}
		}

		strposbuff += (CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG);

	}

	if (txtbx.GetFlag() == 2) {
		if (txtbx.GetSelectid() == 0 && txtbx.GetSelidcount() == maxselid) {
			gstr.Stringset(posbuff.x - (((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) * (maxselid - 1)) / 2) + ((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) * (txtbx.GetSelidcount() - 1)) + ((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) / 2), posbuff.y, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, 0, 0xFF000000 + skinset.SRGB, 1, _T("|"));
		}
		else {
			gstr.Stringset(posbuff.x - (((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) * (maxselid - 1)) / 2) + ((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) * (txtbx.GetSelidcount() - txtbx.GetSelectid() - 1)) + ((CHARM_MIDTXT_SIZE - CHARM_MIDTXT_MARG) / 2), posbuff.y, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, 0, 0xFF000000 + skinset.SRGB, 1, _T("|"));
		}
	}
	
	if (mainkeyleft == 1 || (mainkeyleft > 15 && mainkeyleft % 10 == 0)) {
		txtbx.SetSelectid(txtbx.GetSelectid() + 1);
	}
	if (mainkeyright == 1 || (mainkeyright > 15 && mainkeyright % 10 == 0)) {
		txtbx.SetSelectid(txtbx.GetSelectid() - 1);
	}

	if (txtbx.GetSelectid() < 0) {
		txtbx.SetSelectid(0);
	}
	if (txtbx.GetSelectid() > txtbx.GetSelidcount()) {
		txtbx.SetSelectid(txtbx.GetSelidcount());
	}
	if (txtbx.GetSelectid() > 15) {
		txtbx.SetSelectid(15);
	}
}

unsigned long long NumtxtReturn(textboxclass &txtbx) {

	unsigned long long datacalledbuff = 0;
	unsigned long long xbuff = 1;

	for (int j = 0; j < txtbx.GetSelidcount(); j++) {
		datacalledbuff += txtbx.GetData(j) * xbuff;
		xbuff *= 10;
	}

	return datacalledbuff;
}