#ifndef INCLUDED_ANALYSIS
#define INCLUDED_ANALYSIS

#include <sys/timeb.h>
#include "../main.h"

#include "packet.h"

#include "../debug_code/debug.h"

class Setting {
public:
	int viewdatalange;
	int viewdatalangeold;
	unsigned long long border;
	unsigned long long stime;
	unsigned long long etime;

	unsigned long long stimebef;
	unsigned long long etimebef;

	bool endtimechase;

	bool stopf;
};

class Timeclass {
private:
	struct timeb timebuffer;
	DWORD nowtime[2];
	unsigned long long Capturetime[2];
	unsigned long long stoptime[2];
	DWORD starttime[2];

	DWORD befortime;

	DWORD beforebuff;

public:
	void SetTime() {

		ftime(&timebuffer);

		nowtime[0] = timebuffer.time + (9 * 3600);
		nowtime[1] = timebuffer.millitm;

		Capturetime[0] = 0;
		Capturetime[1] = 0;

		stoptime[0] = 0;
		stoptime[1] = 0;

		befortime = 0;

		starttime[0] = nowtime[0];
		starttime[1] = nowtime[1];

		beforebuff = timeGetTime();

#if _DEBUG
		char textc[8192];
		if (TIME_LOG == 1) {
			sprintf_s(textc, "SetTime--------------------\n");
			sprintf_s(textc, "%sBeforetime: %u\n", textc, befortime);
			sprintf_s(textc, "%sCPTime: %u.%u\n", textc, Capturetime[0], Capturetime[1]);
			sprintf_s(textc, "%s---------------------------\n", textc);
			OutputDebugStringA(textc);

		}
#endif

	}

	DWORD GetNowTime(int flag) {
		return nowtime[flag];
	}

	DWORD GetStartTime(int flag) {
		return starttime[flag];
	}

	DWORD GetBeforTime() {
		return befortime;
	}

	DWORD GetDifferenceBeforeTime(DWORD unitsec) {
		return (Capturetime[0] / unitsec) - (befortime / unitsec);
	}

	void UpdateTime(bool stopf) {

		DWORD nowbuff = timeGetTime();

		ftime(&timebuffer);

		befortime = Capturetime[0];

		nowtime[0] = timebuffer.time + (9 * 3600);
		nowtime[1] = timebuffer.millitm;

		if (stopf == 0) {
			Capturetime[1] += (nowbuff - beforebuff);

			Capturetime[0] += Capturetime[1] / 1000;

			Capturetime[1] = Capturetime[1] % 1000;
		}
		else {
			stoptime[1] += (nowbuff - beforebuff);

			stoptime[0] += stoptime[1] / 1000;

			stoptime[1] = stoptime[1] % 1000;
		}

#if _DEBUG
		char textc[8192];
		if (TIME_LOG == 1) {
			sprintf_s(textc, "Beforetime: %u\n", befortime);
			sprintf_s(textc, "%sNowbuff: %u\n", textc, nowbuff);
			sprintf_s(textc, "%sCPTime: %u.%u\n", textc, Capturetime[0], Capturetime[1]);
			OutputDebugStringA(textc);

		}
#endif

		beforebuff = nowbuff;

	}

	unsigned long long GetCaptureTime(int flag) {
		return Capturetime[flag];
	}

	unsigned long long *GetCaptureTime() {
		return Capturetime;
	}

	unsigned long long GetMoveTime(int flag) {
		switch (flag) {
		case 0:
			return Capturetime[0] + stoptime[0] + ((Capturetime[1] + stoptime[1]) / 1000);
			break;

		case 1:
			return (Capturetime[1] + stoptime[1]) % 1000;
			break;
		}
	}

};

class Protocolclass {
private:
	//char *protocol;
	int id;
	unsigned long long count;
public:
	void Set(/*char *name,*/ int id) {
		//protocol = name;
		this->id = id;
		count = 0;
	}
	void AddCount(int add) {
		count += add;
	}

	void Reset() {
		//protocol = NULL;
		count = 0;
	}

	/*char* GetName() {
		return protocol;
	}*/

	int GetId() {
		return id;
	}

	unsigned long long GetCount() {
		return count;
	}

};

class IPclass {
private:
	int ip[8];
	short version;
	unsigned long long count;

public:
	void Set(int *ip, short version) {
		this->ip[0] = ip[0];
		this->ip[1] = ip[1];
		this->ip[2] = ip[2];
		this->ip[3] = ip[3];
		this->ip[4] = ip[4];
		this->ip[5] = ip[5];
		this->ip[6] = ip[6];
		this->ip[7] = ip[7];
		this->version = version;
	}
	void AddCount(int add) {
		count += add;
	}

	void Reset() {
		ip[0] = NULL;
		ip[1] = NULL;
		ip[2] = NULL;
		ip[3] = NULL;
		ip[4] = NULL;
		ip[5] = NULL;
		ip[6] = NULL;
		ip[7] = NULL;
		count = 0;
	}

	int* GetIP() {
		return ip;
	}

	short GetVersion() {
		return version;
	}

	unsigned long long GetCount() {
		return count;
	}
};

class Portclass {
private:
	int port;
	unsigned long long count;

public:
	void Set(int port) {
		this->port = port;
	}
	void AddCount(int add) {
		count += add;
	}

	void Reset() {
		port = NULL;
		count = 0;
	}

	int GetPort() {
		return port;
	}

	unsigned long long GetCount() {
		return count;
	}
};

class Dataclass {
public:
	unsigned long long time;
	unsigned long long pcount;
	unsigned long long size;
	unsigned long long max;
	unsigned long long min;
	double ave;

	int *ip;
	short version;
	unsigned long long ipmaxcount;

	int port;
	unsigned long long portmaxcount;

	Protocolclass *prtdata;
	unsigned int prtcount;
	IPclass *ipdata;
	unsigned int ipcount;
	Portclass *portdata;
	unsigned int portcount;

	void Set(DWORD time) {
		this->time = time;
		size = 0;
		max = 0;
		min = 65535;
		ave = 0;

		prtdata = (Protocolclass*)malloc(sizeof(Protocolclass) * 1);
		prtdata[0].Reset();
		ipdata = (IPclass*)malloc(sizeof(IPclass) * 1);
		ipdata[0].Reset();
		portdata = (Portclass*)malloc(sizeof(Portclass) * 1);
		portdata[0].Reset();

		pcount = 0;
		prtcount = 0;
		ipcount = 0;
		portcount = 0;

		ipmaxcount = 0;

		port = NULL;
		portmaxcount = 0;

		version = 0;
	}

	void Reset(unsigned long long time) {
		this->time = time;
		size = 0;
		max = 0;
		min = 65535;
		ave = 0;

		pcount = 0;
		prtcount = 0;
		ipcount = 0;
		portcount = 0;

		ipmaxcount = 0;

		port = NULL;
		portmaxcount = 0;

		version = 0;
	}

	void Adddata(Packetstat pdata) {
		pcount++;

		size += pdata.pacsize;

		if (max < pdata.pacsize) max = pdata.pacsize;
		if (pdata.pacsize != 0 && min > pdata.pacsize) min = pdata.pacsize;

		ave = ((double)size / pcount);

		int janaratef = 0;

		for (int i = 0; i < prtcount; i++) {
			if (pdata.protocol == prtdata[i].GetId()) {
				janaratef = 1;
				prtdata[i].AddCount(1);
				break;
			}
		}

		if (janaratef == 0) {
			prtdata = (Protocolclass*)realloc(prtdata, sizeof(Protocolclass) * (prtcount + 1));
			prtdata[prtcount].Reset();
			prtdata[prtcount].Set(/*pdata.cprotocol,*/ pdata.protocol);
			prtdata[prtcount].AddCount(1);
			prtcount++;
		}

		janaratef = 0;

		for (int i = 0; i < ipcount; i++) {
			if (!IpMatchCheck(pdata.atip, ipdata[i].GetIP())) {
				janaratef |= 0b01;
				ipdata[i].AddCount(1);
				if (ipmaxcount < ipdata[i].GetCount()) {
					ipmaxcount = ipdata[i].GetCount();
					ip = ipdata[i].GetIP();
					version = ipdata[i].GetVersion();
				}
			}
			if (!IpMatchCheck(pdata.fromip, ipdata[i].GetIP())) {
				janaratef |= 0b10;
				ipdata[i].AddCount(1);
				if (ipmaxcount < ipdata[i].GetCount()) {
					ipmaxcount = ipdata[i].GetCount();
					ip = ipdata[i].GetIP();
					version = ipdata[i].GetVersion();
				}
			}

			if (janaratef >= 0b11) break;
		}

		if ((janaratef & 0b01) == 0) {
			ipdata = (IPclass*)realloc(ipdata, sizeof(IPclass) * (ipcount + 1));
			ipdata[ipcount].Reset();
			ipdata[ipcount].Set(pdata.atip, pdata.version);
			ipdata[ipcount].AddCount(1);

			if (ipmaxcount < ipdata[ipcount].GetCount()) {
				ipmaxcount = ipdata[ipcount].GetCount();
				ip = ipdata[ipcount].GetIP();
			}

			ipcount++;

		}

		if ((janaratef & 0b10) == 0) {
			ipdata = (IPclass*)realloc(ipdata, sizeof(IPclass) * (ipcount + 1));
			ipdata[ipcount].Reset();
			ipdata[ipcount].Set(pdata.fromip, pdata.version);
			ipdata[ipcount].AddCount(1);

			if (ipmaxcount < ipdata[ipcount].GetCount()) {
				ipmaxcount = ipdata[ipcount].GetCount();
				ip = ipdata[ipcount].GetIP();
			}

			ipcount++;
		}

		janaratef = 0;

		for (int i = 0; i < portcount; i++) {
			if (pdata.atport == portdata[i].GetPort()) {
				janaratef += 1;
				portdata[i].AddCount(1);

				if (portmaxcount < portdata[i].GetCount()) {
					portmaxcount = portdata[i].GetCount();
					port = portdata[i].GetPort();
				}
			}
			if (pdata.fromport == portdata[i].GetPort()) {
				janaratef += 2;
				portdata[i].AddCount(1);

				if (portmaxcount < portdata[i].GetCount()) {
					portmaxcount = portdata[i].GetCount();
					port = portdata[i].GetPort();
				}
			}
			if (janaratef >= 3) break;
		}

		if (janaratef % 2 == 0) {
			portdata = (Portclass*)realloc(portdata, sizeof(Portclass) * (portcount + 1));
			portdata[portcount].Reset();
			portdata[portcount].Set(pdata.atport);
			portdata[portcount].AddCount(1);

			if (portmaxcount < portdata[portcount].GetCount()) {
				portmaxcount = portdata[portcount].GetCount();
				port = portdata[portcount].GetPort();
			}

			portcount++;
		}

		if (janaratef / 2 == 0) {
			portdata = (Portclass*)realloc(portdata, sizeof(Portclass) * (portcount + 1));
			portdata[portcount].Reset();
			portdata[portcount].Set(pdata.fromport);
			portdata[portcount].AddCount(1);

			if (portmaxcount < portdata[portcount].GetCount()) {
				portmaxcount = portdata[portcount].GetCount();
				port = portdata[portcount].GetPort();
			}

			portcount++;
		}

	}

	void Adddata(Dataclass data) {
		size += data.size;
		if (max < data.max) max = data.max;
		if (data.min != 0 && min > data.min) min = data.min;
		pcount += data.pcount;

		bool janaratef;

		for (int i = 0; i < data.prtcount; i++) {
			janaratef = 0;
			for (int j = 0; j < prtcount; j++) {
				if (data.prtdata[i].GetId() == prtdata[j].GetId()) {
					prtdata[j].AddCount(data.prtdata[i].GetCount());
					janaratef = 1;
					break;
				}
			}
			if (janaratef == 0) {
				prtdata = (Protocolclass*)realloc(prtdata, sizeof(Protocolclass) * (prtcount + 1));
				prtdata[prtcount].Reset();
				prtdata[prtcount].Set(/*data.prtdata[i].GetName(),*/ data.prtdata[i].GetId());
				prtdata[prtcount].AddCount(data.prtdata[i].GetCount());
				prtcount++;
			}
			
		}

		

		for (int i = 0; i < data.ipcount; i++) {
			janaratef = 0;
			for (int j = 0; j < ipcount; j++) {
				if (!IpMatchCheck(data.ipdata[i].GetIP(), ipdata[j].GetIP())) {
					ipdata[j].AddCount(data.ipdata[i].GetCount());
					janaratef = 1;
					if (ipmaxcount < ipdata[j].GetCount()) {
						ipmaxcount = ipdata[j].GetCount();
						ip = ipdata[j].GetIP();
						version = ipdata[j].GetVersion();
					}
					break;
				}
			}
			if (janaratef == 0) {
				ipdata = (IPclass*)realloc(ipdata, sizeof(IPclass) * (ipcount + 1));
				ipdata[ipcount].Reset();
				ipdata[ipcount].Set(data.ipdata[i].GetIP(), data.ipdata[i].GetVersion());
				ipdata[ipcount].AddCount(data.ipdata[i].GetCount());

				if (ipmaxcount < ipdata[ipcount].GetCount()) {
					ipmaxcount = ipdata[ipcount].GetCount();
					ip = ipdata[ipcount].GetIP();
				}

				ipcount++;
			}

		}
		

		for (int i = 0; i < data.portcount; i++) {
			janaratef = 0;
			for (int j = 0; j < portcount; j++) {
				if (data.portdata[i].GetPort() == portdata[j].GetPort()) {
					portdata[j].AddCount(data.portdata[i].GetCount());

					if (portmaxcount < portdata[j].GetCount()) {
						portmaxcount = portdata[j].GetCount();
						port = portdata[j].GetPort();
					}
					break;
				}
			}
			if (janaratef == 0) {

				portdata = (Portclass*)realloc(portdata, sizeof(Portclass) * (portcount + 1));
				portdata[portcount].Reset();
				portdata[portcount].Set(data.portdata[i].GetPort());
				portdata[portcount].AddCount(data.portdata[i].GetCount());

				if (portmaxcount < portdata[portcount].GetCount()) {
					portmaxcount = portdata[portcount].GetCount();
					port = portdata[portcount].GetPort();
				}

				portcount++;
			}


		}

		ave = ((double)size / pcount) > 0 ? ((double)size / pcount) : 0;
	}

	void Free() {
		if(prtdata != NULL)free(prtdata);
		prtdata = NULL;
		if (ipdata != NULL)free(ipdata);
		ipdata = NULL;
		if (portdata != NULL)free(portdata);
		portdata = NULL;
	}

};

class Analysisclass {
public:
	Timeclass time;
	Dataclass *data;
	Dataclass alldata;
	Dataclass viewdata;
	unsigned int usecount;
	unsigned int viewsepcount;

	unsigned long long sepmaxsize;
	unsigned long long sepminsize;
	double sepavesize;

	unsigned long long viewsepmaxsize;
	unsigned long long viewsepminsize;
	double viewsepavesize;

	void Reset() {
		data = (Dataclass *)malloc(sizeof(Dataclass) * 1);
		usecount = 0;
		viewsepcount = 0;
		data[usecount].Set(0);

		alldata.Set(0);
		viewdata.Set(0);

		usecount++;
		viewsepcount++;

		time.SetTime();

		sepavesize = 0;
		sepmaxsize = 0;
		sepminsize = 18446744073709551615;

		viewsepavesize = 0;
		viewsepmaxsize = 0;
		viewsepminsize = 18446744073709551615;
	}

	void SetSection() {
		int checkcountbuff = ((time.GetCaptureTime(0) / 10) - (data[usecount - 1].time)) > 0 ? (time.GetCaptureTime(0) / 10) - (data[usecount - 1].time) : 1;

		for (int i = 0; i < checkcountbuff; i++) {
			data = (Dataclass*)realloc(data, sizeof(Dataclass) * (usecount + 1));

			data[usecount].Set(time.GetCaptureTime(0) / 10);
			if (usecount >= 1) {
				if (sepminsize > data[usecount - 1].size) {
					sepminsize = data[usecount - 1].size;
				}
				if (viewsepminsize > data[usecount - 1].size) {
					viewsepminsize = data[usecount - 1].size;
				}
				sepavesize = (usecount > 0 && ((double)alldata.size / usecount) > 0) ? (double)alldata.size / usecount : 0;
				viewsepavesize = (viewsepcount > 0 && ((double)viewdata.size / viewsepcount) > 0) ? (double)viewdata.size / viewsepcount : 0;
			}
			usecount++;
			viewsepcount++;
		}
	}

	void SetData(Packetstat pdata, int modef, Setting setting) {
		
		int timebuff = pdata.arrivaltime[0] / 10;

		data[timebuff].Adddata(pdata);
		alldata.Adddata(pdata);

		if (modef != 3 || setting.endtimechase == 1) {
			viewdata.Adddata(pdata);
		}

		if (sepmaxsize < data[timebuff].size) {
			sepmaxsize = data[timebuff].size;
		}

		if (modef != 3 || setting.endtimechase == 1) {
			if (viewsepmaxsize < data[timebuff].size) {
				viewsepmaxsize = data[timebuff].size;
			}
		}
		sepavesize = (usecount > 0 && ((double)alldata.size / usecount) > 0) ? (double)alldata.size / usecount : 0;
		if (modef != 3 || setting.endtimechase == 1) {
			viewsepavesize = (viewsepcount > 0 && ((double)viewdata.size / viewsepcount) > 0) ? (double)viewdata.size / viewsepcount : 0;
		}
	}

	void UpdataViewData(unsigned long long stime, unsigned long long etime) {
		viewdata.Reset(stime);
		viewsepcount = 0;
		int timeflag = 0;
		for (int i = 0; i < usecount; i++) {
			if (data[i].time >= stime / 10) {
				timeflag = 1;
			}
			if (timeflag == 1) {
				viewdata.Adddata(data[i]);
				viewsepcount++;
			}
			if (data[i].time >= etime) {
				break;
			}
		}
		viewsepavesize = (viewsepcount > 0 && ((double)viewdata.size / viewsepcount) > 0) ?(double)viewdata.size / viewsepcount : 0;
	}

	DWORD GetSectionTime() {
		return data[usecount - 1].time;
	}

	void Free() {
		if (data != NULL) {
			free(data);
		}
		data = NULL;
		usecount = 0;
		viewsepcount = 0;
	}

};


extern Analysisclass analysis;
extern Setting setting;

extern void SetAnalysis();

extern void ReSetAnalysis();

extern void Analysismain(int mode);

extern void Analysisdrmain(int mode);

extern void TrashAnalysis();

#endif
