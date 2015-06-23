#ifndef CUSTOMERS
#define CUSTOMERS

typedef struct {
	unsigned int id;
	char contact_num[15];
	char* name;
	char* email;
} CUSTOMER;

void customers_init(char *configFile);
CUSTOMER *customers_getById(unsigned int id);
CUSTOMER *customers_getByName(char* name);
CUSTOMER *customers_getByEmail(char* email);
bool customers_add(CUSTOMER customer);
bool customers_del(CUSTOMER customer);

#endif
