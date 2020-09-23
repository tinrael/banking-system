#ifndef DOUBLYLINKEDLIST_H_INCLUDED
#define DOUBLYLINKEDLIST_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

typedef struct tagNode {
    int data;

    struct tagNode* next;
    struct tagNode* prev;
} tNode;

// inserts an element to the beginning
void add(tNode** head, int data) {
    tNode* newOne = (tNode*) malloc(sizeof(tNode));
    newOne->data = data;

    newOne->next = *head;
    newOne->prev = NULL;

    if ((*head) != NULL) {
        (*head)->prev = newOne;
    }

    (*head) = newOne;
}

void print(tNode* head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
}

#endif // DOUBLYLINKEDLIST_H_INCLUDED
