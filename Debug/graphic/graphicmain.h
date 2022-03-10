#ifndef INCLUDED_GRAPHICMAIN
#define INCLUDED_GRAPHICMAIN

#define SCREENTEX_SIZE 1920

#define FRONT_X 480
#define FRONT_Y 480

#define BLUR_MARGE 3

//#define FRONT_X 960

#include "visobj.h"

extern double viewx;
extern double vxmaxlength;

extern void graphicinit();
extern void graphicmain();
extern void graphiccharmmain();


extern int skinid;
extern void markervset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv, DWORD col);
extern void linevset(Pos pos1, Pos pos2, double sx, double size, double angl, double *tu, double *tv, DWORD col);
extern void packetvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv, DWORD col);

extern void sizegraphvset(Pos pos, double height, double tu, double *tv);
extern void sizegraphiset();
extern void sizegraphvsetsq(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv, DWORD col);
extern void sizegraphbgvsetsq(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv, DWORD col);

extern void protocolgraphvset(Pos pos, double rad, double angl, double startangl, double *tu, double *tv, DWORD col);
extern void protocolgraphsqvset(Pos pos, double sx, double sy, double *tu, double *tv, DWORD col);
extern void protocolgraphsqvset(Pos pos, double sx, double sy, double *tu, double *tv, DWORD col);

extern void buttonvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv);
extern void selectboxvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv);
extern void selectboxtxtvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv);
extern void textboxvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv);

extern void systemtxtvset(Pos pos, double sx, double sy, double size, double angl, double *tu, double *tv);

extern void graphicRelease();

extern HostIPDataMaster hdata;

extern PacketDataMasterclass pdata;

extern MarkerMasterclass marker;

extern LineMasterclass line;

extern PacketMasterclass packet;

extern RoutingMasterclass route;

extern Pos frontpos;

extern int screentexsize;
extern int frontx, fronty;

#endif
