#include <iostream>
#include "CircularDoublyLinkedList.h"
#include <forward_list>

int main() {
    CircularDoublyLinkedList<int> list;
    list.set_clockwise_direction();
    list.push_next(5);
    list.push_next(6);
    list.push_next(7);
    list.push_next(11);
    list.push_next(13);
    list.set_counterclockwise_direction();
    list.pull();
    list.pull();
    list.pull();
    list.pull();
    list.pull();
    list.pull();

    for (int i = 0; i < list.len(); ++i) {
        std::cout << list.get_node().data << std::endl;
        list.go_next();
    }

    return 0;
}

//CircularLinkedList<int>