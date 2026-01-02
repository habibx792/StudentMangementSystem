
#include <iostream>
using namespace std;
#include "stdNode.h"
template <class T>
class StdNode
{
protected:
    T data;
    StdNode<T> *left;
    StdNode<T> *right;

public:
    int height;
    StdNode(const T &d)
    {
        data = d;
        left = nullptr;
        int height = 1;
        right = nullptr;
    }

    // Getter and setter methods
    T getData() const { return data; }
    StdNode<T> *getLeft() const { return left; }
    StdNode<T> *getRight() const { return right; }
    int getHeight() const { return height; }
    void setHeight(int h) { height = h; }
    void setData(const T &d) { data = d; }
    void setLeft(StdNode<T> *l) { left = l; }
    void setRight(StdNode<T> *r) { right = r; }
};
template <class T>
int getHeight(const StdNode<T> *node)
{
    return node ? node->getHeight() : 0;
}
template <class T>
int getBalanceFactor(const StdNode<T> *node)
{
    if (!node)
    {
        return 0;
    }
    return getHeight(node->getLeft()) - getHeight(node->getRight());
}
template<class T>
StdNode<T> *leftRotation(StdNode<T> *root)
{
    StdNode<T> *newRoot = root->getRight();
    root->setRight(newRoot->getLeft());
    newRoot->setLeft(root);
    root->setHeight(1 + max(getHeight(root->getLeft()), getHeight(root->getRight())));
    newRoot->setHeight(1 + max(getHeight(newRoot->getLeft()), getHeight(newRoot->getRight())));
    return newRoot;
}
template<class T>
StdNode<T> *rightRotation(StdNode<T> *root)
{
    StdNode<T> *newRoot = root->getLeft();
    root->setLeft(newRoot->getRight());
    newRoot->setRight(root);
    root->setHeight(1 + max(getHeight(root->getLeft()), getHeight(root->getRight())));
    newRoot->setHeight(1 + max(getHeight(newRoot->getLeft()), getHeight(newRoot->getRight())));
    return newRoot;
}
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
        else
        {
            return node;
        }
        node->setHeight(1 + max(getHeight(node->getLeft()), getHeight(node->getRight())));
        int balance = getBalanceFactor(node);

        if (balance > 1 && value.getId() < node->getData().getId())
            node->setLeft(insertRec(node->getLeft(), value));
        else if (balance < -1 && value.getId() > node->getData().getId())
            node->setRight(insertRec(node->getRight(), value));
        else if (balance > 1 && value.getId() > node->getData().getId())
        {
            node->setLeft(insertRec(node->getLeft(), value));
            return rotateRight(node);
        }
        else if (balance < -1 && value.getId() < node->getData().getId())
        {
            node->setRight(insertRec(node->getRight(), value));
            return rotateLeft(node);
        }

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
int main()
{
    cout << "Hello";
}