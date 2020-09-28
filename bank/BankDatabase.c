#include "BankDatabase.h"
#include "DoublyLinkedList.h"
#include "Stack.h"
#include <stdlib.h>

FILE* indexesFile = NULL;
FILE* customersFile = NULL;
FILE* customersEmptyBlocksFile = NULL;
FILE* accountsFile = NULL;
FILE* accountsEmptyBlocksFile = NULL;

const char indexesFilename[] = "customers.ind";
const char customersFilename[] = "customers.fl";
const char customersEmptyBlocksFilename[] = "customers-empty-blocks.fl";
const char accountsFilename[] = "accounts.fl";
const char accountsEmptyBlocksFilename[] = "accounts-empty-blocks.fl";

tNode* indexesList = NULL;
tElement* addressesOfEmptyBlocks = NULL;

int initialize(int mode) {
    char* filesMode;
    switch (mode) {
    case 0: // starts a new database
        filesMode = "wb+";
        break;

    case 1: // opens the existing database
        filesMode = "rb+";
        break;

    default:
        return -1;
    }

    indexesFile = fopen(indexesFilename, filesMode);
    if (!indexesFile) {
        fprintf(stderr, "Unable to open the file %s\n", indexesFilename);
        return -1;
    }

    customersFile = fopen(customersFilename, filesMode);
    if (!customersFile) {
        fprintf(stderr, "Unable to open the file %s\n", customersFilename);
        fclose(indexesFile);
        return -1;
    }

    customersEmptyBlocksFile = fopen(customersEmptyBlocksFilename, filesMode);
    if (!customersEmptyBlocksFile) {
        fprintf(stderr, "Unable to open the file %s\n", customersEmptyBlocksFilename);
        fclose(customersFile);
        fclose(indexesFile);
        return -1;
    }

    accountsFile = fopen(accountsFilename, filesMode);
    if (!accountsFile) {
        fprintf(stderr, "Unable to open the file %s\n", accountsFilename);
        fclose(customersEmptyBlocksFile);
        fclose(customersFile);
        fclose(indexesFile);
        return -1;
    }

    accountsEmptyBlocksFile = fopen(accountsEmptyBlocksFilename, filesMode);
    if (!accountsEmptyBlocksFile) {
        fprintf(stderr, "Unable to open the file %s\n", accountsEmptyBlocksFilename);
        fclose(accountsFile);
        fclose(customersEmptyBlocksFile);
        fclose(customersFile);
        fclose(indexesFile);
        return -1;
    }

    uploadData();

    return 0;
}

void uploadData() {
    clearList(&indexesList);
    struct tIndex index;
    fseek(indexesFile, 0L, SEEK_SET);
    while (fread(&index, sizeof(struct tIndex), 1, indexesFile) == 1) {
        addInAscendingOrder(&indexesList, index);
    }

    clearStack(&addressesOfEmptyBlocks);
    long int address;
    fseek(customersEmptyBlocksFile, 0L, SEEK_SET);
    while (fread(&address, sizeof(long int), 1, customersEmptyBlocksFile) == 1) {
        push(&addressesOfEmptyBlocks, address);
    }
}

int finalize() {
    fclose(indexesFile);
    fclose(customersFile);
    fclose(customersEmptyBlocksFile);
    fclose(accountsFile);
    fclose(accountsEmptyBlocksFile);

    FILE* indTempFile = fopen("indexes.tmp", "wb");

    tNode* current = indexesList;
    while (current != NULL) {
        fwrite(&(current->index), sizeof(struct tIndex), 1, indTempFile);
        current = current->next;
    }
    clearList(&indexesList);

    fclose(indTempFile);

    int status = remove(indexesFilename);
    if (status) {
        fprintf(stderr,
                "Unable to remove the file %s.\n", indexesFilename);

        return -1;
    }

    status = rename("indexes.tmp", indexesFilename);
    if (status) {
        fprintf(stderr,
                "Unable to rename the file from 'indexes.tmp' to %s'.\n", indexesFilename);

        return -1;
    }

    FILE* emptyBlocksTempFile = fopen("customers-empty-blocks.tmp", "wb");

    long int address;
    while (!isEmpty(addressesOfEmptyBlocks)) {
        address = pop(&addressesOfEmptyBlocks);
        fwrite(&address, sizeof(long int), 1, emptyBlocksTempFile);
    }

    fclose(emptyBlocksTempFile);

    status = remove(customersEmptyBlocksFilename);
    if (status) {
        fprintf(stderr,
                "Unable to remove the file %s.\n", customersEmptyBlocksFilename);

        return -1;
    }

    status = rename("customers-empty-blocks.tmp", customersEmptyBlocksFilename);
    if (status) {
        fprintf(stderr,
                "Unable to rename the file from 'waste.tmp' to %s.\n", customersEmptyBlocksFilename);

        return -1;
    }

    return 0;
}

void insert_m() {
    struct tCustomerContainer customerContainer;
    customerContainer.isDeleted = false;

    customerContainer.numberOfAccounts = 0;
    customerContainer.addressOfAccountsListHead = -1L;

    printf("ID: ");
    scanf("%d", &(customerContainer.customer.id));

    printf("First Name: ");
    scanf("%63s", customerContainer.customer.firstName);

    printf("Last Name: ");
    scanf("%63s", customerContainer.customer.lastName);

    struct tIndex index;
    index.id = customerContainer.customer.id;

    if (!isEmpty(addressesOfEmptyBlocks)) {
        long int addressOfEmptyBlock = pop(&addressesOfEmptyBlocks);
        fseek(customersFile, addressOfEmptyBlock, SEEK_SET);
    } else {
        fseek(customersFile, 0L, SEEK_END);
    }

    index.address = ftell(customersFile);
    fwrite(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

    addInAscendingOrder(&indexesList, index);
}

bool find_m(struct tIndex* index, int customerId) {
    return findIndex(indexesList, index, customerId);
}

void get_m(int customerId) {
    struct tIndex index;

    if (find_m(&index, customerId)) {
        struct tCustomerContainer customerContainer;

        fseek(customersFile, index.address, SEEK_SET);
        fread(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

        printf("%d %s %s\n",
               customerContainer.customer.id,
               customerContainer.customer.firstName,
               customerContainer.customer.lastName);
    } else {
        printf("Not found.\n");
    }
}

void update_m(int customerId) {
    struct tIndex index;

    if (find_m(&index, customerId)) {
        struct tCustomerContainer customerContainer;

        fseek(customersFile, index.address, SEEK_SET);
        fread(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

        printf("Updating %d %s %s:\n",
               customerContainer.customer.id,
               customerContainer.customer.firstName,
               customerContainer.customer.lastName);

        printf("First Name: ");
        scanf("%63s", customerContainer.customer.firstName);

        printf("Last Name: ");
        scanf("%63s", customerContainer.customer.lastName);

        fseek(customersFile, index.address, SEEK_SET);
        fwrite(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);
    } else {
        printf("Not found.\n");
    }
}

void delete_m(int customerId) {
    struct tIndex index;

    if (find_m(&index, customerId)) {
        struct tCustomerContainer customerContainer;

        fseek(customersFile, index.address, SEEK_SET);
        fread(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

        customerContainer.isDeleted = true;

        fseek(customersFile, index.address, SEEK_SET);
        fwrite(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

        push(&addressesOfEmptyBlocks, index.address);
        eraseFromList(&indexesList, index.id);
    } else {
        printf("Not found.\n");
    }
}

void ut_m() {
    struct tCustomerContainer customerContainer;
    fseek(customersFile, 0L, SEEK_SET);
    printf("\t-----------------------------------------------------------------------------------------\n");
    printf("\tStatus | ID | First Name | Last Name | Number Of Accounts | Address Of Accounts List Head\n");
    printf("\t-----------------------------------------------------------------------------------------\n");
    while (fread(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile) == 1) {
        printf(customerContainer.isDeleted ? "\t[deleted] " : "\t[exists] ");
        printf("%d %s %s %u %ld\n",
               customerContainer.customer.id,
               customerContainer.customer.firstName,
               customerContainer.customer.lastName,
               customerContainer.numberOfAccounts,
               customerContainer.addressOfAccountsListHead);
    }
}
