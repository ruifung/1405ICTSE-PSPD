#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"
#include "resource.h"
#include "conmac.h"

void setup();

int main(int argc, char * args[]){
	{
		SetConTextAttr(FOREGROUND_WHITE);
		SetConsoleTitle("Sport Space Rental System");
		HANDLE icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN));
		SendMessage(GetConsoleWindow(), WM_SETICON, ICON_SMALL, icon);
		char fname[] = "Consolas";
		CONSOLE_FONT_INFOEX finfo = { 0 };
		finfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		finfo.nFont = 11;
		finfo.dwFontSize = (COORD){ 10, 22 };
		finfo.FontWeight = FW_NORMAL;
		finfo.FontFamily = FF_DONTCARE;
		for (int i = 0; i <= strlen(fname); i++)finfo.FaceName[i] = fname[i];
		SetCurrentConsoleFontEx(StdoutHandle, true, &finfo);
	}
	if (!staffs_init()) setup();
	menu_start(menu_guest());
}

void setup(){
}