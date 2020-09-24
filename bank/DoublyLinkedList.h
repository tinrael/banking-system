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
void addToBeginning(tNode** head, int key) {
    tNode* newOne = (tNode*) malloc(sizeof(tNode));
    newOne->key = key;

    newOne->next = *head;
    newOne->prev = NULL;

    if ((*head) != NULL) {
        (*head)->prev = newOne;
    }

    (*head) = newOne;
}

// inserts an element in the ascending order (from the smallest to the largest)
void addInAscendingOrder(tNode** head, int key) {
    tNode* newOne = (tNode*) malloc(sizeof(tNode));
    newOne->key = key;

    if ((*head) == NULL) {
        newOne->next = NULL;
        newOne->prev = NULL;
        *head = newOne;
        return;
    }

    tNode* current = *head;
    while (current != NULL) {
        if (current->key > key) {
            newOne->next = current;
            newOne->prev = current->prev;

            if (current->prev != NULL) {
                current->prev->next = newOne;
            }

            current->prev = newOne;

            if (current == (*head)) {
                *head = newOne;
            }

            return;
        }

        if (current->next == NULL) {
            newOne->next = NULL;
            newOne->prev = current;
            current->next = newOne;
            return;
        }

        current = current->next;
    }
}

// erases (free) the list node pointed at by toDelete
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

// erases (free) the element with the key 'key'
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

// erases (free) all elements
void clearList(tNode** head) {
    tNode* toDelete;
    while ((*head) != NULL) {
        toDelete = *head;
        *head = (*head)->next;

        free(toDelete);
    }
}

#endif // DOUBLYLINKEDLIST_H_INCLUDED
