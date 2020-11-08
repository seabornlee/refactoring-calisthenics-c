#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct LinkedList {
    void *element;
    struct LinkedList *next;
} LinkedList;

LinkedList *newLinkedList();

void addLast(LinkedList *linkedList, void *element);

int len(LinkedList *linkedList);

void *getItem(LinkedList *linkedList, int index);

#endif