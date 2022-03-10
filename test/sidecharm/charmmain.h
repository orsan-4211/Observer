#ifndef INCLUDED_CMAIN
#define INCLUDED_CMAIN

#include "../DirectX/lib/DX3D_11.h"
#include "../graphic/graphicmain.h"
#include "button.h"

#define SQBUTTON_SIZE 48
#define SELECTBOX_SIZE_X 350
#define SELECTBOX_SIZE_Y 48

#define TAB_SIZE_X 32
#define TAB_SIZE_Y 48

#define CHARM_BASETXT_SIZE 18
#define CHARM_BASETXT_MARG 10

#define CHARM_MIDTXT_SIZE 32
#define CHARM_MIDTXT_MARG 16

#define CHARM_LAGTXT_SIZE 60
#define CHARM_LAGTXT_MARG 30


class MenuAreaclass {
private:
	Pos pos;
	double x0, x1, y0, y1;
	bool deletef;
	int id;
public:
	Vertex vert[4];

	void Reset() {
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;

		x0 = 0;
		x1 = 0;
		y0 = 0;
		y1 = 0;

		deletef = 0;
	}

	void SetPos(Pos pos, int id, double x0, double x1, double y0, double y1) {
		
		this->pos = pos;
		this->id = id;

		this->x0 = x0;
		this->x1 = x1;
		this->y0 = y0;
		this->y1 = y1;
		
	}

	void CheckPos(XMFLOAT4X4 view, XMFLOAT4X4 projection, double Width, double Height) {
		
		vert[0].x = this->pos.x - this->x0;
		vert[0].y = this->pos.y - this->y0;
		vert[0].z = 0.0f;

		vert[1].x = this->pos.x + this->x1;
		vert[1].y = this->pos.y + this->y1;
		vert[1].z = 0.0f;

		vert[2].x = this->pos.x - this->x0;
		vert[2].y = this->pos.y + this->y1;
		vert[2].z = 0.0f;

		vert[3].x = this->pos.x + this->x1;
		vert[3].y = this->pos.y - this->y0;
		vert[3].z = 0.0f;


		vert[0].tu = vert[0].x / screentexsize;
		vert[0].tv = vert[0].y / screentexsize;
		vert[0].color[0] = 0.95;
		vert[0].color[1] = 0.95;
		vert[0].color[2] = 0.95;
		vert[0].color[3] = 0.95f;

		vert[1].tu = vert[1].x / screentexsize;
		vert[1].tv = vert[1].y / screentexsize;
		vert[1].color[0] = 0.95;
		vert[1].color[1] = 0.95;
		vert[1].color[2] = 0.95;
		vert[1].color[3] = 0.95f;

		vert[2].tu = vert[2].x / screentexsize;
		vert[2].tv = vert[2].y / screentexsize;
		vert[2].color[0] = 0.95;
		vert[2].color[1] = 0.95;
		vert[2].color[2] = 0.95;
		vert[2].color[3] = 0.95f;

		vert[3].tu = vert[3].x / screentexsize;
		vert[3].tv = vert[3].y / screentexsize;
		vert[3].color[0] = 0.95;
		vert[3].color[1] = 0.95;
		vert[3].color[2] = 0.95;
		vert[3].color[3] = 0.95f;

		Set2DPosvert(vert, Width, Height, 4, 0, view, projection);

	}

	Pos GetPos() {
		return pos;
	}

	int GetPosxy(int flag) {
		switch (flag) {
		case 0:
			return x0;
			break;
		case 1:
			return x1;
			break;
		case 2:
			return y0;
			break;
		case 3:
			return y1;
			break;
		default:
			break;
		}

		return 0;
		
	}

	int GetId() {
		return id;
	}

	void Delete() {
		deletef = 1;
	}

	bool GetDeletef() {
		return deletef;
	}

};

class MenuAreaMasterclass {
public:
	MenuAreaclass *data;
	int usecount;
	int *removebuff;
	int removecount;
	
	void reset() {
		data = (MenuAreaclass *)malloc(sizeof(MenuAreaclass) * 1);
		usecount = 0;
		removebuff = (int *)malloc(sizeof(int) * 1);
		removebuff[0] = -1;
		removecount = 0;
	}

	void jenerate(Pos pos, int id, double x0, double x1, double y0, double y1) {
		data = (MenuAreaclass *)realloc(data, sizeof(MenuAreaclass) * (usecount + 1));
		removebuff = (int *)realloc(removebuff, sizeof(int) * (usecount + 1));

		removebuff[usecount] = -1;

		data[usecount].Reset();
		data[usecount].SetPos(pos, id, x0, x1, y0, y1);

		usecount++;
	}

	void remove() {
		int removecount = 0;
		int useremovecount = 0;
		int usecountbuff = 0;
		for (int i = 0; i < usecount; i++) {
			if (data[i].GetDeletef() == 1) {
				removebuff[removecount] = i;
				removecount++;
			}
			else {
				usecountbuff++;
				if (useremovecount < removecount) {
					data[removebuff[removecount]].SetPos(data[i].GetPos(), data[i].GetId(), data[i].GetPosxy(0), data[i].GetPosxy(1), data[i].GetPosxy(2), data[i].GetPosxy(3));
					useremovecount++;
					removebuff[removecount] = i;
					removecount++;
				}
			}
		}
		usecount = usecountbuff;
	}

	void datafree() {
		free(data);
	}

};


extern MenuAreaMasterclass area;

extern buttonmaster button;
extern selectboxmaster selectbox;
extern textboxmaster textbox;

extern int modef;

extern double charmleftpos;

extern void charmmain();

extern void charmdrmain();

extern void resetbutton();
#endif
