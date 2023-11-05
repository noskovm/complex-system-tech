#ifndef CIRCULARDOUBLYLINKEDLIST_H
#define CIRCULARDOUBLYLINKEDLIST_H

template<typename T>
class Node {
public:
    Node(T p_data, Node<T>* p_prev = nullptr, Node<T>* p_next = nullptr):
    data(p_data), prev(p_prev), next(p_next) {}

    T data;
    Node<T>* prev;
    Node<T>* next;
};

template<typename T>
class CircularDoublyLinkedList {
public:
    CircularDoublyLinkedList(): current(nullptr) {}

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

    void push_next(T data) {
        Node<T>* new_node = new Node<T>(data);

        // если пустой, то current теперь одна нода, зацикленная на себя
        if (!current) {
            current = new_node;
            new_node->next = current;
            new_node->prev = current;
        } else {
            new_node->prev = current;
            (current->next)->prev = new_node;
            new_node->next = current->next;
            current->next = new_node;
            current = new_node;
        }
    }

    void go_next() {
        if (current) {
            current = current->next;
        }
    }

    Node<T> get_node() {
        return *current;
    }

private:
    Direction dir;
    Node<T>* current;
};


#endif //CIRCULARDOUBLYLINKEDLIST_H
