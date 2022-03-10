#include <WindowsX.h>
#include <tchar.h>
#include <crtdbg.h>
#include <sys/timeb.h>
#include <iostream>

#include "../Include/pcap.h"

#include "../main.h"

#include "graphicmain.h"
#include "visobj.h"
#include "../setting/setting.h"

#include "../sidecharm/charmmain.h"

unsigned long long visobjtime[2];//表示計測時間, 0:秒以上,1:ミリ秒
DWORD visobjtimes[2];//表示時刻, 0:秒以上,1:ミリ秒

int popupareaid;


void Markerclass::Set(Pos position, double angl, int *ipadd, int version, int deps, int hostid, int sortid) {
	pos = position;
	viewpos = pos;
	for (int i = 0; i < 8; i++) {
		ip[i] = ipadd[i];
	}
	this->version = version;
	size = 0.0;
	psize = 0;
	anmflag = 0;
	phaseflag = 0;
	this->angl = angl;
	this->deps = deps;
	connectcount = 0;
	this->sortid = sortid;

	this->hostid = hostid;

	packsize = (Packsize *)malloc(sizeof(Packsize) * 1);
	pmoveid = (int *)malloc(sizeof(int) * 1);
	unsigned long long dummy[2] = { 0,0 };
	packsize[0].setData(dummy, 0);

	connectid = (int *)malloc(sizeof(int) * 1);
	connectidcount = 0;
	connectid[0] = 0;

	packsizecount = 0;
	sumsize = 0;
	col = 0xffffffff;
}

void Markerclass::Set_Pos(Pos position) {
	pos = position;
}

void Markerclass::Set_ViewPos(Pos position) {
	viewpos = position;
}

void Markerclass::Set_IP(int *ipadd) {
	//int ipsize = sizeof(ipadd) / sizeof(*ipadd);
	for (int i = 0; i < 8; i++) {
		ip[i] = ipadd[i];
	}
}

void Markerclass::Set_Version(int version) {
	this->version = version;
}

void Markerclass::Set_Size(double setsize) {
	size = setsize;
}

void Markerclass::Set_Angl(double angl) {
	this->angl = angl;
}

void Markerclass::Set_PSize(double setsize) {
	psize = setsize;
}

void Markerclass::Set_Phase(int flag) {
	phaseflag = flag;
}

Pos Markerclass::Get_Pos() {
	Pos posbuf;
	posbuf.x = pos.x;
	posbuf.y = pos.y;
	posbuf.z = pos.z;

	return posbuf;
}

Pos Markerclass::Get_ViewPos() {
	Pos posbuf;
	posbuf.x = viewpos.x;
	posbuf.y = viewpos.y;
	posbuf.z = viewpos.z;

	return posbuf;
}


int* Markerclass::Get_IP() {
	return ip;
}

int Markerclass::Get_Version() {
	return version;
}

double Markerclass::Get_Size() {
	return size;
}

double Markerclass::Get_Angl() {
	return angl;
}

int Markerclass::Get_Phase() {
	return phaseflag;
}

void Markerclass::Move(Pos pos, double angl, int connectcount, int centerf) {
	Pos posbuff = Get_Pos();
	int *ipbuff = Get_IP();

	double anglbuff = Get_Angl();
	double anglcalbuff;
	double anglmarge;
	double langebuff;

	Check_Packsize(analysis.time.GetCaptureTime(), skinset.mlsizeborder, skinset.emphasisborder1[0], skinset.emphasisborder2[0]);

	if (version == 4) {
		if (centerf == 1) {
			anglbuff = 0;

			langebuff = 0;

			posbuff.x = 0;
			posbuff.y = 0;
			posbuff.z = 0;
		}else
		if (deps == 0) {
			anglcalbuff = (360.0 / (connectcount + 1) * (sortid + 1)) - anglbuff;

			if (anglcalbuff > 0) {
				if (0.05 > anglcalbuff) {
					anglbuff = 360.0 / (connectcount + 1) * (sortid + 1);
				}
				else {

					if (50.0 < anglcalbuff) {
						anglbuff += 2.0;
					}
					else if (20.0 < anglcalbuff) {
						anglbuff += 1.0;
					}
					else if (10.0 < anglcalbuff) {
						anglbuff += 0.25;
					}
					else if (5.0 < anglcalbuff) {
						anglbuff += 0.1;
					}
					else if (1.0 < anglcalbuff) {
						anglbuff += 0.05;
					}
					else {
						anglbuff += 0.01;
					}
					
				}
			}
			else if (anglcalbuff < 0) {
				if (-0.05 < anglcalbuff) {
					anglbuff = 360.0 / (connectcount + 1) * (sortid + 1);
				}
				else {

					if (-50.0 > anglcalbuff) {
						anglbuff -= 2.0;
					}
					else if (-20.0 > anglcalbuff) {
						anglbuff -= 1.0;
					}
					else if (-10.0 > anglcalbuff) {
						anglbuff -= 0.25;
					}
					else if (-5.0 > anglcalbuff) {
						anglbuff -= 0.1;
					}
					else if (-1.0 > anglcalbuff) {
						anglbuff -= 0.05;
					}
					else {
						anglbuff -= 0.01;
					}

				}
			}

			langebuff = DEF_HPOS_SET_BASE * (5 / ((deps * 0.5) + 1.0 - ((int)(anglbuff / 360) / 50.0)));

			posbuff.x = 0 + (langebuff * cos(anglbuff * PI / 180.0));
			posbuff.y = 0 + (langebuff * sin(anglbuff * PI / 180.0));
			posbuff.z = 0;

		}else
		if (deps == 1) {
			if (hostid == 0) {
				anglcalbuff = (360.0 / connectcount * sortid) - anglbuff;
				if (anglcalbuff > 0) {
					if (0.05 > anglcalbuff) {
						anglbuff = (360.0 / connectcount * sortid);
					}
					else {

						if (50.0 < anglcalbuff) {
							anglbuff += 2.0;
						}
						else if (20.0 < anglcalbuff) {
							anglbuff += 1.0;
						}
						else if (10.0 < anglcalbuff) {
							anglbuff += 0.25;
						}
						else if (5.0 < anglcalbuff) {
							anglbuff += 0.1;
						}
						else if (1.0 < anglcalbuff) {
							anglbuff += 0.05;
						}
						else {
							anglbuff += 0.01;
						}

					}
					
				}
				else if (anglcalbuff < 0) {
					if (-0.05 < anglcalbuff) {
						anglbuff = (360.0 / connectcount * sortid);
					}
					else {

						if (-50.0 > anglcalbuff) {
							anglbuff -= 2.0;
						}
						else if (-20.0 > anglcalbuff) {
							anglbuff -= 1.0;
						}
						else if (-10.0 > anglcalbuff) {
							anglbuff -= 0.25;
						}
						else if (-5.0 > anglcalbuff) {
							anglbuff -= 0.1;
						}
						else if (-1.0 > anglcalbuff) {
							anglbuff -= 0.05;
						}
						else {
							anglbuff -= 0.01;
						}

					}
				}
				
			}
			else {
				anglcalbuff = (angl + 180) + (360.0 / (connectcount + 1) * (sortid + 1)) - anglbuff;
				if (anglcalbuff > 0) {
					if (0.05 > anglcalbuff) {
						anglbuff = (angl + 180) + (360.0 / (connectcount + 1) * (sortid + 1));
					}
					else {

						if (50.0 < anglcalbuff) {
							anglbuff += 2.0;
						}
						else if (20.0 < anglcalbuff) {
							anglbuff += 1.0;
						}
						else if (10.0 < anglcalbuff) {
							anglbuff += 0.25;
						}
						else if (5.0 < anglcalbuff) {
							anglbuff += 0.1;
						}
						else if (1.0 < anglcalbuff) {
							anglbuff += 0.05;
						}
						else {
							anglbuff += 0.01;
						}

					}
				}
				else if (anglcalbuff < 0) {
					if (-0.05 < anglcalbuff) {
						anglbuff = (angl + 180) + (360.0 / (connectcount + 1) * (sortid + 1));
					}
					else {

						if (-50.0 > anglcalbuff) {
							anglbuff -= 2.0;
						}
						else if (-20.0 > anglcalbuff) {
							anglbuff -= 1.0;
						}
						else if (-10.0 > anglcalbuff) {
							anglbuff -= 0.25;
						}
						else if (-5.0 > anglcalbuff) {
							anglbuff -= 0.1;
						}
						else if (-1.0 > anglcalbuff) {
							anglbuff -= 0.05;
						}
						else {
							anglbuff -= 0.01;
						}
					}
				}
			}

			langebuff = DEF_POS_SET_BASE * (5 / ((deps * 0.5) + 1.0 - ((int)(anglbuff / 360) / 50.0)));

			posbuff.x = pos.x + (langebuff * cos(anglbuff * PI / 180.0));
			posbuff.y = pos.y + (langebuff * sin(anglbuff * PI / 180.0));
			posbuff.z = 0;

		}
		else {
			if (connectcount == 1) {
				anglbuff = angl;
			}
			else {
				anglcalbuff = (180.0 / (connectcount + 1) * (sortid + 1)) + angl - 90 - anglbuff;
				if (anglcalbuff > 0) {
					if (0.05 > anglcalbuff) {
						anglbuff = (180.0 / (connectcount + 1) * (sortid + 1)) + angl - 90;
					}
					else{
						
						if (50.0 < anglcalbuff) {
							anglbuff += 2.0;
						}
						else if (20.0 < anglcalbuff) {
							anglbuff += 1.0;
						}
						else if (10.0 < anglcalbuff) {
							anglbuff += 0.25;
						}
						else if (5.0 < anglcalbuff) {
							anglbuff += 0.1;
						}
						else if (1.0 < anglcalbuff) {
							anglbuff += 0.05;
						}
						else {
							anglbuff += 0.01;
						}

					}
				}
				else if (anglcalbuff < 0) {
					if (-0.05 < anglcalbuff) {
						anglbuff = (180.0 / (connectcount + 1) * (sortid + 1)) + angl - 90;
					}
					else {

						if (-50.0 > anglcalbuff) {
							anglbuff -= 2.0;
						}
						else if (-20.0 > anglcalbuff) {
							anglbuff -= 1.0;
						}
						else if (-10.0 > anglcalbuff) {
							anglbuff -= 0.25;
						}
						else if (-5.0 > anglcalbuff) {
							anglbuff -= 0.1;
						}
						else if (-1.0 > anglcalbuff) {
							anglbuff -= 0.05;
						}
						else {
							anglbuff -= 0.01;
						}

					}
				}
			}

			langebuff = DEF_POS_SET_BASE * (5 / ((deps * 0.5) + 1.0));

			posbuff.x = pos.x + (langebuff * cos(anglbuff * PI / 180.0));
			posbuff.y = pos.y + (langebuff * sin(anglbuff * PI / 180.0));
			posbuff.z = 0;
		}
	}

	Set_Pos(posbuff);
	Set_Angl(anglbuff);

	posbuff.x = posbuff.x + frontpos.x;
	posbuff.y = posbuff.y + frontpos.y;

	
	Set_ViewPos(posbuff);


	if (deps == 0) {
		if (size < 1.5) {
			double sizebuff = 1.5 - size;
			if (sizebuff > 0.001) sizebuff /= 20.0;
			size += sizebuff;
		}
		else if (size > 1.5) {
			double sizebuff = size - 1.5;
			if (sizebuff > 0.001) sizebuff /= 20.0;
			size -= sizebuff;
		}
	}
	else {
		if (size < 1.0) {
			double sizebuff = 1.0 - size;
			if (sizebuff > 0.001) sizebuff /= 20.0;
			size += sizebuff;
		}
		else if (size > 1.0) {
			double sizebuff = size - 1.0;
			if (sizebuff > 0.001) sizebuff /= 20.0;
			size -= sizebuff;
		}
	}


	if (skinset.ipview == 1) {
		if (skinset.markerview == 1) {
			if (Get_Version() == 4) {
				gstrinner.Stringset((posbuff.x * viewx) + frontx, (posbuff.y + (((DEF_MARKER_GSIZE / 2) + 10) * (size * psize))) * viewx + fronty, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%d.%d.%d.%d"), ipbuff[0], ipbuff[1], ipbuff[2], ipbuff[3]);
			}
			else {
				gstrinner.Stringset((posbuff.x * viewx) + frontx, (posbuff.y + (((DEF_MARKER_GSIZE / 2) + 10) * (size * psize))) * viewx + fronty, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%x:%x:%x:%x:%x:%x:%x:%x"), ipbuff[0], ipbuff[1], ipbuff[2], ipbuff[3], ipbuff[4], ipbuff[5], ipbuff[6], ipbuff[7]);
			}
		}
		else {
			if (Get_Version() == 4) {
				gstrinner.Stringset((posbuff.x * viewx) + frontx, (posbuff.y + (((DEF_MARKER_GSIZE / 2) + 10) * (size))) * viewx + fronty, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%d.%d.%d.%d"), ipbuff[0], ipbuff[1], ipbuff[2], ipbuff[3]);
			}
			else {
				gstrinner.Stringset((posbuff.x * viewx) + frontx, (posbuff.y + (((DEF_MARKER_GSIZE / 2) + 10) * (size))) * viewx + fronty, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%x:%x:%x:%x:%x:%x:%x:%x"), ipbuff[0], ipbuff[1], ipbuff[2], ipbuff[3], ipbuff[4], ipbuff[5], ipbuff[6], ipbuff[7]);
			}
		}
	}
	
	if (skinset.sizeview[0] == 1) {
		gstrinner.Stringset((posbuff.x * viewx) + frontx, posbuff.y * viewx + fronty, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%d"), sumsize);
	}
	
	double vxbufx, vxbufy;

	vxbufx = ((pos.x / 1.5) * (pos.x / 1.5));
	vxbufy = (pos.y * pos.y);

	double vxaddbuf = DEF_MARKER_GSIZE * (sqrt(((pos.x / 1.5) * (pos.x / 1.5)) + (pos.y * pos.y)) / 400);

	vxbufx += (vxaddbuf * vxaddbuf);
	vxbufy += (vxaddbuf * vxaddbuf);

	if (vxbufx + vxbufy >= vxmaxlength * vxmaxlength) {
		vxmaxlength = sqrt(vxbufx + vxbufy);
	}

}

void Markerclass::JenMove(int *ipadd, int version, int count, int deps, Pos pos, int hostid, int connectcount, double angl, int sortid) {
	Pos posbuff;
	double anglbuff = 0;
	double anglmarge;

	double langebuff;
	unsigned long long int ipbuff = 0;


	/*if (count == 0) {
		posbuff.x = 0;
		posbuff.y = 0;
		posbuff.z = 0;
		anglbuff = 0;
	}
	else {
		anglmarge = (((rand() % 7000) - 3500) / 1000.0);
		if (deps == 0) {

			langebuff = (DEF_HPOS_SET_BASE * (1.0 + (((count - 1) / DEF_HCOUNT_ONELOOP) / 1.5))) * (0.75 + ((rand() % 50) / 100.0));

			posbuff.x = pos.x + (langebuff * cos(((DEF_HANGL  * (count - 1)) + (11.25 * ((count - 1) / DEF_HCOUNT_ONELOOP) + anglmarge) + ((deps) * 1)) * PI / 180.0));
			posbuff.y = pos.y + (langebuff * sin(((DEF_HANGL  * (count - 1)) + (11.25 * ((count - 1) / DEF_HCOUNT_ONELOOP) + anglmarge) + ((deps) * 1)) * PI / 180.0));
			posbuff.z = 0;

			anglbuff = ((DEF_HANGL  * (count - 1)) + (11.25 * ((count - 1) / DEF_HCOUNT_ONELOOP))) + anglmarge + ((deps) * 1);
		}
		else //if (onhost == 1) {
		{
			langebuff = ((DEF_POS_SET_BASE * (3 / (deps / (deps * 0.75)))) * (1.0 + (((count - 1) / DEF_COUNT_ONELOOP % DEF_COUNT_ONELOOP) / 1.5))) * (0.75 + ((rand() % 50) / 100.0));

			posbuff.x = pos.x + (langebuff * cos(((DEF_ANGL  * (count - 1)) + (11.25 * ((count - 1) / DEF_COUNT_ONELOOP % DEF_COUNT_ONELOOP) + anglmarge) + ((deps) * 17)) * PI / 180.0));
			posbuff.y = pos.y + (langebuff * sin(((DEF_ANGL  * (count - 1)) + (11.25 * ((count - 1) / DEF_COUNT_ONELOOP % DEF_COUNT_ONELOOP) + anglmarge) + ((deps) * 17)) * PI / 180.0));
			posbuff.z = 0;
			
			anglbuff = ((DEF_ANGL  * (count - 1)) + (11.25 * ((count - 1) / DEF_COUNT_ONELOOP))) + anglmarge + ((deps) * 17);

		}
	}*/

	if (count == 0) {
		posbuff.x = 0;
		posbuff.y = 0;
		posbuff.z = 0;
		anglbuff = 0;
	}
	else {
		anglmarge = (((rand() % 7000) - 3500) / 1000.0);

		if (version == 4) {
			if (deps == 0) {
				anglbuff = 360.0 / (connectcount + 1) * (sortid + 1);

				langebuff = DEF_HPOS_SET_BASE * (5 / ((deps * 0.5) + 1.0 - ((int)(anglbuff / 360) / 50.0)));

				posbuff.x = 0 + (langebuff * cos(anglbuff * PI / 180.0));
				posbuff.y = 0 + (langebuff * sin(anglbuff * PI / 180.0));
				posbuff.z = 0;

			}else
			if (deps == 1) {
				if (hostid == 0) {
					anglbuff = (360.0 / connectcount * sortid);
				}
				else {
					anglbuff = (angl + 180) + (360.0 / (connectcount + 1) * (sortid + 1));
				}

				langebuff = DEF_POS_SET_BASE * (5 / ((deps * 0.5) + 1.0 - ((int)(anglbuff / 360) / 50.0)));

				posbuff.x = pos.x + (langebuff * cos(anglbuff * PI / 180.0));
				posbuff.y = pos.y + (langebuff * sin(anglbuff * PI / 180.0));
				posbuff.z = 0;

			}
			else {

				if (connectcount == 1) {
					anglbuff = angl;
				}
				else {
					anglbuff = 180.0 / (connectcount + 1) * (sortid + 1);
					anglbuff += angl - 90;
				}

				langebuff = DEF_POS_SET_BASE * (5 / ((deps * 0.5) + 1.0));

				posbuff.x = pos.x + (langebuff * cos(anglbuff * PI / 180.0));
				posbuff.y = pos.y + (langebuff * sin(anglbuff * PI / 180.0));
				posbuff.z = 0;
			}
		}
		else
		{
			langebuff = DEF_POS_SET_BASE * (5 / ((deps * 0.5) + 1.0));

			anglbuff = ((DEF_ANGL  * (count - 1)) + (11.25 * ((count - 1) / DEF_COUNT_ONELOOP % DEF_COUNT_ONELOOP) + anglmarge) + ((deps) * 17)) * PI / 180.0;
			posbuff.x = pos.x + (langebuff * cos(anglbuff));
			posbuff.y = pos.y + (langebuff * sin(anglbuff));
			posbuff.z = 0;

		}

	}

	Set(posbuff, anglbuff, ipadd, version, deps, hostid, sortid);
}


void Lineclass::Set(Pos position1, Pos position2, int newid, int *ip1, int *ip2) {
	pos[0] = position1;
	pos[1] = position2;
	id = id;
	for (int i = 0; i < 8; i++) {
		ip[0][i] = ip1[i];
		ip[1][i] = ip2[i];
	}
	angl = atan2(pos[0].y - pos[1].y, pos[0].x - pos[1].x) * 180.0 / PI;
	size = 0.0;
	phaseflag = 0;
	anmflag = 0;
	psize = 0;
	packsize = (Packsize *)malloc(sizeof(Packsize) * 1);
	pmoveid = (int *)malloc(sizeof(int) * 1);
	packsizecount = 0;
	sumsize = 0;

	col = 0xffffffff;
}

void Lineclass::Set_Pos(Pos position1, Pos position2) {
	pos[0] = position1;
	pos[1] = position2;
	angl = atan2(pos[0].y - pos[1].y, pos[0].x - pos[1].x) * 180.0 / PI;
}

void Lineclass::Set_IP(int i, int *ip) {
	for (int j = 0; j < 8; j++) {
		this->ip[j][i] = ip[i];
	}
}

void Lineclass::Set_Size(double size) {
	this->size = size;
}

void Lineclass::Set_PSize(double size) {
	this->psize = size;
}

void Lineclass::Set_Phase(int flag) {
	phaseflag = flag;
}

Pos Lineclass::Get_Pos(int i) {
	Pos posbuf;
	posbuf.x = pos[i].x;
	posbuf.y = pos[i].y;
	posbuf.z = pos[i].z;

	return posbuf;
}

double Lineclass::Get_Angl() {
	return angl;
}

int* Lineclass::Get_IP(int i) {
	return ip[i];
}

double Lineclass::Get_Size() {
	return size;
}

int Lineclass::Get_Phase() {
	return phaseflag;
}

void Lineclass::Move(Pos pos1, Pos pos2) {
	Set_Pos(pos1, pos2);

	Check_Packsize(analysis.time.GetCaptureTime(), skinset.mlsizeborder, skinset.emphasisborder1[1], skinset.emphasisborder2[1]);

	if (size < 1.0) {
		double sizebuff = 1.0 - size;
		if (sizebuff > 0.001) sizebuff /= 40.0;
		size += sizebuff;
	}
	else if (size > 1.0) {
		double sizebuff = size - 1.0;
		if (sizebuff > 0.001) sizebuff /= 40.0;
		size -= sizebuff;
	}


	if (skinset.sizeview[1] == 1) {
		gstrinner.Stringset((((pos1.x + pos2.x) / 2.0) * viewx) + frontx, ((pos1.y + pos2.y) / 2.0) * viewx + fronty, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%d"), sumsize);
	}


}

void Lineclass::JenMove(Pos pos1, Pos pos2, int lineid, int *ip1, int *ip2, int count) {
	
	Set(pos1, pos2, lineid, ip1, ip2);

}


void Packetclass::Set(Pos position1, Pos position2, int *ip1, int *ip2, int pacsize, unsigned long long *janeratetime, unsigned long long arrivaltime0, unsigned long long arrivaltime1, int tableid, bool reverse) {
	
	pos[0] = position1;
	pos[1] = position2;
	angl = atan2(pos[0].y - pos[1].y, pos[0].x - pos[1].x) * 180.0 / PI;
	for (int i = 0; i < 8; i++) {
		ip[0][i] = ip1[i];
		ip[1][i] = ip2[i];
	}
	moveflame = 0;
	size = 0.0;
	anmflag = 0;
	this->pacsize = (double)pacsize / skinset.packsizeborder;
	opacsize = pacsize;

	this->janeratetime[0] = janeratetime[0];
	this->janeratetime[1] = janeratetime[1];
	this->arrivaltime[0] = arrivaltime0;
	this->arrivaltime[1] = arrivaltime1;

	this->tableid = tableid;
	this->reverse = reverse;

}

void Packetclass::Set_Pos(Pos position, int id) {
	pos[id] = position;
	angl = atan2(pos[0].y - pos[1].y, pos[0].x - pos[1].x) * 180.0 / PI;
}

void Packetclass::Set_IP(int i, int *ip) {
	for (int j = 0; j < 8; j++) {
		this->ip[j][i] = ip[i];
	}
}

void Packetclass::Set_Size(double size) {
	this->size = size;
}

void Packetclass::Add_Moveflame(int i) {
	moveflame += i;
	
}

Pos Packetclass::Get_Pos(int id) {
	Pos posbuf;
	posbuf.x = pos[id].x;
	posbuf.y = pos[id].y;
	posbuf.z = pos[id].z;

	return posbuf;
}

double Packetclass::Get_Angl() {
	return angl;
}

int Packetclass::Get_Moveflame() {
	return moveflame;
}

int* Packetclass::Get_IP(int i) {
	return ip[i];
}

double Packetclass::Get_Size() {
	return size;
}

void Packetclass::Move(Pos pos1, Pos pos2) {
	Set_Pos(pos1, 0);
	Set_Pos(pos2, 1);
	int anmflagbuf = Get_Anmflag();
	Set_Pacsize((double)opacsize / skinset.packsizeborder);

	if (anmflagbuf == 0) {
		if (size < 1.0) {
			double sizebuff = 1.0 - size;
			if (sizebuff > 0.01) sizebuff /= 20.0;
			size += sizebuff;
		}
		else if (size > 1.0) {
			double sizebuff = size - 1.0;
			if (sizebuff > 0.01) sizebuff /= 20.0;
			size -= sizebuff;
		}
	}
	else if (anmflagbuf == 2) {
		if (size > 0.0) {
			if (size > 0.05) {
				size -= 0.05;
			}
			else {
				size = 0;
			}

		}
		else if (size <= 0.0) {
			Set_Anmflag(-1);
		}
	}
	angl = atan2(pos[0].y - pos[1].y, pos[0].x - pos[1].x) * 180.0 / PI;

	if (anmflagbuf == 0) {
		Add_Moveflame(1);
	}
}

void Packetclass::JenMove(Pos pos1, Pos pos2, int *ip1, int *ip2, int count, int pacsize, unsigned long long *janeratetime, unsigned long long arrivaltime0, unsigned long long arrivaltime1, int tableid, bool reverse) {

	Set(pos1, pos2, ip1, ip2, pacsize, janeratetime, arrivaltime0, arrivaltime1, tableid, reverse);

}


void NecfgCheck(int i) {
	int mkcheckf = 0;
	for (int j = 0; j < marker.getUsecount(); j++) {
		if (!IpMatchCheck(marker.Data(j).Get_IP(), necfgdata.Data(i).connectip)) {
			mkcheckf = 1;
			break;
		}
	}
	if (mkcheckf == 0) {
		for (int j = 0; j < necfgdata.Count(); j++) {
			if (!IpMatchCheck(necfgdata.Data(j).ip, necfgdata.Data(i).connectip)) {
				NecfgCheck(j);
				break;
			}
		}
	}
	marker.jenerate(necfgdata.Data(i).ip, necfgdata.Data(i).version, necfgdata.Data(i).connectip);
	return;
}

void MarkerMasterclass::reset(){
	Pos posbuff;

	data = (Markerclass *)malloc(sizeof(Markerclass) * 1);
	usecount = 0;
	hostcount = 0;
	removebuff = (int *)malloc(sizeof(int) * 1);
	removebuff[0] = -1;
	checkedcount = 0;

	tu[0] = 0.0f;
	tu[1] = 0.3f;
	tv[0] = 0.101f;
	tv[1] = 0.4f;
	int dummyip[8] = { 0,0,0,0,0,0,0,0 };
	if (nefileselect == -1) {
		for (int i = 0; i < pdev.data[selecteddevice].hostipmax; i++) {
			jenerate(hdata.data[pdev.data[selecteddevice].hostipid[i]].ip, hdata.data[pdev.data[selecteddevice].hostipid[i]].version, dummyip);
		}
	}
	else {
		for (int i = 0; i < necfgdata.Count(); i++) {
			NecfgCheck(i);
		}
	}
	posbuff.x = 0;
	posbuff.y = 0;
	posbuff.z = 0;

	popupareaid = area.usecount;

	area.jenerate(posbuff, 10, 0, 150, 0, 0);
}

void MarkerMasterclass::jenerate(int *ip, int version, int *conectip){
	
	bool ipjenflag = 1;

	Pos posbuf;

	posbuf.x = 0;
	posbuf.y = 0;
	posbuf.z = 0;

	int onhostf = 0;
	int conbuf = 1;
	int ibuff = -1;
	int depsbuff = 0;
	double anglbuff = 0;
	int sortidbuff = 0;

	int failedid = -1;
	
	for (int i = 0; i < usecount; i++) {
		int ipbuff[8];
		ipbuff[0] = data[i].Get_IP()[0];
		ipbuff[1] = data[i].Get_IP()[1];
		ipbuff[2] = data[i].Get_IP()[2];
		ipbuff[3] = data[i].Get_IP()[3];
		ipbuff[4] = data[i].Get_IP()[4];
		ipbuff[5] = data[i].Get_IP()[5];
		ipbuff[6] = data[i].Get_IP()[6];
		ipbuff[7] = data[i].Get_IP()[7];

		ipjenflag = IpMatchCheck(ipbuff, ip);

		if (ipjenflag) {
			if (IpMatchCheck(ipbuff, conectip) == 0) {
				posbuf = data[i].Get_Pos();
				depsbuff = data[i].Get_Deps() + 1;
				anglbuff = data[i].Get_Angl();
				onhostf = 1;
				ibuff = i;
			}
		}
		else {
			failedid = i;
			break;
		}
	}

	if (onhostf == 0) {
		posbuf = data[0].Get_Pos();
	}

	if (ipjenflag == 1) {
		data = (Markerclass *)realloc(data, sizeof(Markerclass) * (usecount + 1));
		removebuff = (int *)realloc(removebuff, sizeof(int) * (usecount + 1));
		removebuff[usecount] = -1;

		sortidbuff = 0;

		if (ibuff >= 0) {
			data[ibuff].Add_Connectcount(1);
			conbuf = data[ibuff].Get_Connectcount();
			data[ibuff].Add_Connectid(usecount);
			for (int i = 0; i < data[ibuff].Get_ConnectidCount() - 1; i++) {

				if (data[data[ibuff].Get_Connectid(i)].Get_Sortid() >= sortidbuff) {

					for (int j = 0; j < 8; j++) {
						if (data[data[ibuff].Get_Connectid(i)].Get_IP()[j] < ip[j]) {
							sortidbuff = data[data[ibuff].Get_Connectid(i)].Get_Sortid() + 1;
							break;
						}
						if (version == 4 && j <= 3)break;
					}

				}

			}
			for (int i = 0; i < data[ibuff].Get_ConnectidCount(); i++) {

				if (data[data[ibuff].Get_Connectid(i)].Get_Sortid() >= sortidbuff) {
					data[data[ibuff].Get_Connectid(i)].Set_Sortid(data[data[ibuff].Get_Connectid(i)].Get_Sortid() + 1);
				}

			}
		}
		
		if (depsbuff == 0) {
			data[usecount].JenMove(ip, version, usecount, 0, posbuf, -1, conbuf, anglbuff, sortidbuff);
			hostcount++;
		}
		else {
			data[usecount].JenMove(ip, version, usecount, depsbuff, posbuf, ibuff, conbuf, anglbuff, sortidbuff);
		}
		usecount++;
	}
	else {

		if (failedid >= 0) {
			conbuf = 0;
			for (int i = 0; i < data[failedid].Get_ConnectidCount(); i++) {
				if (!IpMatchCheck(data[data[failedid].Get_Connectid(i)].Get_IP(), conectip)) {
					conbuf = 1;
					break;
				}
			}

			for (int i = 0; i < route.getUsecount(); i++) {
				int checkflag = 0;
				if (!IpMatchCheck(route.Data(i).Get_IP(0), ip)) {
					checkflag++;
				}
				else if (!IpMatchCheck(route.Data(i).Get_IP(1), ip)) {
					checkflag++;
				}

				if (!IpMatchCheck(route.Data(i).Get_IP(0), conectip)) {
					checkflag++;
				}
				else if (!IpMatchCheck(route.Data(i).Get_IP(1), conectip)) {
					checkflag++;
				}

				if (checkflag == 2) {
					conbuf = 1;
					break;
				}
			}

			if (conbuf == 0) {
				ibuff = -1;
				conbuf = 0;
				for (int i = 0; i < usecount; i++) {
					if (!IpMatchCheck(data[i].Get_IP(), conectip)) {
						ibuff = i;
						break;
					}
				}


				if (IpMatchCheck(ip, conectip) && ibuff != -1) {


					for (int i = 0; i < data[ibuff].Get_ConnectidCount(); i++) {
						if (!IpMatchCheck(data[data[ibuff].Get_Connectid(i)].Get_IP(), ip)) {
							conbuf = 1;
							break;
						}
					}

#if _DEBUG
					if (CONNECT_LOG == 1) {
						char debptextc[8192];
						sprintf_s(debptextc, "ibuff     %d.%d.%d.%d \n", data[ibuff].Get_IP()[0], data[ibuff].Get_IP()[1], data[ibuff].Get_IP()[2], data[ibuff].Get_IP()[3]);
						OutputDebugStringA(debptextc);
						sprintf_s(debptextc, "failedid  %d.%d.%d.%d \n", data[failedid].Get_IP()[0], data[failedid].Get_IP()[1], data[failedid].Get_IP()[2], data[failedid].Get_IP()[3]);
						OutputDebugStringA(debptextc);
						sprintf_s(debptextc, "ip        %d.%d.%d.%d \n", ip[0], ip[1], ip[2], ip[3]);
						OutputDebugStringA(debptextc);
						sprintf_s(debptextc, "connectip %d.%d.%d.%d \n\n", conectip[0], conectip[1], conectip[2], conectip[3]);
						OutputDebugStringA(debptextc);
					}
#endif
					if (IpMatchCheck(data[data[failedid].Get_Hostid()].Get_IP(), data[ibuff].Get_IP())) {
						data[failedid].Add_Connectcount(1);
						data[failedid].Add_Connectid(ibuff);
					}

					if (conbuf == 0) {
						if (IpMatchCheck(data[data[ibuff].Get_Hostid()].Get_IP(), data[failedid].Get_IP())) {
							data[ibuff].Add_Connectcount(1);
							data[ibuff].Add_Connectid(failedid);
						}
					}
				}
			}
		}
	}

}

void MarkerMasterclass::sort(){

}

void MarkerMasterclass::remove(){
	int removecount = 0;
}

void MarkerMasterclass::datafree() {
	for (int i = 0; i < usecount; i++) {
		data[i].Free_Packsize();
	}

	if (data != NULL) {
		free(data);
	}
	data = NULL;
	usecount = 0;
	if (removebuff != NULL) {
		free(removebuff);
	}
	removebuff = NULL;
}

void MarkerMasterclass::move() {
	Pos posbuff;
	posbuff.x = 0;
	posbuff.y = 0;
	posbuff.z = 0;

	int viewpopupid = -1;

	area.data[popupareaid].SetPos(posbuff, 10, 0, 0, 0, 0);

	int hostidbuff = -1;
	int connectcountbuff = 0;

	if ((pdatacheckf & 0b00100) == 0b00000) {
		for (int i = 0; i < pdrdata.newdatacount; i++) {
			if ((pdrdata.newdata[i].checked & 0b00100) == 0b00000) {
				jenerate(pdrdata.newdata[i].fromip, pdrdata.newdata[i].version, pdrdata.newdata[i].atip);
				jenerate(pdrdata.newdata[i].atip, pdrdata.newdata[i].version, pdrdata.newdata[i].fromip);

				for (int j = 0; j < marker.getUsecount(); j++) {
					if (!IpMatchCheck(pdrdata.newdata[i].fromip, marker.data[j].Get_IP()) || !IpMatchCheck(pdrdata.newdata[i].atip, marker.data[j].Get_IP())) {
						marker.data[j].Set_Packsize(pdrdata.newdata[i].arrivaltime, pdrdata.newdata[i].pacsize);
					}
				}
			}
			pdrdata.newdata[i].checked |= 0b00100;
			checkedcount++;
		}
		pdatacheckf |= 0b00100;
	}


	for (int i = 0; i < usecount; i++) {
		hostidbuff = data[i].Get_Hostid();
		if (hostidbuff == -1) hostidbuff = 0;
		connectcountbuff = data[hostidbuff].Get_Connectcount();
		if (data[i].Get_Deps() == 0)connectcountbuff = i;

		data[i].Move(data[hostidbuff].Get_Pos(), data[hostidbuff].Get_Angl(), connectcountbuff, i == 0 ? 1 : 0);
		switch (skinset.markercol) {
		case 0:
			data[i].Set_Col((skinset.A[0][0] * 0x01000000) +
				(skinset.R[0][0] * 0x00010000) +
				(skinset.G[0][0] * 0x00000100) +
				(skinset.B[0][0] * 0x00000001));
			break;
		case 1:
			data[i].Set_Col((skinset.A[0][data[i].Get_Phase()] * 0x01000000) +
				(skinset.R[0][data[i].Get_Phase()] * 0x00010000) +
				(skinset.G[0][data[i].Get_Phase()] * 0x00000100) +
				(skinset.B[0][data[i].Get_Phase()] * 0x00000001));
			break;
		default:
			data[i].Set_Col((skinset.A[0][0] * 0x01000000) +
				(skinset.R[0][0] * 0x00010000) +
				(skinset.G[0][0] * 0x00000100) +
				(skinset.B[0][0] * 0x00000001));
			break;
		}

		if (skinset.markerview == 1) {
			markervset(data[i].Get_ViewPos(), DEF_MARKER_GSIZE, DEF_MARKER_GSIZE, data[i].Get_Size()* data[i].Get_PSize(), 0, tu, tv, data[i].Get_Col());
		}
		else {
			markervset(data[i].Get_ViewPos(), DEF_MARKER_GSIZE, DEF_MARKER_GSIZE, data[i].Get_Size(), 0, tu, tv, data[i].Get_Col());
		}

		//ポップアップ
		if (dms.pos.x >= (data[i].Get_ViewPos().x * viewx) + frontx - (((DEF_MARKER_GSIZE / 2) - 0) * (data[i].Get_Size() * data[i].Get_PSize() * viewx)) && dms.pos.x <= (data[i].Get_ViewPos().x * viewx) + frontx + (((DEF_MARKER_GSIZE / 2) - 0) * (data[i].Get_Size() * data[i].Get_PSize() * viewx))
			&& dms.pos.y >= (data[i].Get_ViewPos().y * viewx) + fronty - (((DEF_MARKER_GSIZE / 2) - 0) * (data[i].Get_Size() * data[i].Get_PSize() * viewx)) && dms.pos.y <= (data[i].Get_ViewPos().y * viewx) + fronty + (((DEF_MARKER_GSIZE / 2) - 0) * (data[i].Get_Size() * data[i].Get_PSize() * viewx))) {
			viewpopupid = i;
		}

	}

	if (viewpopupid > -1) {
		hostidbuff = data[viewpopupid].Get_Hostid();

		connectcountbuff = data[viewpopupid].Get_ConnectidCount();

		if (hostidbuff > -1) {
			connectcountbuff++;
		}

		if (dms.pos.x + 205 >= charmleftpos) {
			posbuff.x = charmleftpos - 205;
		}
		else if (dms.pos.x <= 20) {
			posbuff.x = 20;
		}
		else {
			posbuff.x = dms.pos.x;
		}
		if (dms.pos.y - 110 - (connectcountbuff * 20) <= 0) {
			posbuff.y = 110 + (connectcountbuff * 20);

		}
		else if (dms.pos.y >= rc.bottom - 10) {
			posbuff.y = rc.bottom - 10;
		}
		else {
			posbuff.y = dms.pos.y;
		}
		posbuff.z = 0;
		area.data[popupareaid].SetPos(posbuff, 10, 10, 200, 100 + (connectcountbuff * 20), 0);


		if (data[viewpopupid].Get_Version() == 4) {
			gstr.Stringset(posbuff.x, posbuff.y - 80 - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("IP: %d.%d.%d.%d"), data[viewpopupid].Get_IP()[0], data[viewpopupid].Get_IP()[1], data[viewpopupid].Get_IP()[2], data[viewpopupid].Get_IP()[3]);
		}
		else {
			gstr.Stringset(posbuff.x, posbuff.y - 80 - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("IP: %x:%x:%x:%x:%x:%x:%x:%x"), data[viewpopupid].Get_IP()[0], data[viewpopupid].Get_IP()[1], data[viewpopupid].Get_IP()[2], data[viewpopupid].Get_IP()[3], data[viewpopupid].Get_IP()[4], data[viewpopupid].Get_IP()[5], data[viewpopupid].Get_IP()[6], data[viewpopupid].Get_IP()[7]);
		}

		gstr.Stringset(posbuff.x, posbuff.y - 60 - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("sumpacketsize: %d"), data[viewpopupid].Get_SumSize());
		//gstr.Stringset(posbuff.x, posbuff.y - 40 - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("deps: %d"), data[viewpopupid].Get_SumSize());
		gstr.Stringset(posbuff.x, posbuff.y - 20 - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("connectIP: %d"), connectcountbuff);

		if (hostidbuff > -1) {
			if (data[hostidbuff].Get_Version() == 4) {
				gstr.Stringset(posbuff.x, posbuff.y - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("- %d.%d.%d.%d"), data[hostidbuff].Get_IP()[0], data[hostidbuff].Get_IP()[1], data[hostidbuff].Get_IP()[2], data[hostidbuff].Get_IP()[3]);
			}
			else {
				gstr.Stringset(posbuff.x, posbuff.y - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("- %x:%x:%x:%x:%x:%x:%x:%x"), data[hostidbuff].Get_IP()[0], data[hostidbuff].Get_IP()[1], data[hostidbuff].Get_IP()[2], data[hostidbuff].Get_IP()[3], data[hostidbuff].Get_IP()[4], data[hostidbuff].Get_IP()[5], data[hostidbuff].Get_IP()[6], data[hostidbuff].Get_IP()[7]);
			}
		}

		for (int i = 0; i < data[viewpopupid].Get_ConnectidCount(); i++) {
			if (data[data[viewpopupid].Get_Connectid(i)].Get_Version() == 4) {
				if (hostidbuff == -1) {
					gstr.Stringset(posbuff.x, posbuff.y + (20 * i) - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("  %d.%d.%d.%d"), data[data[viewpopupid].Get_Connectid(i)].Get_IP()[0], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[1], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[2], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[3]);
				}
				else {
					gstr.Stringset(posbuff.x, posbuff.y + (20 * (i + 1)) - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("  %d.%d.%d.%d"), data[data[viewpopupid].Get_Connectid(i)].Get_IP()[0], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[1], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[2], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[3]);
				}
			}
			else {
				if (hostidbuff == -1) {
					gstr.Stringset(posbuff.x, posbuff.y + (20 * i) - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("  %x:%x:%x:%x:%x:%x:%x:%x"), data[data[viewpopupid].Get_Connectid(i)].Get_IP()[0], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[1], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[2], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[3], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[4], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[5], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[6], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[7]);
				}
				else {
					gstr.Stringset(posbuff.x, posbuff.y + (20 * (i + 1)) - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("  %x:%x:%x:%x:%x:%x:%x:%x"), data[data[viewpopupid].Get_Connectid(i)].Get_IP()[0], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[1], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[2], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[3], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[4], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[5], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[6], data[data[viewpopupid].Get_Connectid(i)].Get_IP()[7]);
				}
			}
		}

#if _DEBUG

		area.data[popupareaid].SetPos(posbuff, 10, 210, 200, 100 + (connectcountbuff * 20), 0);

		gstr.Stringset(posbuff.x - 200, posbuff.y - 80 - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("ID: %d"), viewpopupid);
		gstr.Stringset(posbuff.x - 200, posbuff.y - 60 - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("angl: %lf"), data[viewpopupid].Get_Angl());
		gstr.Stringset(posbuff.x - 200, posbuff.y - 40 - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("sortid: %d"), data[viewpopupid].Get_Sortid());
		gstr.Stringset(posbuff.x - 200, posbuff.y - 20 - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("deps: %d"), data[viewpopupid].Get_Deps());
		for (int i = 0; i < connectcountbuff - 1; i++) {
			gstr.Stringset(posbuff.x - 200, posbuff.y + (20 * i) - (connectcountbuff * 20), 20, 20, 0, 0, 10, 0xFF000000 + skinset.SRGB, 0, _T("%d"), data[viewpopupid].Get_Connectid(i));
		}

#endif

	}
}


void LineMasterclass::reset() {
	data = (Lineclass *)malloc(sizeof(Lineclass) * 1);
	usecount = 0;
	removebuff = (int *)malloc(sizeof(int) * 1);
	removebuff[0] = -1;
	idcounter = 0;
	checkedcount = 0;

	tu[0] = 0.0f;
	tu[1] = 1.0f;
	tv[0] = 0.0f;
	tv[1] = 0.1f;

	if (nefileselect >= 0) {
		for (int i = 0; i < necfgdata.Count(); i++) {
			if (necfgdata.Data(i).connectip[0] != -1) {
				jenerate(necfgdata.Data(i).ip, necfgdata.Data(i).connectip);
			}
		}
	}
}

int LineMasterclass::jenerate(int *ip1, int *ip2) {


	bool ipjenflag = 1;
	bool ipcheckflag;
	int returnid;

	for (int i = 0; i < usecount; i++) {
		int *ipbuff1 = data[i].Get_IP(0);
		int *ipbuff2 = data[i].Get_IP(1);
		ipjenflag = (IpMatchCheck(ipbuff1, ip1) | IpMatchCheck(ipbuff2, ip2)) & (IpMatchCheck(ipbuff2, ip1) | IpMatchCheck(ipbuff1, ip2));
		if (ipjenflag == 0) {
			returnid = i;
			break;
		}
	}

	if (ipjenflag == 1) {
		data = (Lineclass *)realloc(data, sizeof(Lineclass) * (usecount + 1));
		removebuff = (int *)realloc(removebuff, sizeof(int) * (usecount + 1));
		removebuff[usecount] = -1;

		Pos posbuff1;
		Pos posbuff2;

		posbuff1.x = 0;
		posbuff1.y = 0;
		posbuff1.z = 0;

		posbuff2.x = 0;
		posbuff2.y = 0;
		posbuff2.z = 0;

		data[usecount].JenMove(posbuff1, posbuff2, idcounter, ip1, ip2, usecount);
		returnid = usecount;
		idcounter++;
		usecount++;
	}
	return returnid;
}

void LineMasterclass::sort() {

}

void LineMasterclass::remove() {
	int removecount = 0;
}

void LineMasterclass::datafree() {
	for (int i = 0; i < usecount; i++) {
		data[i].Free_Packsize();
	}

	if (data != NULL) {
		free(data);
	}
	data = NULL;
	usecount = 0;
	if (removebuff != NULL) {
		free(removebuff);
	}
	removebuff = NULL;
}

void LineMasterclass::move(MarkerMasterclass marker) {
	if ((pdatacheckf & 0b01000) == 0b00000) {
		if (marker.getCheckedcount() > checkedcount) {

			for (int i = 0; i < pdrdata.newdatacount; i++) {
				if ((pdrdata.newdata[i].checked & 0b01000) == 0b00000 && (pdrdata.newdata[i].checked & 0b00100) == 0b00100) {

					if (IpMatchCheck(pdrdata.newdata[i].fromip, pdrdata.newdata[i].atip)) {
						int checkibuff = 0;
						int sizeibuff;
							checkibuff = route.check(marker, pdrdata.newdata[i].fromip, pdrdata.newdata[i].version, pdrdata.newdata[i].atip, pdrdata.newdata[i].version);

						for (int k = 0; k < route.Data(checkibuff).Get_Datacount() - 1; k++) {
							sizeibuff = jenerate(marker.Data(route.Data(checkibuff).Data()[k]).Get_IP(), marker.Data(route.Data(checkibuff).Data()[k + 1]).Get_IP());
							line.data[sizeibuff].Set_Packsize(pdrdata.newdata[i].arrivaltime, pdrdata.newdata[i].pacsize);
						}
					}
				}
				pdrdata.newdata[i].checked |= 0b01000;
				checkedcount++;
			}
		}
		pdatacheckf |= 0b01000;
	}
	Pos posbuff1;
	Pos posbuff2;
	int checkf;
	for (int i = 0; i < usecount; i++) {
		checkf = 0;
		for (int j = 0; j < marker.getUsecount(); j++) {
			if (!IpMatchCheck(data[i].Get_IP(0), marker.Data(j).Get_IP())) {
				posbuff1 = marker.Data(j).Get_ViewPos();
				checkf++;
			}
			if (!IpMatchCheck(data[i].Get_IP(1), marker.Data(j).Get_IP())) {
				posbuff2 = marker.Data(j).Get_ViewPos();
				checkf++;
			}
			if (checkf >= 2) {
				break;
			}
		}

			data[i].Move(posbuff1, posbuff2);

			switch (skinset.linecol) {
			case 0:
				data[i].Set_Col((skinset.A[1][0] * 0x01000000) +
					(skinset.R[1][0] * 0x00010000) +
					(skinset.G[1][0] * 0x00000100) +
					(skinset.B[1][0] * 0x00000001));
				break;
			case 1:
				data[i].Set_Col((skinset.A[1][data[i].Get_Phase()] * 0x01000000) +
					(skinset.R[1][data[i].Get_Phase()] * 0x00010000) +
					(skinset.G[1][data[i].Get_Phase()] * 0x00000100) +
					(skinset.B[1][data[i].Get_Phase()] * 0x00000001));
				break;
			default:
				data[i].Set_Col((skinset.A[1][0] * 0x01000000) +
					(skinset.R[1][0] * 0x00010000) +
					(skinset.G[1][0] * 0x00000100) +
					(skinset.B[1][0] * 0x00000001));
				break;
			}
			if (skinset.lineview == 1) {
				linevset(data[i].Get_Pos(0), data[i].Get_Pos(1), DEF_LINE_GSIZE, data[i].Get_Size() * data[i].Get_PSize(), data[i].Get_Angl(), tu, tv, data[i].Get_Col());
			}
			else {
				linevset(data[i].Get_Pos(0), data[i].Get_Pos(1), DEF_LINE_GSIZE, data[i].Get_Size(), data[i].Get_Angl(), tu, tv, data[i].Get_Col());
			}
	}

}


void PacketMasterclass::reset() {
	data = (Packetclass *)malloc(sizeof(Packetclass) * 1);
	usecount = 0;
	removebuff = (int *)malloc(sizeof(int) * 1);
	removebuff[0] = -1;
	idcounter = 0;
	checkedcount = 0;

	checkaddr = 0;

	tu[0] = 0.3f;
	tu[1] = 0.6f;
	tv[0] = 0.101f;
	tv[1] = 0.4f;

	if (nefileselect != -1) {
		for (int i = 0; i < necfgdata.Count(); i++) {
			for (int j = 0; j < necfgdata.Count(); j++) {
				if (i != j) {
					route.check(marker, necfgdata.Data(i).ip, necfgdata.Data(i).version, necfgdata.Data(j).ip, necfgdata.Data(j).version);
				}
			}
		}
	}
	
}

void PacketMasterclass::jenerate(int *ip1, int *ip2, int pacsize, unsigned long long *janeratetime, unsigned long long arrivaltime0, unsigned long long arrivaltime1, int tableid, bool reverse) {


	data = (Packetclass *)realloc(data, sizeof(Packetclass) * (usecount + 1));
	removebuff = (int *)realloc(removebuff, sizeof(int) * (usecount + 1));
	removebuff[usecount] = -1;

	Pos posbuff1;
	Pos posbuff2;

	posbuff1.x = 0;
	posbuff1.y = 0;
	posbuff1.z = 0;

	posbuff2.x = 0;
	posbuff2.y = 0;
	posbuff2.z = 0;

	data[usecount].JenMove(posbuff1, posbuff2, ip1, ip2, usecount, pacsize, janeratetime, arrivaltime0, arrivaltime1, tableid, reverse);
	idcounter++;
	usecount++;

}

void PacketMasterclass::sort() {

}

void PacketMasterclass::remove() {
	int removecount = 0;
	int useremovecount = 0;
	int usecountbuff = 0;
	for (int i = 0; i < usecount; i++) {
		if (data[i].Get_Anmflag() == -1) {
			removebuff[removecount] = i;
			removecount++;
		}
		else {
			usecountbuff++;
			if (useremovecount < removecount) {
				data[removebuff[useremovecount]].Set_All(data[i].Get_Pos(0), data[i].Get_Pos(1), data[i].Get_IP(0), data[i].Get_IP(1),
					data[i].Get_Size(), data[i].Get_Anmflag(), data[i].Get_Moveflame(), data[i].Get_Pacsize(), data[i].Get_OPacsize(), data[i].Get_Janeratetime(), data[i].Get_Arrivaltime(), data[i].Get_TableID(), data[i].Get_ReverseFlag());
				useremovecount++;
				removebuff[removecount] = i;
				removecount++;
			}
		}
	}
	usecount = usecountbuff;
}

void PacketMasterclass::datafree() {
	if (data != NULL) {
		free(data);
	}
	data = NULL;
	usecount = 0;
	if (removebuff != NULL) {
		free(removebuff);
	}
	removebuff = NULL;
}

unsigned long long debugsumsize = 0;

void PacketMasterclass::move(MarkerMasterclass marker) {
	char debptextc[8192];

	int routingcheck = 0;
	int reverseflag = 0;

	int *checkedid;
	bool *checkedidrev;
	int checkedidcount = 0;
	bool checkedidflag = 0;

	checkedid = (int *)malloc(sizeof(int) * 1);
	checkedidrev = (bool *)malloc(sizeof(bool) * 1);


	if ((pdatacheckf & 0b10000) == 0b00000) {
		if (marker.getCheckedcount() > checkedcount) {

			for (int i = 0; i < pdrdata.newdatacount; i++) {
				if ((pdrdata.newdata[i].checked & 0b10000) == 0b00000 && (pdrdata.newdata[i].checked & 0b00100) == 0b00100) {
					if (IpMatchCheck(pdrdata.newdata[i].fromip, pdrdata.newdata[i].atip)) {
						checkedidflag = 0;

						routingcheck = route.check(marker, pdrdata.newdata[i].fromip, pdrdata.newdata[i].version, pdrdata.newdata[i].atip, pdrdata.newdata[i].version);
						reverseflag = 0;
						
						if (!IpMatchCheck(route.Data(routingcheck).Get_IP(1), pdrdata.newdata[i].fromip)) {
							reverseflag++;
						}
						
						if (!IpMatchCheck(route.Data(routingcheck).Get_IP(0), pdrdata.newdata[i].atip)) {
							reverseflag++;
						}

						if (reverseflag == 2) {
							reverseflag = 1;
						}
						else {
							reverseflag = 0;
						}

						for (int j = 0; j < checkedidcount; j++) {
							if (checkedid[j] == routingcheck && checkedidrev[j] == reverseflag) {
								checkedidflag = 1;
								break;
							}
						}

						if (checkedidflag == 0) {
							jenerate(pdrdata.newdata[i].fromip, pdrdata.newdata[i].atip, pdrdata.newdata[i].pacsize, visobjtime, pdrdata.newdata[i].arrivaltime[0] + 5, pdrdata.newdata[i].arrivaltime[1], routingcheck, reverseflag);
							debugsumsize += pdrdata.newdata[i].pacsize;
							checkaddr = i;

							checkedid = (int *)realloc(checkedid, sizeof(int) * (checkedidcount + 1));
							checkedidrev = (bool *)realloc(checkedidrev, sizeof(bool) * (checkedidcount + 1));

							checkedid[checkedidcount] = routingcheck;
							checkedidrev[checkedidcount] = reverseflag;

							checkedidcount++;
						}
					}
				}
				pdrdata.newdata[i].checked |= 0b10000;
				checkedcount++;
			}
		}
		pdatacheckf |= 0b10000;
	}

	if(checkedid != NULL) free(checkedid);
	if(checkedidrev != NULL) free(checkedidrev);

	Pos posbuff1;
	Pos posbuff2;

	posbuff1.x = 0;
	posbuff1.y = 0;
	posbuff1.z = 0;

	posbuff2.x = 0;
	posbuff2.y = 0;
	posbuff2.z = 0;

	if (skinset.packetview == 1) {
		for (int i = 0; i < usecount; i++) {

			double moveonesectiontime;
			double movedperbuf;
			long long int movedtimebuf, movetimebuf;

			moveonesectiontime = PACK_MOVE_TIME / (route.Data(data[i].Get_TableID()).Get_Datacount() - 1);

			movedtimebuf = ((data[i].Get_MovedTime(visobjtime[0], 0) * 1000) + data[i].Get_MovedTime(visobjtime[1], 1));

			movetimebuf = ((data[i].Get_MoveTime(0) * 1000) + data[i].Get_MoveTime(1));

			movedperbuf = (movedtimebuf - (moveonesectiontime * (int)(movedtimebuf / moveonesectiontime))) / moveonesectiontime;

			int sectionbuff[2];
			int sectiondataidbuff[2];
			

			if (movedtimebuf < PACK_MOVE_TIME) {
				if (data[i].Get_ReverseFlag() == 0) {
					sectionbuff[0] = (int)(movedtimebuf / moveonesectiontime);
					sectionbuff[1] = (int)(movedtimebuf / moveonesectiontime) + 1;
				}
				else {
					sectionbuff[0] = route.Data(data[i].Get_TableID()).Get_Datacount() - (int)(movedtimebuf / moveonesectiontime) - 1;
					sectionbuff[1] = route.Data(data[i].Get_TableID()).Get_Datacount() - (int)(movedtimebuf / moveonesectiontime) - 2;
				}
			}
			else {
				if (data[i].Get_ReverseFlag() == 0) {
					sectionbuff[0] = route.Data(data[i].Get_TableID()).Get_Datacount() - 2;
					sectionbuff[1] = route.Data(data[i].Get_TableID()).Get_Datacount() - 1;
				}
				else {
					sectionbuff[0] = 1;
					sectionbuff[1] = 0;
				}
			}
			
			sectiondataidbuff[0] = route.Data(data[i].Get_TableID()).Data()[sectionbuff[0]];
			sectiondataidbuff[1] = route.Data(data[i].Get_TableID()).Data()[sectionbuff[1]];

			

			posbuff1 = marker.Data(sectiondataidbuff[0]).Get_ViewPos();
			posbuff2 = marker.Data(sectiondataidbuff[1]).Get_ViewPos();

			data[i].Move(posbuff2, posbuff1);

			Pos moveposbuff, goalposbuff;

			double distancebff;
			double xbuff, ybuff;

			moveposbuff = data[i].Get_Pos(1);
			goalposbuff = data[i].Get_Pos(0);

			xbuff = goalposbuff.x - moveposbuff.x;
			ybuff = goalposbuff.y - moveposbuff.y;
			distancebff = (xbuff * xbuff) + (ybuff * ybuff);
			distancebff = sqrt(distancebff);

			if (movedtimebuf >= PACK_MOVE_TIME) {
				movedperbuf = 1.0;
			}

			moveposbuff.x = moveposbuff.x + ((distancebff * movedperbuf) * cos(data[i].Get_Angl() * PI / 180.0));
			moveposbuff.y = moveposbuff.y + ((distancebff * movedperbuf) * sin(data[i].Get_Angl() * PI / 180.0));
			moveposbuff.z = 0;

			if (skinset.packetsize == 1) {
				packetvset(moveposbuff, DEF_PACK_GSIZE, DEF_PACK_GSIZE, data[i].Get_Size() * data[i].Get_Pacsize(), data[i].Get_Angl(), tu, tv, 0xffffffff);
			}
			else {
				packetvset(moveposbuff, DEF_PACK_GSIZE, DEF_PACK_GSIZE, 1.0, data[i].Get_Angl(), tu, tv, 0xffffffff);
			}

			if (data[i].Get_Anmflag() != -1 && data[i].Get_Anmflag() != 2) {
				double uvbufx, uvbufy, uvbuflen;

				uvbufx = (moveposbuff.x - goalposbuff.x) * (moveposbuff.x - goalposbuff.x);
				uvbufy = (moveposbuff.y - goalposbuff.y) * (moveposbuff.y - goalposbuff.y);
				uvbuflen = uvbufx + uvbufy;

				if (movedtimebuf >= PACK_MOVE_TIME) {
					data[i].Set_Anmflag(2);
				}
			}

			if (skinset.sizeview[2] == 1) {
				gstrinner.Stringset((moveposbuff.x * viewx) + frontx, (moveposbuff.y * viewx) + fronty, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%d"), data[i].Get_OPacsize());
			}

#if _DEBUG
			if (moveonesectiontime < 5000 && data[i].Get_ReverseFlag() == 0) {
				gstrinner.Stringset((moveposbuff.x * viewx) + frontx, (moveposbuff.y * viewx) + fronty, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%lf"), movedperbuf);

				gstrinner.Stringset((moveposbuff.x * viewx) + frontx, (moveposbuff.y * viewx) + fronty + 20, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%3d :sc %3d(%3d-%3d)"), data[i].Get_TableID(), sectionbuff[0], sectiondataidbuff[0], sectiondataidbuff[1]);
				gstrinner.Stringset((moveposbuff.x * viewx) + frontx, (moveposbuff.y * viewx) + fronty + 40, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%lf"), moveonesectiontime);
				gstrinner.Stringset((moveposbuff.x * viewx) + frontx, (moveposbuff.y * viewx) + fronty + 60, DEF_IPFONTSIZE_X * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), DEF_IPFONTSIZE_Y * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0, 0, DEF_IPFONTSIZE_MARG * ((viewx >= DEF_IPFONTSIZE_UNDER) ? viewx : DEF_IPFONTSIZE_UNDER), 0xFF000000 + skinset.SRGB, 1, _T("%lld"), movedtimebuf);
			}
#endif
		}
	}
	this->remove();

#if _DEBUG

	gstr.Stringset(20, 480, 24, 24, 0, 0, 13, 0xFF000000 + skinset.SRGB, 0, _T("totalsize   : %15llubyte"), debugsumsize);
	gstr.Stringset(20, 510, 24, 24, 0, 0, 13, 0xFF000000 + skinset.SRGB, 0, _T("checkedpack : %15lu"), checkedcount);
	gstr.Stringset(20, 540, 24, 24, 0, 0, 13, 0xFF000000 + skinset.SRGB, 0, _T("grpack      : %15lu"), usecount);
	gstr.Stringset(20, 570, 24, 24, 0, 0, 13, 0xFF000000 + skinset.SRGB, 0, _T("checkpack   : %15lu"), checkaddr);

#endif

}


void RoutingMasterclass::reset() {
	data = (Routingclass *)malloc(sizeof(Routingclass) * 1);
	usecount = 0;

	checkedid = (int *)malloc(sizeof(int) * 1);
	checkedidcount = 0;
}

void RoutingMasterclass::jenerate(int *ip1, int version1, int *ip2, int version2) {
	data = (Routingclass *)realloc(data, sizeof(Routingclass) * (usecount + 1));

	data[usecount].Set(ip1, version1, ip2, version2);

	usecount++;

}

int RoutingMasterclass::check(MarkerMasterclass marker, int *ip1, int version1, int *ip2, int version2) {
	int checkreturn = -1;
	int checkmarkerid = -1;

	for (int i = 0; i < usecount; i++) {
		int checkflag = 0;
		if (!IpMatchCheck(data[i].Get_IP(0), ip1)) {
			checkflag++;
		}
		else if(!IpMatchCheck(data[i].Get_IP(1), ip1)) {
			checkflag++;
		}

		if (!IpMatchCheck(data[i].Get_IP(0), ip2)) {
			checkflag++;
		}
		else if (!IpMatchCheck(data[i].Get_IP(1), ip2)) {
			checkflag++;
		}

		if (checkflag == 2) {
			checkreturn = i;
			break;
		}
	}

	if (checkreturn == -1) {
#if _DEBUG
		char debptextc[8192];
		if (ROUTING_LOG == 1) {
			sprintf_s(debptextc, "ip1 %d.%d.%d.%d \n", ip1[0], ip1[1], ip1[2], ip1[3]);
			OutputDebugStringA(debptextc);
			sprintf_s(debptextc, "ip2 %d.%d.%d.%d \n", ip2[0], ip2[1], ip2[2], ip2[3]);
			OutputDebugStringA(debptextc);
		}
#endif
		jenerate(ip2, version2, ip1, version1);
		for (int i = 0; i < marker.getUsecount(); i++) {
			if (!IpMatchCheck(marker.Data(i).Get_IP(), ip1)) {
				checkmarkerid = i;
				break;
			}
		}

#if _DEBUG
		if (ROUTING_LOG == 1) {
			sprintf_s(debptextc, "\nmake %d, %d \n", checkmarkerid, usecount - 1);
			OutputDebugStringA(debptextc);
		}
#endif
		checkedidcount = 0;
		checkedid[0] = -1;

		mkidcheck(marker, ip2, version2, checkmarkerid, usecount - 1);
		checkreturn = usecount - 1;

#if _DEBUG
		if (ROUTING_LOG == 1) {
			sprintf_s(debptextc, "\nreturn routeid %d\n----------------------------------\n", checkreturn);
			OutputDebugStringA(debptextc);
		}
#endif

	}


	
	return checkreturn;
}

bool RoutingMasterclass::mkidcheck(MarkerMasterclass marker, int *ip, int version, int id, int dataid) {

	bool checkflag = false;
	bool nocheck = false;

	for (int i = 0; i < checkedidcount; i++) {
		if (checkedid[i] == id) {
			nocheck = true;
		}
	}

	if (!nocheck) {

		checkedid = (int *)realloc(checkedid, sizeof(int) * (checkedidcount + 1));
		checkedid[checkedidcount] = id;
		checkedidcount++;

#if _DEBUG
		char debptextc[8192];
#endif

		if (!IpMatchCheck(marker.Data(id).Get_IP(), ip)) {

#if _DEBUG
			if (ROUTING_LOG == 1) {
				sprintf_s(debptextc, "-%d %d.%d.%d.%d \n", id, marker.Data(id).Get_IP()[0], marker.Data(id).Get_IP()[1], marker.Data(id).Get_IP()[2], marker.Data(id).Get_IP()[3]);
				OutputDebugStringA(debptextc);
			}
#endif
			data[dataid].Add(id);
			return true;
		}

		if (marker.Data(id).Get_Hostid() > -1) {
			if (mkidcheck(marker, ip, version, marker.Data(id).Get_Hostid(), dataid)) {
#if _DEBUG
				if (ROUTING_LOG == 1) {
					sprintf_s(debptextc, "-%d %d.%d.%d.%d \n", id, marker.Data(id).Get_IP()[0], marker.Data(id).Get_IP()[1], marker.Data(id).Get_IP()[2], marker.Data(id).Get_IP()[3]);
					OutputDebugStringA(debptextc);
				}
#endif
				data[dataid].Add(id);
				checkflag = true;
			}
		}

		if (checkflag == false) {
			for (int i = 0; i < marker.Data(id).Get_ConnectidCount(); i++) {

				if (mkidcheck(marker, ip, version, marker.Data(id).Get_Connectid(i), dataid)) {
#if _DEBUG
					if (ROUTING_LOG == 1) {
						sprintf_s(debptextc, "-%d %d.%d.%d.%d \n", id, marker.Data(id).Get_IP()[0], marker.Data(id).Get_IP()[1], marker.Data(id).Get_IP()[2], marker.Data(id).Get_IP()[3]);
						OutputDebugStringA(debptextc);
					}
#endif
					data[dataid].Add(id);
					checkflag = true;
					break;
				}
			}
		}
	}

	return checkflag;
}

void DataStart() {
	route.reset();
	marker.reset();
	packet.reset();
	line.reset();
	
}

void DataTrash() {

	marker.datafree();
	packet.datafree();
	line.datafree();

}

void VisobjtimeCheck() {

	visobjtimes[0] = analysis.time.GetNowTime(0) >= PACK_MOVE_TIME_SEC ? analysis.time.GetNowTime(0) - PACK_MOVE_TIME_SEC : 24 + analysis.time.GetNowTime(0) - PACK_MOVE_TIME_SEC;
	visobjtimes[1] = analysis.time.GetNowTime(1);

	gstrinner.Stringset(10, rc.bottom - 58, 24, 24, 0, 0, 12, 0xFF000000 + skinset.SRGB, 0, _T("Capture Time %4llu:%02llu:%02llu:%03llu"), (analysis.time.GetCaptureTime(0) / 3600), analysis.time.GetCaptureTime(0) % 3600 / 60, analysis.time.GetCaptureTime(0) % 60, analysis.time.GetCaptureTime(1));
	gstrinner.Stringset(10, rc.bottom - 34, 24, 24, 0, 0, 12, 0xFF000000 + skinset.SRGB, 0, _T("View Time    %4d:%02d:%02d:%03d"), (visobjtimes[0] / 3600) % 24, visobjtimes[0] % 3600 / 60, visobjtimes[0] % 60, visobjtimes[1]);
	
	visobjtime[0] = analysis.time.GetMoveTime(0);
	visobjtime[1] = analysis.time.GetMoveTime(1);
	if (visobjtime[0] > PACK_MOVE_TIME_SEC) {
		gstrinner.Stringset(10, rc.bottom - 10, 24, 24, 0, 0, 12, 0xFF000000 + skinset.SRGB, 0, _T("             %4llu:%02llu:%02llu:%03llu"), ((visobjtime[0] - PACK_MOVE_TIME_SEC) / 3600), (visobjtime[0] - PACK_MOVE_TIME_SEC) % 3600 / 60, (visobjtime[0] - PACK_MOVE_TIME_SEC) % 60, visobjtime[1]);
	}
	else {
		gstrinner.Stringset(10, rc.bottom - 10, 24, 24, 0, 0, 12, 0xFF000000 + skinset.SRGB, 0, _T("               -%1d:%02d:%02llu:%03llu"), 0, 0, PACK_MOVE_TIME_SEC - visobjtime[0], visobjtime[1]);
	}

}