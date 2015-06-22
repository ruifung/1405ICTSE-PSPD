#ifndef CUSTOMERS
#define CUSTOMERS

typedef struct {
	unsigned int id;
	char contact_num[15];
	wchar_t* name;
	wchar_t* email;
} CUSTOMER;

void customers_init(FILE customer_file);
CUSTOMER *customers_getById(unsigned int id);
CUSTOMER *customers_getByName(char* name);
CUSTOMER *customers_getByEmail(char* email);
_Bool customers_add(CUSTOMER customer);
_Bool customers_del(CUSTOMER customer);

#endif