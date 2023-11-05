#ifndef CIRCULARDOUBLYLINKEDLIST_H
#define CIRCULARDOUBLYLINKEDLIST_H

// Объявление типа данных Node(узел списка)
template<typename T>
class Node {
public:
    Node(T p_data = T(), Node<T>* p_prev = nullptr, Node<T>* p_next = nullptr):
    data(p_data),
    prev(p_prev),
    next(p_next) {}

    T data;
    Node<T>* prev;
    Node<T>* next;
};


// Объявление типа данных Кольцевой Двусвязный список
template<typename T>
class CircularDoublyLinkedList {
public:
    CircularDoublyLinkedList();

    enum Direction {
        CLOCKWISE,
        COUNTERCLOCKWISE
    };
    void set_direction(Direction dir);
    Direction get_direction();

    void push_next(T data);
    void go_next();
    Node<T> get_node();

private:
    Direction dir;
    Node<T>* current;
};


// Реализация методов списка

// Конструктор
template<typename T>
CircularDoublyLinkedList<T>::CircularDoublyLinkedList():
    current(nullptr),
    dir(CircularDoublyLinkedList::Direction::CLOCKWISE) {}

// set_direction принимает на вход enum Direction и задает направление обхода кольцевого списка
template<typename T>
void CircularDoublyLinkedList<T>::set_direction (Direction dir){
    this->dir = dir;
}

// get_direction возвращает направление обхода кольцевого списка
template<typename T>
typename CircularDoublyLinkedList<T>::Direction CircularDoublyLinkedList<T>::get_direction() {
    return this->dir;
}


// push_next добавляет новый узел списка после текущего узла и устанавливает текущий на место вновь прибывшего
template<typename T>
void CircularDoublyLinkedList<T>::push_next(T data) {
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

// go_next меняет узел current на следующий за ним
template<typename T>
void CircularDoublyLinkedList<T>::go_next() {
    if (current) {
        current = current->next;
    }
}

// get_node возвращает объект типа Node
template<typename T>
Node<T> CircularDoublyLinkedList<T>::get_node() {
    return *current;
}

#endif //CIRCULARDOUBLYLINKEDLIST_H
