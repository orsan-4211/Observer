#include <thread>
#include <iostream>
//#include <pcap.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <time.h>
#include <IPHlpApi.h>

#include "../main.h"

#include "../Include/pcap.h"

#include "../keyinput.h"

#include "../sidecharm/charmmain.h"


pcap_if_t *alldevs;
pcap_if_t *d;
pcap_t **handle;
int handlei;
int handlemaxi;

int lockf;
const u_char *pkt_data;

pcap_pkthdr *pkt_header;

HostIPDataMaster hdata;

PacketDataMasterclass pdata;
PacketDataMasterclass pdrdata;
int pdatacheckf;

PcapDeviceMaster pdev;

int deviceselected = 0;
int selecteddevice = 0;


int filelockf;

int doi;
PCHAR doadpname;


bool IpMatchCheck(int *ip1, int *ip2) {
	int ipcheckflag = 8;
	for (int j = 0; j < 8; j++) {
		if (ip1[j] == ip2[j]) {
			ipcheckflag--;
		}
	}
	if (ipcheckflag == 0) return 0;

	return 1;
}

bool IpMatchCheck_NonView(int *ip1, int version) {
	int ipcheckflag = 1;
	if (version == 4) {
		if (ip1[0] == 0) {
			ipcheckflag = 0;
		}else
		if (ip1[0] == 10) {
			ipcheckflag = 0;
		}else
		if (ip1[0] == 127) {
			ipcheckflag = 0;
		}else
		if (ip1[0] == 224) {
			ipcheckflag = 0;
		}else
		if (ip1[0] == 239) {
			ipcheckflag = 0;
		}else
		if (ip1[0] == 240) {
			ipcheckflag = 0;
		}else
		if (ip1[0] == 255) {
			ipcheckflag = 0;
		}
	}
	else if (version == 6) {
		if (ip1[0] == 0x0000 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0000) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x0000 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0001) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x0000 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0xffff) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x0064 &&
			ip1[1] == 0xff9b &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x0064 &&
			ip1[1] == 0xff9b &&
			ip1[2] == 0x0001) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x0100 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000) {
			ipcheckflag = 0;
		}
		else if ((ip1[0] & 0b1110000000000000) == 0x2000 ) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2001 &&
			(ip1[1] & 0b1111111000000000)  == 0x0000) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2001 &&
			ip1[1] == 0x0001 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0001) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2001 &&
			ip1[1] == 0x0001 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0002) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2001 &&
			ip1[1] == 0x0002 &&
			ip1[2] == 0x0000) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2001 &&
			ip1[1] == 0x0003) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2001 &&
			ip1[1] == 0x0004 &&
			ip1[2] == 0x0112) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2001 &&
			ip1[1] == 0x0005) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2001 &&
			(ip1[1] & 0b1111111111100000) == 0x0010) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2001 &&
			(ip1[1] & 0b1111111111100000) == 0x0020) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2001 &&
			ip1[1] == 0x0db8) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2002) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x2620 &&
			ip1[1] == 0x004f &&
			ip1[2] == 0x8000) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0x3ffe) {
			ipcheckflag = 0;
		}
		else if ((ip1[0] & 0b1111111000000000) == 0xfc00) {
			ipcheckflag = 0;
		}
		else if ((ip1[0] & 0b1111111100000000) == 0xfc00) {
			ipcheckflag = 0;
		}
		else if ((ip1[0] & 0b1111111100000000) == 0xfd00) {
			ipcheckflag = 0;
		}
		else if ((ip1[0] & 0b1111111111000000) == 0xfe80) {
			ipcheckflag = 0;
		}
		else if ((ip1[0] & 0b1111111111000000) == 0xfec0) {
			ipcheckflag = 0;
		}
		else if ((ip1[0] & 0b1111111100000000) == 0xff00) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff01) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff01 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0001) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff01 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0002) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0001) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0002) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0004) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0005) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0006) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0007) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0008) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0009) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x000a) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x000b) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x000c) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x000d) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x000e) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0001 &&
			ip1[7] == 0x0001) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0001 &&
			ip1[7] == 0x0002) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff02 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0001 &&
			ip1[7] == 0x0003) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff05) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff05 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x0002) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff05 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0001 &&
			ip1[7] == 0x0003) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff05 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0001 &&
			ip1[7] == 0x0004) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff05 &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0001 &&
			ip1[7] == 0x000c) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff0e) {
			ipcheckflag = 0;
		}
		else if (ip1[0] == 0xff0e &&
			ip1[1] == 0x0000 &&
			ip1[2] == 0x0000 &&
			ip1[3] == 0x0000 &&
			ip1[4] == 0x0000 &&
			ip1[5] == 0x0000 &&
			ip1[6] == 0x0000 &&
			ip1[7] == 0x000c) {
			ipcheckflag = 0;
		}
	}
	
	if (ipcheckflag == 0) return 0;

	return 1;
}

void Packetstat::PacketCheck(int adapterNo, const u_char *pkt_data, int size, unsigned long long *time) {
	this->adapterNo = adapterNo;

	checked = 0;

	version = 0;
	//protocoltype = 0;
	protocol = 0;
	//headersize = 0;
	pacsize = 0;
	for (int i = 0; i < 8; i++) {
		atip[i] = 0;
		fromip[i] = 0;
	}
	//atmac = 0;
	atport = 0;

	//frommac = 0;
	fromport = 0;

	for (int i = 0; i < size; i++) {
		switch (i) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			//atmac = (atmac * 10) + pkt_data[i];
			break;
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			//frommac = (frommac * 10) + pkt_data[i];
			break;
		case 12:
		case 13:
			//protocoltype = (protocoltype * 10) + pkt_data[i];
			break;
		case 14:
			version = pkt_data[i] / 0x10;

			break;
		}
		if (version == 4) {
			switch (i) {
			case 14:
				//headersize = pkt_data[i] % 0x10;
				break;
			case 16:
			case 17:
				pacsize = (pacsize * 10) + pkt_data[i];
				break;
			case 23:
				protocol = pkt_data[i];
				break;
			case 26:
			case 27:
			case 28:
			case 29:
				fromip[i % 26] = pkt_data[i];
				break;
			case 30:
			case 31:
			case 32:
			case 33:
				atip[i % 30] = pkt_data[i];
				break;
			case 34:
			case 35:
				fromport = (fromport * 10) + pkt_data[i];
				break;
			case 36:
			case 37:
				atport = (atport * 10) + pkt_data[i];
				break;
			}
		}
		else
			if (version == 6) {
				switch (i) {
				case 18:
				case 19:
					pacsize = (pacsize * 10) + pkt_data[i];
					break;
				case 22:
				case 23:
				case 24:
				case 25:
				case 26:
				case 27:
				case 28:
				case 29:
				case 30:
				case 31:
				case 32:
				case 33:
				case 34:
				case 35:
				case 36:
				case 37:
					fromip[(i % 22) / 2] = (fromip[(i % 22) / 2] * 256) + pkt_data[i];
					break;
				case 38:
				case 39:
				case 40:
				case 41:
				case 42:
				case 43:
				case 44:
				case 45:
				case 46:
				case 47:
				case 48:
				case 49:
				case 50:
				case 51:
				case 52:
				case 53:
					atip[(i % 38) / 2] = (atip[(i % 38) / 2] * 256) + pkt_data[i];
					break;
				case 54:
				case 55:
					fromport = (fromport * 10) + pkt_data[i];
					break;
				case 56:
				case 57:
					atport = (atport * 10) + pkt_data[i];
					break;
				}

			}
	}

	arrivaltime[0] = time[0];
	arrivaltime[1] = time[1];
	//cprotocol = Protocol();
}

void Packetstat::PacketSet(Packetstat stat) {
	checked = 0;

	version = stat.version;
	protocol = stat.protocol;
	pacsize = stat.pacsize;
	for (int i = 0; i < 8; i++) {
		atip[i] = stat.atip[i];
		fromip[i] = stat.fromip[i];
	}
	atport = stat.atport;

	fromport = stat.fromport;

	arrivaltime[0] = stat.arrivaltime[0];
	arrivaltime[1] = stat.arrivaltime[1];
}

char* Packetstat::Protocol() {
	switch (protocol) {
	case 0x00: return "HOPOPT";
	case 0x01: return "ICMP";
	case 0x02: return "IGMP";
	case 0x03: return "GGP";
	case 0x04: return "IP - in - IP";
	case 0x05: return "ST";
	case 0x06: return "TCP";
	case 0x07: return "CBT";
	case 0x08: return "EGP";
	case 0x09: return "IGP";
	case 0x0A: return "BBN - RCC - MON";
	case 0x0B: return "NVP - II";
	case 0x0C: return "PUP";
	case 0x0D: return "ARGUS";
	case 0x0E: return "EMCON";
	case 0x0F: return "XNET";
	case 0x10: return "CHAOS";
	case 0x11: return "UDP";
	case 0x12: return "MUX";
	case 0x13: return "DCN - MEAS";
	case 0x14: return "HMP";
	case 0x15: return "PRM";
	case 0x16: return "XNS - IDP";
	case 0x17: return "TRUNK - 1";
	case 0x18: return "TRUNK - 2";
	case 0x19: return "LEAF - 1";
	case 0x1A: return "LEAF - 2";
	case 0x1B: return "RDP";
	case 0x1C: return "IRTP";
	case 0x1D: return "ISO - TP4";
	case 0x1E: return "NETBLT";
	case 0x1F: return "MFE - NSP";
	case 0x20: return "MERIT - INP";
	case 0x21: return "DCCP";
	case 0x22: return "3PC";
	case 0x23: return "IDPR";
	case 0x24: return "XTP";
	case 0x25: return "DDP";
	case 0x26: return "IDPR - CMTP";
	case 0x27: return "TP++";
	case 0x28: return "IL";
	case 0x29: return "IPv6";
	case 0x2A: return "SDRP";
	case 0x2B: return "IPv6 - Route";
	case 0x2C: return "IPv6 - Frag";
	case 0x2D: return "IDRP";
	case 0x2E: return "RSVP";
	case 0x2F: return "GREs";
	case 0x30: return "DSR";
	case 0x31: return "BNA";
	case 0x32: return "ESP";
	case 0x33: return "AH";
	case 0x34: return "I - NLSP";
	case 0x35: return "SWIPE";
	case 0x36: return "NARP";
	case 0x37: return "MOBILE";
	case 0x38: return "TLSP";
	case 0x39: return "SKIP";
	case 0x3A: return "IPv6 - ICMP";
	case 0x3B: return "IPv6 - NoNxt";
	case 0x3C: return "IPv6 - Opts";
	case 0x3E: return "CFTP";
	case 0x40: return "SAT - EXPAK";
	case 0x41: return "KRYPTOLAN";
	case 0x42: return "RVD";
	case 0x43: return "IPPC";
	case 0x45: return "SAT - MON";
	case 0x46: return "VISA";
	case 0x47: return "IPCU";
	case 0x48: return "CPNX";
	case 0x49: return "CPHB";
	case 0x4A: return "WSN";
	case 0x4B: return "PVP";
	case 0x4C: return "BR - SAT - MON";
	case 0x4D: return "SUN - ND";
	case 0x4E: return "WB - MON";
	case 0x4F: return "WB - EXPAK";
	case 0x50: return "ISO - IP";
	case 0x51: return "VMTP";
	case 0x52: return "SECURE - VMTP";
	case 0x53: return "VINES";
	case 0x54: return "TTP or IPTM";
	case 0x55: return "NSFNET - IGP";
	case 0x56: return "DGP";
	case 0x57: return "TCF";
	case 0x58: return "EIGRP";
	case 0x59: return "OSPF";
	case 0x5A: return "Sprite - RPC";
	case 0x5B: return "LARP";
	case 0x5C: return "MTP";
	case 0x5D: return "AX.25";
	case 0x5E: return "OS";
	case 0x5F: return "MICP";
	case 0x60: return "SCC - SP";
	case 0x61: return "ETHERIP";
	case 0x62: return "ENCAP";
	case 0x64: return "GMTP";
	case 0x65: return "IFMP";
	case 0x66: return "PNNI";
	case 0x67: return "PIM";
	case 0x68: return "ARIS";
	case 0x69: return "SCPS";
	case 0x6A: return "QNX";
	case 0x6B: return "A / N";
	case 0x6C: return "IPComp";
	case 0x6D: return "SNP";
	case 0x6E: return "Compaq - Peer";
	case 0x6F: return "IPX - in - IP";
	case 0x70: return "VRRP";
	case 0x71: return "PGM";
	case 0x73: return "L2TP";
	case 0x74: return "DDX";
	case 0x75: return "IATP";
	case 0x76: return "STP";
	case 0x77: return "SRP";
	case 0x78: return "UTI";
	case 0x79: return "SMP";
	case 0x7A: return "SM";
	case 0x7B: return "PTP";
	case 0x7C: return "IS - IS over IPv4";
	case 0x7D: return "FIRE";
	case 0x7E: return "CRTP";
	case 0x7F: return "CRUDP";
	case 0x80: return "SSCOPMCE";
	case 0x81: return "IPLT";
	case 0x82: return "SPS";
	case 0x83: return "PIPE";
	case 0x84: return "SCTP";
	case 0x85: return "FC";
	case 0x86: return "RSVP - E2E - IGNORE";
	case 0x88: return "UDPLite";
	case 0x89: return "MPLS - in - IP";
	case 0x8A: return "manet";
	case 0x8B: return "HIP";
	case 0x8C: return "Shim6";
	case 0x8D: return "WESP";
	case 0x8E: return "ROHC";
	default: return "etc.";
	}
}

void PacketDataMasterclass::reset() {
	//data = (PacketData *)malloc(sizeof(PacketData) * 1);
	//usecount = 0;
	newdata = (Packetstat *)malloc(sizeof(Packetstat) * 1);
	newdatacount = 0;
	packcount = 0;
}

void PacketDataMasterclass::jenerate(int adapterNo, const u_char *pkt_data, int size, unsigned long long *time) {
	//data = (PacketData *)realloc(data, sizeof(PacketData) * (usecount + 1));
	newdata = (Packetstat *)realloc(newdata, sizeof(Packetstat) * (newdatacount + 1));

	newdata[newdatacount].PacketCheck(adapterNo, pkt_data, pkt_header->caplen, time);
#if _DEBUG
	if (PACKET_LOG == 1) newdata[newdatacount].DebugLogPrint();
#endif
	if (newdata[newdatacount].pacsize != 0) {
		//usecount++;
		packcount++;
		newdatacount++;
	}
}

void PacketDataMasterclass::jenerate(int adapterNo, const u_char *pkt_data, int size, unsigned long long time, unsigned long long timems) {
	//data = (PacketData *)realloc(data, sizeof(PacketData) * (usecount + 1));
	newdata = (Packetstat *)realloc(newdata, sizeof(Packetstat) * (newdatacount + 1));
	unsigned long long timebuff[2] = { time, timems };
	newdata[newdatacount].PacketCheck(adapterNo, pkt_data, pkt_header->caplen, timebuff);

#if _DEBUG
	if(PACKET_LOG == 1) newdata[newdatacount].DebugLogPrint();
#endif

	if (newdata[newdatacount].pacsize > 0 && IpMatchCheck_NonView(newdata[newdatacount].atip, newdata[newdatacount].version) && IpMatchCheck_NonView(newdata[newdatacount].fromip, newdata[newdatacount].version)) {
		//data[usecount].dat = pkt_data;
		//usecount++;
		packcount++;
		newdatacount++;
	}
}

void PacketDataMasterclass::jenerate(Packetstat stat) {
	newdata = (Packetstat *)realloc(newdata, sizeof(Packetstat) * (newdatacount + 1));
	newdata[newdatacount].PacketSet(stat);

	packcount++;
	newdatacount++;
}

void PacketDataMasterclass::remove() {

}

void PacketDataMasterclass::newdatacheck() {
	int *newdatamovei = (int *)malloc(sizeof(int) * newdatacount);
	for (int i = 0; i < newdatacount; i++) {
		newdatamovei[i] = -1;
	}

	int movebuff = 0;
	int movedbuff = 0;
	int newdatacountbuff = 0;

	for (int i = 0; i < newdatacount; i++) {
		if (newdata[i].checked == 0b11111) {
			newdatamovei[movebuff] = i;
			movebuff++;
		}
		else {
			newdatacountbuff++;
			if (newdatamovei[movedbuff] != -1) {
				newdata[newdatamovei[movedbuff]] = newdata[i];
				
				newdatamovei[movedbuff] = -1;
				movedbuff++;

				newdatamovei[movebuff] = i;
				movebuff++;
			}
		}
	}
	newdatacount = newdatacountbuff;

	if(newdatamovei != NULL) free(newdatamovei);
	newdatamovei = NULL;
}

void PacketDataMasterclass::datafree() {
	//if (data != NULL) {
	//	free(data);
	//}
	//data = NULL;
	//usecount = 0;

	if (newdata != NULL) {
		free(newdata);
	}
	newdata = NULL;
	newdatacount = 0;

}


void PcapDeviceMaster::reset() {
	data = (PcapDeviceData *)malloc(sizeof(PcapDeviceData) * 1);
	usecount = 0;
}

void PcapDeviceMaster::jenerate(int adapterNo, PCHAR adaptername, PWCHAR FriendlyName) {
	data = (PcapDeviceData *)realloc(data, sizeof(PcapDeviceData) * (usecount + 1));

	data[usecount].adapterNo = adapterNo;
	data[usecount].adaptername = adaptername;
	data[usecount].FriendlyName = FriendlyName;
	data[usecount].hostipmax = 0;
	for (int i = 0; i < 32; i++) {
		data[usecount].hostipid[i] = -1;
	}
	usecount++;
}

void PcapDeviceMaster::remove() {

}

void PcapDeviceMaster::datafree() {
	if (data != NULL) {
		free(data);
	}
	usecount = 0;
	data = NULL;
	
}

void HostIPDataMaster::reset() {
	data = (HostIPData *)malloc(sizeof(HostIPData) * 1);
	usecount = 0;
}

void HostIPDataMaster::jenerate(int adapterNo, int *ip, int version, PCHAR adaptername) {
	data = (HostIPData *)realloc(data, sizeof(HostIPData) * (usecount + 1));

	data[usecount].adapterNo = adapterNo;
	for (int i = 0; i < 8; i++) {
		data[usecount].ip[i] = ip[i];
	}
	data[usecount].version = version;
	data[usecount].adaptername = adaptername;

	usecount++;
}

void HostIPDataMaster::remove() {

}

void HostIPDataMaster::datafree() {
	if (data != NULL) {
		free(data);
	}
	usecount = 0;
	data = NULL;

}


void DevSelect(int devcount) {

	for (int i = -1; i < devcount; i++) {
		if ((view.Height / 2 - (22 * (devcount / 2.0))) + (22 * i) - 11 <= dms.pos.y
			&& (view.Height / 2 - (22 * (devcount / 2.0))) + (22 * i) + 11 >= dms.pos.y) {
			selecteddevice = i;
		}
	}

	if (mouseleft == 1
		&& (view.Width / 2) - 128 <= dms.pos.x
		&& (view.Width / 2) + 128 >= dms.pos.x
		&& (view.Height / 2 - (22 * (devcount / 2.0))) + (22 * selecteddevice) - 11 <= dms.pos.y
		&& (view.Height / 2 - (22 * (devcount / 2.0))) + (22 * selecteddevice) + 11 >= dms.pos.y) {
		if (selecteddevice == -1) {
			resetf = 1;
			threadfinf = 1;
			deviceselected = -1;
		}else
		if (pdev.data[selecteddevice].adapterNo != -1) {

			doi = pdev.data[selecteddevice].adapterNo;
			doadpname = pdev.data[selecteddevice].adaptername;
			deviceselected = 1;

		}
	}

}

#define IPTOSBUFFERS	12
char *iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3 * 4 + 3 + 1];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	sprintf_s(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}

char* ip6tos(struct sockaddr *sockaddr, char *address, int addrlen)
{
	socklen_t sockaddrlen;

#ifdef WIN32
	sockaddrlen = sizeof(struct sockaddr_in6);
#else
	sockaddrlen = sizeof(struct sockaddr_storage);
#endif


	if (getnameinfo(sockaddr,
		sockaddrlen,
		address,
		addrlen,
		NULL,
		0,
		NI_NUMERICHOST) != 0) address = NULL;

	return address;
}

//char *ip6tos(u_long in)
//{
//	static char output[IPTOSBUFFERS][3 * 4 + 3 + 1];
//	static short which;
//	u_char *p;
//
//	p = (u_char *)&in;
//	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
//	sprintf_s(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
//	return output[which];
//}

void PacDevSet() {
	handle = (pcap_t **)malloc(sizeof(pcap_t*) * 1);

	pdata.reset();
	pdrdata.reset();
	pdatacheckf = 0b11111;

	hdata.reset();
	pdev.reset();

	char /**dev,*/ errbuf[PCAP_ERRBUF_SIZE];

	//dev = pcap_lookupdev(errbuf);
	TCHAR text[2048];

	char textc[8192];

	PIP_ADAPTER_ADDRESSES pAdapterAddresses, pAA;
	DWORD dwRet, dwSize;

#if _DEBUG
	if (DEVICE_LOG == 1) {
		sprintf_s(textc, "IPHlpApi---------------------------------------------------------\n");
		OutputDebugStringA(textc);
	}
#endif

	dwRet = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &dwSize);
	if (dwRet != ERROR_BUFFER_OVERFLOW) {
		exit(1);
	}

	pAdapterAddresses = (PIP_ADAPTER_ADDRESSES)malloc(dwSize);
	if (pAdapterAddresses == NULL) {
		exit(1);
	}

	dwRet = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX,
		NULL, pAdapterAddresses, &dwSize);
	if (dwRet != ERROR_SUCCESS) {
		exit(1);
	}

	int adptcount = 0;
	
	for (pAA = pAdapterAddresses; pAA->Next != NULL; pAA = pAA->Next) {
		adptcount++;
		char szAdapterName[BUFSIZ];
		int len;


		len = WideCharToMultiByte(CP_ACP, 0, pAA->FriendlyName, wcslen(pAA->FriendlyName),
			szAdapterName, sizeof(szAdapterName), NULL, NULL);

		for (int i = 0; i < BUFSIZ; i++) {
			if (szAdapterName[i] == -52) {
				szAdapterName[i] = 0;
				break;
			}
		}

#if _DEBUG
		if (DEVICE_LOG == 1) {
			sprintf_s(textc, "%s", szAdapterName);

			sprintf_s(textc, "friendly name: %s\n", szAdapterName);
			OutputDebugStringA(textc);
			sprintf_s(textc, "ifindex: %u\n", pAA->IfIndex);
			OutputDebugStringA(textc);

			sprintf_s(textc, "adapter name: %s\n", pAA->AdapterName);
			OutputDebugStringA(textc);
		}
#endif

		pdev.jenerate(-1, pAA->AdapterName, pAA->FriendlyName);

		if (pAA->FirstUnicastAddress != NULL) {

#if _DEBUG
			if (DEVICE_LOG == 1) {
				sprintf_s(textc, "Unicast\n");
				OutputDebugStringA(textc);
			}
#endif

			for (PIP_ADAPTER_UNICAST_ADDRESS adr = pAA->FirstUnicastAddress; adr != NULL; adr = adr->Next) {
				char ip6str[128];
				int ipbuff[8];
				char ipbuffc[8][5];
				int flagbuffcount[2];
				int ipsepcount;

				if (adr == NULL)break;
				switch (adr->Address.lpSockaddr->sa_family)
				{
				case AF_INET:
					inet_ntop(adr->Address.lpSockaddr->sa_family, &((struct sockaddr_in *)adr->Address.lpSockaddr)->sin_addr, ip6str, sizeof(ip6str));

#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(textc, "address: %s\n", ip6str);
						OutputDebugStringA(textc);
					}
#endif

					if (ip6str != NULL) {
						sscanf_s(ip6str, "%d.%d.%d.%d", &ipbuff[0], &ipbuff[1], &ipbuff[2], &ipbuff[3]);
						ipbuff[4] = 0;
						ipbuff[5] = 0;
						ipbuff[6] = 0;
						ipbuff[7] = 0;
						hdata.jenerate(adptcount, ipbuff, 4, pAA->AdapterName);
					}
					break;
				case AF_INET6:
					inet_ntop(adr->Address.lpSockaddr->sa_family, &((struct sockaddr_in6 *)adr->Address.lpSockaddr)->sin6_addr, ip6str, sizeof(ip6str));

#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(textc, "address: %s\n", ip6str);
						OutputDebugStringA(textc);
					}
#endif
					if (ip6str != NULL) {
						flagbuffcount[0] = 0;
						flagbuffcount[1] = 0;
						ipsepcount = 0;

						for (int i = 0; i < 128; i++) {
							if (ip6str[i] == ':') {
								ipsepcount++;
							}
							if (ip6str[i] == 0) break;
						}

						for (int i = 0; i < 128; i++) {
							if (ip6str[i] == '%' || ip6str[i] == 'Ì') {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
								break;
							}
							else if (ip6str[i] == 0) {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
								break;
							}
							else if (ip6str[i] == ':') {
								if (flagbuffcount[1] == 0) {
									for (int j = 0; j < 8 - ipsepcount; j++) {
										ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = '0';
										ipbuffc[flagbuffcount[0]][flagbuffcount[1] + 1] = 0;
										flagbuffcount[0]++;
									}
								}
								else {
									ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
									flagbuffcount[0]++;
								}
								flagbuffcount[1] = 0;
							}
							else {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = ip6str[i];
								flagbuffcount[1]++;
							}

						}
#if _DEBUG
						if (DEVICE_LOG == 1) {
							sprintf_s(textc, "(%s:%s:%s:%s:%s:%s:%s:%s)\n", ipbuffc[0], ipbuffc[1], ipbuffc[2], ipbuffc[3], ipbuffc[4], ipbuffc[5], ipbuffc[6], ipbuffc[7]);
							OutputDebugStringA(textc);
						}
#endif
						for (int i = 0; i < 8; i++) {
							sscanf_s(ipbuffc[i], "%x", &ipbuff[i]);
						}
						hdata.jenerate(adptcount, ipbuff, 6, pAA->AdapterName);
					}
					break;
				default:
					break;
				}
			}
		}

		if (pAA->FirstAnycastAddress != NULL) {
#if _DEBUG
			if (DEVICE_LOG == 1) {
				sprintf_s(textc, "Anycast\n");
				OutputDebugStringA(textc);
			}
#endif
			for (PIP_ADAPTER_ANYCAST_ADDRESS adr = pAA->FirstAnycastAddress; adr != NULL; adr = adr->Next) {
				char ip6str[128];
				int ipbuff[8];
				char ipbuffc[8][5];
				int flagbuffcount[2];
				int ipsepcount;

				if (adr == NULL)break;
				switch (adr->Address.lpSockaddr->sa_family)
				{
				case AF_INET:
					inet_ntop(adr->Address.lpSockaddr->sa_family, &((struct sockaddr_in *)adr->Address.lpSockaddr)->sin_addr, ip6str, sizeof(ip6str));

#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(textc, "address: %s\n", ip6str);
						OutputDebugStringA(textc);
					}
#endif

					if (ip6str != NULL) {
						sscanf_s(ip6str, "%d.%d.%d.%d", &ipbuff[0], &ipbuff[1], &ipbuff[2], &ipbuff[3]);
						ipbuff[4] = 0;
						ipbuff[5] = 0;
						ipbuff[6] = 0;
						ipbuff[7] = 0;
						hdata.jenerate(adptcount, ipbuff, 4, pAA->AdapterName);
					}
					break;
				case AF_INET6:
					inet_ntop(adr->Address.lpSockaddr->sa_family, &((struct sockaddr_in6 *)adr->Address.lpSockaddr)->sin6_addr, ip6str, sizeof(ip6str));
#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(textc, "address: %s\n", ip6str);
						OutputDebugStringA(textc);
					}
#endif

					if (ip6str != NULL) {
						flagbuffcount[0] = 0;
						flagbuffcount[1] = 0;
						ipsepcount = 0;

						for (int i = 0; i < 128; i++) {
							if (ip6str[i] == ':') {
								ipsepcount++;
							}
							if (ip6str[i] == 0) break;
						}

						for (int i = 0; i < 128; i++) {
							if (ip6str[i] == '%' || ip6str[i] == 'Ì') {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
								break;
							}
							else if (ip6str[i] == 0) {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
								break;
							}
							else if (ip6str[i] == ':') {
								if (flagbuffcount[1] == 0) {
									for (int j = 0; j < 8 - ipsepcount; j++) {
										ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = '0';
										ipbuffc[flagbuffcount[0]][flagbuffcount[1] + 1] = 0;
										flagbuffcount[0]++;
									}
								}
								else {
									ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
									flagbuffcount[0]++;
								}
								flagbuffcount[1] = 0;
							}
							else {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = ip6str[i];
								flagbuffcount[1]++;
							}

						}
#if _DEBUG
						if (DEVICE_LOG == 1) {
							sprintf_s(textc, "(%s:%s:%s:%s:%s:%s:%s:%s)\n", ipbuffc[0], ipbuffc[1], ipbuffc[2], ipbuffc[3], ipbuffc[4], ipbuffc[5], ipbuffc[6], ipbuffc[7]);
							OutputDebugStringA(textc);
						}
#endif

						for (int i = 0; i < 8; i++) {
							sscanf_s(ipbuffc[i], "%x", &ipbuff[i]);
						}
						hdata.jenerate(adptcount, ipbuff, 6, pAA->AdapterName);
					}
					break;
				default:
					break;
				}
			}
		}

		if (pAA->FirstMulticastAddress != NULL) {
#if _DEBUG
			if (DEVICE_LOG == 1) {
				sprintf_s(textc, "Multicast\n");
				OutputDebugStringA(textc);
			}
#endif
			for (PIP_ADAPTER_MULTICAST_ADDRESS adr = pAA->FirstMulticastAddress; adr != NULL; adr = adr->Next) {
				char ip6str[128];
				int ipbuff[8];
				char ipbuffc[8][5];
				int flagbuffcount[2];
				int ipsepcount;

				if (adr == NULL)break;
				switch (adr->Address.lpSockaddr->sa_family)
				{
				case AF_INET:
					inet_ntop(adr->Address.lpSockaddr->sa_family, &((struct sockaddr_in *)adr->Address.lpSockaddr)->sin_addr, ip6str, sizeof(ip6str));

#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(textc, "address: %s\n", ip6str);
						OutputDebugStringA(textc);
					}
#endif

					if (ip6str != NULL) {
						sscanf_s(ip6str, "%d.%d.%d.%d", &ipbuff[0], &ipbuff[1], &ipbuff[2], &ipbuff[3]);
						ipbuff[4] = 0;
						ipbuff[5] = 0;
						ipbuff[6] = 0;
						ipbuff[7] = 0;
						hdata.jenerate(adptcount, ipbuff, 4, pAA->AdapterName);
					}

					break;
				case AF_INET6:
					inet_ntop(adr->Address.lpSockaddr->sa_family, &((struct sockaddr_in6 *)adr->Address.lpSockaddr)->sin6_addr, ip6str, sizeof(ip6str));

#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(textc, "address: %s\n", ip6str);
						OutputDebugStringA(textc);
					}
#endif

					if (ip6str != NULL) {
						flagbuffcount[0] = 0;
						flagbuffcount[1] = 0;
						ipsepcount = 0;

						for (int i = 0; i < 128; i++) {
							if (ip6str[i] == ':') {
								ipsepcount++;
							}
							if (ip6str[i] == 0) break;
						}

						for (int i = 0; i < 128; i++) {
							if (ip6str[i] == '%' || ip6str[i] == 'Ì') {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
								break;
							}
							else if (ip6str[i] == 0) {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
								break;
							}
							else if (ip6str[i] == ':') {
								if (flagbuffcount[1] == 0) {
									for (int j = 0; j < 8 - ipsepcount; j++) {
										ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = '0';
										ipbuffc[flagbuffcount[0]][flagbuffcount[1] + 1] = 0;
										flagbuffcount[0]++;
									}
								}
								else {
									ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
									flagbuffcount[0]++;
								}
								flagbuffcount[1] = 0;
							}
							else {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = ip6str[i];
								flagbuffcount[1]++;
							}

						}
#if _DEBUG
						if (DEVICE_LOG == 1) {
							sprintf_s(textc, "(%s:%s:%s:%s:%s:%s:%s:%s)\n", ipbuffc[0], ipbuffc[1], ipbuffc[2], ipbuffc[3], ipbuffc[4], ipbuffc[5], ipbuffc[6], ipbuffc[7]);
							OutputDebugStringA(textc);
						}
#endif

						for (int i = 0; i < 8; i++) {
							sscanf_s(ipbuffc[i], "%x", &ipbuff[i]);
						}
						hdata.jenerate(adptcount, ipbuff, 6, pAA->AdapterName);
					}
					break;
				default:
					break;
				}
			}
		}

		if (pAA->FirstDnsServerAddress != NULL) {

#if _DEBUG
			if (DEVICE_LOG == 1) {
				sprintf_s(textc, "DnsServer\n");
				OutputDebugStringA(textc);
			}
#endif
			for (PIP_ADAPTER_DNS_SERVER_ADDRESS adr = pAA->FirstDnsServerAddress; adr != NULL; adr = adr->Next) {
				char ip6str[128];
				int ipbuff[8];
				char ipbuffc[8][5];
				int flagbuffcount[2];
				int ipsepcount;

				if (adr == NULL)break;
				switch (adr->Address.lpSockaddr->sa_family)
				{
				case AF_INET:
					inet_ntop(adr->Address.lpSockaddr->sa_family, &((struct sockaddr_in *)adr->Address.lpSockaddr)->sin_addr, ip6str, sizeof(ip6str));
#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(textc, "address: %s\n", ip6str);
						OutputDebugStringA(textc);
					}
#endif

					if (ip6str != NULL) {
						sscanf_s(ip6str, "%d.%d.%d.%d", &ipbuff[0], &ipbuff[1], &ipbuff[2], &ipbuff[3]);
						ipbuff[4] = 0;
						ipbuff[5] = 0;
						ipbuff[6] = 0;
						ipbuff[7] = 0;
						hdata.jenerate(adptcount, ipbuff, 4, pAA->AdapterName);
					}
					break;

				case AF_INET6:
					inet_ntop(adr->Address.lpSockaddr->sa_family, &((struct sockaddr_in6 *)adr->Address.lpSockaddr)->sin6_addr, ip6str, sizeof(ip6str));
					
#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(textc, "address: %s\n", ip6str);
						OutputDebugStringA(textc);
					}
#endif

					if (ip6str != NULL) {
						flagbuffcount[0] = 0;
						flagbuffcount[1] = 0;
						ipsepcount = 0;

						for (int i = 0; i < 128; i++) {
							if (ip6str[i] == ':') {
								ipsepcount++;
							}
							if (ip6str[i] == 0) break;
						}

						for (int i = 0; i < 128; i++) {
							if (ip6str[i] == '%' || ip6str[i] == 'Ì') {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
								break;
							}
							else if (ip6str[i] == 0) {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
								break;
							}
							else if (ip6str[i] == ':') {
								if (flagbuffcount[1] == 0) {
									for (int j = 0; j < 8 - ipsepcount; j++) {
										ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = '0';
										ipbuffc[flagbuffcount[0]][flagbuffcount[1] + 1] = 0;
										flagbuffcount[0]++;
									}
								}
								else {
									ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
									flagbuffcount[0]++;
								}
								flagbuffcount[1] = 0;
							}
							else {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = ip6str[i];
								flagbuffcount[1]++;
							}

						}

#if _DEBUG
						if (DEVICE_LOG == 1) {
							sprintf_s(textc, "(%s:%s:%s:%s:%s:%s:%s:%s)\n", ipbuffc[0], ipbuffc[1], ipbuffc[2], ipbuffc[3], ipbuffc[4], ipbuffc[5], ipbuffc[6], ipbuffc[7]);
							OutputDebugStringA(textc);
						}
#endif

						for (int i = 0; i < 8; i++) {
							sscanf_s(ipbuffc[i], "%x", &ipbuff[i]);
						}
						hdata.jenerate(adptcount, ipbuff, 6, pAA->AdapterName);
					}
					break;
				default:
					break;
				}
			}
		}



#if _DEBUG
		if (DEVICE_LOG == 1) {
			sprintf_s(textc, "\n");
			OutputDebugStringA(textc);
		}
#endif

	}



#if _DEBUG
	if (DEVICE_LOG == 1) {
		sprintf_s(textc, "Winpcap---------------------------------------------------------\n");
		OutputDebugStringA(textc);
	}
#endif

	////////////////////////////////////////////////////////////////////////////////////////////////
#if _DEBUG
	if (DEVICE_LOG == 1) {
		sprintf_s(textc, "winpcap version %s\n", pcap_lib_version());
		OutputDebugStringA(textc);
	}
#endif
	do {
		int errorcheck = pcap_findalldevs(&alldevs, errbuf);

		if (errorcheck == -1) {
			wsprintf((LPWSTR)text, TEXT("pcap_findalldevs‚É‚ÄƒGƒ‰[‚ª”­¶‚µ‚Ü‚µ‚½: %s\n"), errbuf);

			OutputDebugString(text);
			exit(1);
		}
	} while (alldevs == NULL);

	for (d = alldevs; d->next != NULL; d = d->next) {

		sprintf_s(textc, "%d. %s\n", ++handlemaxi, d->name);
#if _DEBUG
			if (DEVICE_LOG == 1) {
			OutputDebugStringA(textc);
		}
#endif

		char *namebuff; 
		int namesizebuf = strlen(d->name) /*/ sizeof(char)*/;
		namebuff = (char*)malloc(sizeof(char) * (namesizebuf));

		for (int i = 0; i < namesizebuf; i++) {
			namebuff[i] = d->name[i];
		}
		
		int moveposbuf = -1;

		for (int i = 0;i < namesizebuf;i++) {
			if (namebuff[i] == '{') {
				moveposbuf = 0;
			}
			if (moveposbuf >= 0) {
				namebuff[moveposbuf] = namebuff[i];
				moveposbuf++;
			}
			if (i == namesizebuf - 1) namebuff[moveposbuf] = 0;
		}

#if _DEBUG
		if (DEVICE_LOG == 1) {
			sprintf_s(textc, "(%s)\n", namebuff);
			OutputDebugStringA(textc);
		}
#endif

		handle = (pcap_t **)realloc(handle, sizeof(pcap_t *) * (handlemaxi));

		if ((handle[handlemaxi - 1] = pcap_open_live(d->name, 65536, 1, 1000, errbuf)) == NULL){
#if _DEBUG
			sprintf_s(textc, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
			OutputDebugStringA(textc);
#endif
			//pcap_freealldevs(alldevs);
			continue;
		}

		for (int i = 0; i < pdev.usecount; i++) {
			if (strcmp(pdev.data[i].adaptername, namebuff) == 0) {
				pdev.data[i].adapterNo = handlemaxi - 1;
				break;
			}
		}

#if _DEBUG
		if (DEVICE_LOG == 1) {
			sprintf_s(textc, "%d\n", pcap_snapshot(handle[handlemaxi - 1]));

			OutputDebugStringA(textc);
		}
#endif

		if (d->description) {
			//sockaddr_in *sa = (sockaddr_in*)d->addresses->addr;
			// IP address (unsigned long)
			//unsigned long addressbuff = sa->sin_addr.s_addr;
			//sprintf_s(textc, "(%s)\n(%lu)\n", d->description, addressbuff);

#if _DEBUG
			if (DEVICE_LOG == 1) {
				sprintf_s(textc, "(%s)\n", d->description);

				OutputDebugStringA(textc);
			}
#endif

			pcap_addr_t *dev_addr;
			

			
			for (dev_addr = d->addresses; dev_addr != NULL; dev_addr = dev_addr->next) {
				if (dev_addr == NULL) break;
				char ip6str[128];
				int ipbuff[8];
				char ipbuffc[8][5];
				int flagbuffcount[2];
				int ipsepcount;


				switch (dev_addr->addr->sa_family)
				{
				case AF_INET:

#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(textc, "address family name: AF_INET\n");
						OutputDebugStringA(textc);
					}
#endif

					if (dev_addr->addr) {
						sprintf_s(ip6str, "%s", iptos(((struct sockaddr_in *)dev_addr->addr)->sin_addr.s_addr));

#if _DEBUG
						if (DEVICE_LOG == 1) {
							sprintf_s(textc, "address: %s\n", ip6str);
							OutputDebugStringA(textc);
						}
#endif

						if (ip6str != NULL) {
							sscanf_s(ip6str, "%d.%d.%d.%d", &ipbuff[0], &ipbuff[1], &ipbuff[2], &ipbuff[3]);
						}

						sscanf_s(ip6str, "%d.%d.%d.%d", &ipbuff[0], &ipbuff[1], &ipbuff[2], &ipbuff[3]);
					}

#if _DEBUG
					if (DEVICE_LOG == 1) {
						if (dev_addr->netmask) {
							sprintf_s(textc, "netmask: %s\n", iptos(((struct sockaddr_in *)dev_addr->netmask)->sin_addr.s_addr));
							OutputDebugStringA(textc);
						}
						if (dev_addr->broadaddr) {
							sprintf_s(textc, "broadcast address: %s\n", iptos(((struct sockaddr_in *)dev_addr->broadaddr)->sin_addr.s_addr));
							OutputDebugStringA(textc);
						}
						if (dev_addr->dstaddr) {
							sprintf_s(textc, "destination address: %s\n", iptos(((struct sockaddr_in *)dev_addr->dstaddr)->sin_addr.s_addr));
							OutputDebugStringA(textc);
						}
					}
#endif

					break;

				case AF_INET6:

#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(textc, "address family name: AF_INET6\n");
						OutputDebugStringA(textc);
					}
#endif

#ifndef __MINGW32__ 
					if (dev_addr->addr) {

#if _DEBUG
						if (DEVICE_LOG == 1) {
							sprintf_s(textc, "address: %s\n", ip6tos(dev_addr->addr, ip6str, sizeof(ip6str)));
							OutputDebugStringA(textc);
						}
#endif

						flagbuffcount[0] = 0;
						flagbuffcount[1] = 0;
						ipsepcount = 0;

						for (int i = 0; i < 128; i++) {
							if (ip6str[i] == ':') {
								ipsepcount++;
							}
							if (ip6str[i] == 0) break;
						}

						for (int i = 0; i < 128; i++) {
							if (ip6str[i] == '%' || ip6str[i] == 'Ì') {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
								break;
							}
							else if (ip6str[i] == 0) {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
								break;
							}
							else if (ip6str[i] == ':') {
								if (flagbuffcount[1] == 0) {
									for (int j = 0; j < 8 - ipsepcount; j++) {
										ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = '0';
										ipbuffc[flagbuffcount[0]][flagbuffcount[1] + 1] = 0;
										flagbuffcount[0]++;
									}
								}
								else {
									ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = 0;
									flagbuffcount[0]++;
								}
								flagbuffcount[1] = 0;
							}
							else  {
								ipbuffc[flagbuffcount[0]][flagbuffcount[1]] = ip6str[i];
								flagbuffcount[1]++;
							}

						}

#if _DEBUG
						if (DEVICE_LOG == 1) {
							sprintf_s(textc, "(%s:%s:%s:%s:%s:%s:%s:%s)\n", ipbuffc[0], ipbuffc[1], ipbuffc[2], ipbuffc[3], ipbuffc[4], ipbuffc[5], ipbuffc[6], ipbuffc[7]);
							OutputDebugStringA(textc);
						}
#endif

						for (int i = 0; i < 8; i++) {
							sscanf_s(ipbuffc[i], "%x", &ipbuff[i]);
						}

					}

#endif
					break;

				default:

#if _DEBUG
					if (DEVICE_LOG == 1) {
						printf("address family name: unknown\n");
						OutputDebugStringA(textc);
					}
#endif

					break;
				}


			}
			
		}
		else {
			wsprintf((LPWSTR)text, TEXT("(No description available)\n"));

			OutputDebugString(text);
		}


#if _DEBUG
		if (DEVICE_LOG == 1) {
			sprintf_s(textc, "\n");
			OutputDebugStringA(textc);
		}
#endif
		free(namebuff);
	}
	
	for (int i = 0; i < hdata.usecount; i++) {
		//hdata.data[i].adapterNo = -1;
		
		for (int j = 0; j < pdev.usecount; j++) {
			if (strcmp(hdata.data[i].adaptername, pdev.data[j].adaptername) == 0 && IpMatchCheck_NonView(hdata.data[i].ip, hdata.data[i].version) > 0) {
				hdata.data[i].adapterNo = pdev.data[j].adapterNo;
				pdev.data[j].hostipid[pdev.data[j].hostipmax] = i;
				pdev.data[j].hostipmax++;
				break;
			}
		}
	}

	deviceselected = -1;

}

void PacDevClose() {
	TCHAR text[2048];

#if _DEBUG
	char textc[8192];
#endif

	if (handlemaxi == 0) {

		wsprintf((LPWSTR)text, TEXT("\nNo interfaces found! Make sure WinPcap is installed.\n"));

		OutputDebugString(text);


	}
	else {
		for (int j = 0; j < handlemaxi; j++) {
			if (handle[j] != NULL) {
				pcap_close(handle[j]);
			}
			handle[j] = NULL;

		}

		pcap_freealldevs(alldevs);

	}

	deviceselected = -1;

	handlei = 0;
	handlemaxi = 0;
	pdata.datafree();

	//free(d);
	if (handle != NULL) {
		free(handle);
	}
	handle = NULL;
	pdev.datafree();

}

void PacCapture() {

	char ptextc[8192];
	int res;
	
	FILE *fp;

	//PacketData data;
	while (quitf == 0 && resetf == 0)
	{

		while (setting.stopf == 1) {
			if (resetf == 1 || quitf == 1)break;
		}

		lockf = 1;

		if (resetf == 0) {
			if (handlemaxi <= doi) {
				handlei = 0;
			}
			else if (res = pcap_next_ex(handle[doi], &pkt_header, &pkt_data) >= 0) {
				if (pkt_data != NULL) {
					analysis.time.UpdateTime(0);
					pdata.jenerate(doi + 1, pkt_data, pkt_header->caplen, analysis.time.GetCaptureTime(0), analysis.time.GetCaptureTime(1));

#if _DEBUG
					if (DEVICE_LOG == 1) {
						sprintf_s(ptextc, "%d\n", pdata.packcount - 1);
						OutputDebugStringA(ptextc);
					}
#endif

					if (pdata.newdatacount > 0) {
						sprintf_s(ptextc, "%d.%d.%d.%d.%d.%d.%d.%d\n", pdata.newdata[pdata.newdatacount - 1].fromip[0], pdata.newdata[pdata.newdatacount - 1].fromip[1], pdata.newdata[pdata.newdatacount - 1].fromip[2], pdata.newdata[pdata.newdatacount - 1].fromip[3],
							pdata.newdata[pdata.newdatacount - 1].fromip[4], pdata.newdata[pdata.newdatacount - 1].fromip[5], pdata.newdata[pdata.newdatacount - 1].fromip[6], pdata.newdata[pdata.newdatacount - 1].fromip[7]);


						sprintf_s(ptextc, "%d.%d.%d.%d.%d.%d.%d.%d\n", pdata.newdata[pdata.newdatacount - 1].atip[0], pdata.newdata[pdata.newdatacount - 1].atip[1], pdata.newdata[pdata.newdatacount - 1].atip[2], pdata.newdata[pdata.newdatacount - 1].atip[3],
							pdata.newdata[pdata.newdatacount - 1].atip[4], pdata.newdata[pdata.newdatacount - 1].atip[5], pdata.newdata[pdata.newdatacount - 1].atip[6], pdata.newdata[pdata.newdatacount - 1].atip[7]);

						sprintf_s(ptextc, "\n");
					}

				}

			}
			else if (res == 0) {
				PacDevClose();
				PacDevSet();
				for (int i = 0; i < pdev.usecount && quitf == 0 && resetf == 0; i++) {
					int sizebuff[2];

					sizebuff[0] = sizeof(doadpname) / sizeof(CHAR);
					sizebuff[1] = sizeof(pdev.data[i].adaptername) / sizeof(CHAR);
					if (sizebuff[0] == sizebuff[1]) {
						bool matchf = 1;
						for (int j = 0; j < sizebuff[0]; j++) {
							if (doadpname[j] != pdev.data[i].adaptername[j]) matchf = 0;
						}
						if (matchf == 1) {
							selecteddevice = i;

							doi = pdev.data[selecteddevice].adapterNo;
							doadpname = pdev.data[selecteddevice].adaptername;
						}
						else {
							resetbutton();
						}
					}
					else {
						resetbutton();
					}
				}
			}

			
			lockf = 0;
		}
		
		if (pdatacheckf == 0b11111) {
			pdrdata.newdatacheck();
			for (int i = 0; i < pdata.newdatacount; i++) {
				pdrdata.jenerate(pdata.newdata[i]);
				pdata.newdata[i].checked = 0b11111;
			}

			pdata.newdatacheck();
			pdatacheckf = 0;
		}

	}

	threadfinf = true;

}



void PacCapture_Single() {

	char ptextc[8192];
	int res;
	int doi;
	FILE *fp;
	//PacketData data;

	doi = handlei;
	handlei++;

	if (handlemaxi <= doi) {
		handlei = 0;
	}
	else if (res = pcap_next_ex(handle[doi], &pkt_header, &pkt_data) >= 0) {
		if (res == 0) {
		}
		else {
			//pkt_header->ts.tv_sec;

#if _DEBUG
			if (DEVICE_LOG == 1) {
				sprintf_s(ptextc, "%d. %.6d len:%d\ndata:", doi + 1, pkt_header->ts.tv_usec, pkt_header->caplen);
				OutputDebugStringA(ptextc);
			}
#endif

			if (pkt_data != NULL) {
				for (int i = 0; i < pkt_header->caplen; i++) {
					sprintf_s(ptextc, "%x ", pkt_data[i]);
					OutputDebugStringA(ptextc);
				}
			}
			sprintf_s(ptextc, "\n");
			OutputDebugStringA(ptextc);

		}
	}

}

void PacTrash() {
	pdata.datafree();
	hdata.datafree();
}