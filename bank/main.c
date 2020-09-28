#include "BankDatabase.h"
#include <stdio.h>

int main() {
    int status = initialize(0);
    if (status == -1) {
        fprintf(stderr, "The initial setup of the database is failed.\n");
        return -1;
    }

    printf("-1 - exit\n");
    printf("0 - insert_m\n");
    printf("1 - insert_s\n");
    printf("2 - get_m\n");
    printf("3 - del_m\n");
    printf("4 - del_s\n");
    printf("5 - update_m\n");
    printf("6 - update_s\n");
    printf("7 - ut_m\n");
    printf("8 - ut_s\n");
    printf("9 - see options\n\n");

    int option;
    int customerId;
    int accountNumber;

    while (true) {
        printf("Choose an option (-1 to exit | 9 to see options): ");
        scanf("%d", &option);

        if (option == -1) {
            break;
        }

        switch (option) {
        case 0:
            printf("[insert_m]\n");
            insert_m();
            break;

        case 1:
            printf("[insert_s]\n");
            printf("Customer ID: ");
            scanf("%d", &customerId);
            insert_s(customerId);
            break;

        case 2:
            printf("[get_m]\n");
            printf("Customer ID: ");
            scanf("%d", &customerId);
            get_m(customerId);
            break;

        case 3:
            printf("[del_m]\n");
            printf("Customer ID: ");
            scanf("%d", &customerId);
            del_m(customerId);
            break;

        case 4:
            printf("[del_s]\n");
            printf("Customer ID: ");
            scanf("%d", &customerId);
            printf("Account Number: ");
            scanf("%d", &accountNumber);
            del_s(customerId, accountNumber);
            break;

        case 5:
            printf("[update_m]\n");
            printf("Customer ID: ");
            scanf("%d", &customerId);
            update_m(customerId);
            break;

        case 6:
            printf("[update_s]\n");
            printf("Customer ID: ");
            scanf("%d", &customerId);
            printf("Bank Account Number: ");
            scanf("%d", &accountNumber);
            update_s(customerId, accountNumber);
            break;

        case 7:
            printf("[ut_m]\n");
            ut_m();
            break;

        case 8:
            printf("[ut_s]\n");
            ut_s();
            break;

        case 9:
            printf("-1 - exit\n");
            printf("0 - insert_m\n");
            printf("1 - insert_s\n");
            printf("2 - get_m\n");
            printf("3 - del_m\n");
            printf("4 - del_s\n");
            printf("5 - update_m\n");
            printf("6 - update_s\n");
            printf("7 - ut_m\n");
            printf("8 - ut_s\n");
            printf("9 - see options\n\n");
            break;

        default:
            printf("[!] Incorrect input. Try again\n");
        }

    }

    status = finalize();
    if (status == -1) {
        fprintf(stderr, "The database has ended with errors.\n");
        return -1;
    }

    return 0;
}
