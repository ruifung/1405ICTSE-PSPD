#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "simcon.h"
#include "menu.h"
#include "staffs.h"
#include "admin.h"

SCI_MENU * m_admin_edit = NULL;
UINT admin_target;

bool menu_admin_edit_callback(UINT);

SCI_MENU * menu_admin_edit(UINT target){
	if (m_admin_edit == NULL){
		SCI_MENU menu = menu_create("");
		m_admin_edit = malloc(sizeof(SCI_MENU));
		memmove(m_admin_edit, &menu, sizeof(SCI_MENU));
		menu_add_item(m_admin_edit, sci_item("View Details", &menu_admin_edit_callback));
		menu_add_item(m_admin_edit, sci_item("Edit Name", &menu_admin_edit_callback));
		menu_add_item(m_admin_edit, sci_item("Edit Username", &menu_admin_edit_callback));
		menu_add_item(m_admin_edit, sci_item("Change Password", &menu_admin_edit_callback));
		menu_add_item(m_admin_edit, sci_item("Change Permission", &menu_admin_edit_callback));
		menu_add_item(m_admin_edit, sci_item("Back", &menu_admin_edit_callback));
	}
	sprintf(m_admin_edit->title, "%04d: %s", staffs.data[target].id, staffs.data[target].username);
	admin_target = target;
	return m_admin_edit;
}

bool menu_admin_edit_callback(UINT index){
	STAFF * staff = &staffs.data[admin_target];
	char username[25] = "", pwd1[25] = "", pwd2[25] = "";
	switch (index){
	case 0:
		printf("%-10s%04d\n", "ID:", staff->id);
		printf("%-10s%s\n", "Name:", staff->name);
		printf("%-10s%s\n", "Username:", staff->username);
		printf("%-10s%s\n", "Admin:", staff->admin ? "Yes" : "No");
		pause();
		break;
	case 1:
		printf("Please enter a new name:\n");
		getstr(staff->name, 65, 0, false);
		break;
	case 2:
		printf("Please enter a new username:\n");
		strcpy(username, staff->username);
		getstr(username, 25, 0, false);
		if (strlen(username) < 5){
			printf("Username must be at least 5 characters!\n");
			pause();
		} else if (strcmp(staff->username, username) == 0){
			return true;
		} else if (staff_by_name(username) != NULL){
			printf("This username has be taken!");
			pause();
		} else {
			strcpy(staff->username, username);
		}
		break;
	case 3:
		pwd1[0] = pwd2[0] = 0;
		printf("%-10s", "Password:");
		getstr(pwd1, 25, '*', false);
		if (strlen(pwd1) < 8){
			printf("Password must be at least 8 characters!");
			pause();
			return true;
		}
		printf("%-10s", "Confirm:");
		getstr(pwd2, 25, '*', false);
		if (strcmp(pwd1, pwd2) != 0){
			printf("Password does't match!");
			pause();
		} else {
			staff_set_pwd(staff, pwd1);
		}
		break;
	case 4:
		printf("This account is %san administrator so far.\n", staff->admin ? "" : "not ");
		staff->admin = confirm("Is this account is an administrator?");
		break;
	case 5:
		menu_switch(menu_admin());
		return false;
		break;
	}
	staffs_save();
	return true;
}