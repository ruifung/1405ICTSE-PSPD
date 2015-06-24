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
	uint lastID;
	uint length;
	RESERVATION **data;
} reservations;

void utoa(unsigned int uint, char *byteArray);
_Bool courts_load();
_Bool courts_save();
uint courts_getFirstBlockSlot(uint block, char courtId);
int courts_cmpr(BST_CMPR_ARGS);
bool courts_isBlockRangeEmptyRecur(BST_NODE *node, uint lowerBlock, uint upperBlock);
uint courts_getReservationIndex(uint id);

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
	reservations.lastID = 0;
	reservations.length = 0;
	reservations.data = NULL;
	reservations_file = reservationsFile;
	courts_load();
}

_Bool courts_load() {
	RESERVATION *tmpPtr = NULL;
	DWORD attr = GetFileAttributes(reservations_file);
	if ((attr == INVALID_FILE_ATTRIBUTES)) {
		return false;
	}
	if (attr & FILE_ATTRIBUTE_DIRECTORY) {
		_pause("ERROR:%s is a folder!", reservations_file);
		return false;
	}
	FILE *file = fopen(reservations_file, "rb");
	if (file == NULL)
		return false;
	reservations_read(&reservations.lastID, sizeof(uint), 1, file);
	reservations_read(&reservations.length, sizeof(uint), 1, file);
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
	DWORD attr = GetFileAttributes(reservations_file);
	if ((attr == INVALID_FILE_ATTRIBUTES)) {
		return false;
	}
	if (attr & FILE_ATTRIBUTE_DIRECTORY) {
		_pause("ERROR:%s is a folder!", reservations_file);
		return false;
	}
	FILE *file = fopen(reservations_file, "wb");
	if (file == NULL)
		return false;
	fwrite(&reservations.lastID, sizeof(uint), 1, file);
	fwrite(&reservations.length, sizeof(uint), 1, file);
	for (uint i = 0; i < reservations.length; i++) {
		fwrite(reservations.data[i], sizeof(RESERVATION), 1, file);
	}
	fclose(file);
	return true;
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

//Gets reservation by reservation id. Uses binary search.
RESERVATION *courts_getReservation(uint id) {
	uint index = courts_getReservationIndex(id);
	if (index == 0) {
		if (reservations.data[index]->id != id)
			return NULL;
	}
	return reservations.data[index];
}

//Can return 0 if not found, additional checks required.
uint courts_getReservationIndex(uint id) {
	uint upperIndex = reservations.length - 1;
	uint lowerIndex = 0;
	uint middle, cmpr;
	while (lowerIndex) {
		middle = lowerIndex + ((upperIndex - lowerIndex) / 2);
		cmpr = id - reservations.data[middle]->id;
		if (cmpr > 0)
			lowerIndex = middle + 1;
		else if (cmpr < 0)
			upperIndex = middle - 1;
		else if (cmpr == 0)
			return middle;
		//If lowerIndex == upperIndex... nothing left to search.
		else if (lowerIndex == upperIndex)
			break;
	}
	return 0;
}

unsigned int courts_countCourtReservations(char courtId) {
	return bst_countChilds(courts[courtId].reservations);
}

void courts_getCourtReservations(char courtId, RESERVATION **dataArray, unsigned int maxCount) {
	//Allocate temporary array.
	BST_NODE *tmpArr = malloc(maxCount * sizeof(tmpArr));
	//Get all reservations under courtId.
	bst_getChilds(courts[courtId].reservations,tmpArr,maxCount);
	//Copy only reservation data to storage array.
	for (unsigned int i = 0; i < maxCount; i++) {
		dataArray[i] = (RESERVATION *)tmpArr[i].data;
	}
	//Deallocate temporary array.
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

bool courts_isBlockRangeEmpty(char courtId, uint lowerBlock, uint upperBlock) {
	return courts_isBlockRangeEmptyRecur(courts[courtId].reservations,lowerBlock,upperBlock);
}

bool courts_isBlockRangeEmptyRecur(BST_NODE *node, uint lowerBlock, uint upperBlock) {
	RESERVATION *rsvp = node->data;
	bool left = false;
	bool right = false;
	//If the start time is in block range, return false;
	if (rsvp->startTime >= lowerBlock && rsvp->startTime <= upperBlock)
		return false;
	//If the start time is less then the lowerBlock given.
	if (rsvp->startTime <= lowerBlock) {
		//Check if endBlock is inside range.
		uint endBlock = rsvp->startTime + rsvp->blockCount;
		if (endBlock >= lowerBlock && endBlock <= upperBlock)
			return false;
	}
	//If its not, check left & right sides.
	//Check right only if current key is less then upperBlock
	if (node->right != NULL && *(uint *)node->key < upperBlock){
		if (*(uint *)(node->right->key) <= upperBlock)
			right = courts_isBlockRangeEmptyRecur(node->left, lowerBlock, upperBlock);
	}
	//Check left only if current key is greater then or equal to lowerBlock
	if (node->left != NULL && *(uint *)node->key > lowerBlock) {
		if (*(uint *)(node->left->key) <= upperBlock)
			left = courts_isBlockRangeEmptyRecur(node->left, lowerBlock, upperBlock);
	}
	return left || right;
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
	bst_addNode(courts[courtId].reservations, &startTime, sizeof(startTime), rsvp, &courts_cmpr);
	return rsvp;
}

bool courts_delReservation(RESERVATION *reservation) {
	//Look for the reservation entry in the tree
	BST_NODE *node = bst_search(courts[reservation->court_id].reservations, &reservation->startTime, sizeof(uint), &courts_cmpr);
	//If such a reservation is found in the tree.
	if (node != NULL) {
		//Check if the id matches.
		RESERVATION *rsvp = node->data;
		if (rsvp->id == reservation->id) {
			//Count number of childs under it.
			uint count = bst_countChilds(node);
			//Allocate temporary array to store childs.
			BST_NODE *tmpStore = malloc(count * sizeof(BST_NODE));
			//Get the childs.
			bst_getChilds(node, tmpStore, count);
			//Deallocate entire subtree.
			bst_deallocate(node);
			//Re-add childs to tree.
			for (uint i = 0; i < count; i++) {
				bst_addNode(&courts[reservation->court_id].reservations, tmpStore[i].key, tmpStore[i].keylen, tmpStore[i].data, &courts_cmpr);
			}
		}
	}
	else return false;
	//Look for reservation in global reservation array.
	uint index = courts_getReservationIndex(reservation->id);
	if (index == 0) {
		if (reservations.data[index]->id != reservation->id)
			return false;
	}
	//Copy pointer to variable.
	RESERVATION *tmpData = reservations.data[index];
	//Calculate length of data after index.
	uint len = (reservations.length - index - 1)*sizeof(reservations.data);
	//Shift elements after index to left by 1.
	memmove(&reservations.data[index],&reservations.data[index+1],len);
	//Shrink array memory size.
	RESERVATION **newArr = realloc(reservations.data, --reservations.length * sizeof(newArr));
	if (newArr == NULL)
		return false;
	else
		reservations.data = newArr;
	//Free unused reservation data.
	free(tmpData);
	return true;
}

int courts_cmpr(BST_CMPR_ARGS) {
	//Cast pointer type from void * to uint * and subtract.
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