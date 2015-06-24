#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"

#define MENU_STAFF_ITEMS_MAX 5

SCI_MENU * m_staff = NULL;

void menu_staff_pre();
bool menu_staff_admin_callback(UINT);
bool menu_staff_cpwd_callback(UINT);
bool menu_staff_callback(UINT);

SCI_ITEM menu_staff_items[MENU_STAFF_ITEMS_MAX];

SCI_MENU * menu_staff(){
	if (m_staff == NULL){
		SCI_MENU menu = menu_create("Sport Space Rental System");
		m_staff = malloc(sizeof(SCI_MENU));
		memcpy(m_staff, &menu, sizeof(SCI_MENU));
		m_staff->pre = &menu_staff_pre;
		strcpy(menu_staff_items[0].name, "Admin");
		menu_staff_items[0].callback = &menu_staff_admin_callback;
		strcpy(menu_staff_items[1].name, "Courts");
		menu_staff_items[1].callback = &menu_staff_callback;
		strcpy(menu_staff_items[2].name, "Change Password");
		menu_staff_items[2].callback = &menu_staff_cpwd_callback;
		strcpy(menu_staff_items[3].name, "Logout");
		menu_staff_items[3].callback = &menu_staff_callback;
		strcpy(menu_staff_items[4].name, "Exit");
		menu_staff_items[4].callback = &menu_staff_callback;
	}
	return m_staff;
}

void menu_staff_pre(){
	if (logged_staff == NULL){
		menu_switch(menu_guest());
		return;
	}
	if (logged_staff->admin){
		memmove(m_staff->items, menu_staff_items, MENU_STAFF_ITEMS_MAX * sizeof(SCI_ITEM));
		m_staff->size = MENU_STAFF_ITEMS_MAX;
	} else {
		memmove(m_staff->items, &menu_staff_items[1], (MENU_STAFF_ITEMS_MAX - 1) * sizeof(SCI_ITEM));
		m_staff->size = MENU_STAFF_ITEMS_MAX - 1;
	}
}

bool menu_staff_admin_callback(UINT index){
	menu_switch(menu_admin());
	return false;
}

bool menu_staff_cpwd_callback(UINT index){
	char opwd[25] = "", npwd[25] = "", rpwd[25] = "";
	printf("%-19s", "Original Password:");
	if (!getstr(opwd, 25, '*', false)) return true;
	if (strlen(opwd) < 8){
		printf("\nPassword must be at least 8 characters!\n");
			pause();
		return true;
	}
	printf("%-19s", "New Password:");
	if (!getstr(npwd, 25, '*', false)) return true;
	if (strlen(npwd) < 8){
		printf("\nPassword must be at least 8 characters!\n");
		pause();
		return true;
	}
	printf("%-19s", "Confirm Password:");
	if (!getstr(rpwd, 25, '*', false)) return true;
	if (strlen(rpwd) < 8){
		printf("\nPassword must be at least 8 characters!\n");
		pause();
		return true;
	}
	if (!staff_cmp_pwd(*logged_staff, opwd)){
		printf("\nOriginal password doesn't match!\n");
		pause();
		return true;
	}
	if (strcmp(npwd, rpwd) != 0){
		printf("\nTwo password doesn't match!\n");
		pause();
		return true;
	}
	staff_set_pwd(logged_staff, npwd);
	staffs_save();
	printf("\nPassword changed!\n");
	return true;
}

bool menu_staff_callback(UINT index){
	if (logged_staff->admin) index -= 1;
	switch (index){
	case 0:
		menu_switch(menu_courts());
		break;
	case 2:
		if (confirm("Are you sure to logout?")){
			logged_staff = NULL;
			printf("Logged out succesful!\n");
			pause();
		} else return true;
		break;
	case 3:
		if (confirm("Are you sure want to exit?")) menu_switch(NULL);
		else return true;
		break;
	}
	return false;
}