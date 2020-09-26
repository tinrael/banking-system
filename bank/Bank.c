#include "Bank.h"
#include "DoublyLinkedList.h"
#include <stdlib.h>

tNode* indexesList = NULL;

void initialize(FILE* ind) {
    struct tIndex index;

    fseek(ind, 0L, SEEK_SET);

    while (fread(&index, sizeof(struct tIndex), 1, ind) == 1) {
        addInAscendingOrder(&indexesList, index);
    }
}

void finilize(FILE* ind) {
    tNode* current = indexesList;

    fseek(ind, 0L, SEEK_SET);

    while (current != NULL) {
        fwrite(&(current->index), sizeof(struct tIndex), 1, ind);

        current = current->next;
    }

    clearList(&indexesList);
}

void insert_m(FILE* fl) {
    struct tCustomerContainer customerContainer;
    customerContainer.isDeleted = false;

    printf("ID: ");
    scanf("%d", &(customerContainer.customer.id));

    printf("First Name: ");
    scanf("%63s", customerContainer.customer.firstName);

    printf("Last Name: ");
    scanf("%63s", customerContainer.customer.lastName);

    struct tIndex index;
    index.id = customerContainer.customer.id;

    fseek(fl, 0L, SEEK_END);
    index.address = ftell(fl);
    fwrite(&customerContainer, sizeof(struct tCustomerContainer), 1, fl);

    addInAscendingOrder(&indexesList, index);
}

bool find_m(struct tIndex* index, int customerId) {
    return findIndex(indexesList, index, customerId);
}

void get_m(FILE* fl, int customerId) {
    struct tIndex index;

    if (find_m(&index, customerId)) {
        struct tCustomerContainer customerContainer;

        fseek(fl, index.address, SEEK_SET);
        fread(&customerContainer, sizeof(struct tCustomerContainer), 1, fl);

        printf("%d %s %s\n",
               customerContainer.customer.id,
               customerContainer.customer.firstName,
               customerContainer.customer.lastName);
    } else {
        printf("Not found.\n");
    }
}

void update_m(FILE* fl, int customerId) {
    struct tIndex index;

    if (find_m(&index, customerId)) {
        struct tCustomerContainer customerContainer;

        fseek(fl, index.address, SEEK_SET);
        fread(&customerContainer, sizeof(struct tCustomerContainer), 1, fl);

        printf("Updating %d %s %s:\n",
               customerContainer.customer.id,
               customerContainer.customer.firstName,
               customerContainer.customer.lastName);

        printf("First Name: ");
        scanf("%63s", customerContainer.customer.firstName);

        printf("Last Name: ");
        scanf("%63s", customerContainer.customer.lastName);

        fseek(fl, index.address, SEEK_SET);
        fwrite(&customerContainer, sizeof(struct tCustomerContainer), 1, fl);
    } else {
        printf("Not found.\n");
    }
}

void delete_m(FILE* ind, FILE* fl, int customerId) {
    struct tIndex index;

    if (find_m(&index, customerId)) {
        struct tCustomerContainer customerContainer;

        fseek(fl, index.address, SEEK_SET);
        fread(&customerContainer, sizeof(struct tCustomerContainer), 1, fl);

        customerContainer.isDeleted = true;

        fseek(fl, index.address, SEEK_SET);
        fwrite(&customerContainer, sizeof(struct tCustomerContainer), 1, fl);
    } else {
        printf("Not found.\n");
    }
}
