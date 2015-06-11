#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"

bool menu_admin_select_back(UINT);

SCI_MENU * m_admin_select = NULL;
SCI_ITEM ma_itm_prev, ma_itm_forw;
UINT admin_sel_page;

SCI_MENU * menu_admin_select(UINT page){
	if (m_admin_select == NULL){
		SCI_MENU menu = menu_create("Select a Staff");
		m_admin_select = malloc(sizeof(SCI_MENU));
		memmove(m_admin_select, &menu, sizeof(SCI_MENU));
		strcpy(m_admin_select->items[0].name, "Back");
		m_admin_select->items[0].callback = &menu_admin_select_back;
	}
	admin_sel_page = page;
	UINT start = (page * 17) + 1;
	UINT end = start + (page == 0 ? 18 : 17);
	for (; start <= end; start++){

	}
	return m_admin_select;
}

bool menu_admin_select_back(UINT index){
	menu_switch(menu_admin());
	return false;
}