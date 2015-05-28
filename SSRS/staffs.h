#ifndef STAFF_HEADER			

#define STAFF_HEADER
#define STAFF_FILE "staffs.dat"	
#define STAFF_LIMIT 100

typedef struct STAFF{
	unsigned int id;
	char username[25];
	char name[65];
	char password[25];
	_Bool admin;
} STAFF;

typedef struct STAFFS{
	STAFF data[STAFF_LIMIT];
	unsigned int length;
	unsigned int last_id;
} STAFFS;

STAFFS staffs;
STAFF * logged_staff;

_Bool staffs_init();
void staffs_save();
void staffs_load();
void staffs_reset();
STAFF * staff_by_id(unsigned int);
STAFF * staff_by_name(char[]);
_Bool staffs_push(STAFF);
void staffs_delete(unsigned int);
_Bool staffs_insert(unsigned int, STAFF);

void staff_set_pwd(STAFF*, char[]);
_Bool staff_cmp_pwd(STAFF, char[]);

#endif