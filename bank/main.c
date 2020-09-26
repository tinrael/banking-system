#include "Bank.h"
#include <stdio.h>

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

    initialize(ind);

    int id;
    printf("Search | ID: ");
    scanf("%d", &id);
    get_m(fl, id);

    finilize(ind);

    fclose(fl);
    fclose(ind);

    return 0;
}
