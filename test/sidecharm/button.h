#ifndef INCLUDED_BUTTON
#define INCLUDED_BUTTON

#include "../main.h"
#include "../DirectX/lib/DX3D_11.h"

class buttonclass {
private:
	Pos pos;
	Pos size;
	int id;
	short flag;//通常 0, クリックされたとき 1, ボタン無効時 -1, ボタン削除時 -2
	double tu[2], tv[2];
public:
	void Set(Pos pos, Pos size, int id, short flag, double *tu, double *tv){
		this->pos = pos;
		this->size = size;
		this->id = id;
		this->flag = flag;
		this->tu[0] = tu[0];
		this->tu[1] = tu[1];
		this->tv[0] = tv[0];
		this->tv[1] = tv[1];
	}

	void SetPos(Pos pos) {
		this->pos = pos;
	}

	void SetSize(Pos size) {
		this->size = size;
	}

	void SetFlag(short flag) {
		this->flag = flag;
	}

	void SetTuTv(double *tu, double *tv) {
		this->tu[0] = tu[0];
		this->tu[1] = tu[1];
		this->tv[0] = tv[0];
		this->tv[1] = tv[1];
	}

	void Check(Pos cursol, int click) {
		if (flag != -1) {
			if (cursol.x >= pos.x - (size.x / 2) && cursol.x <= pos.x + (size.x / 2)
				&& cursol.y >= pos.y - (size.y / 2) && cursol.y <= pos.y + (size.y / 2)) {
				if (click == 1) {
					flag = 1;
				}
				else {
					flag = 0;
				}
			}
			else {
				flag = 0;
			}
		}
	}

	Pos GetPos() {
		return pos;
	}

	Pos GetSize() {
		return size;
	}

	int GetId() {
		return id;
	}

	short GetFlag() {
		return flag;
	}

	double *GetTuTv(int id) {
		if (id == 0) {
			return tu;
		}

		return tv;
	}
};

class buttonmaster {
private:
	buttonclass *data;
	int usecount;
	int *moveid;
	
public:
	void Set() {
		data = (buttonclass *)malloc(sizeof(buttonclass) * 1);
		moveid = (int *)malloc(sizeof(int) * 1);

		moveid[0] = -1;
		usecount = 0;
	}

	void Add(Pos pos, Pos size, int id, short flag, double *tu, double *tv) {
		data = (buttonclass *)realloc(data, sizeof(buttonclass) * (usecount + 1));
		moveid = (int *)realloc(moveid, sizeof(int) * (usecount + 1));

		data[usecount].Set(pos, size, id, flag, tu, tv);
		moveid[usecount] = -1;

		usecount++;
	}

	buttonclass &Get(int id) {
		return data[id];
	}

	void Checkdel(){
		int checkusecount = 0;
		int checkmoveid = 0;
		int checkedmoveid = 0;

		for (int i = 0; i < usecount; i++) {
			if (data[i].GetFlag() == -2) {
				moveid[checkmoveid] = i;
				checkmoveid++;
			}
			else {
				checkusecount++;
				if (moveid[checkedmoveid] != -1) {
					data[moveid[checkedmoveid]].Set(data[i].GetPos(), data[i].GetSize(), data[i].GetId(), data[i].GetFlag(), data[i].GetTuTv(0), data[i].GetTuTv(1));
					
					moveid[checkmoveid] = i;
					checkmoveid++;

					moveid[checkedmoveid] = -1;
					checkedmoveid++;
				}
			}
		}
		usecount = checkusecount;
	}


	int GetUsecount() {
		return usecount;
	}

};

class selectboxclass {
private:
	Pos pos;
	Pos size;
	int id;

	int selidcount;
	int selectid;
	short flag;//通常 0, クリックされたとき 1, オンカーソル 2, ボタン無効時 -1, ボタン削除時 -2
	double tu[2], tv[2];
	double txttu[2], txttv[2];
public:
	void Set(Pos pos, Pos size, int id, int selidcount, int selectid, short flag, double *tu, double *tv) {
		this->pos = pos;
		this->size = size;
		this->id = id;

		this->selidcount = selidcount;
		this->selectid = selectid;

		this->flag = flag;
		this->tu[0] = tu[0];
		this->tu[1] = tu[1];
		this->tv[0] = tv[0];
		this->tv[1] = tv[1];
	}

	void SetAll(Pos pos, Pos size, int id, int selidcount, int selectid, short flag, double *tu, double *tv, double *txttu, double *txttv) {
		this->pos = pos;
		this->size = size;
		this->id = id;

		this->selidcount = selidcount;
		this->selectid = selectid;

		this->flag = flag;
		this->tu[0] = tu[0];
		this->tu[1] = tu[1];
		this->tv[0] = tv[0];
		this->tv[1] = tv[1];

		this->txttu[0] = txttu[0];
		this->txttu[1] = txttu[1];
		this->txttv[0] = txttv[0];
		this->txttv[1] = txttv[1];
	}

	void SetPos(Pos pos) {
		this->pos = pos;
	}

	void SetSize(Pos size) {
		this->size = size;
	}

	void SetFlag(short flag) {
		this->flag = flag;
	}

	void SetTuTv(double *tu, double *tv) {

		this->tu[0] = tu[0];
		this->tu[1] = tu[1];
		this->tv[0] = tv[0];
		this->tv[1] = tv[1];

	}


	void SetTxtTuTv(double *tu, double *tv) {
		this->txttu[0] = tu[0];
		this->txttu[1] = tu[1];
		this->txttv[0] = tv[0];
		this->txttv[1] = tv[1];
	}

	void Check(Pos cursol, int click) {
		bool checkflag = 0;
		if (flag != -1) {

			if (cursol.x >= pos.x - (size.x / 2) && cursol.x <= pos.x + (size.x / 2)
				&& cursol.y >= pos.y - (size.y / 2) && cursol.y <= pos.y + (size.y / 2)) {
				flag = 2;
				checkflag = 1;
			}

			if (cursol.x >= pos.x - (size.x / 2) && cursol.x <= pos.x - (size.x / 4)
				&& cursol.y >= pos.y - (size.y / 2) && cursol.y <= pos.y + (size.y / 2)) {
				if (click == 1) {
					flag = 1;
					selectid--;
					if (selectid < 0) selectid = selidcount - 1;
					checkflag = 1;
				}
			}

			if (cursol.x >= pos.x + (size.x / 4) && cursol.x <= pos.x + (size.x / 2)
				&& cursol.y >= pos.y - (size.y / 2) && cursol.y <= pos.y + (size.y / 2)) {
				if (click == 1) {
					flag = 1;
					selectid++;
					if (selectid >= selidcount) selectid = 0;
					checkflag = 1;
				}
			}
			
			if(checkflag == 0){
				flag = 0;
			}
		}
	}

	void SetSelidcount(int selidcount) {
		this->selidcount = selidcount;
	}

	Pos GetPos() {
		return pos;
	}

	Pos GetSize() {
		return size;
	}

	int GetId() {
		return id;
	}

	short GetFlag() {
		return flag;
	}

	int GetSelectid() {
		return selectid;
	}

	int GetSelidcount() {
		return selidcount;
	}

	double *GetTuTv(int id) {
		if (id == 0) {
			return tu;
		}

		return tv;
	}

	double *GetTxtTuTv(int id) {
		if (id == 0) {
			return txttu;
		}

		return txttv;
	}

};

class selectboxmaster {
private:
	selectboxclass *data;
	int usecount;
	int *moveid;

public:

	void Set() {
		data = (selectboxclass *)malloc(sizeof(selectboxclass) * 1);
		moveid = (int *)malloc(sizeof(int) * 1);

		moveid[0] = -1;
		usecount = 0;
	}

	void Add(Pos pos, Pos size, int id, int selidcount, int selectid, short flag, double *tu, double *tv) {
		data = (selectboxclass *)realloc(data, sizeof(selectboxclass) * (usecount + 1));
		moveid = (int *)realloc(moveid, sizeof(int) * (usecount + 1));

		data[usecount].Set(pos, size, id, selidcount, selectid, flag, tu, tv);
		moveid[usecount] = -1;

		usecount++;
	}

	selectboxclass &Get(int id) {
		return data[id];
	}

	void Checkdel() {
		int checkusecount = 0;
		int checkmoveid = 0;
		int checkedmoveid = 0;

		for (int i = 0; i < usecount; i++) {
			if (data[i].GetFlag() == -2) {
				moveid[checkmoveid] = i;
				checkmoveid++;
			}
			else {
				checkusecount++;
				if (moveid[checkedmoveid] != -1) {
					data[moveid[checkedmoveid]].SetAll(data[i].GetPos(), data[i].GetSize(), data[i].GetId(), data[i].GetSelidcount(), data[i].GetSelectid(), data[i].GetFlag(), data[i].GetTxtTuTv(0), data[i].GetTuTv(1), data[i].GetTxtTuTv(0), data[i].GetTuTv(1));

					moveid[checkmoveid] = i;
					checkmoveid++;

					moveid[checkedmoveid] = -1;
					checkedmoveid++;
				}
			}
		}
		usecount = checkusecount;
	}

	int GetUsecount() {
		return usecount;
	}

};

class textboxclass {
private:
	Pos pos;
	Pos size;
	int id;

	int selidcount;
	int selectid;
	int *data;
	short flag;//通常 0, クリックされたとき 1, ボタン無効時 -1, ボタン削除時 -1
	double tu[2], tv[2];
	double txttu[2], txttv[2];
public:
	void Set(Pos pos, Pos size, int id, int *data, int selidcount, short flag, double *tu, double *tv) {
		this->pos = pos;
		this->size = size;
		this->id = id;

		this->flag = flag;
		this->tu[0] = tu[0];
		this->tu[1] = tu[1];
		this->tv[0] = tv[0];
		this->tv[1] = tv[1];

		this->selidcount = selidcount;
		this->selectid = 0;

		this->data = (int *)malloc(sizeof(int) * selidcount);

		for (int i = 0; i < selidcount; i++) {
			this->data[i] = data[i];
		}
	}

	void SetAll(Pos pos, Pos size, int id, int *data, int selidcount, int selectid, short flag, double *tu, double *tv, double *txttu, double *txttv) {
		this->pos = pos;
		this->size = size;
		this->id = id;

		this->selidcount = selidcount;
		this->selectid = selectid;

		this->flag = flag;
		this->tu[0] = tu[0];
		this->tu[1] = tu[1];
		this->tv[0] = tv[0];
		this->tv[1] = tv[1];

		this->txttu[0] = txttu[0];
		this->txttu[1] = txttu[1];
		this->txttv[0] = txttv[0];
		this->txttv[1] = txttv[1];

		if (this->data == NULL) {
			this->data = (int *)malloc(sizeof(int) * selidcount);
		}
		else {
			this->data = (int *)realloc(this->data, sizeof(int) * selidcount);

		}

		for (int i = 0; i < selidcount; i++) {
			this->data[i] = data[i];
		}
	}

	void SetPos(Pos pos) {
		this->pos = pos;
	}

	void SetSize(Pos size) {
		this->size = size;
	}

	void SetFlag(short flag) {
		this->flag = flag;
	}

	void SetTuTv(double *tu, double *tv) {

		this->tu[0] = tu[0];
		this->tu[1] = tu[1];
		this->tv[0] = tv[0];
		this->tv[1] = tv[1];

	}
	void SetSelectid(int selectid){
		this->selectid = selectid;
	}

	void SetSelidcount(int selidcount) {
		this->selidcount = selidcount;
	}

	void SetData(int data) {
		if (selectid >= selidcount) {
			this->data = (int *)realloc(this->data, sizeof(int) * (selidcount + 1));
			selidcount++;
		}

		this->data[selectid] = data;
		
	}


	void SetTxtTuTv(double *tu, double *tv) {
		this->txttu[0] = tu[0];
		this->txttu[1] = tu[1];
		this->txttv[0] = tv[0];
		this->txttv[1] = tv[1];
	}

	void Check(Pos cursol, int click) {
		if (flag != -1) {
			if (flag == 1) flag = 2;
			if (click == 1) {
				if (cursol.x >= pos.x - (size.x / 2) && cursol.x <= pos.x + (size.x / 2)
					&& cursol.y >= pos.y - (size.y / 2) && cursol.y <= pos.y + (size.y / 2)) {

					flag = 1;
				}
				else {
					flag = 0;
				}
			}
			if(dkb.GetKeySt(KEY_ESCAPE)) flag = 0;
		}
	}

	Pos GetPos() {
		return pos;
	}

	Pos GetSize() {
		return size;
	}

	int GetId() {
		return id;
	}

	short GetFlag() {
		return flag;
	}

	int GetSelectid() {
		return selectid;
	}

	int GetSelidcount() {
		return selidcount;
	}

	int GetData(int id) {
		return data[id];
	}

	int *GetData() {
		return data;
	}

	double *GetTuTv(int id) {
		if (id == 0) {
			return tu;
		}

		return tv;
	}

	double *GetTxtTuTv(int id) {
		if (id == 0) {
			return txttu;
		}

		return txttv;
	}

	void Free() {
		if (data != NULL) {
			free(data);
		}
		data = NULL;
	}

};

class textboxmaster {
private:
	textboxclass *data;
	int usecount;
	int *moveid;

public:

	void Set() {
		data = (textboxclass *)malloc(sizeof(textboxclass) * 1);
		moveid = (int *)malloc(sizeof(int) * 1);

		moveid[0] = -1;
		usecount = 0;
	}

	void Add(Pos pos, Pos size, int id, int *data, int selidcount, short flag, double *tu, double *tv) {
		this->data = (textboxclass *)realloc(this->data, sizeof(textboxclass) * (usecount + 1));
		moveid = (int *)realloc(moveid, sizeof(int) * (usecount + 1));

		this->data[usecount].Set(pos, size, id, data, selidcount, flag, tu, tv);
		moveid[usecount] = -1;

		usecount++;
	}

	textboxclass &Get(int id) {
		return data[id];
	}

	void Checkdel() {
		int checkusecount = 0;
		int checkmoveid = 0;
		int checkedmoveid = 0;

		for (int i = 0; i < usecount; i++) {
			if (data[i].GetFlag() == -2) {
				moveid[checkmoveid] = i;
				checkmoveid++;
				data[i].Free();
			}
			else {
				checkusecount++;
				if (moveid[checkedmoveid] != -1) {
					data[moveid[checkedmoveid]].SetAll(data[i].GetPos(), data[i].GetSize(), data[i].GetId(), data[i].GetData(), data[i].GetSelidcount(), data[i].GetSelectid(), data[i].GetFlag(), data[i].GetTxtTuTv(0), data[i].GetTuTv(1), data[i].GetTxtTuTv(0), data[i].GetTuTv(1));

					moveid[checkmoveid] = i;
					checkmoveid++;

					moveid[checkedmoveid] = -1;
					checkedmoveid++;
				}
			}
			
		}
		usecount = checkusecount;
	}

	int GetUsecount() {
		return usecount;
	}

};

#endif