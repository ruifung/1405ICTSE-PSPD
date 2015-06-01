#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"

#define MENU_STAFF_ITEMS_MAX 4

SCI_MENU * m_staff = NULL;

void menu_staff_pre();
bool menu_staff_admin_callback(UINT);
bool menu_staff_callback(UINT);

SCI_ITEM menu_staff_items[MENU_STAFF_ITEMS_MAX];

SCI_MENU * menu_staff(){
	if (m_staff == NULL){
		SCI_MENU menu = menu_create("Sport Space Rental System");
		m_staff = malloc(sizeof(SCI_MENU));
		memcpy(m_staff, &menu, sizeof(SCI_MENU));
		m_staff->pre = &menu_staff_pre;
		strcpy(menu_staff_items[0].name, "Staffs");
		menu_staff_items[0].callback = &menu_staff_admin_callback;
		strcpy(menu_staff_items[1].name, "Courts");
		menu_staff_items[1].callback = &menu_staff_callback;
		strcpy(menu_staff_items[2].name, "Logout");
		menu_staff_items[2].callback = &menu_staff_callback;
		strcpy(menu_staff_items[3].name, "Exit");
		menu_staff_items[3].callback = &menu_staff_callback;
	}
	return m_staff;
}

void menu_staff_pre(){
	if (logged_staff == NULL){
		menu_switch(menu_guest());
		return;
	}
	if (logged_staff->admin){
		memmove(m_staff->items, menu_staff_items, 4 * sizeof(SCI_ITEM));
		m_staff->size = 4;
	} else {
		memmove(m_staff->items, &menu_staff_items[1], 3 * sizeof(SCI_ITEM));
		m_staff->size = 3;
	}
}

bool menu_staff_admin_callback(UINT index){

}

bool menu_staff_callback(UINT index){
	if (logged_staff->admin) index -= 1;
	switch (index){
	case 0:
		return true;
		break;
	case 1:
		if (confirm("Are you sure to logout?")){
			logged_staff = NULL;
			printf("Logged out succesful!\n");
			pause();
		} else return true;
		break;
	case 2:
		if (confirm("Are you sure want to exit?")) menu_switch(NULL);
		else return true;
		break;
	}
	return false;
}