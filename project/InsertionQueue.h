#define _HAS_STD_BYTE 0
#ifndef InsertionQueue_H
#define InsertionQueue_H
#include <string>
#include <utility>
#include <iostream>
#include "stdNode.h"
using namespace std;
template<class T>
class InsertionQueue
{
private:
    stdNode<T> *fr;
    stdNode<T>   *rear;
    int count;

public:
    InsertionQueue()
    {
        fr = rear = NULL;
        count = 0;
    }

    InsertionQueue(stdNode<T> *node)
    {
        fr = rear = node;
        count = (node != NULL) ? 1 : 0;
        if (node != NULL)
            node->setNext(NULL);
    }

    void enqueue(stdNode<T> *node)
    {
        if (node == NULL)
            return;

        node->setNext(NULL);

        if (fr == NULL)
        {
            fr = rear = node;
        }
        else
        {
            rear->setNext(node);
            rear = node;
        }
        count++;
    }

    void enqueue(T)
    {
        stdNode *node = new stdNode(id, table);
        enqueue(node);
    }

    stdNode<T> *dequeue()
    {
        if (fr == NULL)
            return NULL;

        stdNode<T> *temp = fr;
        fr = fr->getNext();

        if (fr == NULL)
            rear = NULL;

        count--;
        temp->setNext(NULL);
        return temp;
    }

    bool isEmpty() const
    {
        return fr == NULL;
    }

    int size() const
    {
        return count;
    }

    stdNode<T> *getFront() const
    {
        return fr;
    }

    stdNode<T> *getRear() const
    {
        return rear;
    }

    void printQueue() const
    {
        stdNode *curr = fr;
        while (curr != NULL)
        {
            auto [id, table] = curr->getMetaData();
            cout << "Id: " << id << ", Table: " << table << endl;
            curr = curr->getNext();
        }
    }
    stdNode<T> *searchQueue(int id)
    {
        stdNode<T> *curr = fr;
        while (curr != NULL)
        {
            T data = curr->getData();
            if (data.getId() == id)
                return curr;
            curr = curr->getNext();
        }
        return NULL;
    }
};

#endif // InsertionQueue_H