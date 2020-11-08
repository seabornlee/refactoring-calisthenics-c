#include <gtest/gtest.h>

extern "C" {
#include "linked_map.h"
}

TEST(LinkedListTest, should_put_item) {
    LinkedMap *map = newLinkedMap();

    putItem(map, "hello", (void *) "world");
    ASSERT_STREQ("hello", (char *) map->next->key);
    ASSERT_STREQ("world", (char *) map->next->value);

    putItem(map, "good", (void *) "bye");
    ASSERT_STREQ("good", (char *) map->next->next->key);
    ASSERT_STREQ("bye", (char *) map->next->next->value);
}

TEST(LinkedListTest, should_get_item) {
    LinkedMap *map = newLinkedMap();
    putItem(map, "hello", (void *) "world");
    putItem(map, "good", (void *) "bye");
    char *v1 = (char *) getItemBy(map, "hello");
    char *v2 = (char *) getItemBy(map, "good");

    ASSERT_STREQ("world", v1);
    ASSERT_STREQ("bye", v2);
}

TEST(LinkedListTest, should_get_item_when_exist) {
    LinkedMap *map = newLinkedMap();
    putItem(map, "hello", (void *) "world");
    char *v1 = (char *) getOrDefault(map, "hello", (void *) "");

    ASSERT_STREQ("world", v1);
}

TEST(LinkedListTest, should_get_default_valuee_when_not_exist) {
    LinkedMap *map = newLinkedMap();
    char *v1 = (char *) getOrDefault(map, "hello", (void *) "defaultValue");

    ASSERT_STREQ("defaultValue", v1);
}

TEST(LinkedListTest, should_hold_linked_list_as_value) {
    LinkedList *pList = newLinkedList();
    addLast(pList, (void *) "apple");
    addLast(pList, (void *) "and");

    LinkedMap *map = newLinkedMap();
    putItem(map, "hello", (void *) pList);
    LinkedList *v1 = (LinkedList *) getItemBy(map, "hello");

    ASSERT_STREQ("apple", (char *) getItem(pList, 0));
    ASSERT_STREQ("apple", (char *) getItem(v1, 0));
}

TEST(LinkedListTest, should_get_keys) {
    LinkedMap *map = newLinkedMap();
    putItem(map, "hello", (void *) "world");
    putItem(map, "good", (void *) "bye");
    LinkedList *keys = keysOf(map);

    ASSERT_STREQ("hello", (char *) getItem(keys, 0));
    ASSERT_STREQ("good", (char *) getItem(keys, 1));
}