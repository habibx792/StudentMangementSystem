#define _HAS_STD_BYTE 0
#ifndef DELETEQUEUES_H
#define DELETEQUEUES_H

#include <string>
#include <utility>
#include <iostream>
#include "delNod.h"
using namespace std;

class deleteQueue
{
private:
    deleteNode *fr;
    deleteNode *rear;
    int count;

public:
    deleteQueue()
    {
        fr = rear = NULL;
        count = 0;
    }

    deleteQueue(deleteNode *node)
    {
        fr = rear = node;
        count = (node != NULL) ? 1 : 0;
        if (node != NULL)
            node->setNext(NULL);
    }
    void enqueue(deleteNode *node)
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
        deleteNode *node = new deleteNode(id, table);
        enqueue(node);
    }

    deleteNode *dequeue()
    {
        if (fr == NULL)
            return NULL;

        deleteNode *temp = fr;
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

    deleteNode *getFront() const
    {
        return fr;
    }

    deleteNode *getRear() const
    {
        return rear;
    }
    deleteNode * searchQueue(int id)
    {
        deleteNode* curr = fr;
        while (curr != NULL)
        {
            auto [searchId, table] = curr->getMetaData();
            if (searchId == id)
                return curr;
            curr = curr->getNext();
        }
        return NULL;
    }
    void printQueue() const
    {
        deleteNode *curr = fr;
        while (curr != NULL)
        {
            auto [id, table] = curr->getMetaData();
            cout << "Id: " << id << ", Table: " << table << endl;
            curr = curr->getNext();
        }
    }
};

#endif // DELETEQUEUES_H