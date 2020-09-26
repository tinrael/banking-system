#include "DoublyLinkedList.h"
#include <stdlib.h>

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

// TODO: need code refactoring.
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

void clearList(tNode** head) {
    tNode* toDelete;
    while ((*head) != NULL) {
        toDelete = *head;
        *head = (*head)->next;

        free(toDelete);
    }
}
