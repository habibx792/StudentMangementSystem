#ifndef UPDATIONQUEUE_H
#define UPDATIONQUEUE_H

#include <string>
#include <utility>
#include <iostream>
#include "upNode.h"
using namespace std;

class upQueue
{
private:
    upNode *fr;
    upNode *rear;
    int count;

public:
    upQueue()
    {
        fr = rear = NULL;
        count = 0;
    }

    upQueue(upNode *node)
    {
        fr = rear = node;
        count = (node != NULL) ? 1 : 0;
        if (node != NULL)
            node->setNext(NULL);
    }

    void enqueue(upNode *node)
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

    void enqueue(int id, const string &table)
    {
        upNode *node = new upNode(id, table);
        enqueue(node);
    }

    upNode *dequeue()
    {
        if (fr == NULL)
            return NULL;

        upNode *temp = fr;
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

    upNode *getFront() const
    {
        return fr;
    }

    upNode *getRear() const
    {
        return rear;
    }

    void printQueue() const
    {
        upNode *curr = fr;
        while (curr != NULL)
        {
            auto [id, table] = curr->getMetaData();
            cout << "Id: " << id << ", Table: " << table << endl;
            curr = curr->getNext();
        }
    }
};

#endif // UPDATIONQUEUE_H