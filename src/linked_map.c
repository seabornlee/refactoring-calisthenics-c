#include <string.h>
#include "linked_map.h"
#include "stdlib.h"
#include "stdio.h"

void putItem(LinkedMap *map, char *key, void *value) {
    LinkedMap *node = (LinkedMap *) malloc(sizeof(LinkedMap));

    node->key = key;
    node->value = value;
    node->next = NULL;

    LinkedMap *p = map;
    while (p->next != NULL) {
        p = p->next;
    }

    p->next = node;
}

void *getItemBy(LinkedMap *map, char *key) {
    LinkedMap *p = map;
    while (p != NULL) {
        if (p->key != NULL && strcmp(p->key, key) == 0) {
            return p->value;
        }
        p = p->next;
    }
    return (void *)0;
}

LinkedMap *newLinkedMap() {
    LinkedMap *pMap = (LinkedMap *) malloc(sizeof(LinkedMap));
    if (pMap == NULL) {
        printf("内存不足");
        return pMap;
    }

    pMap->key = NULL;
    pMap->value = NULL;
    pMap->next = NULL;
    return pMap;
}

void *getOrDefault(LinkedMap *map, char *key, void *defaultValue) {
    void *value = getItemBy(map, key);
    if (value) {
        return value;
    }
    return defaultValue;
}

LinkedList *keysOf(LinkedMap *map) {
    LinkedList *keys = newLinkedList();

    LinkedMap *p = map;
    while (p != NULL) {
        if (p->key != NULL) {
            addLast(keys, p->key);
        }
        p = p->next;
    }

    return keys;
}