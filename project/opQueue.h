#ifndef OPQUEUE_H
#define OPQUEUE_H

#include <iostream>
#include <string>
#include "stdNode.h"
using namespace std;

template <class T>
class opNode : public StdNode<T>
{
private:
    opNode<T> *next;

public:
    opNode(const T &value) : StdNode<T>(value)
    {
        next = NULL;
    }

    opNode<T> *getNext() const
    {
        return next;
    }

    T getData() const
    {
        return this->data;
    }

    void setNext(opNode<T> *n)
    {
        next = n;
    }

    void setData(const T &d)
    {
        this->data = d;
    }
};

template <class T>
class opQueue
{
private:
    opNode<T> *front;
    opNode<T> *rear;
    int count;

public:
    opQueue()
    {
        front = NULL;
        rear = NULL;
        count = 0;
    }

    opQueue(opNode<T> *node)
    {
        front = node;
        rear = node;
        count = 1;

        if (node != NULL)
            node->setNext(NULL);
    }

    void enqueueNewStd(const T &value)
    {
        opNode<T> *node = new opNode<T>(value);

        if (front == NULL)
        {
            front = rear = node;
        }
        else
        {
            rear->setNext(node);
            rear = node;
        }
        count++;
    }

    void dequeueNewStd()
    {
        if (front == NULL)
            return;

        opNode<T> *temp = front;
        front = front->getNext();

        if (front == NULL)
            rear = NULL;

        delete temp;
        count--;
    }

    bool isEmpty() const
    {
        return front == NULL;
    }

    int size() const
    {
        return count;
    }

    opNode<T> *getFrontNewStd() const
    {
        return front;
    }

    opNode<T> *getRearNewStd() const
    {
        return rear;
    }
    void printQueue()
    {
        opNode<T> *current = front;
        while (current != NULL)
        {
            std::cout << current->getData() << " ";
            current = current->getNext();
        }
        std::cout << std::endl;
    }
};

#endif // OPQUEUE_H