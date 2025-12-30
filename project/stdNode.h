#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stireg.h>
#include <sstream>
#include <vector>
#include <limits>
#include <string>
#include "student.h"

#ifndef StdNode_H
#define StdNode_H

using namespace std;
template <class T>
class StdNode
{
private:
    // nothing added here

public:
    T data;
    StdNode<T> *left;
    stdNode<T> *right;

    StdNode(T d)
    {
        data(d);
        this->left = left;
        this->right = right;
    }

    // ===== Getters =====
    T getData() const
    {
        return data;
    }

    StdNode<T>* getLeft() const
    {
        return left;
    }

    stdNode<T>* getRight() const
    {
        return right;
    }

    // ===== Setters =====
    void setData(const T& value)
    {
        data = value;
    }

    void setLeft(StdNode<T>* node)
    {
        left = node;
    }

    void setRight(stdNode<T>* node)
    {
        right = node;
    }
};
#endif // CLASSNAME_H