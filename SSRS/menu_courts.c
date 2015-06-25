#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>

#include "simcon.h"
#include "courts.h"
#include "menu.h"
#include "menu_courts.h"
#include "booking.h"

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
		getstr(booking_cache.customer, 65, 0, false);
		if (strlen(booking_cache.customer) == 0){
			printf("Invalid customer name!\n");
			pause();
			return true;
		}
		menu_switch(menu_courts_book());
		break;
	case 2:
		// just find and check the receipt by reference number from files,
		// my idea is save the receipts which generated into files in same folder
		// and the name is the ref no. so that we no need to handle the search by ourselves.
		break;
	case 3:
		menu_switch(menu_staff());
	}
	return false;
}