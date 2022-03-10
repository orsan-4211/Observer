#ifndef INCLUDED_PACKET
#define INCLUDED_PACKET

#define PAC_DATA_LIMIT 1024

#include <pcap.h>

extern int deviceselected;
extern int selecteddevice;

extern bool IpMatchCheck(int *ip1, int *ip2);
extern bool IpMatchCheck_NonView(int *ip1, int version);

class HostIPData {
public:
	int ip[8];
	int version;
	int adapterNo;
	PCHAR adaptername;
};

class HostIPDataMaster {
public:
	HostIPData *data;
	unsigned int usecount;

	void reset();
	void jenerate(int adapterNo, int *ip, int version, PCHAR adaptername);
	void remove();
	void datafree();

};

class PcapDeviceData {
public:
	int adapterNo;
	PCHAR adaptername;
	PWCHAR FriendlyName;
	int hostipid[32];
	short hostipmax;
};

class PcapDeviceMaster {
public:
	PcapDeviceData *data;
	unsigned int usecount;

	void reset();
	void jenerate(int adapterNo, PCHAR adaptername, PWCHAR FriendlyName);
	void remove();
	void datafree();

};

//class PacketData {
//public:
//	const u_char *dat;
//};

class Packetstat {
public:
	int checked;

	int adapterNo;


	int version;
	//int protocoltype;
	int protocol;
	//char *cprotocol;
	//int headersize;
	int pacsize;
	int atip[8];
	//int atmac;
	int atport;
	int fromip[8];
	//int frommac;
	int fromport;

	unsigned long long arrivaltime[2];

	void PacketCheck(int adapterNo, const u_char *pkt_data, int size, unsigned long long *time);
	void PacketSet(Packetstat stat);
	char* Protocol();

	void DebugLogPrint() {
		char debptextc[8192];
		sprintf_s(debptextc, "version: %d\n", version);
		//sprintf_s(debptextc, "%sprotocoltype: %d\n", debptextc, protocoltype);
		//sprintf_s(debptextc, "%sprotocol: %s(%d)\n", debptextc, cprotocol, protocol);
		sprintf_s(debptextc, "%sprotocol: %d\n", debptextc, protocol);
		//sprintf_s(debptextc, "%sheadersize: %d\n", debptextc, headersize);
		sprintf_s(debptextc, "%spacsize: %d\n", debptextc, pacsize);
		if (version == 4) {
			sprintf_s(debptextc, "%satip: %d.%d.%d.%d\n", debptextc, atip[0], atip[1], atip[2], atip[3]);
		}
		else {
			sprintf_s(debptextc, "%satip: %x:%x:%x:%x:%x:%x:%x:%x\n", debptextc, atip[0], atip[1], atip[2], atip[3], atip[4], atip[5], atip[6], atip[7]);
		}
		//sprintf_s(debptextc, "%satmac: %d\n", debptextc, atmac);
		sprintf_s(debptextc, "%satport: %d\n", debptextc, atport);
		if (version == 4) {
			sprintf_s(debptextc, "%sfromip: %d.%d.%d.%d\n", debptextc, fromip[0], fromip[1], fromip[2], fromip[3]);
		}
		else {
			sprintf_s(debptextc, "%sfromip: %x:%x:%x:%x:%x:%x:%x:%x\n", debptextc, fromip[0], fromip[1], fromip[2], fromip[3], fromip[4], fromip[5], fromip[6], fromip[7]);
		}
		//sprintf_s(debptextc, "%sfrommac: %d\n", debptextc, frommac);
		sprintf_s(debptextc, "%sfromport: %d\n", debptextc, fromport);
		sprintf_s(debptextc, "%s\n", debptextc);
		OutputDebugStringA(debptextc);

	};
};

class PacketDataMasterclass {
public:
	/*PacketData *data;
	unsigned int usecount;*/
	unsigned long long packcount;


	int *removebuff;

	Packetstat *newdata;
	int newdatacount;
	
	void reset();
	void jenerate(int adapterNo, const u_char *pkt_data, int size, unsigned long long *time);
	void jenerate(int adapterNo, const u_char *pkt_data, int size, unsigned long long time, unsigned long long timems);
	void jenerate(Packetstat stat);
	void remove();

	void newdatacheck();

	void datafree();
};

extern int myipaddress[16][8];


extern void PacDevSet();
extern void PacDevClose();
extern void PacCapture();
extern void PacCapture_Single();
extern void PacTrash();

extern void DevSelect(int devcount);

extern PacketDataMasterclass pdata;
extern PacketDataMasterclass pdrdata;
extern int pdatacheckf;

extern PcapDeviceMaster pdev;

extern HostIPDataMaster hdata;

#endif
