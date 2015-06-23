#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#include "bst.h"
#include "simcon.h"
#include "courts.h"

#define reservations_read(buffer,elemSize,count,file) fread_s(buffer, count * elemSize, elemSize, count, file);
const int endianCheck = 1;
#define is_bigendian() ( (*(char*)&endianCheck) == 0 )

COURT courts[COURTS_COUNT];
char *reservations_file;
struct {
	uint length;
	uint lastID;
	RESERVATION **data;
} reservations;

void utoa(unsigned int uint, char *byteArray);
_Bool courts_load();
_Bool courts_save();
uint courts_getFirstBlockSlot(uint block, char courtId);
int courts_cmpr(BST_CMPR_ARGS);

void courts_init(char *reservationsFile){
	courts[0] = (COURT){ 0, COURT_TYPE_BATMINTON, 'A', 14, 44, 8.0f, NULL };
	courts[1] = (COURT){ 1, COURT_TYPE_BATMINTON, 'B', 14, 44, 8.0f, NULL };
	courts[2] = (COURT){ 2, COURT_TYPE_BATMINTON, 'C', 14, 44, 8.0f, NULL };
	courts[3] = (COURT){ 3, COURT_TYPE_BATMINTON, 'D', 14, 44, 8.0f, NULL };
	courts[4] = (COURT){ 4, COURT_TYPE_SQUASH, 'A', 20, 44, 5.0f, NULL };
	courts[5] = (COURT){ 5, COURT_TYPE_SQUASH, 'B', 20, 44, 5.0f, NULL };
	courts[6] = (COURT){ 6, COURT_TYPE_TENNIS, 'A', 16, 40, 15.0f, NULL };
	courts[7] = (COURT){ 7, COURT_TYPE_TENNIS, 'B', 16, 40, 15.0f, NULL };
	courts[8] = (COURT){ 8, COURT_TYPE_TENNIS, 'C', 16, 40, 15.0f, NULL };
	courts[9] = (COURT){ 9,  COURT_TYPE_FUTSAL, 'A', 14, 44, 20.0f, NULL };
	courts[10] = (COURT){ 10, COURT_TYPE_FUTSAL, 'B', 14, 44, 20.0f, NULL };
	courts[11] = (COURT){ 11, COURT_TYPE_FUTSAL, 'C', 14, 44, 20.0f, NULL };
	reservations_file = reservationsFile;
}

_Bool courts_load() {
	RESERVATION *tmpPtr = NULL;
	FILE *file = fopen(reservations_file, "rb");
	if (file == NULL)
		return false;
	reservations_read(&reservations.length, sizeof(long int), 1, file);
	if (reservations.length == 0)
		return true;
	reservations.data = malloc(reservations.length * sizeof(tmpPtr));
	if (reservations.data == NULL)
		return false;
	for (unsigned long int i = 0; i < reservations.length; i++) {
		tmpPtr = malloc(sizeof(RESERVATION));
		if (tmpPtr == NULL)
			return false;
		reservations_read(tmpPtr, sizeof(RESERVATION), 1, file);
		reservations.data[i] = tmpPtr;
		bst_addNode(&courts[tmpPtr->court_id].reservations,&(tmpPtr->startTime),sizeof(uint),tmpPtr,&courts_cmpr);
	}
	free(tmpPtr);
	fclose(file);
	return true;
}

_Bool courts_save() {
	return false;
}

char* courts_typeIDStr(int type) {
	switch (type) {
	case COURT_TYPE_BATMINTON:
		return COURT_NAME_BATMINTON;
	case COURT_TYPE_FUTSAL:
		return COURT_NAME_FUTSAL;
	case COURT_TYPE_SQUASH:
		return COURT_NAME_SQUASH;
	case COURT_TYPE_TENNIS:
		return COURT_NAME_TENNIS;
	default:
		return NULL;
	}
}

RESERVATION *courts_getReservation(unsigned int id) {
	uint upperIndex = reservations.length - 1;
	uint lowerIndex = 0;
	while (true) {
		
	}
}

unsigned int courts_countCourtReservations(char courtId) {
	return bst_countChilds(courts[courtId].reservations);
}

void courts_getCourtReservations(char courtId, RESERVATION **dataArray, unsigned int maxCount) {
	BST_NODE *tmpArr = malloc(maxCount * sizeof(tmpArr));
	bst_getChilds(courts[courtId].reservations,tmpArr,maxCount);
	for (unsigned int i = 0; i < maxCount; i++) {
		dataArray[i] = (RESERVATION *)tmpArr[i].data;
	}
	free(tmpArr);
}

RESERVATION *courts_getBlockReservation(char courtId, uint block) {
	//Get the first time block of that day. (Obviously bookings won't stretch past a day)
	uint firstBlockOfDay = courts_getFirstBlockSlot(block, courtId);
	//Search for any booking that starts on the specified time block.
	BST_NODE *node = bst_search(courts[courtId].reservations, &block, sizeof(uint), &courts_cmpr);
	//If no results...
	if (node == NULL) {
		//Check each block before it up to the first block of the day.
		for (uint blk = block;blk > firstBlockOfDay;blk--) {
			BST_NODE *tmpNode;
			tmpNode = bst_search(courts[courtId].reservations, &block, sizeof(uint), &courts_cmpr);
			//If a reservation exists for that block...
			if (tmpNode != NULL) {
				RESERVATION *rsvpPtr = tmpNode->data;
				//Check if its ending block is AFTER the specified block.
				if ((rsvpPtr->startTime + rsvpPtr->blockCount) >= block) {
					return rsvpPtr;
				}
			}
		}
	}
	if (node == NULL) return NULL;
	else return (RESERVATION *)node->data;
}

bool courts_checkBlockRange(unsigned int lowerBlock, unsigned int upperBlock) {
	return false;
}

RESERVATION *courts_addReservation(unsigned int customerId, char courtId, unsigned int startTime, unsigned int blockCount) {
	RESERVATION *rsvp = malloc(sizeof(RESERVATION));
	rsvp->id = ++reservations.lastID;
	rsvp->customer_id = customerId;
	rsvp->court_id = courtId;
	rsvp->startTime = startTime;
	rsvp->blockCount = blockCount;
	RESERVATION **newData = realloc(reservations.data, ++reservations.length * sizeof(rsvp));
	if (newData == NULL) return NULL;
	reservations.data[reservations.length - 1] = rsvp;
	return rsvp;
}

bool *courts_delReservation(RESERVATION *reservation) {
	return false;
}

int courts_cmpr(BST_CMPR_ARGS) {
	return *(uint *)key1 - *(uint *)key2;
}

uint courts_getFirstBlockSlot(uint block, char courtId) {
	time_t blockTime = (time_t)(block * BLOCK_DURATION);
	time_t now = time(&blockTime);
	struct tm *date = localtime(&now);
	date->tm_hour = 0;
	date->tm_min = 0;
	date->tm_sec = 0;
	getdate(date, false);
	time_t start = mktime(date);
	return (start / BLOCK_DURATION) + courts[courtId].startBlock;
}