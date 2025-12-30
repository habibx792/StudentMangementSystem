#ifndef NODE_H
#define NODE_H

template <class T>
class Node
{
    T data;
    Node<T>* next;

public:
    Node(const T& value) : data(value), next(nullptr) {}

    void setNext(Node<T>* n) { next = n; }
    Node<T>* getNext() const { return next; }

    T& getValue() { return data; }
};

#endif
