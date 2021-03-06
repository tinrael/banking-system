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
tElement* addressesOfCustomersEmptyBlocks = NULL;
tElement* addressesOfAccountsEmptyBlocks = NULL;

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

    long int addressOfCustomersEmptyBlock;
    clearStack(&addressesOfCustomersEmptyBlocks);
    fseek(customersEmptyBlocksFile, 0L, SEEK_SET);
    while (fread(&addressOfCustomersEmptyBlock, sizeof(long int), 1, customersEmptyBlocksFile) == 1) {
        push(&addressesOfCustomersEmptyBlocks, addressOfCustomersEmptyBlock);
    }

    long int addressOfAccountsEmptyBlock;
    clearStack(&addressesOfAccountsEmptyBlocks);
    fseek(accountsEmptyBlocksFile, 0L, SEEK_SET);
    while (fread(&addressOfAccountsEmptyBlock, sizeof(long int), 1, accountsEmptyBlocksFile) == 1) {
        push(&addressesOfAccountsEmptyBlocks, addressOfAccountsEmptyBlock);
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

    FILE* customersEmptyBlocksTempFile = fopen("customers-empty-blocks.tmp", "wb");

    long int addressOfCustomersEmptyBlock;
    while (!isEmpty(addressesOfCustomersEmptyBlocks)) {
        addressOfCustomersEmptyBlock = pop(&addressesOfCustomersEmptyBlocks);
        fwrite(&addressOfCustomersEmptyBlock, sizeof(long int), 1, customersEmptyBlocksTempFile);
    }

    fclose(customersEmptyBlocksTempFile);

    status = remove(customersEmptyBlocksFilename);
    if (status) {
        fprintf(stderr,
                "Unable to remove the file %s.\n", customersEmptyBlocksFilename);

        return -1;
    }

    status = rename("customers-empty-blocks.tmp", customersEmptyBlocksFilename);
    if (status) {
        fprintf(stderr,
                "Unable to rename the file from 'customers-empty-blocks.tmp' to %s.\n", customersEmptyBlocksFilename);

        return -1;
    }

    FILE* accountsEmptyBlocksTempFile = fopen("accounts-empty-blocks.tmp", "wb");

    long int addressOfAccountsEmptyBlock;
    while (!isEmpty(addressesOfAccountsEmptyBlocks)) {
        addressOfAccountsEmptyBlock = pop(&addressesOfAccountsEmptyBlocks);
        fwrite(&addressOfAccountsEmptyBlock, sizeof(long int), 1, accountsEmptyBlocksTempFile);
    }

    fclose(accountsEmptyBlocksTempFile);

    status = remove(accountsEmptyBlocksFilename);
    if (status) {
        fprintf(stderr,
                "Unable to remove the file %s.\n", accountsEmptyBlocksFilename);

        return -1;
    }

    status = rename("accounts-empty-blocks.tmp", accountsEmptyBlocksFilename);
    if (status) {
        fprintf(stderr,
                "Unable to rename the file from 'accounts-empty-blocks.tmp' to %s.\n", accountsEmptyBlocksFilename);

        return -1;
    }

    return 0;
}

void insert_m() {
    struct tCustomerContainer customerContainer;
    customerContainer.isDeleted = false;

    customerContainer.numberOfAccounts = 0;
    customerContainer.addressOfAccountsListHead = -1L;

    printf("Add a new customer:\n");

    printf("\tID: ");
    scanf("%d", &(customerContainer.customer.id));

    printf("\tFirst Name: ");
    scanf("%63s", customerContainer.customer.firstName);

    printf("\tLast Name: ");
    scanf("%63s", customerContainer.customer.lastName);

    struct tIndex index;
    index.id = customerContainer.customer.id;

    if (!isEmpty(addressesOfCustomersEmptyBlocks)) {
        long int addressOfEmptyBlock = pop(&addressesOfCustomersEmptyBlocks);
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

        printf("\t%d %s %s\n",
               customerContainer.customer.id,
               customerContainer.customer.firstName,
               customerContainer.customer.lastName);

        printf("\t\tAccounts:\n");

        if (customerContainer.addressOfAccountsListHead == -1L) {
            printf("\t\t\tAccounts not found.\n");
        } else {
            printf("\t\t\t----------------\n");
            printf("\t\t\tNumber | Balance\n");
            printf("\t\t\t----------------\n");

            struct tAccountContainer accountContainer;
            long int curAccountAddress = customerContainer.addressOfAccountsListHead;
            while (curAccountAddress != -1L) {
                fseek(accountsFile, curAccountAddress, SEEK_SET);
                fread(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile);

                printf("\t\t\t%d %lf\n",
                       accountContainer.account.number,
                       accountContainer.account.balance);

                curAccountAddress = accountContainer.addressOfNext;
            }
        }

    } else {
        printf("Not found customer with id %d.\n", customerId);
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

        printf("\tFirst Name: ");
        scanf("%63s", customerContainer.customer.firstName);

        printf("\tLast Name: ");
        scanf("%63s", customerContainer.customer.lastName);

        fseek(customersFile, index.address, SEEK_SET);
        fwrite(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);
    } else {
        printf("Not found customer with id %d.\n", customerId);
    }
}

void del_m(int customerId) {
    struct tIndex index;

    if (find_m(&index, customerId)) {
        struct tCustomerContainer customerContainer;

        fseek(customersFile, index.address, SEEK_SET);
        fread(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

        del_all_s(customerContainer.addressOfAccountsListHead);
        customerContainer.addressOfAccountsListHead = -1L;
        customerContainer.numberOfAccounts = 0;
        customerContainer.isDeleted = true;

        fseek(customersFile, index.address, SEEK_SET);
        fwrite(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

        push(&addressesOfCustomersEmptyBlocks, index.address);
        eraseFromList(&indexesList, index.id);
    } else {
        printf("Not found customer with id %d.\n", customerId);
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
        printf("%d \t %s \t %s \t %u \t %ld\n",
               customerContainer.customer.id,
               customerContainer.customer.firstName,
               customerContainer.customer.lastName,
               customerContainer.numberOfAccounts,
               customerContainer.addressOfAccountsListHead);
    }
}

void insert_s(int customerId) {
    struct tIndex index;
    if (find_m(&index, customerId)) {
        struct tCustomerContainer customerContainer;

        fseek(customersFile, index.address, SEEK_SET);
        fread(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

        struct tAccountContainer accountContainer;
        accountContainer.isDeleted = false;
        accountContainer.addressOfNext = customerContainer.addressOfAccountsListHead;

        printf("Create a new bank account for [%d] %s %s:\n",
               customerContainer.customer.id,
               customerContainer.customer.firstName,
               customerContainer.customer.lastName);

        printf("\tNumber: ");
        scanf("%d", &(accountContainer.account.number));

        printf("\tBalance: ");
        scanf("%lf", &(accountContainer.account.balance));

        if (!isEmpty(addressesOfAccountsEmptyBlocks)) {
            long int addressOfEmptyBlock = pop(&addressesOfAccountsEmptyBlocks);
            fseek(accountsFile, addressOfEmptyBlock, SEEK_SET);
        } else {
            fseek(accountsFile, 0L, SEEK_END);
        }

        long int addressOfNewAccount = ftell(accountsFile);
        fwrite(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile);

        customerContainer.addressOfAccountsListHead = addressOfNewAccount;
        customerContainer.numberOfAccounts++;

        fseek(customersFile, index.address, SEEK_SET);
        fwrite(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);
    } else {
        printf("Not found customer with id %d\n", customerId);
    }
}

void update_s(int customerId, int accountNumber) {
    struct tIndex index;
    if (find_m(&index, customerId)) {
        struct tCustomerContainer customerContainer;

        fseek(customersFile, index.address, SEEK_SET);
        fread(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

        long int curAccountAddress = customerContainer.addressOfAccountsListHead;

        struct tAccountContainer accountContainer;
        while (curAccountAddress != -1L) {
            fseek(accountsFile, curAccountAddress, SEEK_SET);
            fread(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile);

            if (accountContainer.account.number == accountNumber) {
                printf("Updating the bank account Number: %d: Balance: %lf\n",
                       accountContainer.account.number,
                       accountContainer.account.balance);

                printf("\tBalance: ");
                scanf("%lf", &(accountContainer.account.balance));

                fseek(accountsFile, curAccountAddress, SEEK_SET);
                fwrite(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile);

                return;
            }

            curAccountAddress = accountContainer.addressOfNext;
        }

        printf("Customer [%d] %s %s does not have the account with number '%d'.\n",
               customerContainer.customer.id,
               customerContainer.customer.firstName,
               customerContainer.customer.lastName,
               accountNumber);
    } else {
        printf("Not found customer with id %d\n", customerId);
    }
}

void ut_s() {
    struct tAccountContainer accountContainer;
    fseek(accountsFile, 0L, SEEK_SET);
    printf("\t-----------------------------------------------\n");
    printf("\tStatus | Number | Balance | Address Of The Next\n");
    printf("\t-----------------------------------------------\n");
    while (fread(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile) == 1) {
        printf(accountContainer.isDeleted ? "\t[deleted] " : "\t[exists] ");
        printf("%d \t %lf \t %ld\n",
               accountContainer.account.number,
               accountContainer.account.balance,
               accountContainer.addressOfNext);
    }
}

void del_s(int customerId, int accountNumber) {
    struct tIndex index;
    if (find_m(&index, customerId)) {
        struct tCustomerContainer customerContainer;

        fseek(customersFile, index.address, SEEK_SET);
        fread(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

        // address of the previous account container
        long int prevAccountAddress = -1L;
        // address of the current account container
        long int curAccountAddress = customerContainer.addressOfAccountsListHead;

        struct tAccountContainer accountContainer;
        while (curAccountAddress != -1L) {
            fseek(accountsFile, curAccountAddress, SEEK_SET);
            fread(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile);

            if (accountContainer.account.number == accountNumber) {
                push(&addressesOfAccountsEmptyBlocks, curAccountAddress);
                long int addressOfNext = accountContainer.addressOfNext;

                accountContainer.isDeleted = true;
                fseek(accountsFile, curAccountAddress, SEEK_SET);
                fwrite(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile);

                if (prevAccountAddress == -1L) {
                    customerContainer.addressOfAccountsListHead = addressOfNext;
                } else {
                    fseek(accountsFile, prevAccountAddress, SEEK_SET);
                    fread(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile);

                    accountContainer.addressOfNext = addressOfNext;

                    fseek(accountsFile, prevAccountAddress, SEEK_SET);
                    fwrite(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile);
                }

                customerContainer.numberOfAccounts--;

                fseek(customersFile, index.address, SEEK_SET);
                fwrite(&customerContainer, sizeof(struct tCustomerContainer), 1, customersFile);

                return;
            }

            prevAccountAddress = curAccountAddress;
            curAccountAddress = accountContainer.addressOfNext;
        }

        printf("Customer [%d] %s %s does not have the account with number '%d'.\n",
               customerContainer.customer.id,
               customerContainer.customer.firstName,
               customerContainer.customer.lastName,
               accountNumber);
    } else {
        printf("Not found customer with id %d.\n", customerId);
    }
}

void del_all_s(long int addressOfAccountsListHead) {
    long int curAccountAddress = addressOfAccountsListHead;

    struct tAccountContainer accountContainer;
    while (curAccountAddress != -1L) {
        push(&addressesOfAccountsEmptyBlocks, curAccountAddress);

        fseek(accountsFile, curAccountAddress, SEEK_SET);
        fread(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile);

        accountContainer.isDeleted = true;

        fseek(accountsFile, curAccountAddress, SEEK_SET);
        fwrite(&accountContainer, sizeof(struct tAccountContainer), 1, accountsFile);

        curAccountAddress = accountContainer.addressOfNext;
    }
}
