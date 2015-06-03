#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"
#include "admin.h"

SCI_MENU * m_admin = NULL;

void menu_admin_pre();
bool menu_admin_callback(UINT);

SCI_MENU * menu_admin(){
	if (m_admin == NULL){
		SCI_MENU menu = menu_create("Admin");
		m_admin = malloc(sizeof(SCI_MENU));
		memcpy(m_admin, &menu, sizeof(SCI_MENU));
		m_admin->pre = &menu_admin_pre;
		SCI_ITEM item;
		item.callback = &menu_admin_callback;
		strcpy(item.name, "Add Staff");
		menu_add_item(m_admin, item);
		strcpy(item.name, "Remove Staff");
		menu_add_item(m_admin, item);
		strcpy(item.name, "Edit Staff");
		menu_add_item(m_admin, item);
		strcpy(item.name, "Back");
		menu_add_item(m_admin, item);
		m_admin->size = 0;
	}
	return m_admin;
}

void menu_admin_pre(){
	if (!admin_login){
		char pwd[25] = "";
		printf("Password: ");
		if (!getstr(pwd, 25, '*', true)){
			printf("\nYou canceled.\n");
			_pause("Press any key to go back.");
			menu_switch(menu_staff());
			return;
		}
		if (!staff_cmp_pwd(*logged_staff, pwd)){
			printf("\nWrong password!\n");
			_pause("Press any key to go back.");
			menu_switch(menu_staff());
			return;
		}
		m_admin->size = 4;
		admin_login = true;
	}
}

bool menu_admin_callback(UINT index){
	switch (index){
	case 0:
		break;
	case 1:
		admin_action = ADMIN_ACTION_DELETE;
		admin_selection_page = 0;
		
		break;
	case 2:
		admin_action = ADMIN_ACTION_EDIT;
		admin_selection_page = 1;

		break;
	case 3:
		m_admin->size = 0;
		admin_login = false;
		menu_switch(menu_staff());
		break;
	}
	return false;
}