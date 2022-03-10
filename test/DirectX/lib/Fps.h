#ifndef INCLUDED_FPS
#define INCLUDED_FPS

#include <time.h>

class Fpschecker{
public:
 DWORD	fpsstarttime ,fpsendtime; 
 int		fpsflame;
 double		fpsresult;
 double		Fpscheck();
 double		RealtimeFps(clock_t runtime);
};
class Runtimechecker{
public:
 DWORD	checkmemst,checkmemet;
 double		checkresult;
 void		Countstart();
 double		Countresult();
};

#endif
