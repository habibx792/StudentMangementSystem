#ifndef list_H
#define list_H

#include <string>
#include "stdNode.h"
#include "student.h"
using namespace std;

class queue
{
private:
    stdNode *front;
    stdNode *rear;
    int count;

public:
    // constructors
    queue()
    {
        this->front = rear = NULL;
        this->count = 0;
    }
    void enQue(stdNode *node)
    {
        node->setNextNode(NULL);
        if (rear == NULL)
        {
            this->rear = front = node;
        }
        else
        {
            rear->setNextNode(node);
            rear = node;
        }

        count++;
    }
    void deQueue()
    {
        if (front == NULL)
        {
            return;
        }
        stdNode *temp = front;
        front = front->getNext();
        delete temp;
        count--;

        if (front == nullptr)
            rear = nullptr;
    }
    int size()
    {
        return count;
    }
    bool isEmpty()
    {
        return front == NULL;
    }
    stdNode *getFront() const
    {
        return front;
    }
    void printQueue()
    {
        stdNode *temp = front;
        while (temp != nullptr)
        {
            Student std = temp->getValue();
            cout << "Name: " << std.getName() << ", Age: " << std.getAge() << endl;
            temp = temp->getNext();
        }
    }
};

#endif // list_H