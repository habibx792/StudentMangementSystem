#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "stdNode.h"

template <class T>
class Queue
{
    Node<T>* front;
    Node<T>* rear;
    int count;

public:
    Queue() : front(nullptr), rear(nullptr), count(0) {}

    void enQueue(const T& value)
    {
        Node<T>* node = new Node<T>(value);

        if (!rear)
            front = rear = node;
        else
        {
            rear->setNext(node);
            rear = node;
        }
        count++;
    }

    void deQueue()
    {
        if (!front) return;

        Node<T>* temp = front;
        front = front->getNext();
        delete temp;
        count--;

        if (!front) rear = nullptr;
    }
    Node<T>* getFront()
    {
        return front;
    }
    void printQueue()
    {
        Node<T>* temp = front;
        while (temp)
        {
            std::cout << temp->getValue().getName()
                      << " | Age: " << temp->getValue().getAge()
                      << std::endl;
            temp = temp->getNext();
        }
    }
};

#endif
