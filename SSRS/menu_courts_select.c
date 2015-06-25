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
void choose_slot(uint);
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
	if (courts_action == COURTS_ACTION_PLACE);// choose_slot(date);
	else;//check_slot();
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
	bool available;
	for (uint i = 0; i < half; i++){
		t = (block + i) * BLOCK_DURATION;
		available = courts_getBlockReservation(courts_selected, block + i) == NULL;
		strftime(cache[0], 6, "%H:%M", localtime(&t));
		t = (block + i + 1) * BLOCK_DURATION;
		strftime(cache[1], 6, "%H:%M", localtime(&t));
		printf(" %2d. %s to %s - %-13s   ", i + 1, cache[0], cache[1], available ? "Available" : "Not Available");
		int h = half + i;
		if (court->startBlock + h < court->endBlock){
			t = (block + h) * BLOCK_DURATION;
			available = courts_getBlockReservation(courts_selected, block + h) == NULL;
			strftime(cache[0], 6, "%H:%M", localtime(&t));
			t = (block + h + 1) * BLOCK_DURATION;
			strftime(cache[1], 6, "%H:%M", localtime(&t));
			printf("%2d. %s to %s - %sAvailable\n", h + 1, cache[0], cache[1], available ? "" : "Not");
		}
	}
}