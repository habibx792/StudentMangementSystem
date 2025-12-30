#ifndef BST_H
#define BST_H
#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stireg.h>
#include <sstream>
#include <vector>
#include <limits>
#include "stdNode.h"
#include "student.h"
using namespace std;
template<class T>
class BST {
private:
   stdNode<T>* root;
    stdNode<T> *logSearch(stdNode<T> *node, int id)
    {
        if (node == NULL)
            return NULL;

        if (node->data.getId() == id)
            return node;

        if (id < node->data.getId())
            return logSearch(node->left, id);
        else
            return logSearch(node->right, id);
    }
    stdNode<T> *insertRec(stdNode<T> *node, T value)
    {
        if (node == NULL)
            return new Node<T>(value);

        if (value.getId() < node->data.getId())
            node->left = insertRec(node->left, value);
        else if (value.getId() > node->data.getId())
            node->right = insertRec(node->right, value);

        return node;
    }


public:
    Node<T> *root;
    BST()
    {
        this->root = NULL;
    }
    BST(Node<T> *nod)
    {
        this->root = nod;
    }
    void buildBst(T value)
    {
        root = insertRec(root, value);
    }
    Node<T> *search(int id)
    {
        return logSearch(root, id);
    }
};

#endif // BST_H