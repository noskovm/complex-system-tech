#include <iostream>
#include "DoublyLinkedCircularList.h"

CircularLinkedList::Direction Josephus_problem (CircularLinkedList our_list, CircularLinkedList::Direction dir, unsigned step) {
    our_list.SetDirection(dir);
    return our_list.GetDirection();
}

int main() {
    CircularLinkedList linkedList;
    linkedList.SetDirection(CircularLinkedList::COUNTERCLOCKWISE);
    std::cout << linkedList.GetDirection();
    std::cout << Josephus_problem(linkedList, CircularLinkedList::Direction::CLOCKWISE, 2);

    return 0;
}

//CircularLinkedList<int>