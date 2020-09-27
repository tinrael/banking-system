#include "BankDatabase.h"
#include <stdio.h>

int main() {
    initialize(1);
    ut_m();

    int id;
    for (int i = 0; i < 10; i++) {
        printf("Search | ID: ");
        scanf("%d", &id);
        get_m(id);
    }

    finilize();
    return 0;
}
