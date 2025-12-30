#ifndef DataHandle_H
#define DataHandle_H
#include <iostream>
#include <string>
#include "course.h"
#include "classList.h"
#include "stdNode.h"
#include "student.h"
using namespace std;
template <class T>
class TreeNode
{
    T data;
    TreeNode *left;
    TreeNode *right;

public:
    TreeNode(T data)
    {
        this->data = data;

        this->left = NULL;
        this->right = NULL;
    }
    TreeNode()
    {
        this->data();
        this->left = NULL;
        this->right = NULL;
    }
    void setData(const T &value) { data = value; }
    T &getData() { return data; }

    void setLeft(TreeNode<T> *l) { left = l; }
    void setRight(TreeNode<T> *r) { right = r; }

    TreeNode<T> *getLeft() const 
    { 
        return left;
     }
    TreeNode<T> *getRight() const 
    {
         return right;
         }
};
template <class T>
class BST
{
    TreeNode<T>* root;

    TreeNode<T>* insert(TreeNode<T>* node, const T& value)
    {
        if (!node)
            return new TreeNode<T>(value);

        if (value.getId() < node->getData().getId())
            node->setLeft(insert(node->getLeft(), value));
        else
            node->setRight(insert(node->getRight(), value));
        return node;
    }

public:
    BST() : root(nullptr) {}

    void insert(const T& value)
    {
        root = insert(root, value);
    }

    TreeNode<T>* getRoot() const
    {
        return root;
    }
    TreeNode<T> * searchData(Const T& value)
    {
        if(root==NULL)
        {
            return root;
        }
        T curr= id=root->getData();
        if(curr.getId()==value.getId)
        {
            return root;
        }
        if(value.getId()<curr.)
    }
};

class DataHandle
{
private:
public:
    // constructors
    DataHandle();
    DataHandle();
    template <class T>
    Queue<T> getStdView()
    {
        return Queue<T>;
    }

    // getters
    string getmemberName() const;

    // setters
    void setmemberName(const string &value);
};

#endif // DataHandle_H