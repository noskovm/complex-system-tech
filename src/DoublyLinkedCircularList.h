#ifndef TCS_DOUBLYLINKEDCIRCULARLIST_H
#define TCS_DOUBLYLINKEDCIRCULARLIST_H

template<typename T>
class Node {
public:
    Node(T p_data): data(p_data), prev(nullptr), next(nullptr) {}

    T data;
    T* prev;
    T* next;
};


class CircularLinkedList {
public:
    enum Direction {
        CLOCKWISE,
        COUNTERCLOCKWISE
    };

    void SetDirection (Direction dir) {
        this->dir = dir;
    }

    Direction GetDirection() {
        return this->dir;
    }

private:
    Direction dir;
};


#endif //TCS_DOUBLYLINKEDCIRCULARLIST_H
