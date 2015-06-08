#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"

SCI_MENU * m_admin_select = NULL;
UINT admin_sel_page;

SCI_MENU * menu_admin_select(UINT page){
	if (m_admin_select == NULL){
		SCI_MENU menu = menu_create("Select a Staff");
		m_admin_select = malloc(sizeof(SCI_MENU));
		memmove(m_admin_select, &menu, sizeof(SCI_MENU));
	}
}