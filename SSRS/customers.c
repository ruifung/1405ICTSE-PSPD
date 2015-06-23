#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "customers.h"
#include "bst.h"

//INTERNAL USE STUFF.
#define cust_readDynamicLengthString(buffer,length,file) fread_s(buffer, length * sizeof(char), sizeof(char), length, file);
//INTERNAL USE GLOBAL VARIABLES.
struct {
	unsigned int lastID;
	long int length;
	CUSTOMER *data;
} customers;
char *customer_file;
BST_NODE *nameSearch = NULL;


void customers_init(char* configFile) {
	customer_file = configFile;
	customers_readfile();
}

void customers_readfile() {
	FILE *file = fopen(customer_file, "rb");
	//Read fixed length variables.
	fscanf_s(file, "%u%ld", &customers.lastID, &customers.length);
	//Allocate memory for customers array.
	customers.data = malloc(customers.length * sizeof(CUSTOMER));
	//Read customers.
	for (long int i = 0; i < customers.length; i++) {
		customers.data[i] = customers_readcustomer(file);
	}
	fclose(file);
}

CUSTOMER customers_readcustomer(FILE *file) {
	CUSTOMER customer;
	char nameLen, emailLen;
	//Read fixed size data for single customer.
	fscanf_s(file, "%u%15c%c%c", &customer.id, &customer.contact_num, &nameLen, &emailLen);
	//Allocate memory for dynamic sized data.
	customer.name = malloc(nameLen * sizeof(char));
	customer.email = malloc(emailLen * sizeof(char));
	//Read dynamic sized data from file.
	cust_readDynamicLengthString(customer.name, nameLen, file);
	cust_readDynamicLengthString(customer.email, emailLen, file);
	return customer;
}

void customers_savefile() {
	FILE *file = fopen(customer_file, "wb");
	CUSTOMER customer;
	size_t nameLen, emailLen;
	fwrite(&customers.lastID, sizeof(unsigned int), 1, file);
	fwrite(&customers.length, sizeof(long int), 1, file);
	for (long int i = 0; i < customers.length; i++) {
		customer = customers.data[i];
		//Write fixed length variables.
		fwrite(&customer.id, sizeof(unsigned int), 1, file);
		fwrite(&customer.contact_num, sizeof(unsigned int), 1, file);
		//Calculate and write length of dyanmic length variables.
		nameLen = wcslen(customer.name) + 1;
		emailLen = wcslen(customer.email) + 1;
		fwrite(&nameLen, sizeof(char), 1, file);
		fwrite(&emailLen, sizeof(char), 1, file);
		//Write dynamic length data.
		fwrite(customer.name, nameLen * sizeof(char), 1, file);
		fwrite(customer.email, emailLen * sizeof(char), 1, file);
	}
	fclose(file);
}

CUSTOMER *customers_getById(unsigned int id) {

}

CUSTOMER *customers_getByName(char* name) {

}

CUSTOMER *customers_getByEmail(char* email) {

}

bool customers_add(CUSTOMER customer) {

}

bool customers_del(CUSTOMER customer) {

}