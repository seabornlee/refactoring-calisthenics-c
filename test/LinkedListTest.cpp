#include <gtest/gtest.h>

extern "C" {
#include <linked_list.h>
}

TEST(LinkedListTest, should_add_item) {
    LinkedList *list = newLinkedList();
    addLast(list, (void *) "xxx");
    addLast(list, (void *) "yyy");

    ASSERT_STREQ("xxx", (char *) list->next->element);
    ASSERT_STREQ("yyy", (char *) list->next->next->element);
}

TEST(LinkedListTest, should_get_length) {
    LinkedList *list = newLinkedList();

    addLast(list, (void *) "xxx");
    ASSERT_EQ(1, len(list));

    addLast(list, (void *) "yyy");
    ASSERT_EQ(2, len(list));
}

TEST(LinkedListTest, should_get_item_by_index) {
    LinkedList *list = newLinkedList();
    addLast(list, (void *) "xxx");
    addLast(list, (void *) "yyy");

    ASSERT_STREQ("xxx", (char *) getItem(list, 0));
    ASSERT_STREQ("yyy", (char *) getItem(list, 1));
}

TEST(LinkedListTest, should_nested) {
    LinkedList *list = newLinkedList();
    addLast(list, (void *) "xxx");
    addLast(list, (void *) "yyy");

    LinkedList *pList = newLinkedList();
    addLast(pList, list);

    LinkedList *item = static_cast<LinkedList *>(getItem(pList, 0));
    ASSERT_STREQ("xxx", (char *) getItem(item, 0));
    ASSERT_STREQ("yyy", (char *) getItem(item, 1));
}