#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>

void push(tElement** top, long int address) {
    tElement* newOne = (tElement*) malloc(sizeof(tElement));
    newOne->address = address;

    newOne->next = *top;
    *top = newOne;
}

long int pop(tElement** top) {
    long int address = (*top)->address;

    tElement* toDelete = *top;
    *top = (*top)->next;
    free(toDelete);

    return address;
}

bool isEmpty(tElement* top) {
    if (top == NULL) {
        return true;
    }

    return false;
}

void clearStack(tElement** top) {
    tElement* toDelete;
    while ((*top) != NULL) {
        toDelete = *top;
        *top = (*top)->next;

        free(toDelete);
    }
}

void printStack(tElement* top) {
    while(top != NULL) {
        printf("%ld ", top->address);
        top = top->next;
    }
    printf("\n");
}
