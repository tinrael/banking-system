#ifndef DOUBLYLINKEDLIST_H_INCLUDED
#define DOUBLYLINKEDLIST_H_INCLUDED

#include "Bank.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct tagNode {
    struct tIndex index;

    struct tagNode* next;
    struct tagNode* prev;
} tNode;

// inserts an element to the beginning
void addToBeginning(tNode** head, struct tIndex index) {
    tNode* newOne = (tNode*) malloc(sizeof(tNode));
    newOne->index = index;

    newOne->next = *head;
    newOne->prev = NULL;

    if ((*head) != NULL) {
        (*head)->prev = newOne;
    }

    (*head) = newOne;
}

/* TODO: need code refactoring.
 *
 * inserts an element in the ascending order by the index's ID  (tNode::index::id)
 * (from the smallest to the largest)
 */
void addInAscendingOrder(tNode** head, struct tIndex index) {
    tNode* newOne = (tNode*) malloc(sizeof(tNode));
    newOne->index = index;

    if ((*head) == NULL) {
        newOne->next = NULL;
        newOne->prev = NULL;
        *head = newOne;
        return;
    }

    tNode* current = *head;
    while (current != NULL) {
        if ((current->index).id > index.id) {
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

/* Searches for the list element, which contains structure tIndex with the id equal to 'customerId'.
 * If tNode::tIndex::id is equal to 'customerId', returns true
 * and copy the found tIndex structure to the memory pointed by 'index'.
 * Otherwise, returns false.
 */
bool findIndex(tNode* head, struct tIndex* index, int customerId) {
    while (head != NULL) {
        if ((head->index).id == customerId) {
            (*index) = head->index;
            return true;
        }

        head = head->next;
    }

    return false;
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

// erases (free) the element, which contains structure tIndex with the id 'indexId'
void eraseFromList(tNode** head, int indexId) {
    tNode* current = *head;
    while (current != NULL) {
        if ((current->index).id == indexId) {
            deleteListNode(head, current);
            return;
        }
        current = current->next;
    }
}

void printList(tNode* head) {
    while (head != NULL) {
        printf("%d -> ", (head->index).id);
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
