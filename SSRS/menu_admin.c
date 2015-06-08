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
void admin_add_staff();

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
		admin_add_staff();
		break;
	case 1:
		admin_action = ADMIN_ACTION_DELETE;
		menu_switch(menu_admin_select(0));
		break;
	case 2:
		admin_action = ADMIN_ACTION_EDIT;
		menu_switch(menu_admin_select(0));
		break;
	case 3:
		m_admin->size = 0;
		admin_login = false;
		menu_switch(menu_staff());
		break;
	}
	return false;
}

void admin_add_staff(){
	STAFF staff = { 0 };
	char pwd1[25] = "", pwd2[25] = "";
	printf("New staff's name:\n");
	if (!getstr(staff.name, 65, 0, true)){
		printf("\nYou canceled.\n");
		pause();
		return;
	}
	printf("\nNew staff's username:\n");
	if (!getstr(staff.username, 25, 0, true)){
		printf("\nYou canceled.\n");
		pause();
		return;
	}
	if (strlen(staff.username) < 5){
		printf("\nUsername must atleast 5 characters!\n");
		pause();
		return;
	}
	if (staff_by_name(staff.username) != NULL){
		printf("\nThis username has been taken!\n");
		pause();
		return;
	}
	printf("\nPassword for new staff: \n");
	if (!getstr(pwd1, 25, '*', true)){
		printf("\nYou canceled.\n");
		pause();
		return;
	}
	if (strlen(pwd1) < 8){
		printf("\nUsername must atleast 8 characters!\n");
		pause();
		return;
	}
	printf("\nConfirm password: \n");
	if (!getstr(pwd2, 25, '*', true)){
		printf("\nYou canceled.\n");
		pause();
		return;
	}
	if (strcmp(pwd1, pwd2) != 0){
		printf("\nPassword does't match!\n");
		pause();
		return;
	}
	staff_set_pwd(&staff, pwd1);
	staff.admin = confirm("\nIs this account is an administrator?");
	printf("\nNew Staff Account:\n");
	printf("%-10s%s\n", "Name:", staff.name);
	printf("%-10s%s\n", "Username:", staff.username);
	printf("%-10s%s\n", "Admin:", staff.admin ? "Yes" : "No");
	if (confirm("Are you sure to create this account?")){
		if (staffs_push(staff)){
			printf("Account created successful.\n");
			staffs_save();
		} else ("Failed to create account, maximum staff amount (100) reached!\n");
	}
	pause();
}