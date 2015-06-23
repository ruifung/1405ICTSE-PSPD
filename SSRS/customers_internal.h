//FOR INTERNAL USE ONLY.
#ifndef CUSTOMERS_INTERNAL
#define CUSTOMERS_INTERNAL

#define cust_readDynamicLengthString(buffer,length,file) fread_s(buffer, length * sizeof(char), sizeof(char), length, file);

struct {
	unsigned int lastID;
	long int length;
	CUSTOMER *data;
} customers;


char *customer_file;

#endif
