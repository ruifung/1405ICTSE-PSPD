//FOR INTERNAL USE ONLY.
#ifndef CUSTOMERS_INTERNAL
#define CUSTOMERS_INTERNAL

#define cust_readDynamicLengthString(buffer,length,file) fread_s(buffer, length * sizeof(wchar_t), sizeof(wchar_t), length, file);

struct {
	unsigned int lastID;
	long int length;
	CUSTOMER *data;
} customers;

typedef struct {
	wchar_t *string;
	CUSTOMER *customer;
	CUSTOMER_BST_NODE *left;
	CUSTOMER_BST_NODE *right;
} CUSTOMER_BST_NODE;

char *customer_file;

#endif
