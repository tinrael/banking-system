#ifndef DOUBLYLINKEDLIST_H_INCLUDED
#define DOUBLYLINKEDLIST_H_INCLUDED

#include "BankDatabase.h"

typedef struct tagNode {
    struct tIndex index;

    struct tagNode* next;
    struct tagNode* prev;
} tNode;

// inserts an element to the beginning
void addToBeginning(tNode** head, struct tIndex index);

/* inserts an element in the ascending order by the index's ID  (tNode::index::id)
 * (from the smallest to the largest)
 */
void addInAscendingOrder(tNode** head, struct tIndex index);

/* Searches for the list element, which contains structure tIndex with the id equal to 'customerId'.
 * If tNode::tIndex::id is equal to 'customerId', returns true
 * and copy the found tIndex structure to the memory pointed by 'index'.
 * Otherwise, returns false.
 */
bool findIndex(tNode* head, struct tIndex* index, int customerId);

// erases (free) the list node pointed at by toDelete
void deleteListNode(tNode** head, tNode* toDelete);

// erases (free) the element, which contains structure tIndex with the id 'indexId'
void eraseFromList(tNode** head, int indexId);

void printList(tNode* head);

/* Erases (free) all elements.
 * The list head (*head) is equal to NULL after this operation.
 */
void clearList(tNode** head);

#endif // DOUBLYLINKEDLIST_H_INCLUDED
