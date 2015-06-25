#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>

#include "simcon.h"
#include "staffs.h"
#include "courts.h"
#include "menu.h"
#include "menu_courts.h"

SCI_MENU * m_courts = NULL;

bool menu_courts_callback(UINT);

SCI_MENU * menu_courts(){
	if (m_courts == NULL){
		m_courts = malloc(sizeof(SCI_MENU));
		memset(m_courts, 0, sizeof(SCI_MENU));
		strcpy(m_courts->title, "Sports Space Rental System");
		menu_add_item(m_courts, sci_item("Check Availability", &menu_courts_callback));
		menu_add_item(m_courts, sci_item("Place a Booking", &menu_courts_callback));
		menu_add_item(m_courts, sci_item("Check Receipt", &menu_courts_callback));
		menu_add_item(m_courts, sci_item("Back", &menu_courts_callback));
	}
	return m_courts;
}

bool menu_courts_callback(UINT index){
	switch (index){
	case 0:
		courts_action = index; // 0 is check availability, 1 is place booking, same as defined macro
		menu_switch(menu_courts_select());
		break;
	case 1:
		printf("Customer name: ");
		char customer[65] = "";
		getstr(customer, 65, 0, false);
		if (strlen(customer) == 0){
			printf("Invalid customer name!\n");
			pause();
			return true;
		}
		pending = courts_newRef(customer, time(NULL), logged_staff->id);
		menu_switch(menu_courts_book());
		break;
	case 2:
		printf("Please enter the bill code for check:");
		char code[9] = "";
		getstr(code, 9, 0, false);
		char cache = 0;
		uint ref_num;
		if (sscanf(code, "%x%c", &ref_num, &cache) != 1){
			printf("Invalid bill code!\n");
			pause();
			return true;
		}
		RSVP_REF * ref = courts_getRefItem(ref_num);
		if (ref == NULL){
			printf("This code doesn't exist!\n");
			pause();
			return true;
		}
		courts_printRefReceipt(stdout, ref->ref_num);
		if (confirm("\nDo you wish to print out the receipt?")){
			system("if not exist \"receipts\" (mkdir receipts>nul)");
			char path[22];
			char cmd[36];
			sprintf(path, "receipts/%08X.txt", ref->ref_num);
			FILE * file_ptr = fopen(path, "w");
			if (file_ptr != NULL){
				courts_printRefReceipt(file_ptr, ref->ref_num);
				fclose(file_ptr);
				sprintf(cmd, "notepad \"%s\">nul", path);
				system(cmd);
			}
			else _pause("ERROR: Failed to create %s", path);
		}
		break;
	case 3:
		menu_switch(menu_staff());
	}
	return false;
}