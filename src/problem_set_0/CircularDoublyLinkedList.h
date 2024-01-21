#ifndef CIRCULARDOUBLYLINKEDLIST_H
#define CIRCULARDOUBLYLINKEDLIST_H
#include <iostream>

// Объявление типа данных Node(узел списка)
template<typename T>
class Node {
public:
    //Узел будет хранить Индекс(Обязательный параметр) и Данные(если не зададут, то будет вызван конструктор по умолчанию)
    Node(int p_index, T p_data = T(), Node<T>* p_prev = nullptr, Node<T>* p_next = nullptr):
    index(p_index),
    data(p_data),
    prev(p_prev),
    next(p_next) {}


    int index;
    T data;
    Node<T>* prev;
    Node<T>* next;
};


// Объявление типа данных Кольцевой Двусвязный список
template<typename T>
class CircularDoublyLinkedList {
public:

    CircularDoublyLinkedList();
    //~CircularDoublyLinkedList();

    enum Direction {
        CLOCKWISE,
        COUNTERCLOCKWISE
    };

    void del();
    void set_clockwise_direction();
    void set_counterclockwise_direction();
    Direction get_direction();

    void push_next(T data);
    int pull();
    void go_next();
    Node<T> get_node();
    int len();

private:
    Direction dir;
    Node<T>* head;
    Node<T>* current;
    int length;
};


// Реализация методов списка

// Конструктор
template<typename T>
CircularDoublyLinkedList<T>::CircularDoublyLinkedList():
    current(nullptr),
    dir(CircularDoublyLinkedList::Direction::CLOCKWISE),
    length(0) {}

template<typename T>
void CircularDoublyLinkedList<T>::del() {
    for (int i = 0; i < length; ++i) {
        std::cout << "Удаляю элемент под индексом ";
        std::cout << pull();
    }
}

// set_clockwise_direction задает направление обхода списка по часовой стрелке
template<typename T>
void CircularDoublyLinkedList<T>::set_clockwise_direction(){
    this->dir = Direction::CLOCKWISE;
}

// set_counterclockwise_direction задает направление обхода списка против часовой стрелки
template<typename T>
void CircularDoublyLinkedList<T>::set_counterclockwise_direction(){
    this->dir = Direction::COUNTERCLOCKWISE;
}

// get_direction возвращает направление обхода кольцевого списка
template<typename T>
typename CircularDoublyLinkedList<T>::Direction CircularDoublyLinkedList<T>::get_direction() {
    return this->dir;
}


// push_next добавляет новый узел списка после текущего узла и устанавливает current на место вновь прибывшего
template<typename T>
void CircularDoublyLinkedList<T>::push_next(T data) {
    Node<T>* newNode = new Node<T>(len(), data); // index = текущая длина, удобно, когда нумерация с 0

    // если пустой, то current теперь одна нода, зацикленная на себя
    if (!current) {
        current = newNode;
        newNode->next = current;
        newNode->prev = current;
    } else { // если не пустой, то смотрим в каком направлении вставлять
        if (dir == CircularDoublyLinkedList<T>::Direction::CLOCKWISE) {
            newNode->prev = current;
            (current->next)->prev = newNode;
            newNode->next = current->next;
            current->next = newNode;
        } else {
            newNode->next = current;
            newNode->prev =current->prev;
            current->prev = newNode;
            (newNode->prev)->next = newNode;
        }
        current = newNode;
    }
    length += 1;
}

template<typename T>
int CircularDoublyLinkedList<T>::pull() {
    if (len() > 0) {
        int indexToReturn = current->index;

        // перебрасываем связи, но current, все еще хранит инфу о соседях
        (current->prev)->next = current->next;
        (current->next)->prev = current->prev;

        //запоминаем местонахождение следующего current в зависимости от направления
        Node<T>* currentNextLoc = current->next;
        if (dir == Direction::COUNTERCLOCKWISE)
            currentNextLoc = current->prev;

        delete current;

        current = currentNextLoc;

        length -= 1;
        return indexToReturn;
    }
    return -1;
}



// go_next меняет узел current на следующий за ним
template<typename T>
void CircularDoublyLinkedList<T>::go_next() {
    if (current) {
        if (dir == Direction::CLOCKWISE) {
            current = current->next;
        } else {
            current = current->prev;
        }
    }
}

// get_node возвращает объект типа Node
template<typename T>
Node<T> CircularDoublyLinkedList<T>::get_node() {
    return *current;
}

template<typename T>
int CircularDoublyLinkedList<T>::len() {
    return this->length;
}

#endif //CIRCULARDOUBLYLINKEDLIST_H
