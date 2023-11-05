#include <iostream>
#include "CircularDoublyLinkedList.h"
#include <forward_list>

int main() {
    CircularDoublyLinkedList<int> list;
    list.push_next(5);
    list.push_next(6);
    list.push_next(7);
    list.push_next(11);
    list.push_next(13);

    for (int i = 0; i < 3; ++i) {
        std::cout << list.get_node().data;
        list.go_next();
    }

    return 0;
}

//CircularLinkedList<int>