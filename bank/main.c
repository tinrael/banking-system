#include "Bank.h"
#include <stdio.h>

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
