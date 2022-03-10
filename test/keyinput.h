#ifndef INCLUDED_KEYINPUT
#define INCLUDED_KEYINPUT

extern int mainkeyup;
extern int mainkeydown;
extern int mainkeyleft;
extern int mainkeyright;

extern int mainkeyq;
extern int mainkeyw;
extern int mainkeyr;
extern int mainkey01;
extern int mainkey02;
extern int mainkey03;
extern int mainkeyesc;
extern int mainkeyctrl;
extern int mainkeyenter;

extern int keynumkey[10];
extern int keybackspace;

extern int mouseleft;
extern int mouseright;

extern int mousewheel;
extern POINT oldpos;

extern void Checkkeyinput();

#endif
