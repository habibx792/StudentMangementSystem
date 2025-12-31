#define _HAS_STD_BYTE 0
#ifndef STDBST_H
#define STDBST_H

#include "stdNode.h"

template <class T>
class BST
{
private:
    StdNode<T> *root;

    StdNode<T> *insertRec(StdNode<T> *node, const T &value)
    {
        if (!node)
            return new StdNode<T>(value);

        if (value.getId() < node->getData().getId())
            node->setLeft(insertRec(node->getLeft(), value));
        else if (value.getId() > node->getData().getId())
            node->setRight(insertRec(node->getRight(), value));

        return node;
    }

    StdNode<T> *searchRec(StdNode<T> *node, int id) const
    {
        if (!node)
            return nullptr;

        if (node->getData().getId() == id)
            return node;

        if (id < node->getData().getId())
            return searchRec(node->getLeft(), id);
        else
            return searchRec(node->getRight(), id);
    }

public:
    BST() : root(nullptr) {}

    void insert(const T &value)
    {
        root = insertRec(root, value);
    }

    StdNode<T> *search(int id) const
    {
        return searchRec(root, id);
    }

    StdNode<T> *getRoot() const
    {
        return root;
    }
};

#endif // BST_H