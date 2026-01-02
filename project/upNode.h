#define _HAS_STD_BYTE 0
#ifndef UPNODE_H
#define UPNODE_H

#include <string>
using namespace std;

class upNode
{
private:
    int id;
    int id2;
    string table;

public:
    upNode *next;

    upNode(int id, const string &table)
    {
        this->id = id;
        this->table = table;
        this->next = NULL;
    }
    upNode(int id, int id2, const string &table)
    {
        this->id = id;
        this->id2 = id2;
        this->table = table;
        this->next = NULL;
    }

    upNode()
    {
        this->id = -1;
        this->id2 = -1;
        this->table = "NULL";
        this->next = NULL;
    }

    upNode *getNext() const
    {
        return next;
    }

    pair<pair<int, int>, string> getMetaData() const
    {
        return {{id, id2},   table};
    }
    int getId1()const
    {
        return id;
    }

    int getId2()const
    {
        return id2;
    }
    string & getTable()
    {
        return table;
    }
    void setNext(upNode *n)
    {
        next = n;
    }

    void setMetaData(int id, const string &table)
    {
        this->id = id;
        this->table = table;
    }
    void setMetaData(int id, int id2, const string &table)
    {
        this->id = id;
        this->id2 = id2;
        this->table = table;
    }
};

#endif // UPNODE_H