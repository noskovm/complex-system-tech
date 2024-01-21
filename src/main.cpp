#include <iostream>
#include "problem_set_0/CircularDoublyLinkedList.h"
#include <forward_list>

int main() {

    std::setlocale(LC_ALL, "ru");
    CircularDoublyLinkedList<int> list;
    list.set_clockwise_direction();
    list.push_next(5);
    list.push_next(6);
    list.push_next(7);
    list.push_next(11);
    list.push_next(13);

    list.del();


    return 0;
}

template<typename T>
std::pair<int, T> Josephus_problem(CircularDoublyLinkedList<T> list, typename CircularDoublyLinkedList<T>::Direction dir, int step) {
    //
}

//CircularLinkedList<int>