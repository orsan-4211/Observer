#include <thread>
#include <iostream>
//#include <pcap.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <time.h>
#include <tchar.h>

#include "../main.h"

#include "../Include/pcap.h"

#include "packet.h"
#include "analysis.h"
#include "../sidecharm/charmmain.h"



Analysisclass analysis;
Setting setting;

void Mode0strdr();
void Mode2strdr();
void Mode3strdr();
void SizeGraph(double x, double y, double sizex, double sizey, int startsep, int endsep);
void setPGraphTex(int id, double *tubuff, double *tvbuff);



void SetAnalysis() {

	analysis.Reset();
	setting.viewdatalange = 0;
	setting.endtimechase = 1;

	setting.stime = 0;

	setting.etime = 0;

	setting.stopf = 0;
}

void ReSetAnalysis() {
	analysis.Reset();
	setting.viewdatalange = 0;
	setting.endtimechase = 1;

	setting.stime = 0;

	setting.etime = 0;
}

TCHAR* Protocol(int protocol) {
	switch (protocol) {
	case 0x00: return _T("HOPOPT");
	case 0x01: return _T("ICMP");
	case 0x02: return _T("IGMP");
	case 0x03: return _T("GGP");
	case 0x04: return _T("IP - in - IP");
	case 0x05: return _T("ST");
	case 0x06: return _T("TCP");
	case 0x07: return _T("CBT");
	case 0x08: return _T("EGP");
	case 0x09: return _T("IGP");
	case 0x0A: return _T("BBN - RCC - MON");
	case 0x0B: return _T("NVP - II");
	case 0x0C: return _T("PUP");
	case 0x0D: return _T("ARGUS");
	case 0x0E: return _T("EMCON");
	case 0x0F: return _T("XNET");
	case 0x10: return _T("CHAOS");
	case 0x11: return _T("UDP");
	case 0x12: return _T("MUX");
	case 0x13: return _T("DCN - MEAS");
	case 0x14: return _T("HMP");
	case 0x15: return _T("PRM");
	case 0x16: return _T("XNS - IDP");
	case 0x17: return _T("TRUNK - 1");
	case 0x18: return _T("TRUNK - 2");
	case 0x19: return _T("LEAF - 1");
	case 0x1A: return _T("LEAF - 2");
	case 0x1B: return _T("RDP");
	case 0x1C: return _T("IRTP");
	case 0x1D: return _T("ISO - TP4");
	case 0x1E: return _T("NETBLT");
	case 0x1F: return _T("MFE - NSP");
	case 0x20: return _T("MERIT - INP");
	case 0x21: return _T("DCCP");
	case 0x22: return _T("3PC");
	case 0x23: return _T("IDPR");
	case 0x24: return _T("XTP");
	case 0x25: return _T("DDP");
	case 0x26: return _T("IDPR - CMTP");
	case 0x27: return _T("TP++");
	case 0x28: return _T("IL");
	case 0x29: return _T("IPv6");
	case 0x2A: return _T("SDRP");
	case 0x2B: return _T("IPv6 - Route");
	case 0x2C: return _T("IPv6 - Frag");
	case 0x2D: return _T("IDRP");
	case 0x2E: return _T("RSVP");
	case 0x2F: return _T("GREs");
	case 0x30: return _T("DSR");
	case 0x31: return _T("BNA");
	case 0x32: return _T("ESP");
	case 0x33: return _T("AH");
	case 0x34: return _T("I - NLSP");
	case 0x35: return _T("SWIPE");
	case 0x36: return _T("NARP");
	case 0x37: return _T("MOBILE");
	case 0x38: return _T("TLSP");
	case 0x39: return _T("SKIP");
	case 0x3A: return _T("IPv6 - ICMP");
	case 0x3B: return _T("IPv6 - NoNxt");
	case 0x3C: return _T("IPv6 - Opts");
	case 0x3E: return _T("CFTP");
	case 0x40: return _T("SAT - EXPAK");
	case 0x41: return _T("KRYPTOLAN");
	case 0x42: return _T("RVD");
	case 0x43: return _T("IPPC");
	case 0x45: return _T("SAT - MON");
	case 0x46: return _T("VISA");
	case 0x47: return _T("IPCU");
	case 0x48: return _T("CPNX");
	case 0x49: return _T("CPHB");
	case 0x4A: return _T("WSN");
	case 0x4B: return _T("PVP");
	case 0x4C: return _T("BR - SAT - MON");
	case 0x4D: return _T("SUN - ND");
	case 0x4E: return _T("WB - MON");
	case 0x4F: return _T("WB - EXPAK");
	case 0x50: return _T("ISO - IP");
	case 0x51: return _T("VMTP");
	case 0x52: return _T("SECURE - VMTP");
	case 0x53: return _T("VINES");
	case 0x54: return _T("TTP or IPTM");
	case 0x55: return _T("NSFNET - IGP");
	case 0x56: return _T("DGP");
	case 0x57: return _T("TCF");
	case 0x58: return _T("EIGRP");
	case 0x59: return _T("OSPF");
	case 0x5A: return _T("Sprite - RPC");
	case 0x5B: return _T("LARP");
	case 0x5C: return _T("MTP");
	case 0x5D: return _T("AX.25");
	case 0x5E: return _T("OS");
	case 0x5F: return _T("MICP");
	case 0x60: return _T("SCC - SP");
	case 0x61: return _T("ETHERIP");
	case 0x62: return _T("ENCAP");
	case 0x64: return _T("GMTP");
	case 0x65: return _T("IFMP");
	case 0x66: return _T("PNNI");
	case 0x67: return _T("PIM");
	case 0x68: return _T("ARIS");
	case 0x69: return _T("SCPS");
	case 0x6A: return _T("QNX");
	case 0x6B: return _T("A / N");
	case 0x6C: return _T("IPComp");
	case 0x6D: return _T("SNP");
	case 0x6E: return _T("Compaq - Peer");
	case 0x6F: return _T("IPX - in - IP");
	case 0x70: return _T("VRRP");
	case 0x71: return _T("PGM");
	case 0x73: return _T("L2TP");
	case 0x74: return _T("DDX");
	case 0x75: return _T("IATP");
	case 0x76: return _T("STP");
	case 0x77: return _T("SRP");
	case 0x78: return _T("UTI");
	case 0x79: return _T("SMP");
	case 0x7A: return _T("SM");
	case 0x7B: return _T("PTP");
	case 0x7C: return _T("IS - IS over IPv4");
	case 0x7D: return _T("FIRE");
	case 0x7E: return _T("CRTP");
	case 0x7F: return _T("CRUDP");
	case 0x80: return _T("SSCOPMCE");
	case 0x81: return _T("IPLT");
	case 0x82: return _T("SPS");
	case 0x83: return _T("PIPE");
	case 0x84: return _T("SCTP");
	case 0x85: return _T("FC");
	case 0x86: return _T("RSVP - E2E - IGNORE");
	case 0x88: return _T("UDPLite");
	case 0x89: return _T("MPLS - in - IP");
	case 0x8A: return _T("manet");
	case 0x8B: return _T("HIP");
	case 0x8C: return _T("Shim6");
	case 0x8D: return _T("WESP");
	case 0x8E: return _T("ROHC");
	default: return _T("etc.");
	}
}

void Analysismain(int mode) {
	analysis.time.UpdateTime(setting.stopf);
	if (analysis.time.GetCaptureTime(0) / 10 > analysis.GetSectionTime()) {
		analysis.SetSection();
	}
	if ((pdatacheckf & 0b00001) == 0b00000) {
		for (int i = 0; i < pdrdata.newdatacount; i++) {
			if ((pdrdata.newdata[i].checked & 0b00001) == 0b00000) {
				analysis.SetData(pdrdata.newdata[i], mode, setting);
			}
			pdrdata.newdata[i].checked |= 0b00001;
		}
		pdatacheckf |= 0b00001;
	}

	
	//gstr.Stringset(10, rc.bottom - 58, 18, 24, 0, 0, 5, 0xFFFFFFFF, 0, _T("Capture Time %4d:%02d:%02d:%03d"), (analysis.time.GetCaptureTime(0) / 3600) % 24, analysis.time.GetCaptureTime(0) % 3600 / 60, analysis.time.GetCaptureTime(0) % 60, analysis.time.GetCaptureTime(1));
}

void Analysisdrmain(int mode) {
	switch (mode) {
	case 0:
		Mode0strdr();
		break;
	case 2:
		Mode2strdr();
		break;
	case 3:
		Mode3strdr();
		break;
	default:
		break;
	}
	if (setting.stopf == 1) {
		gstr.Stringset(view.Width / 2, CHARM_BASETXT_SIZE - 8, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 1, _T("< || Capture Pause >"));
	}
}

void updataviewdata(unsigned long long stime, unsigned long long etime) {
		analysis.UpdataViewData(stime, etime);
}

void Mode0strdr() {
	Pos posbuff;
	double anglbuff = 0;
	double tubuff[2], tvbuff[2];

	int sepbuff[2];

	double charmleftposbuf;
	for (int j = 0; j < area.usecount; j++) {
		if (area.data[j].GetId() == 0) {
			charmleftposbuf = area.data[j].GetPos().x - area.data[j].GetPosxy(0);
			break;
		}
	}

	switch (setting.viewdatalange) {
	case 0:
		setting.stime = 0;
		break;
	case 1:
		setting.stime = analysis.time.GetCaptureTime(0) >= 60 ? (analysis.time.GetCaptureTime(0) - 60) : 0;
		break;
	case 2:
		setting.stime = analysis.time.GetCaptureTime(0) >= (60 * 5) ? (analysis.time.GetCaptureTime(0) - (60 * 5)) : 0;
		break;
	case 3:
		setting.stime = analysis.time.GetCaptureTime(0) >= (60 * 10) ? (analysis.time.GetCaptureTime(0) - (60 * 10)) : 0;
		break;
	case 4:
		setting.stime = analysis.time.GetCaptureTime(0) >= (60 * 30) ? (analysis.time.GetCaptureTime(0) - (60 * 30)) : 0;
		break;
	default:
		break;
	}

	setting.etime = analysis.time.GetCaptureTime(0);


	sepbuff[0] = setting.stime / 10;
	sepbuff[1] = (setting.etime / 10 + 1);

	posbuff.z = 0;

	if (setting.viewdatalange != 0 || (setting.viewdatalangeold != 0 && setting.viewdatalange == 0)) {
		updataviewdata(setting.stime, setting.etime);
		//プロトコルグラフ
		for (int i = 0; i < analysis.viewdata.prtcount; i++) {
			gstr.Stringset(charmleftposbuf + 250, 50 + (20 * i), CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%-20s:%5.1lf%%"), Protocol(analysis.viewdata.prtdata[i].GetId()), (analysis.viewdata.prtdata[i].GetCount() / (double)analysis.viewdata.pcount) * 100.0);

			posbuff.x = charmleftposbuf + 125;
			posbuff.y = 128;

			setPGraphTex(i, tubuff, tvbuff);

			protocolgraphvset(posbuff, 100, 360 * (analysis.viewdata.prtdata[i].GetCount() / (double)analysis.viewdata.pcount)
				, anglbuff, tubuff, tvbuff, 0xffffffff);
			anglbuff += 360 * (analysis.viewdata.prtdata[i].GetCount() / (double)analysis.viewdata.pcount);

			posbuff.x = charmleftposbuf + 240;
			posbuff.y = 50 + (20 * i);

			setPGraphTex(i, tubuff, tvbuff);
			protocolgraphsqvset(posbuff, 12, 12, tubuff, tvbuff, 0xffffffff);

		}
		if (analysis.viewdata.prtcount == 0) {

			posbuff.x = charmleftposbuf + 125;
			posbuff.y = 128;

			setPGraphTex(0, tubuff, tvbuff);

			protocolgraphvset(posbuff, 100, 360, 0, tubuff, tvbuff, 0xff606060);

		}
	}
	else {
		//プロトコルグラフ
		for (int i = 0; i < analysis.alldata.prtcount; i++) {
			gstr.Stringset(charmleftposbuf + 250, 50 + (20 * i), CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%-20s:%5.1lf%%"), Protocol(analysis.alldata.prtdata[i].GetId()), (analysis.alldata.prtdata[i].GetCount() / (double)analysis.alldata.pcount) * 100.0);

			posbuff.x = charmleftposbuf + 125;
			posbuff.y = 128;

			setPGraphTex(i, tubuff, tvbuff);

			protocolgraphvset(posbuff, 100, 360 * (analysis.alldata.prtdata[i].GetCount() / (double)analysis.alldata.pcount)
				, anglbuff, tubuff, tvbuff, 0xffffffff);
			anglbuff += 360 * (analysis.alldata.prtdata[i].GetCount() / (double)analysis.alldata.pcount);

			posbuff.x = charmleftposbuf + 240;
			posbuff.y = 50 + (20 * i);

			setPGraphTex(i, tubuff, tvbuff);
			protocolgraphsqvset(posbuff, 12, 12, tubuff, tvbuff, 0xffffffff);

		}
		if (analysis.alldata.prtcount == 0) {

			posbuff.x = charmleftposbuf + 125;
			posbuff.y = 128;

			setPGraphTex(0, tubuff, tvbuff);

			protocolgraphvset(posbuff, 100, 360, 0, tubuff, tvbuff, 0xff606060);

		}
	}


	if (analysis.usecount > 0) {
		SizeGraph(charmleftposbuf + 12, 500, view.Width - (charmleftposbuf + 12), 200, sepbuff[0], sepbuff[1]);
	}

	gstr.Stringset(charmleftposbuf + 12, 520, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Time  :%4lluh%02llum%02llus%03llums"), (analysis.time.GetCaptureTime(0) / 3600), analysis.time.GetCaptureTime(0) % 3600 / 60, analysis.time.GetCaptureTime(0) % 60, analysis.time.GetCaptureTime(1));
	gstr.Stringset(charmleftposbuf + 12, 540, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MAX   :%16llu"), analysis.viewdata.max);
	gstr.Stringset(charmleftposbuf + 12, 560, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :%16llu"), analysis.viewdata.min);
	gstr.Stringset(charmleftposbuf + 12, 580, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("AVE   :%16.1lf"), analysis.viewdata.ave);
	gstr.Stringset(charmleftposbuf + 12, 600, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("SIZE  :%16llu[Total %16llu]"), analysis.viewdata.size, analysis.alldata.size);
	gstr.Stringset(charmleftposbuf + 12, 620, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Packet:%16llu[Total %16llu]"), analysis.viewdata.pcount, analysis.alldata.pcount);


	gstr.Stringset(charmleftposbuf + 212, 520, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Section Data"));
	gstr.Stringset(charmleftposbuf + 212, 540, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MAX  :%16llu"), analysis.viewsepmaxsize);
	if (analysis.viewsepminsize == 18446744073709551615) {
		gstr.Stringset(charmleftposbuf + 212, 560, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN  :Being measured..."));
	}
	else {
		gstr.Stringset(charmleftposbuf + 212, 560, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN  :%16llu"), analysis.viewsepminsize);
	}
	gstr.Stringset(charmleftposbuf + 212, 580, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("AVE  :%16.1lf"), analysis.viewsepavesize);

	setting.viewdatalangeold = setting.viewdatalange;
}

void Mode2strdr() {
	Pos posbuff;
	double anglbuff = 0;
	double tubuff[2], tvbuff[2];

	int sepbuff[2];

	double charmleftposbuf;
	for (int j = 0; j < area.usecount; j++) {
		if (area.data[j].GetId() == 0) {
			charmleftposbuf = area.data[j].GetPos().x - area.data[j].GetPosxy(0);
			break;
		}
	}

	switch (setting.viewdatalange) {
	case 0:
		setting.stime = 0;
		break;
	case 1:
		setting.stime = analysis.time.GetCaptureTime(0) >= 60 ? (analysis.time.GetCaptureTime(0) - 60) : 0;
		break;
	case 2:
		setting.stime = analysis.time.GetCaptureTime(0) >= (60 * 5) ? (analysis.time.GetCaptureTime(0) - (60 * 5)) : 0;
		break;
	case 3:
		setting.stime = analysis.time.GetCaptureTime(0) >= (60 * 10) ? (analysis.time.GetCaptureTime(0) - (60 * 10)) : 0;
		break;
	case 4:
		setting.stime = analysis.time.GetCaptureTime(0) >= (60 * 30) ? (analysis.time.GetCaptureTime(0) - (60 * 30)) : 0;
		break;
	default:
		break;
	}

	setting.etime = analysis.time.GetCaptureTime(0);

	

	sepbuff[0] = setting.stime / 10;
	sepbuff[1] = (setting.etime / 10 + 1);

	posbuff.z = 0;
	if (setting.viewdatalange != 0 || (setting.viewdatalangeold != 0 && setting.viewdatalange == 0)) {
		updataviewdata(setting.stime, setting.etime);
		//プロトコルグラフ
		for (int i = 0; i < analysis.viewdata.prtcount; i++) {
			gstr.Stringset(charmleftposbuf + 250, 50 + (20 * i), CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%-20s:%5.1lf%%[%5llu]"), Protocol(analysis.viewdata.prtdata[i].GetId()), (analysis.viewdata.prtdata[i].GetCount() / (double)analysis.viewdata.pcount) * 100.0, analysis.viewdata.prtdata[i].GetCount());

			posbuff.x = charmleftposbuf + 125;
			posbuff.y = 128;

			setPGraphTex(i, tubuff, tvbuff);

			protocolgraphvset(posbuff, 100, 360 * (analysis.viewdata.prtdata[i].GetCount() / (double)analysis.viewdata.pcount)
				, anglbuff, tubuff, tvbuff, 0xffffffff);
			anglbuff += 360 * (analysis.viewdata.prtdata[i].GetCount() / (double)analysis.viewdata.pcount);

			posbuff.x = charmleftposbuf + 240;
			posbuff.y = 50 + (20 * i);

			setPGraphTex(i, tubuff, tvbuff);

			protocolgraphsqvset(posbuff, 12, 12, tubuff, tvbuff, 0xffffffff);

		}
		if (analysis.viewdata.prtcount == 0) {

			posbuff.x = charmleftposbuf + 125;
			posbuff.y = 128;

			setPGraphTex(0, tubuff, tvbuff);

			protocolgraphvset(posbuff, 100, 360, 0, tubuff, tvbuff, 0xff606060);

		}
	}
	else {
		//プロトコルグラフ
		for (int i = 0; i < analysis.alldata.prtcount; i++) {
			gstr.Stringset(charmleftposbuf + 250, 50 + (20 * i), CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%-20s:%5.1lf%%[%5llu]"), Protocol(analysis.alldata.prtdata[i].GetId()), (analysis.alldata.prtdata[i].GetCount() / (double)analysis.alldata.pcount) * 100.0, analysis.alldata.prtdata[i].GetCount());

			posbuff.x = charmleftposbuf + 125;
			posbuff.y = 128;

			setPGraphTex(i, tubuff, tvbuff);

			protocolgraphvset(posbuff, 100, 360 * (analysis.alldata.prtdata[i].GetCount() / (double)analysis.alldata.pcount)
				, anglbuff, tubuff, tvbuff, 0xffffffff);
			anglbuff += 360 * (analysis.alldata.prtdata[i].GetCount() / (double)analysis.alldata.pcount);

			posbuff.x = charmleftposbuf + 240;
			posbuff.y = 50 + (20 * i);

			setPGraphTex(i, tubuff, tvbuff);
			protocolgraphsqvset(posbuff, 12, 12, tubuff, tvbuff, 0xffffffff);

		}
		if (analysis.alldata.prtcount == 0) {

			posbuff.x = charmleftposbuf + 125;
			posbuff.y = 128;

			setPGraphTex(0, tubuff, tvbuff);

			protocolgraphvset(posbuff, 100, 360, 0, tubuff, tvbuff, 0xff606060);

		}
	}

	//サイズグラフ
	if (analysis.usecount > 0) {
		SizeGraph(charmleftposbuf + 12, 500, 250, 200, sepbuff[0], sepbuff[1]);
	}

	//全体のデータ
	gstr.Stringset(charmleftposbuf + 272, 320, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("TotalPacket"));
	gstr.Stringset(charmleftposbuf + 272, 340, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MAX   :%16llu"), analysis.alldata.max);
	gstr.Stringset(charmleftposbuf + 272, 360, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :%16llu"), analysis.alldata.min);
	gstr.Stringset(charmleftposbuf + 272, 380, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("AVE   :%16.3lf"), analysis.alldata.ave);
	gstr.Stringset(charmleftposbuf + 272, 400, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("SIZE  :%16llu"), analysis.alldata.size);
	gstr.Stringset(charmleftposbuf + 272, 420, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Packet:%16llu"), analysis.alldata.pcount);

	gstr.Stringset(charmleftposbuf + 472, 320, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Section Data"));
	gstr.Stringset(charmleftposbuf + 472, 340, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MAX   :%16llu"), analysis.sepmaxsize);
	if (analysis.sepminsize == 4294967295) {
		gstr.Stringset(charmleftposbuf + 472, 360, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :Being measured..."));
	}
	else {
		gstr.Stringset(charmleftposbuf + 472, 360, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :%16llu"), analysis.sepminsize);
	}
	gstr.Stringset(charmleftposbuf + 472, 380, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("AVE   :%16.3lf"), analysis.sepavesize);


	//指定範囲のデータ
	switch (setting.viewdatalange) {
	case 0:
		gstr.Stringset(charmleftposbuf + 272, 445, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("All"));
		break;
	case 1:
		gstr.Stringset(charmleftposbuf + 272, 445, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Latest1min"));
		break;
	case 2:
		gstr.Stringset(charmleftposbuf + 272, 445, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Latest5min"));
		break;
	case 3:
		gstr.Stringset(charmleftposbuf + 272, 445, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Latest10min"));
		break;
	case 4:
		gstr.Stringset(charmleftposbuf + 272, 445, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Latest30min"));
		break;
	default:
		break;
	}
	
	gstr.Stringset(charmleftposbuf + 272, 465, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MAX   :%16llu"), analysis.viewdata.max);
	gstr.Stringset(charmleftposbuf + 272, 485, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :%16llu"), analysis.viewdata.min);
	gstr.Stringset(charmleftposbuf + 272, 505, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("AVE   :%16.3lf"), analysis.viewdata.ave);
	gstr.Stringset(charmleftposbuf + 272, 525, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("SIZE  :%16llu"), analysis.viewdata.size);
	gstr.Stringset(charmleftposbuf + 272, 545, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Packet:%16llu"), analysis.viewdata.pcount);

	gstr.Stringset(charmleftposbuf + 472, 445, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Section Data"));
	gstr.Stringset(charmleftposbuf + 472, 465, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MAX   :%16llu"), analysis.viewsepmaxsize);
	if (analysis.viewsepminsize == 18446744073709551615) {
		gstr.Stringset(charmleftposbuf + 472, 485, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :Being measured..."));
	}
	else {
		gstr.Stringset(charmleftposbuf + 472, 485, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :%16llu"), analysis.viewsepminsize);
	}
	gstr.Stringset(charmleftposbuf + 472, 505, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("AVE   :%16.3lf"), analysis.viewsepavesize);

	//時間
	gstr.Stringset(charmleftposbuf + 680, 500, CHARM_LAGTXT_SIZE, CHARM_LAGTXT_SIZE, 0, 0, CHARM_LAGTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Caputuer Time"));
	gstr.Stringset(charmleftposbuf + 680, 562, CHARM_LAGTXT_SIZE, CHARM_LAGTXT_SIZE, 0, 0, CHARM_LAGTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%4lluh%02llum%02llus%03llums"), (analysis.time.GetCaptureTime(0) / 3600), analysis.time.GetCaptureTime(0) % 3600 / 60, analysis.time.GetCaptureTime(0) % 60, analysis.time.GetCaptureTime(1));

	gstr.Stringset(charmleftposbuf + 680, 626, CHARM_LAGTXT_SIZE, CHARM_LAGTXT_SIZE, 0, 0, CHARM_LAGTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Start Time"));
	gstr.Stringset(charmleftposbuf + 680, 688, CHARM_LAGTXT_SIZE, CHARM_LAGTXT_SIZE, 0, 0, CHARM_LAGTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%4d:%02d:%02d.%03d"), (analysis.time.GetStartTime(0) / 3600) % 24, analysis.time.GetStartTime(0) % 3600 / 60, analysis.time.GetStartTime(0) % 60, analysis.time.GetStartTime(1));

	gstr.Stringset(charmleftposbuf + 680, 750, CHARM_LAGTXT_SIZE, CHARM_LAGTXT_SIZE, 0, 0, CHARM_LAGTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Now Time"));
	gstr.Stringset(charmleftposbuf + 680, 812, CHARM_LAGTXT_SIZE, CHARM_LAGTXT_SIZE, 0, 0, CHARM_LAGTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%4d:%02d:%02d.%03d"), (analysis.time.GetNowTime(0) / 3600) % 24, analysis.time.GetNowTime(0) % 3600 / 60, analysis.time.GetNowTime(0) % 60, analysis.time.GetNowTime(1));

	setting.viewdatalangeold = setting.viewdatalange;
}

void Mode3strdr() {
	Pos posbuff;
	double anglbuff = 0;
	double tubuff[2], tvbuff[2];

	int sepbuff[2];

	double charmleftposbuf;
	for (int j = 0; j < area.usecount; j++) {
		if (area.data[j].GetId() == 0) {
			charmleftposbuf = area.data[j].GetPos().x - area.data[j].GetPosxy(0);
			break;
		}
	}

	if (setting.endtimechase == 1) {
		setting.etime = analysis.time.GetCaptureTime(0);
	}

	updataviewdata(setting.stime, setting.etime);
	
	sepbuff[0] = setting.stime / 10;
	sepbuff[1] = (setting.etime / 10 + 1);

	posbuff.z = 0;


	//プロトコルグラフ
	for (int i = 0; i < analysis.viewdata.prtcount; i++) {
		gstr.Stringset(charmleftposbuf + 902, 648 + (20 * i), CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%-20s:%5.1lf%%[%5d]"), Protocol(analysis.viewdata.prtdata[i].GetId()), (analysis.viewdata.prtdata[i].GetCount() / (double)analysis.viewdata.pcount) * 100.0, analysis.viewdata.prtdata[i].GetCount());

		posbuff.x = charmleftposbuf + 777;
		posbuff.y = 726;

		setPGraphTex(i, tubuff, tvbuff);

		protocolgraphvset(posbuff, 100, 360 * (analysis.viewdata.prtdata[i].GetCount() / (double)analysis.viewdata.pcount)
			, anglbuff, tubuff, tvbuff, 0xffffffff);
		anglbuff += 360 * (analysis.viewdata.prtdata[i].GetCount() / (double)analysis.viewdata.pcount);

		posbuff.x = charmleftposbuf + 892;
		posbuff.y = 648 + (20 * i);

		setPGraphTex(i, tubuff, tvbuff);
		protocolgraphsqvset(posbuff, 12, 12, tubuff, tvbuff, 0xffffffff);

	}
	if (analysis.viewdata.prtcount == 0) {

		posbuff.x = charmleftposbuf + 777;
		posbuff.y = 726;

		setPGraphTex(0, tubuff, tvbuff);

		protocolgraphvset(posbuff, 100, 360, 0, tubuff, tvbuff, 0xff606060);

	}


	//サイズグラフ
	if (analysis.usecount > 0) {
		SizeGraph(charmleftposbuf + 12, 400, 1150, 325, sepbuff[0], sepbuff[1]);
	}

	//全体のデータ
	gstr.Stringset(charmleftposbuf + 264, 420, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 1, _T("TotalPacket"));
	
	gstr.Stringset(charmleftposbuf + 72, 440, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Packet Data"));
	gstr.Stringset(charmleftposbuf + 72, 460, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MAX   :%16llu"), analysis.alldata.max);
	gstr.Stringset(charmleftposbuf + 72, 480, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :%16llu"), analysis.alldata.min);
	gstr.Stringset(charmleftposbuf + 72, 500, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("AVE   :%16.3lf"), analysis.alldata.ave);
	gstr.Stringset(charmleftposbuf + 72, 520, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("SIZE  :%16llu"), analysis.alldata.size);
	gstr.Stringset(charmleftposbuf + 72, 540, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Packet:%16llu"), analysis.alldata.pcount);
	switch (analysis.alldata.version) {
	case 4:
		gstr.Stringset(charmleftposbuf + 72, 560, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("IP    : %d.%d.%d.%d"),
			analysis.alldata.ip[0], analysis.alldata.ip[1], analysis.alldata.ip[2], analysis.alldata.ip[3]);
		break;

	case 6:
		gstr.Stringset(charmleftposbuf + 72, 560, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("IP    : %x:%x:%x:%x:%x:%x:%x:%x"),
			analysis.alldata.ip[0], analysis.alldata.ip[1], analysis.alldata.ip[2], analysis.alldata.ip[3], analysis.alldata.ip[4], analysis.alldata.ip[5], analysis.alldata.ip[6], analysis.alldata.ip[7]);
		break;

	default:
		gstr.Stringset(charmleftposbuf + 72, 560, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("IP    : ----:----:----:----:----:----:----:----"));
		break;
	}
	gstr.Stringset(charmleftposbuf + 72, 580, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0,     _T("Port  :%16d"), analysis.alldata.port);


	gstr.Stringset(charmleftposbuf + 272, 440, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Section Data"));
	gstr.Stringset(charmleftposbuf + 272, 460, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MAX   :%16llu"), analysis.sepmaxsize);
	if (analysis.sepminsize == 4294967295) {
		gstr.Stringset(charmleftposbuf + 272, 480, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :Being measured..."));
	}
	else {
		gstr.Stringset(charmleftposbuf + 272, 480, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :%16llu"), analysis.sepminsize);
	}
	gstr.Stringset(charmleftposbuf + 272, 500, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("AVE   :%16.3lf"), analysis.sepavesize);


	//指定範囲のデータ
	gstr.Stringset(charmleftposbuf + 664, 420, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0x80000000 + skinset.SRGB, 1, _T("   h  m  s    h  m  s"));
	gstr.Stringset(charmleftposbuf + 664, 420, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 1, _T("%3llu %02llu %02llu ~%3llu %02llu %02llu "),
		(setting.stime / 3600), setting.stime % 3600 / 60, setting.stime % 60,
		(setting.etime / 3600), setting.etime % 3600 / 60, setting.etime % 60);

	gstr.Stringset(charmleftposbuf + 472, 440, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Packet Data"));
	gstr.Stringset(charmleftposbuf + 472, 460, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MAX   :%16llu"), analysis.viewdata.max);
	gstr.Stringset(charmleftposbuf + 472, 480, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :%16llu"), analysis.viewdata.min);
	gstr.Stringset(charmleftposbuf + 472, 500, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("AVE   :%16.3lf"), analysis.viewdata.ave);
	gstr.Stringset(charmleftposbuf + 472, 520, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("SIZE  :%16llu"), analysis.viewdata.size);
	gstr.Stringset(charmleftposbuf + 472, 540, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Packet:%16llu"), analysis.viewdata.pcount);
	switch (analysis.viewdata.version) {
	case 4:
		gstr.Stringset(charmleftposbuf + 472, 560, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("IP    : %d.%d.%d.%d"),
			analysis.viewdata.ip[0], analysis.viewdata.ip[1], analysis.viewdata.ip[2], analysis.viewdata.ip[3]);
		break;

	case 6:
		gstr.Stringset(charmleftposbuf + 472, 560, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("IP    : %x:%x:%x:%x:%x:%x:%x:%x"),
			analysis.viewdata.ip[0], analysis.viewdata.ip[1], analysis.viewdata.ip[2], analysis.viewdata.ip[3], analysis.viewdata.ip[4], analysis.viewdata.ip[5], analysis.viewdata.ip[6], analysis.viewdata.ip[7]);
		break;

	default:
		gstr.Stringset(charmleftposbuf + 472, 560, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("IP    : ----:----:----:----:----:----:----:----"));

		break;
	}
	gstr.Stringset(charmleftposbuf + 472, 580, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Port  :%16d"), analysis.viewdata.port);



	gstr.Stringset(charmleftposbuf + 672, 440, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Section Data"));
	gstr.Stringset(charmleftposbuf + 672, 460, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MAX   :%16llu"), analysis.viewsepmaxsize);
	if (analysis.viewsepminsize == 18446744073709551615) {
		gstr.Stringset(charmleftposbuf + 672, 480, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :Being measured..."));
	}
	else {
		gstr.Stringset(charmleftposbuf + 672, 480, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("MIN   :%16llu"), analysis.viewsepminsize);
	}
	gstr.Stringset(charmleftposbuf + 672, 500, CHARM_BASETXT_SIZE, CHARM_BASETXT_SIZE, 0, 0, CHARM_BASETXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("AVE   :%16.3lf"), analysis.viewsepavesize);

	
	//時間
	gstr.Stringset(charmleftposbuf + 872, 420, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, CHARM_MIDTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Caputuer Time"));
	gstr.Stringset(charmleftposbuf + 872, 454, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, CHARM_MIDTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%4lluh%02llum%02llus%03llums"), (analysis.time.GetCaptureTime(0) / 3600), analysis.time.GetCaptureTime(0) % 3600 / 60, analysis.time.GetCaptureTime(0) % 60, analysis.time.GetCaptureTime(1));

	gstr.Stringset(charmleftposbuf + 872, 488, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, CHARM_MIDTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Start Time"));
	gstr.Stringset(charmleftposbuf + 872, 522, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, CHARM_MIDTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%4d:%02d:%02d.%03d"), (analysis.time.GetStartTime(0) / 3600) % 24, analysis.time.GetStartTime(0) % 3600 / 60, analysis.time.GetStartTime(0) % 60, analysis.time.GetStartTime(1));

	gstr.Stringset(charmleftposbuf + 872, 556, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, CHARM_MIDTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("Now Time"));
	gstr.Stringset(charmleftposbuf + 872, 590, CHARM_MIDTXT_SIZE, CHARM_MIDTXT_SIZE, 0, 0, CHARM_MIDTXT_MARG, 0xFF000000 + skinset.SRGB, 0, _T("%4d:%02d:%02d.%03d"), (analysis.time.GetNowTime(0) / 3600) % 24, analysis.time.GetNowTime(0) % 3600 / 60, analysis.time.GetNowTime(0) % 60, analysis.time.GetNowTime(1));

}

void SizeGraph(double x, double y, double sizex, double sizey, int startsep, int endsep) {
	double bufftu[] = { 0.0, 1.0 };
	double bufftv[] = { 0.1, 0.5 };
	
	double sizebuff;
	Pos posbuff;

	posbuff.x = x + (sizex / 2);
	posbuff.y = y - (sizey / 2);
	posbuff.z = 0;

	bufftu[0] = 0;
	bufftu[1] = 0.3 * (sizex / sizey);

	bufftv[0] = 0.7;
	bufftv[1] = 1.0;

	sizegraphbgvsetsq(posbuff, sizex, sizey, 1.0, 0, bufftu, bufftv, 0xFFFFFFFF);

	bufftu[0] = 0;
	bufftu[1] = (sizex + 3) / 128.0;

	bufftv[0] = 0.0;
	bufftv[1] = 0.025;

	posbuff.x = x + (sizex / 2) - 5;
	posbuff.y = y + 5;
	posbuff.z = 0;

	sizegraphbgvsetsq(posbuff, sizex + 10, 10, 1.0, 0, bufftu, bufftv, 0xFFFFFFFF);

	posbuff.x = x - 5;
	posbuff.y = y - (sizey / 2) + 5;
	posbuff.z = 0;

	bufftu[0] = 0;
	bufftu[1] = (sizex + 3) / 128.0;

	bufftv[0] = 0.025;
	bufftv[1] = 0.0;

	sizegraphbgvsetsq(posbuff, sizey + 10, 10, 1.0, -90, bufftu, bufftv, 0xFFFFFFFF);


	bufftv[0] = 0.1;
	bufftv[1] = 0.5;

	if (analysis.usecount < startsep) startsep = analysis.usecount;

	if (analysis.usecount < endsep) endsep = analysis.usecount;

	for (int i = 0; i < (endsep - startsep); i++) {

		if ((endsep - startsep) == 1) {
			posbuff.x = x;
		}
		else {
			
			posbuff.x = x + ((sizex / (endsep - startsep - 1)) * i);
		}
		posbuff.y = y;
		posbuff.z = 0;
		if (analysis.alldata.size == 0) {
			sizebuff = 0;
		}
		else {
			sizebuff = ((double)analysis.data[startsep + i].size / analysis.sepmaxsize) <= 1 ? ((double)analysis.data[startsep + i].size / analysis.sepmaxsize) : 1.0;
		}
		bufftv[0] = 0.5 - (0.4 * sizebuff);
		if ((endsep - startsep) == 1) {
			sizegraphvset(posbuff, sizebuff * sizey, 0.0, bufftv);
		}
		else {

			sizegraphvset(posbuff, sizebuff * sizey, ((sizex / (endsep - startsep - 1)) * i) / 128.0, bufftv);
		}
		
		if (i > 0) {
			sizegraphiset();
		}
	}
	if (endsep - startsep == 1) {
		posbuff.x = x + sizex;
		posbuff.y = y;
		posbuff.z = 0;
		sizegraphvset(posbuff, sizebuff * sizey, sizex / 128.0, bufftv);
		sizegraphiset();
	}
	if (endsep - startsep == 0) {
		posbuff.x = x;
		posbuff.y = y;
		posbuff.z = 0;
		sizegraphvset(posbuff, 0, 0.0, bufftv);

		posbuff.x = x + sizex;
		posbuff.y = y;
		posbuff.z = 0;
		sizegraphvset(posbuff, 0, sizex / 128.0, bufftv);
		sizegraphiset();
	}

	bufftu[0] = 0;
	bufftu[1] = (sizex + 3) / 128.0;

	bufftv[0] = 0.5 - (0.4 * sizebuff);
	bufftv[1] = 0.5 - (0.4 * sizebuff) + 0.001;

	posbuff.x = x + (sizex / 2);
	posbuff.y = y - (sizebuff * sizey) + 1.5;
	posbuff.z = 0;

	sizegraphvsetsq(posbuff, sizex, 3, 1.0, 0, bufftu, bufftv, 0xFF404040);


	gstr.Stringset(x + sizex, y - 10 - (sizebuff * sizey), 18, 24, 0, 0, 8, 0xFF000000 + skinset.SRGB, 2, _T("%10llu"), analysis.data[endsep - 1].size);
}

void setPGraphTex(int id, double *tubuff, double *tvbuff) {
	tubuff[0] = 0.1 * id;
	tubuff[1] = 0.1 * (id + 1);

	tvbuff[0] = 0.5;
	tvbuff[1] = 0.6;
}

void TrashAnalysis() {
	analysis.Free();
}