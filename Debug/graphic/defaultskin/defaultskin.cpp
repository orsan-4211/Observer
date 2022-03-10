#include <WindowsX.h>
#include <tchar.h>
#include <crtdbg.h>
#include <iostream>


#include "../main.h"

#include "../skinmanager/skinmanager.h"


void defaultskinload() {
	//Skin_Reset();

	Skin_TexLoad(0, _T("defaultgraph\\00.png"));
	Skin_TexLoad(1, _T("defaultgraph\\01.png"));
	Skin_TexLoad(2, _T("defaultgraph\\02.png"));
	Skin_TexLoad(3, _T("defaultgraph\\03.png"));
	Skin_TexLoad(4, _T("defaultgraph\\04.png"));
	Skin_TexLoad(5, _T("defaultgraph\\05.png"));

	Skin_TexLoad(6, _T("defaultgraph\\text.png"));

	Skin_Settexid(0, 6);

	Skin_Settexid(1, 0);
	/*Skin_Settexuv(1, 0.0f, 0.3f, 0.101f, 0.4f);
	Skin_Settexmovuv(1, 0.0, 0.0, 0.0, 0.0);*/

	Skin_Settexid(2, 0);
	/*Skin_Settexuv(2, 0.0f, 1.0f, 0.0f, 0.1f);
	Skin_Settexmovuv(2, 0.0, 0.0, 0.0, 0.0);*/

	Skin_Settexid(3, 0);
	//Skin_Settexuv(3, 0.3f, 0.6f, 0.101f, 0.4f);
	//Skin_Settexmovuv(3, 0.0, 0.0, 0.0, 0.0);

	Skin_Settexid(5, 1);

	Skin_Settexid(9, 1);

	Skin_Settexid(19, 3);

	Skin_Settexid(25, 3);

	Skin_Settexid(26, 5);

	Skin_Settexid(27, 3);

	Skin_Settexid(28, 4);

	Skin_Settexid(29, 2);
}