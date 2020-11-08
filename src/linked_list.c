#include <string.h>
#include "linked_list.h"
#include "stdlib.h"

LinkedList *newLinkedList() {
    LinkedList *head = (LinkedList *) malloc(sizeof(LinkedList));

    if (head == NULL) {
        printf("空间不足");
        return head;
    }

    head->element = NULL;
    head->next = NULL;
    return head;
}

void addLast(LinkedList *linkedList, void *element) {
    LinkedList *node = newLinkedList();
    node->element = element;

    LinkedList *lastNode = linkedList;
    while (lastNode->next != NULL) {
        lastNode = lastNode->next;
    }

    lastNode->next = node;
}

int len(LinkedList *linkedList) {
    int len = 0;
    LinkedList *currentNode = linkedList;
    while (currentNode != NULL) {
        len++;
        currentNode = currentNode->next;
    }
    return len - 1;
}

void *getItem(LinkedList *linkedList, int index) {
    if (index >= len(linkedList)) {
        printf("下标越界: %d\r\n", index);
        return NULL;
    }

    LinkedList *currentNode = linkedList;

    for (int i = 0; i <= index; ++i) {
        currentNode = currentNode->next;
    }
    return currentNode->element;
}
