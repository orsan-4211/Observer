#include <windows.h>
#include "Fps.h"

#pragma comment(lib,"winmm.lib")

double Fpschecker::Fpscheck(){
	if(fpsflame==0){
		fpsstarttime= timeGetTime();
	//CLOCKS_PER_SEC	
	}
	fpsflame++;
	if(fpsflame==60){
		fpsendtime= timeGetTime();
		fpsresult = 1000.0f / ((fpsendtime-fpsstarttime) / 60.0f);
		fpsflame=0;
	}
	
	return fpsresult;
}

double Fpschecker::RealtimeFps(clock_t runtime){
	double result;
	if(runtime!=0){
	result=60.0f*(runtime/1000.0f);
	}else{
		return 0;
	}
	return result;
}

void Runtimechecker::Countstart(){
	checkmemst=timeGetTime();
	//CLOCKS_PER_SEC
	return;
}

double Runtimechecker::Countresult(){
	checkmemet= timeGetTime();
	//CLOCKS_PER_SEC
	checkresult=double(checkmemet-checkmemst);
	return checkresult;
}