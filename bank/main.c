#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct tCustomer {
    int id;
    char firstName[64];
    char lastName[64];
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
    struct tCustomer customer;
    printf("ID: ");
    scanf("%d", &(customer.id));

    printf("First Name: ");
    scanf("%63s", customer.firstName);

    printf("Last Name: ");
    scanf("%63s", customer.lastName);

    struct tIndex index;
    index.id = customer.id;

    fseek(fl, 0L, SEEK_END);
    index.address = ftell(fl);
    fwrite(&customer, sizeof(struct tCustomer), 1, fl);

    fseek(ind, 0L, SEEK_END);
    fwrite(&index, sizeof(struct tIndex), 1, ind);
}

void get_m(FILE* ind, FILE* fl, int customerId) {
    struct tIndex index;

    fseek(ind, 0L, SEEK_SET);

    bool isFound = false;
    while (!feof(ind)) {
        fread(&index, sizeof(struct tIndex), 1, ind);

        if (index.id == customerId) {
            isFound = true;
            break;
        }
    }

    if (isFound) {
        struct tCustomer customer;

        fseek(fl, index.address, SEEK_SET);
        fread(&customer, sizeof(struct tCustomer), 1, fl);

        printf("%d %s %s\n", customer.id, customer.firstName, customer.lastName);
    } else {
        printf("Not found.\n");
    }
}

int main() {
    FILE* ind = fopen("customers.ind", "w+");
    if (!ind) {
        fprintf(stderr, "Unable to open the file.");
        return -1;
    }

    FILE* fl = fopen("customers.fl", "w+");
    if (!fl) {
        fprintf(stderr, "Unable to open the file.");
        return -1;
    }

    insert_m(ind, fl);

    int id;
    printf("ID: ");
    scanf("%d", &id);
    get_m(ind, fl, id);

    fclose(fl);
    fclose(ind);

    return 0;
}
