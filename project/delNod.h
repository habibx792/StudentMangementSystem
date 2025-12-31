#define _HAS_STD_BYTE 0
#ifndef DELETENODE_H
#define DELETENODE_H
#include <string>
using namespace std;

class deleteNode
{
private:
    int id;
    string table;

public:
    deleteNode *next;

    deleteNode(int id, const string &table)
    {
        this->id = id;
        this->table = table;
        this->next = NULL;
    }

    deleteNode()
    {
        this->id = -1;
        this->table = "NULL";
        this->next = NULL;
    }

    deleteNode *getNext() const
    {
        return next;
    }

    pair<int, string> getMetaData() const
    {
        return {id, table};
    }

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

#endif // DELETENODE_H