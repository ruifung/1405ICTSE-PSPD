#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"
#include "courts.h"
#include "resource.h"
#include "conmac.h"
#include "lang.h"

void setup();

int main(int argc, char * args[]){
	SetConTextAttr(FOREGROUND_WHITE);
	SetConsoleTitle(program_name);
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
	courts_init("reservations.dat");
	if (!courts_load()){
		printf(error_rsvp_load);
		_pause(msg_press_to_exit);
		return;
	}
	menu_start(menu_guest());
}

void setup(){
	clrscr;
	SetConTextAttr(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf(warn_nosetup);
	SetConTextAttr(FOREGROUND_WHITE);
	_pause(msg_start_setup);
	clrscr;
	char name[65] = "Administrator", user[25] = "admin", pass[25] = "", temp[25] = "";
	printf(prompt_name, "Admin");
	getstr(name, 65, 0, false);
	printf("\n");
	printf(prompt_username,name);
	getstr(user, 25, 0, false);
	if (strlen(user) < 5){
		SetConTextAttr(FOREGROUND_RED | FOREGROUND_INTENSITY);
		putch('\n');
		printf(warn_usermin);
		SetConTextAttr(FOREGROUND_WHITE);
		pause();
		return;
	}
	printf(prompt_password,name);
	getstr(pass, 25, '*', false);
	if (strlen(pass) < 8){
		SetConTextAttr(FOREGROUND_RED | FOREGROUND_INTENSITY);
		putch('\n');
		printf(warn_passmin);
		SetConTextAttr(FOREGROUND_WHITE);
		pause();
		return;
	}
	printf(prompt_confirmpass);
	getstr(temp, 25, '*', false);
	_putch('\n');
	if (strcmp(pass, temp)){
		SetConTextAttr(FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf(warn_pass_mismatch);
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