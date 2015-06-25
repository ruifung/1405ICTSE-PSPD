#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>

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
			//printf("%-12s%-8s%-8s%-8s%-8s%-10s\n", "Sport", "Court", "Start", "End", "Rate", "Amount(RM)");
			printf("Sport       Court   Start   End     Rate    Amount(RM)\n");
			printf("------------------------------------------------------\n");
			RSVP_LINK * rsvp = pending->list;
			do{
				time_t start = rsvp->item->startTime * BLOCK_DURATION;
				time_t end = start + (rsvp->item->blockCount + 1 * BLOCK_DURATION);
				char s_str[6], e_str[6];
				float amount = rsvp->item->blockCount *
					courts[rsvp->item->court_id].rate;
				if (localtime(end)->tm_hour > 17){
					int charged = (localtime(end)->tm_hour - 17) / 2;
					charged += localtime(end)->tm_min / 30;
					amount += (charged * courts[rsvp->item->court_id].rate * 0.2);
				}
				strftime(s_str, 6, "%H:%M", localtime(start));
				strftime(e_str, 6, "%H:%M", localtime(end));
				printf("%-12s%-8c%-8s%-8s%7.2f %9.2f\n",
					courts_typeIDStr(courts[rsvp->item->court_id].type),
					courts[rsvp->item->court_id].label,
					s_str, e_str, courts[rsvp->item->court_id].rate,
					amount);
				rsvp = rsvp->next;
			} while (rsvp);
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

	case 3:
		if (courts_countRefReservations(pending->ref_num) > 0)
			if (!confirm("Are you sure to cancel this booking?")){
				courts_delRef(pending->ref_num);
				pending = NULL;
				return true;
			}
		menu_switch(menu_courts());
		break;
	}
	return false;
}