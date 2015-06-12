#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"
#include "admin.h"

SCI_MENU * m_admin_edit = NULL;
UINT target;

bool menu_admin_edit_callback(UINT);

SCI_MENU * menu_admin_edit(UINT target){
	if (m_admin_edit == NULL){
		SCI_MENU menu = menu_create("");
		m_admin_edit = malloc(sizeof(SCI_MENU));
		memmove(m_admin_edit, &menu, sizeof(SCI_MENU));
		menu_add_item(m_admin_edit, sci_item("Edit Name", &menu_admin_edit_callback));
		menu_add_item(m_admin_edit, sci_item("Edit Username", &menu_admin_edit_callback));
		menu_add_item(m_admin_edit, sci_item("Change Password", &menu_admin_edit_callback));
		menu_add_item(m_admin_edit, sci_item("Change Permission", &menu_admin_edit_callback));
		menu_add_item(m_admin_edit, sci_item("Cancel", &menu_admin_edit_callback));
	}
	sprintf("%04d: %s", staffs.data[target].id, staffs.data[target].name);
	return m_admin_edit;
}

bool menu_admin_edit_callback(UINT index){
	STAFF staff = staffs.data[target];
	char username[25] = "", pwd1[25] = "", pwd2[25] = "";
	switch (index){
	case 0:
		
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		printf("This account is %san administrator so far.\n", staff.admin ? "" : "not");
		staff.admin = confirm("Is this account is an administrator?");
		break;
	case 4:
		menu_switch(menu_admin());
		break;
	}
	staffs_save();
	return true;
}