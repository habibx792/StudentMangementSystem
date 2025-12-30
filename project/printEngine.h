#ifndef PRINTEINGE_H
#define PRINTEINGE_H

#include <string>
#include "stdQueue.h"
#include "stdBst.h"
#include "deleteQueue.h"
#include "updationQueue.h"
using namespace std;

template <class T>
class printEinge
{
public:
    printEinge() {}

    // Print updated students
    void printUpdatedStd(BST<T> &tree, upQueue &q)
    {
        upNode *curr = q.getFront();
        while (curr != nullptr)
        {
            auto [id, table] = curr->getMetaData();
            StdNode<T> *node = tree.search(id);
            if (node)
            {
                T obj = node->getData();
                obj.print();
            }
            curr = curr->getNext();
        }
    }

    // Print deleted students
    void printDeletionStd(BST<T> &tree, deleteQueue &q)
    {
        deleteNode *curr = q.getFront();
        while (curr != nullptr)
        {
            auto [id, table] = curr->getMetaData();
            StdNode<T> *node = tree.search(id);
            if (node)
            {
                T obj = node->getData();
                obj.print();
            }
            curr = curr->getNext();
        }
    }

    // Print newly inserted students
    void printNewInsertions(const opQueue<T> &q)
    {
        opNode<T> *curr = q.getFront();
        while (curr != nullptr)
        {
            T obj = curr->getData();
            obj.print();
            curr = curr->getNext();
        }
    }
};

#endif // PRINTEINGE_H
