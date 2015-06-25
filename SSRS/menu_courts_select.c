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

SCI_MENU * m_courts_sel = NULL;

bool menu_courts_sel_callback(UINT);
bool menu_courts_sel_back(UINT);

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

	return false;
}

bool menu_courts_sel_back(UINT index){
	if(courts_action == COURTS_ACTION_CHECK)menu_switch(menu_courts());
	else menu_switch(menu_courts_book());
	return false;
}