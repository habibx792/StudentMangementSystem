#ifndef UPNODE_H
#define UPNODE_H

#include <string>
using namespace std;

class upNode
{
private:
    int id;
    string table;

public:
    upNode *next;

    upNode(int id, const string &table)
    {
        this->id = id;
        this->table = table;
        this->next = NULL;
    }

    upNode()
    {
        this->id = -1;
        this->table = "NULL";
        this->next = NULL;
    }

    upNode *getNext() const
    {
        return next;
    }

    pair<int, string> getMetaData() const
    {
        return {id, table};
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
};

#endif // UPNODE_H