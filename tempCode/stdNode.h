#ifndef stdNode_H
#define stdNode_H

#include <iostream>
#include "student.h"

using namespace std;

class stdNode
{
    Student std;
    stdNode *next;

public:
    stdNode(Student std)
    {
        this->std = std;
        this->next = NULL;
    }
    stdNode()
    {
        this->next = NULL;
        ;
    }
    void setSTD(Student std)
    {
        this->std = std;
    }
    Student &getValue()
    {
        return std;
    }
    void setNextNode(stdNode *next)
    {
        this->next = next;
    }
    stdNode *getNext()
    {
        return next;
    }
};
#endif