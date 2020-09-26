#ifndef BANK_H_INCLUDED
#define BANK_H_INCLUDED

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

// upload indexes from the file to the memory
void initialize(FILE* ind);

void insert_m(FILE* ind, FILE* fl);

bool find_m(FILE* ind, struct tIndex* index, int customerId);

void get_m(FILE* ind, FILE* fl, int customerId);

void update_m(FILE* ind, FILE* fl, int customerId);

void delete_m(FILE* ind, FILE* fl, int customerId);

#endif // BANK_H_INCLUDED
