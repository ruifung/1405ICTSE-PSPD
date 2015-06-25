#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#include "bst.h"
#include "courts.h"
#include "staffs.h"
#include "simcon.h"

#define reservations_read(buffer,elemSize,count,file) fread_s(buffer, count * elemSize, elemSize, count, file);

//Structs and types for internal use.

COURT courts[COURTS_COUNT];
char *reservations_file;
struct {
	uint lastID;
	uint length;
	RESERVATION **data;
} reservations;
struct {
	uint lastRef;
	uint refLen;
	RSVP_REF *rsvpRef;
} references;

int courts_cmpr(BST_CMPR_ARGS);
bool courts_isBlockRangeEmptyRecur(BST_NODE *node, uint lowerBlock, uint upperBlock);
uint courts_getReservationIndex(uint id);
void courts_refLinkRecur(RSVP_LINK **link, RSVP_LINK *parent, RESERVATION *item);
int courts_cmprRef(const RSVP_REF *item1, const RSVP_REF *item2);
uint courts_getRefItemIndex(uint ref);

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
	references.lastRef = 0;
	references.refLen = 0;
	references.rsvpRef = malloc(sizeof(RSVP_REF));
	reservations.lastID = 0;
	reservations.length = 0;
	reservations.data = malloc(sizeof(RESERVATION*));
	reservations_file = reservationsFile;
}

_Bool courts_load() {
	uint refCurLen = 0;
	bool refSet = false;
	RESERVATION *tmpPtr = NULL;
	//VERIFY FILE IS VALID well.. file.
	DWORD attr = GetFileAttributes(reservations_file);
	if ((attr == INVALID_FILE_ATTRIBUTES)) {
		DWORD err = GetLastError();
		if (err == ERROR_FILE_NOT_FOUND)
			return true;
	}
	if (attr & FILE_ATTRIBUTE_DIRECTORY) {
		_pause("ERROR:%s is a folder!", reservations_file);
		return false;
	}
	FILE *file = fopen(reservations_file, "rb");
	if (file == NULL)
		return false;

	//Load reference number data
	reservations_read(&references.lastRef, sizeof(uint), 1, file);
	reservations_read(&references.refLen, sizeof(uint), 1, file);
	if (references.refLen > 0)
		references.rsvpRef = realloc(references.rsvpRef, references.refLen * sizeof(RSVP_REF));
	if (references.rsvpRef == NULL)
		return false;
	for (uint i = 0; i < references.refLen; i++) {
		RSVP_REF tmpRef = { 0 };
		uint nameLen = 0;
		reservations_read(&tmpRef.ref_num, sizeof(uint), 1, file);
		reservations_read(&tmpRef.date, sizeof(time_t), 1, file);
		reservations_read(&tmpRef.staff_id, sizeof(uint), 1, file);
		reservations_read(&nameLen, sizeof(uint), 1, file);
		tmpRef.customerName = malloc(nameLen);
		reservations_read(tmpRef.customerName, nameLen, 1, file);
		tmpRef.list = NULL;
		references.rsvpRef[i] = tmpRef;
	}

	//Load reservations
	reservations_read(&reservations.lastID, sizeof(uint), 1, file);
	reservations_read(&reservations.length, sizeof(uint), 1, file);
	if (reservations.length > 0)
		reservations.data = realloc(reservations.data, reservations.length * sizeof(tmpPtr));
	if (reservations.data == NULL)
		return false;
	for (uint i = 0; i < reservations.length; i++) {
		refSet = false;
		tmpPtr = malloc(sizeof(RESERVATION));
		if (tmpPtr == NULL)
			return false;
		reservations_read(tmpPtr, sizeof(RESERVATION), 1, file);
		reservations.data[i] = tmpPtr;
		bst_addNode(&courts[tmpPtr->court_id].reservations,&(tmpPtr->startTime),sizeof(uint),tmpPtr,&courts_cmpr);
		RSVP_REF *ref = courts_getRefItem(tmpPtr->ref_num);
		if (ref == NULL) return false;
		courts_refLinkRecur(&ref->list, NULL, tmpPtr);
	}

	if (reservations.length > 0)
		qsort(references.rsvpRef, references.refLen, sizeof(RSVP_REF), &courts_cmprRef);
	if (tmpPtr != NULL)
		free(tmpPtr);
	fclose(file);
	return true;
}

_Bool courts_save() {
	DWORD attr = GetFileAttributes(reservations_file);
	if (attr == INVALID_FILE_ATTRIBUTES) {
		//DO NOTHING IF FILE NOT FOUND.. We are going to write the file anyway.
		if (GetLastError() == ERROR_FILE_NOT_FOUND);
		else return false;
	} else if (attr & FILE_ATTRIBUTE_DIRECTORY) {
		_pause("ERROR:%s is a folder!", reservations_file);
		return false;
	} 
	FILE *file = fopen(reservations_file, "wb");
	if (file == NULL)
		return false;
	//Save reference number data
	fwrite(&references.lastRef, sizeof(uint), 1, file);
	fwrite(&references.refLen, sizeof(uint), 1, file);
	references.rsvpRef = malloc(references.refLen * sizeof(RSVP_REF));
	for (uint i = 0; i < references.refLen; i++) {
		uint nameLen = (strlen(references.rsvpRef[i].customerName) + 1) * sizeof(char);
		fwrite(&references.rsvpRef[i].ref_num, sizeof(uint), 1, file);
		fwrite(&references.rsvpRef[i].date, sizeof(time_t), 1, file);
		fwrite(&references.rsvpRef[i].staff_id, sizeof(uint), 1, file);
		fwrite(&nameLen, sizeof(uint), 1, file);
		fwrite(references.rsvpRef[i].customerName, nameLen, 1, file);
	}
	//Save reservation data
	fwrite(&reservations.lastID, sizeof(uint), 1, file);
	fwrite(&reservations.length, sizeof(uint), 1, file);
	for (uint i = 0; i < reservations.length; i++) {
		fwrite(reservations.data[i], sizeof(RESERVATION), 1, file);
	}
	fclose(file);
	return true;
}

char* courts_typeIDStr(uint type) {
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
	uint middle;
	int cmpr;
	while (true) {
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
	BST_NODE *tmpArr = malloc(maxCount * sizeof(BST_NODE));
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
		for (uint blk = block;blk >= firstBlockOfDay;blk--) {
			node = bst_search(courts[courtId].reservations, &blk, sizeof(uint), &courts_cmpr);
			//If a reservation exists for that block...
			if (node != NULL) {
				RESERVATION *rsvpPtr = node->data;
				//Check if its ending block is AFTER the specified block.
				if ((rsvpPtr->startTime + rsvpPtr->blockCount - 1) >= block)
					break;
				else
					node = NULL;
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
		uint endBlock = rsvp->startTime + rsvp->blockCount - 1;
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

RESERVATION *courts_addReservation(uint ref_num, char courtId, uint startTime, uint blockCount) {
	RSVP_REF *ref = courts_getRefItem(ref_num);
	if (ref == NULL) return NULL;
	RESERVATION *rsvp = malloc(sizeof(RESERVATION));
	if (rsvp == NULL) return NULL;
	rsvp->id = ++reservations.lastID;
	rsvp->ref_num = ref_num;
	rsvp->court_id = courtId;
	rsvp->startTime = startTime;
	rsvp->blockCount = blockCount;
	RESERVATION **newData = realloc(reservations.data, ++reservations.length * sizeof(rsvp));
	if (newData == NULL) {
		free(rsvp);
		return NULL;
	}
	reservations.data[(++reservations.length) - 1] = rsvp;
	bst_addNode(&courts[courtId].reservations, &startTime, sizeof(startTime), rsvp, &courts_cmpr);
	courts_refLinkRecur(&ref->list,NULL,rsvp);
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
	//Handle reference numbers
	RSVP_REF *ref = courts_getRefItem(reservation->ref_num);
	if (ref != NULL) {
		RSVP_LINK *linkItem = ref->list;
		if (linkItem->item->id == reservation->id) {
			if (linkItem->next != NULL)
				linkItem->next->prev = linkItem->prev;
			if (linkItem->prev != NULL)
				linkItem->prev->next = linkItem->next;
			else {
				ref->list = linkItem->next;
			}
			free(linkItem);
		}
	}
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
	RESERVATION **newArr = NULL;
	uint newSize = --reservations.length;
	if (newSize > 0) {
		newArr = realloc(reservations.data, newSize * sizeof(newArr));
		if (newArr == NULL)
			return false;
		else
			reservations.data = newArr;
	}
	//Free unused reservation data.
	free(tmpData);
	return true;
}

int courts_cmpr(BST_CMPR_ARGS) {
	//Cast pointer type from void * to uint * and subtract.
	if (*(uint *)key1 < *(uint *)key2)
		return -1;
	else if (*(uint *)key1 > *(uint *)key2)
		return 1;
	else if (*(uint *)key1 == *(uint *)key2)
		return 0;
}

uint courts_getFirstBlockSlot(uint block, char courtId) {
	time_t now = (time_t)(block * BLOCK_DURATION);
	//time_t now = time(&blockTime);
	struct tm *date = localtime(&now);
	date->tm_hour = 0;
	date->tm_min = 0;
	date->tm_sec = 0;
	time_t start = mktime(date);
	return (start / BLOCK_DURATION) + courts[courtId].startBlock;
}

//Due to this function, there will never be a ref num of 0;
RSVP_REF *courts_newRef(char *custName, time_t date, uint staff_id) {
	uint ref_num = ++references.lastRef;
	uint nameLen = (strlen(custName) + 1) * sizeof(char);
	char *custName2 = malloc(nameLen);
	if (custName2 == NULL)
		return NULL;
	RSVP_REF refItem = { 0 };
	RSVP_REF *tmpArr = realloc(references.rsvpRef, ++references.refLen * sizeof(RSVP_REF));
	strcpy_s(custName2, nameLen, custName);
	//IF FAILED TO EXTEND ARRAY, UNDO INCREMENTS AND RETURN
	if (tmpArr == NULL) {
		references.lastRef--;
		references.refLen--;
		free(custName2);
		return NULL;
	}
	refItem = (RSVP_REF) { ref_num, date, staff_id, custName2, NULL };
	tmpArr[references.refLen - 1] = refItem;
	references.rsvpRef = tmpArr;
	return &references.rsvpRef[references.refLen - 1];
}

void courts_delRef(uint ref_num) {
	uint index = courts_getRefItemIndex(ref_num);
	RSVP_REF *ref = &references.rsvpRef[index];
	RSVP_LINK *linkItem,*tmpLink = NULL;
	if (ref->ref_num != ref_num)
		return;
	linkItem = ref->list;
	//Walk the linked list to the end.
	for (;;) {
		if (linkItem->next != NULL)
			linkItem = linkItem->next;
		else
			break;
	}
	//Walk it in reverse, freeing it as it goes along.
	while (linkItem != NULL) {
		tmpLink = linkItem;
		linkItem = linkItem->prev;
		free(tmpLink);
	}
	//Shift all array elements right of item to the left by 1
	for (uint i = index; i < references.refLen; i++) {
		if ((i + 1) < references.refLen) {
			references.rsvpRef[i] = references.rsvpRef[i + 1];
		}
	}
	//Free the reference item.
	free(ref->customerName);
	free(ref);
	//Shrink memory allocation
	uint newLen = --references.refLen;
	if (newLen > 0) {
		ref = realloc(references.rsvpRef, newLen *sizeof(RSVP_REF));
		if (ref != NULL)
			references.rsvpRef = ref;
	}
}

RSVP_REF *courts_getRefItem(uint ref) {
	uint index = courts_getRefItemIndex(ref);
	if (index == 0) {
		if (references.rsvpRef[index].ref_num != ref) 
			return NULL;
	}
	return &references.rsvpRef[index];
}

uint courts_getRefItemIndex(uint ref) {
	uint upperIndex = references.refLen - 1;
	uint lowerIndex = 0;
	uint middle;
	while (lowerIndex) {
		middle = lowerIndex + ((upperIndex - lowerIndex) / 2);
		if (references.rsvpRef[middle].ref_num < ref)
			lowerIndex = middle + 1;
		else if (references.rsvpRef[middle].ref_num > ref)
			upperIndex = middle - 1;
		else if (references.rsvpRef[middle].ref_num == ref)
			return middle;
		//If lowerIndex == upperIndex... nothing left to search.
		else if (lowerIndex == upperIndex)
			break;
	}
	return 0;
}
uint courts_countRefReservations(uint ref) {
	uint count = 0;
	RSVP_LINK *curLink = NULL;
	RSVP_REF *refItem = courts_getRefItem(ref);
	if (refItem == NULL) {
		return 0;
	}
	curLink = refItem->list;
	while (curLink != NULL) {
		count++;
		curLink = curLink->next;
	}
	return count;
}

void courts_refLinkRecur(RSVP_LINK **link, RSVP_LINK *parent, RESERVATION *item) {
	if (*link == NULL) {
		*link = malloc(sizeof(RSVP_LINK));
		(*link)->item = item;
		(*link)->next = NULL;
		(*link)->prev = parent;
	}
	else {
		courts_refLinkRecur(&(*link)->next,*link,item);
	}
}

int courts_cmprRef(const RSVP_REF *item1, const RSVP_REF *item2) {
	return item1->ref_num - item2->ref_num;
}

bool courts_printRefReceipt(FILE * file, uint ref_num){
	RSVP_REF * ref = courts_getRefItem(ref_num);
	if (ref == NULL) return false;
	char date[11];
	strftime(date, 11, "%Y-%m-%d", localtime(&ref->date));
	fprintf(file, "Genome Sport Centre                              Bill Code: %08X\n", ref_num);
	fprintf(file, "No. 123, Sri Petaling,\n");
	fprintf(file, "Kuala Lumpur\n");
	fprintf(file, "Tel: 03-89119223\n");
	fprintf(file, "Staff: %s\n", staff_by_id(ref->staff_id)->name);
	fprintf(file, "                                                    Date: %s\n", date);
	fprintf(file, "Customer Name: %s\n", ref->customerName);
	fprintf(file, "====================================================================\n");
	courts_printRefDetails(file, ref_num);
	fprintf(file, "====================================================================\n");
	fprintf(file, "  This is a computer-generated receipt, No sign or stamp required.\n");
	return true;
}

bool courts_printRefDetails(FILE * file, uint ref_num){
	RSVP_REF * ref = courts_getRefItem(ref_num);
	if (ref == NULL) return false;
	if (courts_countRefReservations(ref->ref_num) == 0) return false;
	float total = 0.0f;
	fprintf(file, "Sport       Court   Date        Start   End     Rate(RM)  Amount(RM)\n");
	fprintf(file, "--------------------------------------------------------------------\n");
	RSVP_LINK * rsvp = ref->list;
	do{
		time_t start = rsvp->item->startTime * BLOCK_DURATION;
		time_t end = (rsvp->item->startTime + rsvp->item->blockCount) *
			BLOCK_DURATION;
		char s_str[6], e_str[6], d_str[11];
		float amount = rsvp->item->blockCount *
			courts[rsvp->item->court_id].rate;
		if (localtime(&end)->tm_hour >= 17){
			int charged = (localtime(&end)->tm_hour - 17) * 2;
			charged += localtime(&end)->tm_min / 30;
			amount += (charged * courts[rsvp->item->court_id].rate * 0.2);
		}
		strftime(s_str, 6, "%H:%M", localtime(&start));
		strftime(e_str, 6, "%H:%M", localtime(&end));
		strftime(d_str, 11, "%Y-%m-%d", localtime(&end));
		fprintf(file, "%-12s%-8c%-12s%-8s%-8s%9.2f %10.2f\n",
			courts_typeIDStr(courts[rsvp->item->court_id].type),
			courts[rsvp->item->court_id].label, d_str,
			s_str, e_str, courts[rsvp->item->court_id].rate * 2,
			amount);
		total += amount;
		rsvp = rsvp->next;
	} while (rsvp != NULL);
	fprintf(file, "====================================================================\n");
	fprintf(file, "%48s%10s%10.2f\n", "", "Total:", total);
	return true;
}