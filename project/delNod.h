#define _HAS_STD_BYTE 0
#ifndef DELETENODE_H
#define DELETENODE_H
#include <string>
using namespace std;

class deleteNode
{
private:
    int id;
    int idTwo;
    string table;

public:
    deleteNode *next;

    deleteNode(int id, const string &table)
    {
        this->id = id;
        this->table = table;
        this->next = NULL;
        this->idTwo = -1;
        
    }
    deleteNode(int id, int idTwo, const string &table)
    {
        this->id = id;
        this->idTwo = idTwo;
        this->table = table;
        this->next = NULL;
    }
    bool hasSecondId() const
    {
        return idTwo != -1;
    }
    deleteNode()
    {
        this->id = -1;
        this->idTwo = -1;
        this->table = "NULL";
        this->next = NULL;
    }

    deleteNode *getNext() const
    {
        return next;
    }

    pair<pair<int, int>, string> getMetaData() const
    {
        return {{id, idTwo}, table};
    }
    int getFirstId()const
    {
        return id;
    }
    int getSecondId()const
    {
        return idTwo;
    }
    string &getTable()
    {
        return table;
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
    void setMetaData(int id, int idTwo, const string &table)
    {
        this->id = id;
        this->idTwo = idTwo;
        this->table = table;
    }
   
};

#endif // DELETENODE_H