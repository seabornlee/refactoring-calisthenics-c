#ifndef REFACTOR_CALISTHENICS_C_LINKED_MAP_H
#define REFACTOR_CALISTHENICS_C_LINKED_MAP_H

#include "linked_list.h"

typedef struct linked_map {
    void *key;
    void *value;
    struct linked_map *next;
} LinkedMap;

LinkedMap *newLinkedMap();
void putItem(LinkedMap *map, char *key, void *value);
void *getItemBy(LinkedMap *map, char *key);
void *getOrDefault(LinkedMap *map, char *key, void *defaultValue);
LinkedList *keysOf(LinkedMap *map);
#endif //REFACTOR_CALISTHENICS_C_LINKED_MAP_H
