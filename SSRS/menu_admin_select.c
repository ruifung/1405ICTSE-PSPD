#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"
#include "admin.h"

void menu_admin_selelect_pre();
bool menu_admin_select_back(UINT);
bool menu_admin_select_selected(UINT);
bool menu_admin_select_prev(UINT);
bool menu_admin_select_next(UINT);

SCI_MENU * m_admin_sel = NULL;
SCI_ITEM ma_itm_prev, ma_itm_forw;
UINT admin_sel_page;

SCI_MENU * menu_admin_select(UINT page){
	if (m_admin_sel == NULL){
		SCI_MENU menu = menu_create("Select a Staff");
		m_admin_sel = malloc(sizeof(SCI_MENU));
		memmove(m_admin_sel, &menu, sizeof(SCI_MENU));
		m_admin_sel->pre = &menu_admin_selelect_pre;
		strcpy(m_admin_sel->items[0].name, "Back");
		m_admin_sel->items[0].callback = &menu_admin_select_back;
	}
	m_admin_sel->size = 1;
	admin_sel_page = page;
	return m_admin_sel;
}

void menu_admin_selelect_pre(){
	m_admin_sel->size = 1;
	UINT start = (admin_sel_page * 7) + (admin_sel_page == 0 ? 0 : 1);
	UINT end = start + (admin_sel_page  == 0 ? 8 : 7);
	if (admin_sel_page != 0) menu_add_item(m_admin_sel, sci_item("Previous Page", &menu_admin_select_prev));
	for (; start < end && start < staffs.length; start++){
		SCI_ITEM itm = { "", &menu_admin_select_selected };
		sprintf(itm.name, "%04d: %s", staffs.data[start].id, staffs.data[start].username);
		menu_add_item(m_admin_sel, itm);
	}
	if (end < staffs.length - 2)menu_add_item(m_admin_sel, sci_item("Next Page", &menu_admin_select_next));
}

bool menu_admin_select_back(UINT index){
	menu_switch(menu_admin());
	return false;
}

bool menu_admin_select_prev(UINT index){
	admin_sel_page -= 1;
	return false;
}

bool menu_admin_select_next(UINT index){
	admin_sel_page += 1;
	return false;
}

bool menu_admin_select_selected(UINT index){
	UINT selected = (index - (admin_sel_page == 0 ? 1 : 2)) + (admin_sel_page * 7) + (admin_sel_page == 0 ? 0 : 1);
	STAFF staff = staffs.data[selected];
	printf("%-10s%04d\n", "ID:", staff.id);
	printf("%-10s%s\n", "Name:", staff.name);
	printf("%-10s%s\n", "Username:", staff.username);
	printf("%-10s%s\n\n", "Admin:", staff.admin ? "Yes" : "No");
	switch (admin_action){
	case ADMIN_ACTION_DELETE:
		if (selected == 0){
			printf("The owner's account cannot be deleted!\n");
			pause();
			return true;
		} else {
			if (confirm("Are you sure to delete this staff's account?")){
				staffs_delete(selected);
				staffs_save();
				printf("Account deleted successful!\n");
			}
			pause();
		}
		menu_switch(menu_admin());
		break;
	case ADMIN_ACTION_EDIT:
		if (selected == 0 && logged_staff->id != 1){
			printf("The owner's account only can be edited by owner!\n");
			pause();
			return true;
		}
		_pause("Press any key to edit this staff.");
		menu_switch(menu_admin_edit(selected));
		break;
	}
	return false;
}