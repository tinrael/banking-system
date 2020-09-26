#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdbool.h>

typedef struct tagElement {
    long int address;

    struct tagElement* next;
} tElement;

// inserts an element at the top
void push(tElement** top, long int address);

/* Returns the top element and free the memory, occupied by this top element.
 * This function assumes that the stack is not empty.
 */
long int pop(tElement** top);

// checks whether the stack is empty
bool isEmpty(tElement* top);

void printStack(tElement* top);

#endif // STACK_H_INCLUDED
