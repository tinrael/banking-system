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

    if (find_m(ind, &index, customerId)) {
        struct tCustomer customer;

        fseek(fl, index.address, SEEK_SET);
        fread(&customer, sizeof(struct tCustomer), 1, fl);

        printf("%d %s %s\n", customer.id, customer.firstName, customer.lastName);
    } else {
        printf("Not found.\n");
    }
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

void update_m(FILE* ind, FILE* fl, int customerId) {
    struct tIndex index;

    if (find_m(ind, &index, customerId)) {
        struct tCustomer customer;

        fseek(fl, index.address, SEEK_SET);
        fread(&customer, sizeof(struct tCustomer), 1, fl);

        printf("Updating %d %s %s:\n", customer.id, customer.firstName, customer.lastName);

        printf("First Name: ");
        scanf("%63s", customer.firstName);

        printf("Last Name: ");
        scanf("%63s", customer.lastName);

        fseek(fl, index.address, SEEK_SET);
        fwrite(&customer, sizeof(struct tCustomer), 1, fl);
    } else {
        printf("Not found.\n");
    }
}

int main() {
    FILE* ind = fopen("customers.ind", "rb+");
    if (!ind) {
        fprintf(stderr, "Unable to open the file.\n");
        return -1;
    }

    FILE* fl = fopen("customers.fl", "rb+");
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
