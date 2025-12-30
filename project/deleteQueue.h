#ifndef DELETEQUEUE_H
#define DELETEQUEUE_H

#include <string>
#include <utility>
#include <iostream>
using namespace std;

// ================== Node ==================
class deleteNode
{
private:
    int id;
    string table;

public:
    deleteNode *next;

    // Parameterized constructor
    deleteNode(int id, const string &table)
    {
        this->id = id;
        this->table = table;
        this->next = NULL; // must initialize next
    }

    // Default constructor
    deleteNode()
    {
        this->id = -1;
        this->table = "NULL";
        this->next = NULL;
    }

    // ===== Getters =====
    deleteNode *getNext() const
    {
        return next;
    }

    pair<int, string> getMetaData() const
    {
        return {id, table};
    }

    // ===== Setters =====
    void setNext(deleteNode *n)
    {
        next = n;
    }

    void setMetaData(int id, const string &table)
    {
        this->id = id;
        this->table = table;
    }
};

// ================== Queue ==================
class deleteQueue
{
private:
    deleteNode *fr;
    deleteNode *rear;
    int count;

public:
    // ===== Constructors =====
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

    // ===== Queue Operations =====
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
        temp->setNext(NULL); // detach node
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

   

    // ===== Print Queue =====
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

#endif // DELETEQUEUE_H
