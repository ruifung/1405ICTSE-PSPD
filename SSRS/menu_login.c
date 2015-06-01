#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "simcon.h"
#include "menu.h"

SCI_MENU * m_login = NULL;

void menu_login_pre();

SCI_MENU * menu_login(){
	if (m_login == NULL){
		SCI_MENU menu = menu_create("User Login");
		m_login = malloc(sizeof(SCI_MENU));
		memcpy(m_login, &menu, sizeof(SCI_MENU));
		m_login->pre = &menu_login_pre;
	}
	return m_login;
}

void menu_login_pre(){
	printf("Username: ");
	char un[25] = "";
	char pass[25] = "";
	if (!getstr(un, 25, 0, true)){
		printf("\nYou canceled.\n");
		pause();
		menu_switch(menu_guest());
		return;
	}
	printf("Password: ");
	if (!getstr(pass, 25, '*', true)){
		printf("\nYou canceled.\n");
		pause();
		menu_switch(menu_guest());
		return;
	}
	if (strlen(pass) < 8){
		printf("\nPassword must be at least 8 characters!\n");
		if (confirm("Do you wish to continue?")) return;
		pause();
		menu_switch(menu_guest());
		return;
	}
	
}