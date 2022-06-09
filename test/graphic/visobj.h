#ifndef INCLUDED_VISOBJ
#define INCLUDED_VISOBJ

#include "../DirectX/lib/DX3D_11.h"

#define DEF_ANGL 360/12.0
#define DEF_COUNT_ONELOOP 12
#define DEF_POS_SET_BASE 1000

#define DEF_HANGL 360/8.0
#define DEF_HCOUNT_ONELOOP 8
#define DEF_HPOS_SET_BASE 2500

#define DEF_MARKER_GSIZE 384
#define DEF_LINE_GSIZE 128

#define DEF_PACK_GSIZE 384

#define DEF_IPFONTSIZE_X 48
#define DEF_IPFONTSIZE_Y 48
#define DEF_IPFONTSIZE_MARG 26
#define DEF_IPFONTSIZE_UNDER 0.4

#define BASE_PACKSIZE 100
#define PACK_MOVESPEED 4.0

#define PACK_MOVE_TIME 5000.0
#define PACK_MOVE_TIME_SEC (int)PACK_MOVE_TIME / 1000

class Packsize {
private:
	
	unsigned long long janeratetime[2];
	int size;

public:

	void setData(unsigned long long *janeratetime, int size) {
		
		this->janeratetime[0] = janeratetime[0];
		this->janeratetime[1] = janeratetime[1];

		this->size = size;

	}

	unsigned long long getTime(int id) {
		return janeratetime[id];
	}

	unsigned long long *getTime() {
		return janeratetime;
	}

	int getSize() {
		return size;
	}

};

class Markerclass {
private:
	Pos pos;
	Pos viewpos;
	int ip[8];
	int version;
	double size;
	double psize;
	int anmflag;
	int phaseflag;
	double angl;
	long deps;
	long connectcount;
	long hostid;
	long *connectid;
	long connectidcount;
	long sortid;

	bool distflag;

	Packsize *packsize;
	long *pmoveid;
	long packsizecount;
	int sumsize;

	DWORD col;

public:
	void Set(Pos position, double angl,int *ipadd, int version, int deps, int hostid, int sortid);

	void Set_Pos(Pos position);
	void Set_ViewPos(Pos position);
	void Set_IP(int *ipadd);
	void Set_Version(int version);
	void Set_Size(double setsize);
	void Set_Angl(double angl);
	void Set_PSize(double setsize);
	void Set_Phase(int flag);
	void Set_Anmflag(int flag) {
		anmflag = flag;;
	}
	void Set_Deps(int flag) {
		deps = flag;
	}

	void Set_Sortid(int id) {
		sortid = id;
	}

	void Set_Hostid(int id){
		hostid = id;
	}

	void Set_Col(DWORD col) {
		this->col = col;
	}

	void Set_Distflag(bool flag) {
		distflag = flag;
	}

	void Set_Packsize(unsigned long long *janeratetime, int size) {
		packsize = (Packsize *)realloc(packsize, sizeof(Packsize) * (packsizecount + 1));
		pmoveid = (long *)realloc(pmoveid, sizeof(long) * (packsizecount + 1));

		packsize[packsizecount].setData(janeratetime, size);
		pmoveid[packsizecount] = -1;

		packsizecount++;
	}

	void Check_Packsize(unsigned long long *nowtime, int mlsizeborder, int bordersize1, int bordersize2) {
		int datacount = 0;
		int moveidcheck = 0;
		int moveidchecked = 0;

		sumsize = 0;
		for (long i = 0; i < packsizecount; i++) {
			if (10 < nowtime[0] - packsize[i].getTime(0) - (nowtime[1] - packsize[i].getTime(1) < 0 ? 1 : 0)) {
				pmoveid[moveidcheck] = i;
				moveidcheck++;
			}
			else {
				datacount++;
				sumsize += packsize[i].getSize();
				if (pmoveid[moveidchecked] != -1) {
					packsize[pmoveid[moveidchecked]].setData(packsize[i].getTime(), packsize[i].getSize());

					pmoveid[moveidchecked] = -1;

					moveidchecked++;

					pmoveid[moveidcheck] = i;
					moveidcheck++;
				}
			}
		}

		phaseflag = 0;
		if (bordersize1 <= sumsize) {
			phaseflag = 1;
		}
		if (bordersize2 <= sumsize) {
			phaseflag = 2;
		}

		if (psize > (sumsize / (double)mlsizeborder)) {
			psize -= (psize - (sumsize / (double)mlsizeborder )) / 100.0;
			if ((psize - (sumsize / (double)mlsizeborder)) < 0.0001) psize = (sumsize / (double)mlsizeborder);
		}
		else if (psize < sumsize / (double)mlsizeborder) {
			psize += ((sumsize / (double)mlsizeborder ) - psize) / 100.0;
			if (((sumsize / (double)mlsizeborder ) - psize) < 0.0001) psize = (sumsize / (double)mlsizeborder);
		}

		if (psize < 0.5) {
			psize = 0.5;
		}
		else if (psize > 2.0) {
			psize = 2.0;
		}

		packsizecount = datacount;
	}

	void Add_Connectcount(long add) {
		connectcount += add;
	};

	void Add_Connectid(long id) {
		connectid = (long *)realloc(connectid, sizeof(long) * (connectidcount + 1));
		
		connectid[connectidcount] = id;
		
		connectidcount++;
	}

	Pos Get_Pos();
	Pos Get_ViewPos();
	int* Get_IP();
	int Get_Version();
	double Get_Size();
	double Get_Angl();

	double Get_PSize() {
		return psize;
	};

	int Get_SumSize() {
		return sumsize;
	};

	int Get_Phase();
	int Get_Anmflag() {
		return anmflag;
	}
	long Get_Deps() {
		return deps;
	}
	long Get_Connectcount() {
		return connectcount;
	};

	long Get_Sortid() {
		return sortid;
	}

	long Get_Connectid(int id) {
		return connectid[id];
	}

	long Get_ConnectidCount() {
		return connectidcount;
	}

	int Get_Hostid() {
		return hostid;
	}

	DWORD Get_Col() {
		return col;
	}

	Packsize &Get_Packsize(int id) {
		return packsize[id];
	}

	int Get_Packsizecount() {
		return packsizecount;
	}

	bool Get_Distflag() {
		return distflag;
	}

	void Free_Packsize() {
		if (packsize != NULL) {
			free(packsize);
		}
		packsize = NULL;

		if (pmoveid != NULL) {
			free(pmoveid);
		}
		pmoveid = NULL;
	}

	void Free_Connectid() {
		if (connectid != NULL) {
			free(connectid);
			connectidcount = 0;
		}
	}

	void Move(Pos pos, double angl, int connectcount, int centerf);
	void JenMove(int *ipadd, int version, int count, int deps, Pos pos, int hostid, int connectcount, double angl, int sortid);

};

class Lineclass {
private:
	Pos pos[2];
	int id;
	int ip[2][8];
	double size;
	double psize;
	int anmflag;
	int phaseflag;
	double angl;

	Packsize *packsize;
	long *pmoveid;
	long packsizecount;
	int sumsize;

	DWORD col;

public:
	void Set(Pos position1, Pos position2, int newid, int *ip1, int *ip2);
	void Set_Pos(Pos position1, Pos position2);
	void Set_IP(int i, int *ip);
	void Set_Size(double size);
	void Set_PSize(double size);
	void Set_Phase(int flag);
	void Set_Anmflag(int flag) {
		anmflag = flag;
	}
	void Set_Col(DWORD col) {
		this->col = col;
	}
	
	void Set_Packsize(unsigned long long *janeratetime, int size) {
		packsize = (Packsize *)realloc(packsize, sizeof(Packsize) * (packsizecount + 1));
		pmoveid = (long *)realloc(pmoveid, sizeof(long) * (packsizecount + 1));

		packsize[packsizecount].setData(janeratetime, size);
		pmoveid[packsizecount] = -1;

		packsizecount++;
	}

	void Check_Packsize(unsigned long long *nowtime, int mlsizeborder, int bordersize1, int bordersize2) {
		int datacount = 0;
		int moveidcheck = 0;
		int moveidchecked = 0;

		sumsize = 0;
		for (int i = 0; i < packsizecount; i++) {
			if (10 < nowtime[0] - packsize[i].getTime(0) - (nowtime[1] - packsize[i].getTime(1) < 0 ? 1 : 0)) {
				pmoveid[moveidcheck] = i;
				moveidcheck++;
			}
			else {
				datacount++;
				sumsize += packsize[i].getSize();
				if (pmoveid[moveidchecked] != -1) {
					packsize[pmoveid[moveidchecked]].setData(packsize[i].getTime(), packsize[i].getSize());

					pmoveid[moveidchecked] = -1;

					moveidchecked++;

					pmoveid[moveidcheck] = i;
					moveidcheck++;
				}
			}
		}

		phaseflag = 0;
		if (bordersize1 <= sumsize) {
			phaseflag = 1;
		}
		if (bordersize2 <= sumsize) {
			phaseflag = 2;
		}

		if (psize > (sumsize * 2 / (double)mlsizeborder)) {
			psize -= (psize - (sumsize * 2 / (double)mlsizeborder)) / 100.0;
			if ((psize - (sumsize * 2 / (double)mlsizeborder)) < 0.0001) psize = (sumsize * 2 / (double)mlsizeborder);
		}
		else if (psize < (sumsize * 2 / (double)mlsizeborder)) {
			psize += ((sumsize * 2 / (double)mlsizeborder) - psize) / 100.0;
			if (((sumsize * 2 / (double)mlsizeborder) - psize) < 0.0001) psize = (sumsize * 2 / (double)mlsizeborder);
		}

		if (psize < 0.5) {
			psize = 0.5;
		}
		else if (psize > 2) {
			psize = 2;
		}

		packsizecount = datacount;
	}

	Pos Get_Pos(int i);
	double Get_Angl();
	int *Get_IP(int i);
	double Get_Size();

	double Get_PSize() {
		return psize;
	};

	int Get_SumSize() {
		return sumsize;
	};

	int Get_Phase();
	int Get_Anmflag() {
		return anmflag;
	}

	Packsize &Get_Packsize(int id) {
		return packsize[id];
	}

	int Get_Packsizecount() {
		return packsizecount;
	}

	DWORD Get_Col() {
		return col;
	}

	void Free_Packsize() {
		if (packsize != NULL) {
			free(packsize);
		}
		packsize = NULL;

		if (pmoveid != NULL) {
			free(pmoveid);
		}
		pmoveid = NULL;
	}

	void Move(Pos pos1, Pos pos2);
	void JenMove(Pos pos1, Pos pos2, int newid, int *ip1, int *ip2, int count);
};

class Packetclass {
private:
	Pos pos[2];
	int ip[2][8];//0:from, 1;at
	double size;
	int anmflag;
	double angl;
	int moveflame;
	double pacsize;
	int opacsize;

	long tableid;
	bool reverse;

	unsigned long long janeratetime[2];
	unsigned long long arrivaltime[2];

	DWORD col;

public:
	void Set(Pos position1, Pos position2, int *ip1, int *ip2, int pacsize, unsigned long long *janeratetime, unsigned long long arrivaltime0, unsigned long long arrivaltime1, int tableid, bool reverse);

	void Set_Pos(Pos position, int id);
	void Set_Size(double size);
	void Set_Pacsize(double pacsize) {
		this->pacsize = pacsize;
	}
	void Set_IP(int i, int *ip);
	void Set_Anmflag(int flag) {
		anmflag = flag;
	}
	void Set_Arrivaltime(unsigned long long *time) {
		arrivaltime[0] = time[0];
		arrivaltime[1] = time[1];
	}
	void Set_Janeratetime(unsigned long long *time) {
		janeratetime[0] = time[0];
		janeratetime[1] = time[1];
	}

	void Set_Col(DWORD col) {
		this->col = col;
	}

	void Set_All(Pos position1, Pos position2, int *ip1, int *ip2,
		double size, int flag, int moveflame, double pacsize, int opacsize, unsigned long long *janeratetime, unsigned long long *arrivaltime, int tableid, bool reverse) {
		pos[0] = position1;
		pos[1] = position2;
		angl = atan2(pos[0].y - pos[1].y, pos[0].x - pos[1].x) * 180.0 / PI;
		for (int i = 0; i < 8; i++) {
			ip[0][i] = ip1[i];
			ip[1][i] = ip2[i];
		}
		this->size = size;
		this->anmflag = flag;
		this->moveflame = moveflame;
		this->pacsize = pacsize;
		this->opacsize = opacsize;

		this->arrivaltime[0] = arrivaltime[0];
		this->arrivaltime[1] = arrivaltime[1];
		this->janeratetime[0] = janeratetime[0];
		this->janeratetime[1] = janeratetime[1];

		this->tableid = tableid;
		this->reverse = reverse;
		
	}

	void Add_Moveflame(int i);

	Pos Get_Pos(int id);
	double Get_Angl();
	int Get_Moveflame();
	int *Get_IP(int i);
	double Get_Size();
	int Get_Anmflag(){
		return anmflag;
	}
	double Get_Pacsize() {
		return pacsize;
	}

	int Get_OPacsize() {
		return opacsize;
	}

	unsigned long long Get_Arrivaltime(int flag) {
		return arrivaltime[flag];
	}

	unsigned long long Get_Janeratetime(int flag) {
		return janeratetime[flag];
	}

	unsigned long long *Get_Arrivaltime() {
		return arrivaltime;
	}

	unsigned long long *Get_Janeratetime() {
		return janeratetime;
	}

	long int Get_MoveTime(int flag) {
		return (long int)arrivaltime[flag] - janeratetime[flag];
	}

	long int Get_MovedTime(long int time,int flag) {
		return (long int)time - janeratetime[flag];
	}

	int Get_TableID() {
		return tableid;
	}

	int Get_ReverseFlag() {
		return reverse;
	}

	DWORD Get_Col() {
		return col;
	}

	void Move(Pos pos1, Pos pos2);
	void JenMove(Pos pos1, Pos pos2, int *ip1, int *ip2, int count, int pacsize, unsigned long long *janeratetime, unsigned long long arrivaltime0, unsigned long long arrivaltime1, int tableid, bool reverse);
};

class MarkerMasterclass {
private:

	unsigned int usecount;
	unsigned int hostcount;
	long *removebuff;
	
	unsigned int checkedcount;

	double tu[2], tv[2];

	Markerclass *data;

public:

	

	void reset();
	void jenerate(int *ip, int version, int *conectip);
	void sort();
	void remove();
	void datafree();

	void move();

	Markerclass &Data(int id) {
		return data[id];
	}
	long getUsecount() {
		return usecount;
	}

	long getCheckedcount() {
		return checkedcount;
	}
	

	void setUv(double u1, double u2, double v1, double v2) {
		tu[0] = u1;
		tu[1] = u2;

		tv[0] = v1;
		tv[1] = v2;
	}

	double getUv(int flag, int id) {
		if (flag = 0) {
			return tu[id];
		}
		else {
			return tv[id];
		}
	}

};

class LineMasterclass {
private:
	
	unsigned int usecount;
	long *removebuff;
	unsigned int idcounter;

	unsigned int checkedcount;

	double tu[2], tv[2];
	
	Lineclass *data;

public:
	

	void reset();
	unsigned int jenerate(int *ip1, int *ip2);
	void sort();
	void remove();
	void datafree();

	void move(MarkerMasterclass maker);

	Lineclass &Data(int id) {
		return data[id];
	}
	unsigned int getUsecount() {
		return usecount;
	}
	unsigned int getIdcounter() {
		return idcounter;
	}

	unsigned int getCheckedcount() {
		return checkedcount;
	}

	void setUv(double u1, double u2, double v1, double v2) {
		tu[0] = u1;
		tu[1] = u2;

		tv[0] = v1;
		tv[1] = v2;
	}

	double getUv(int flag, int id) {
		if (flag = 0) {
			return tu[id];
		}
		else {
			return tv[id];
		}
	}

};

class PacketMasterclass {
private:
	
	unsigned int usecount;
	long *removebuff;
	unsigned int idcounter;

	unsigned int checkedcount;
	unsigned int checkaddr;

	double tu[2], tv[2];
	
	Packetclass *data;

public:
	

	void reset();
	void jenerate(int *ip1, int *ip2, int pacsize, unsigned long long *janeratetime, unsigned long long arrivaltime0, unsigned long long arrivaltime1, unsigned int tableid, bool reverse);
	void sort();
	void remove();
	void datafree();

	void move(MarkerMasterclass marker);

	Packetclass &Data(int id) {
		return data[id];
	}
	unsigned int getUsecount() {
		return usecount;
	}
	unsigned int getIdcounter() {
		return idcounter;
	}

	unsigned int getCheckedcount() {
		return checkedcount;
	}

	void setUv(double u1, double u2, double v1, double v2) {
		tu[0] = u1;
		tu[1] = u2;

		tv[0] = v1;
		tv[1] = v2;
	}

	double getUv(int flag, int id) {
		if (flag = 0) {
			return tu[id];
		}
		else {
			return tv[id];
		}
	}

};

class Routingclass {
private:
	int ip[2][8];
	int version[2];

	long *data;
	unsigned int datacount;
public:

	void Set(int *ip1, int version1, int *ip2, int version2) {
		ip[0][0] = ip1[0];
		ip[0][1] = ip1[1];
		ip[0][2] = ip1[2];
		ip[0][3] = ip1[3];
		ip[0][4] = ip1[4];
		ip[0][5] = ip1[5];
		ip[0][6] = ip1[6];
		ip[0][7] = ip1[7];

		version[0] = version1;

		ip[1][0] = ip2[0];
		ip[1][1] = ip2[1];
		ip[1][2] = ip2[2];
		ip[1][3] = ip2[3];
		ip[1][4] = ip2[4];
		ip[1][5] = ip2[5];
		ip[1][6] = ip2[6];
		ip[1][7] = ip2[7];

		version[1] = version2;

		data = (long *)malloc(sizeof(long) * 1);

		data[0] = 0;
		datacount = 0;
	}

	void Add(int id) {
		data = (long *)realloc(data, sizeof(long) * (datacount + 1));
		
		data[datacount] = id;

		datacount++;
	}

	void DataFree() {
		if (data != NULL) {
			free(data);
		}

		datacount = 0;
	}

	int *Get_IP(int id) {
		return ip[id];
	}

	int Get_Version(int id) {
		return version[id];
	}

	long *Data() {
		return data;
	}

	unsigned int Get_Datacount() {
		return datacount;
	}
};

class RoutingMasterclass {
private:

	Routingclass *data;
	unsigned int usecount;

	long *checkedid;
	unsigned int checkedidcount;

public:

	void reset();
	void jenerate(int *ip1, int version1, int *ip2, int version2);

	long check(MarkerMasterclass marker, int *ip1, int version1, int *ip2, int version2);

	bool mkidcheck(MarkerMasterclass marker, int *ip, int version, int id, int dataid);

	Routingclass &Data(int id) {
		return data[id];
	}
	unsigned int getUsecount() {
		return usecount;
	}

	void datafree() {
		for (int i = 0; i < usecount; i++) {
			data[i].DataFree();
		}

		if (data != NULL) {
			free(data);
		}

		if (checkedid != NULL) {
			free(checkedid);
		}

		usecount = 0;
		checkedidcount = 0;
	}

};

extern void DataStart();

extern void DataTrash();

extern void VisobjtimeCheck();

#endif