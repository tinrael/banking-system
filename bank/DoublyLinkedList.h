#ifndef DOUBLYLINKEDLIST_H_INCLUDED
#define DOUBLYLINKEDLIST_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

typedef struct tagNode {
    int key;

    struct tagNode* next;
    struct tagNode* prev;
} tNode;

// inserts an element to the beginning
void addToList(tNode** head, int key) {
    tNode* newOne = (tNode*) malloc(sizeof(tNode));
    newOne->key = key;

    newOne->next = *head;
    newOne->prev = NULL;

    if ((*head) != NULL) {
        (*head)->prev = newOne;
    }

    (*head) = newOne;
}

// erases the list node pointed at by toDelete
void deleteListNode(tNode** head, tNode* toDelete) {
    if ((*head) != NULL && toDelete != NULL) {
        if ((*head) == toDelete) {
            (*head) = toDelete->next;
        }

        if (toDelete->next != NULL) {
            toDelete->next->prev = toDelete->prev;
        }

        if (toDelete->prev != NULL) {
            toDelete->prev->next = toDelete->next;
        }

        free(toDelete);
    }
}

// erases the element with the key 'key'
void eraseFromList(tNode** head, int key) {
    tNode* current = *head;
    while (current != NULL) {
        if (current->key == key) {
            deleteListNode(head, current);
            return;
        }
        current = current->next;
    }
}

void printList(tNode* head) {
    while (head != NULL) {
        printf("%d -> ", head->key);
        head = head->next;
    }
    printf("\n");
}

#endif // DOUBLYLINKEDLIST_H_INCLUDED
