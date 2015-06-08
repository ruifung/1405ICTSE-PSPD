#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>

#include "simcon.h"
#include "staffs.h"

long header[] = { 2037411651, 1751607666, 808591476,
				  1126184241, 1700012107, 539782497,
				  543976513, 1751607634, 1377858420,
				  1919251301, 778331510 };

char table[] = { 21, 12, 9, 18, 2, 16, 19, 1, 19, 2, 23, 4, 15, 18, 18, 18, 16, 8, 14, 22, 23,
				 21, 5, 6, 0, 1, 8, 9, 14, 10, 5, 13, 18, 16, 20, 12, 4, 2, 11, 22, 7, 17, 19,
				 3, 6, 21, 15, 23 };

bool staffs_init(){
	DWORD attr = GetFileAttributes(STAFF_FILE);
	if ((attr == INVALID_FILE_ATTRIBUTES)){
		return 0;
	}
	if (attr & FILE_ATTRIBUTE_DIRECTORY){
		_pause("ERROR:%s is a folder!\n\nPress any key to exit.", "asdf",STAFF_FILE);
		exit(0);
	}
	staffs_load();
	return 1;
}

void staffs_save(){
	FILE *f = fopen(STAFF_FILE, "wb");
	if (NULL == f){
		_pause("ERROR:Cannot open %s!\n\nPress any key to exit.", STAFF_FILE);
		exit(0);
	}
	fwrite(header, sizeof(long), 11, f);
	fwrite(&staffs.length, sizeof(int), 1, f);
	fwrite(&staffs.last_id, sizeof(int), 1, f);
	fwrite(staffs.data, sizeof(STAFF), staffs.length, f);
	fclose(f);
}

void staffs_load(){
	FILE *f = fopen(STAFF_FILE, "rb");
	if (NULL == f){
		_pause("ERROR:Cannot open %s!\n\nPress any key to exit.", STAFF_FILE);
		exit(0);
	}
	char buffer[44];
	fread(buffer, sizeof(long), 11, f);
	if (memcmp(header, buffer, 44) != 0 ){
		_pause("ERROR:%s is corrupted!\n\nPress any key to exit.", STAFF_FILE);
		exit(0);
	}
	fread(&staffs.length, sizeof(int), 1, f);
	fread(&staffs.last_id, sizeof(int), 1, f);
	fread(staffs.data, sizeof(STAFF), staffs.length, f);
	fclose(f);
}

STAFF * staff_by_id(unsigned int id){
	for (unsigned int i = 0; i < staffs.length; i++) if (staffs.data[i].id == id) return &(staffs.data[i]);
	return NULL;
}

STAFF * staff_by_name(char name[]){
	for (unsigned int i = 0; i < staffs.length; i++) if (strcmp(staffs.data[i].username, name) == 0) return &(staffs.data[i]);
	return NULL;
}

_Bool staffs_push(STAFF n){
	if (staffs.length < (STAFF_LIMIT - 1)){
		memmove(&(staffs.data[staffs.length]), &n, sizeof(STAFF));
		staffs.data[staffs.length].id = ++staffs.last_id;
		staffs.length++;
		return 1;
	}
	return 0;
}

void staffs_delete(unsigned int index){
	if (index < staffs.length){
		memmove(
			&(staffs.data[index]),
			&(staffs.data[index + 1]),
			(staffs.length - (index + 1)) * sizeof(STAFF)
			);
		staffs.length--;
	}
}

_Bool staffs_insert(unsigned int index, STAFF n){
	if (staffs.length < (STAFF_LIMIT - 1)){
		STAFF cache[STAFF_LIMIT];
		memmove(cache, staffs.data, STAFF_LIMIT * sizeof(STAFF));
		memmove(&(staffs.data[index]), &n, sizeof(STAFF));
		memmove(
			&(staffs.data[index + 1]),
			&cache[index],
			(staffs.length - index) * sizeof(STAFF)
			);
		staffs.data[index].id = ++staffs.last_id;
		staffs.length++;
		return 1;
	}
	return 0;
}

unsigned int staff_cal_key(char user[], char pass[]){
	char pwd[25];
	strcpy(pwd, pass);
	memset(pwd + strlen(pwd), 0, 25 - strlen(pwd));
	memset(user + strlen(user), 0, 25 - strlen(user));
	for (int i = 0; i < 24; i++) pwd[i] ^= user[table[i]];
	short *st1 = (void*)pwd, st2[6];
	for (int i = 0; i < 6; i++) st2[i] = st1[i * 2] ^ st1[(i * 2) + 1];
	unsigned int *ii = (void*)st2;
	ii[0] = ii[0] << 13 | ii[0] >> 19;
	ii[1] = ii[1] << 10 | ii[1] >> 22;
	ii[2] = ii[2] << 20 | ii[2] >> 12;
	ii[0] ^= (ii[1] & ii[2]);
	ii[0] ^= 0xC574EB84;
	return ii[0];
}

void staff_set_pwd(STAFF *s, char pwd[]){
	s->key = staff_cal_key(s->username, pwd);
}

_Bool staff_cmp_pwd(STAFF s, char pwd[]){
	return (s.key == staff_cal_key(s.username, pwd));
}