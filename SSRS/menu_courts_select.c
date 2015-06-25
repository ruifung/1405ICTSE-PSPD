#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>

#include "simcon.h"
#include "menu.h"
#include "courts.h"
#include "menu_courts.h"
#include "conmac.h"

SCI_MENU * m_courts_sel = NULL;

bool menu_courts_sel_callback(UINT);
bool menu_courts_sel_back(UINT);
void print_slots();
void book_slot(uint);
void check_slot(uint);

SCI_MENU * menu_courts_select(){
	if (m_courts_sel == NULL){
		m_courts_sel = malloc(sizeof(SCI_MENU));
		memset(m_courts_sel, 0, sizeof(SCI_MENU));
		strcpy(m_courts_sel->title, "Please select a court");
		for (int i = 0; i < COURTS_COUNT; i++){
			char name[31];
			SCI_ITEM item = sci_item("", &menu_courts_sel_callback);
			sprintf(item.name, "%s Court %c", courts_typeIDStr(courts[i].type), courts[i].label);
			menu_add_item(m_courts_sel, item);
		}
		menu_add_item(m_courts_sel, sci_item("Back", &menu_courts_sel_back));
	}
	return m_courts_sel;
}

bool menu_courts_sel_callback(UINT index){
	courts_selected = index;
	date_selected = time(NULL); // get the time_t which represent the time now
	printf("Please select a date:");
	struct tm * date = localtime(&date_selected), today;
	date->tm_hour = 0;
	date->tm_min = 0;
	date->tm_sec = 0;
	memmove(&today, date, sizeof(struct tm));
	getdate(date, false);
	if (mktime(date) < mktime(&today) && courts_action == COURTS_ACTION_PLACE){
		printf("You can't book from a date before today!\n");
		pause();
		return true;
	}
	if (date->tm_wday == 1){ // 0 is sunday, 6 is saturday
		printf("Sorry, Monday is not available for any booking.\n");
		pause();
		return true;
	}
	date_selected = mktime(date);
	clrscr;
	char date_str[30];
	strftime(date_str, 30, "%Y-%m-%d (%A)", date);
	printf("Availability of %s Court %c for %s:\n\n", 
		courts_typeIDStr(courts[courts_selected].type),
		courts[courts_selected].label, date_str);
	print_slots();
	printf("\nPlease select a slot for %s, choose 0 or press escape to cancel.\n", 
		courts_action == COURTS_ACTION_CHECK ? "details" : "booking");
	printf("Slot: ");
	int slot = getint(2, true);
	if (slot <= 0) return true;
	if (slot > courts[courts_selected].endBlock - courts[courts_selected].startBlock){
		printf("Invalid slot!\n");
		pause();
	}
	if (courts_action == COURTS_ACTION_PLACE) book_slot(slot);
	else check_slot(slot);
	return false;
}

bool menu_courts_sel_back(UINT index){
	if(courts_action == COURTS_ACTION_CHECK)menu_switch(menu_courts());
	else menu_switch(menu_courts_book());
	return false;
}

void print_slots(){
	COURT * court = &courts[courts_selected];
	uint block = courts_getFirstBlockSlot(date_selected / BLOCK_DURATION, courts_selected);
	uint half = (court->endBlock - court->startBlock + 1) / 2;
	time_t t;
	char cache[2][6];
	void * available;
	for (uint i = 0; i < half; i++){
		t = (block + i) * BLOCK_DURATION;
		available = courts_getBlockReservation(courts_selected, block + i);
		strftime(cache[0], 6, "%H:%M", localtime(&t));
		t = (block + i + 1) * BLOCK_DURATION;
		strftime(cache[1], 6, "%H:%M", localtime(&t));
		printf(" %2d. %s to %s - %-13s   ", i + 1, cache[0], cache[1], (!available) ? "Empty" : "Reserved");
		int h = half + i;
		if (court->startBlock + h < court->endBlock){
			t = (block + h) * BLOCK_DURATION;
			available = courts_getBlockReservation(courts_selected, block + h) == NULL;
			strftime(cache[0], 6, "%H:%M", localtime(&t));
			t = (block + h + 1) * BLOCK_DURATION;
			strftime(cache[1], 6, "%H:%M", localtime(&t));
			printf("%2d. %s to %s - %-13s\n", h + 1, cache[0], cache[1], available ? "Empty" : "Reserved");
		}
	}
}

void check_slot(uint slot){
	uint block = courts_getFirstBlockSlot(date_selected / BLOCK_DURATION,
		courts_selected) + slot - 1;
	RESERVATION * res = courts_getBlockReservation(courts_selected, block);
	if (res != NULL){
		char cache[6];
		RSVP_REF * ref = courts_getRefItem(res->ref_num);
		printf("Customer Name: %s\n", ref->customerName);
		printf("Bill Code:     %08X", ref->ref_num);
		time_t t = res->startTime * BLOCK_DURATION;
		strftime(cache, 6, "%H:%M", localtime(&t));
		printf("Start Time:    %s\n", cache);
		t = (res->startTime + res->blockCount + 1) * BLOCK_DURATION;
		strftime(cache, 6, "%H:%M", localtime(&t));
		printf("End Time:      %s\n", cache);
		printf("Duration:      %d hour(s) %d minute(s)\n",
			res->blockCount / 2, (res->blockCount % 1) * 30);
	} else {
		printf("This slot is currently empty.\n");
	}
	pause();
}

void book_slot(uint slot){
	COURT * c_sel = &courts[courts_selected];
	uint block = courts_getFirstBlockSlot(date_selected / BLOCK_DURATION,
		courts_selected) + slot - 1;
	void * r = courts_getBlockReservation(courts_selected, block);
	if (r != NULL){
		printf("This slot is not available!\n");
		pause();
		return;
	}
	uint max = 1;
	while (true){
		r = courts_getBlockReservation(courts_selected, block + max);
		if (r != NULL) break;
		if (c_sel->startBlock + slot + max - 1 >=
			c_sel->endBlock) break;
		max++;
	}
	printf("Please enter a duration(count as number of slot),\n");
	printf("The maximum number of slot you can book is %d: ", max);
	uint duration = getint(2, false);
	if (duration == 0 || duration > max){
		printf("Invalid duration\n");
		return;
	}
	clrscr;
	printf("Reservation:");
	time_t start = block * BLOCK_DURATION;
	time_t end = (block + duration) * BLOCK_DURATION;
	char s_str[6], e_str[6], d_str[11];
	strftime(s_str, 6, "%H:%M", localtime(&start));
	strftime(e_str, 6, "%H:%M", localtime(&end));
	strftime(d_str, 11, "%Y-%m-%d", localtime(&start));
	printf("\nDate:          %s\n", d_str);
	printf("Court:         %s Court %c", courts_typeIDStr(c_sel->id), c_sel->label);
	printf("Start Time:    %s\n", s_str);
	printf("End Time:      %s\n", e_str);
	printf("Duration:      %d hour(s) %d minute(s)\n", duration / 2, (duration % 1) * 30);
	printf("Rate per Hour: RM %.2f\n", c_sel->rate * 2);
	float price = duration * c_sel->rate;
	if (localtime(&end)->tm_hour >= 17){
		int charged = (localtime(&end)->tm_hour - 17) * 2;
		charged += localtime(&end)->tm_min / 30;
		price += (charged * c_sel->rate * 0.2);
	}
	printf("Amount:        RM %.2f\n", price);
	printf("\nNote: An additional 20%% will be charged for those slot during peak hours(on 5PM onwards daily).\n");
	if (confirm("Confirm add this into booking list?")){
		RESERVATION *rsvp = courts_addReservation(pending->ref_num, courts_selected, block, duration);
	}
}