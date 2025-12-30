#ifndef DELETEQUEUE_H
#define DELETEQUEUE_H
#include <string>
#include <utility>
#include <iostream>
using namespace std;

// ================== Node ==================
class upNode
{
private:
    int id;
    string table;

public:
    upNode *next;

    // Parameterized constructor
    upNode(int id, const string &table)
    {
        this->id = id;
        this->table = table;
        this->next = NULL; // must initialize next
    }

    // Default constructor
    upNode()
    {
        this->id = -1;
        this->table = "NULL";
        this->next = NULL;
    }

    // ===== Getters =====
    upNode *getNext() const
    {
        return next;
    }

    pair<int, string> getMetaData() const
    {
        return {id, table};
    }

    // ===== Setters =====
    void setNext(upNode *n)
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
class upQueue
{
private:
    upNode *fr;
    upNode *rear;
    int count;

public:
    // ===== Constructors =====
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

    // ===== Queue Operations =====
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

    upNode *getFront() const
    {
        return fr;
    }

    upNode *getRear() const
    {
        return rear;
    }

       // ===== Print Queue =====
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

#endif // DELETEQUEUE_H
