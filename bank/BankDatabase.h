#ifndef BANKDATABASE_H_INCLUDED
#define BANKDATABASE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

struct tCustomer {
    int id;
    char firstName[64];
    char lastName[64];
};

struct tCustomerContainer {
    bool isDeleted; // identify if the record is logically deleted
    struct tCustomer customer;
};

struct tAccount {
    int number;
    double balance;
};

struct tIndex {
    int id; // tCustomer::id
    long int address; // the address of the corresponding customer in the database
};

// read indexes from the file to the RAM
void initialize(FILE* ind, FILE* ws);

// write indexes from the RAM to the disk file and free the RAM, occupied by these indexes
void finilize(FILE* ind, FILE* ws);

void insert_m(FILE* fl);

bool find_m(struct tIndex* index, int customerId);

void get_m(FILE* fl, int customerId);

void update_m(FILE* fl, int customerId);

void delete_m(FILE* ind, FILE* fl, int customerId);

#endif // BANKDATABASE_H_INCLUDED
