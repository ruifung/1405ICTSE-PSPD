#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>

#include "conmac.h"
#include "simcon.h"
#include "courts.h"
#include "menu.h"
#include "menu_courts.h"

SCI_MENU * m_courts_book = NULL;

bool menu_courts_book_callback(UINT);

SCI_MENU * menu_courts_book(){
	if (m_courts_book == NULL){
		m_courts_book = malloc(sizeof(SCI_MENU));
		memset(m_courts_book, 0, sizeof(SCI_MENU));
		strcpy(m_courts_book->title, "Booking");
		menu_add_item(m_courts_book, sci_item("View Current Details", &menu_courts_book_callback));
		menu_add_item(m_courts_book, sci_item("Add a New Booking", &menu_courts_book_callback));
		menu_add_item(m_courts_book, sci_item("Checkout", &menu_courts_book_callback));
		menu_add_item(m_courts_book, sci_item("Cancel", &menu_courts_book_callback));
	}
	return m_courts_book;
}

bool menu_courts_book_callback(UINT index){
	switch (index){
	case 0:
		printf("Customer: %s\n\n", pending->customerName);
		if (courts_countRefReservations(pending->ref_num) > 0){
			courts_printRefDetails(stdout, pending->ref_num);
		} else {
			printf("No any bookings yet.\n");
		}
		pause();
		return true;
	case 1:
		courts_action = COURTS_ACTION_PLACE;
		menu_switch(menu_courts_select());
		break;
	case 2:
		if (courts_countRefReservations(pending->ref_num) == 0){
			printf("Nothing to checkout!\n");
			pause();
			return true;
		};
		//courts_save(); wont save until ruifung fix it
		clrscr;
		courts_printRefReceipt(stdout, pending->ref_num);
		if (confirm("\nDo you wish to print out the receipt?")){
			system("if not exist \"receipts\" (mkdir receipts>nul)");
			char path[22];
			char cmd[36];
			sprintf(path, "receipts/%08X.txt", pending->ref_num);
			FILE * file_ptr = fopen(path, "w");
			if (file_ptr != NULL){
				courts_printRefReceipt(file_ptr, pending->ref_num);
				fclose(file_ptr);
				sprintf(cmd, "notepad \"%s\">nul", path);
				system(cmd);
			} else _pause("ERROR: Failed to create %s", path);
		}
		menu_switch(menu_courts());
		break;
	case 3:
		if (courts_countRefReservations(pending->ref_num) > 0)
			if (!confirm("Are you sure to cancel this booking?"))
				return true;
		courts_delRef(pending->ref_num);
		pending = NULL;
		menu_switch(menu_courts());
		break;
	}
	return false;
}