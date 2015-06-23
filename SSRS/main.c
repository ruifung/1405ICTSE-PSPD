#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"
#include "resource.h"
#include "conmac.h"

void setup();

int main(int argc, char * args[]){
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
	
	while (!staffs_init()) setup();
	menu_start(menu_guest());
}

void setup(){
	clrscr;
	SetConTextAttr(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("WARNING: This program haven't setup, please setup before use!\n");
	SetConTextAttr(FOREGROUND_WHITE);
	_pause("Press any key to start setup.");
	clrscr;
	char name[65] = "Administrator", user[25] = "admin", pass[25] = "", temp[25] = "";
	printf("Please enter a name for Admin:\n");
	getstr(name, 65, 0, false);
	printf("\nPlease enter a username for Admin (for login):\n");
	getstr(user, 25, 0, false);
	if (strlen(user) < 5){
		SetConTextAttr(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("\nThe username must be at least 5 characters!\n");
		SetConTextAttr(FOREGROUND_WHITE);
		pause();
		return;
	}
	printf("\nPlease enter a password for Admin (at least 8 characters):\n");
	getstr(pass, 25, '*', false);
	if (strlen(pass) < 8){
		SetConTextAttr(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("\nThe password must be at least 8 characters!\n");
		SetConTextAttr(FOREGROUND_WHITE);
		pause();
		return;
	}
	printf("\nPlease confirm the password:\n");
	getstr(temp, 25, '*', false);
	_putch('\n');
	if (strcmp(pass, temp)){
		SetConTextAttr(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("\nTwo password doesn't match!\n");
		SetConTextAttr(FOREGROUND_WHITE);
		pause();
		return;
	}
	STAFF admin = {0};
	admin.id = 1;
	admin.admin = true;
	strcpy(admin.name, name);
	strcpy(admin.username, user);
	admin.key = staff_cal_key(admin.username, pass);
	staffs_push(admin);
	staffs_save();
}