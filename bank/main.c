#include <stdio.h>
#include <stdlib.h>

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

    fclose(fl);
    fclose(ind);

    return 0;
}
