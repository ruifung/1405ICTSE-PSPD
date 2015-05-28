#include <stdio.h>
#include <Windows.h>

#include "simcon.h"
#include "menu.h"
#include "resource.h"

void setup();

int main(int argc, char * args[]){
	SetConsoleTitle("Sport Space Rental System");
	HANDLE icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN));
	SendMessage(GetConsoleWindow(), WM_SETICON, ICON_SMALL, icon);

	menu_start(menu_guest());
}

void setup(){

}