#include <stdio.h>
#include <stdlib.h>
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

void insert_m(FILE* ind, FILE* fl) {
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

    fseek(ind, 0L, SEEK_END);
    fwrite(&index, sizeof(struct tIndex), 1, ind);
}

bool find_m(FILE* ind, struct tIndex* index, int customerId) {
    fseek(ind, 0L, SEEK_SET);

    while (fread(index, sizeof(struct tIndex), 1, ind) == 1) {
        if (index->id == customerId) {
            return true;
        }
    }
    return false;
}

void get_m(FILE* ind, FILE* fl, int customerId) {
    struct tIndex index;

    if (find_m(ind, &index, customerId)) {
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

void update_m(FILE* ind, FILE* fl, int customerId) {
    struct tIndex index;

    if (find_m(ind, &index, customerId)) {
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

int main() {
    FILE* ind = fopen("customers.ind", "wb+");
    if (!ind) {
        fprintf(stderr, "Unable to open the file.\n");
        return -1;
    }

    FILE* fl = fopen("customers.fl", "wb+");
    if (!fl) {
        fprintf(stderr, "Unable to open the file.\n");
        fclose(ind);
        return -1;
    }

    int id;

    printf("Search | ID: ");
    scanf("%d", &id);
    get_m(ind, fl, id);

    fclose(fl);
    fclose(ind);

    return 0;
}
