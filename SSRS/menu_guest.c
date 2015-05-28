#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>

#include "simcon.h"
#include "menu.h"

SCI_MENU * m_guest = NULL;

bool menu_guest_callback(UINT);

SCI_MENU * menu_guest(){
	if (m_guest == NULL){
		SCI_MENU menu = menu_create("Sport Space Rental System");
		m_guest = malloc(sizeof(SCI_MENU));
		memcpy(m_guest, &menu, sizeof(SCI_MENU));
		SCI_ITEM item;
		strcpy_s(item.name, 25, "Login");
		item.callback = &menu_guest_callback;
		menu_add_item(m_guest, item);
		strcpy_s(item.name, 25, "Exit");
		menu_add_item(m_guest, item);
	}
	return m_guest;
}

bool menu_guest_callback(UINT index){
	switch (index){
	case 0:
		menu_switch(menu_login());
		return false;
		break;
	case 1:
		if (confirm("Are you sure want to exit?")) menu_switch(NULL);
		else return true;
	}
	return false;
}