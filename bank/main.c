#include "BankDatabase.h"
#include <stdio.h>

int main() {
    int status = initialize(1);
    if (status == -1) {
        fprintf(stderr, "The initial setup of the database is failed.\n");
        return -1;
    }

    ut_m();

    int id;
    for (int i = 0; i < 10; i++) {
        printf("Search | ID: ");
        scanf("%d", &id);
        get_m(id);
    }

    status = finalize();
    if (status == -1) {
        fprintf(stderr, "The database has ended with errors.\n");
        return -1;
    }

    return 0;
}
