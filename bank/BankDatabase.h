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

    unsigned int numberOfAccounts; // number of customer's bank accounts
    long int addressOfAccountsListHead; // the address of the head node in the accounts list
};

struct tAccount {
    int number;
    double balance;
};

struct tAccountContainer {
    bool isDeleted; // identify if the record is logically deleted

    struct tAccount account;

    long int addressOfNext; // the address of the next bank account in the list
};

struct tIndex {
    int id; // tCustomer::id
    long int address; // the address of the corresponding customer in the database
};

/* Starts the work with the database.
 * Returns 0 on success starting. Otherwise, returns -1.
 *
 * 'mode' parameter defines in which mode starts the database:
 * 0 - to start a new database,
 * 1 - to open the existing database (database files must exist).
 *
 * Opens database files.
 * Uploads the data from disk files to RAM.
 */
int initialize(int mode);

// reads addresses of empty blocks and indexes from the file to the RAM
void uploadData();

/* Finishes the work with the database.
 * Returns 0 on success finishing. Otherwise, returns -1.
 *
 * Writes addresses of empty blocks and indexes from the RAM to disk files.
 * Frees the RAM, occupied by these addresses of empty blocks and indexes.
 * Closes all opened by database files.
 */
int finalize();

void insert_m();

bool find_m(struct tIndex* index, int customerId);

void get_m(int customerId);

void update_m(int customerId);

void delete_m(int customerId);

void ut_m();

#endif // BANKDATABASE_H_INCLUDED
