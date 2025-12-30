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

    TreeNode<T>* search(TreeNode<T>* node, const T& value)
    {
        if (!node)
            return nullptr;

        if (node->getData().getId() == value.getId())
            return node;

        if (value.getId() < node->getData().getId())
            return search(node->getLeft(), value);
        else
            return search(node->getRight(), value);
    }

public:
    BST() : root(nullptr) {}

    void insert(const T& value)
    {
        root = insert(root, value);
    }

    TreeNode<T>* searchData(const T& value)
    {
        return search(root, value);
    }

    TreeNode<T>* getRoot() const
    {
        return root;
    }
};

class DataHandle
{
private:
    Queue<Student> studentQueue;
    Queue<Course>  courseQueue;
    BST<Student>   studentTree;

public:
    // ===== Store data =====
    void setStudentQueue(const Queue<Student>& q)
    {
        studentQueue = q;   // copy
    }

    void setCourseQueue(const Queue<Course>& q)
    {
        courseQueue = q;
    }

    // ===== Build BST from stored queue =====
    void buildStudentBST()
    {
        Node<Student>* curr = studentQueue.getFront();
        while (curr)
        {
            studentTree.insert(curr->getValue());
            curr = curr->getNext();
        }
    }

    // ===== Print functions =====
    void printStudentsQueue() const
    {
        studentQueue.printQueue();
    }

    void printCoursesQueue() const
    {
        Node<Course>* curr = courseQueue.getFront();
        while (curr)
        {
            curr->getValue().coursePrint();
            curr = curr->getNext();
        }
    }

    void printStudentsBST() const
    {
        inorder(studentTree.getRoot());
    }

private:
    // inorder traversal helper
    static void inorder(TreeNode<Student>* node)
    {
        if (!node) return;
        inorder(node->getLeft());
        node->getData().printStd();
        inorder(node->getRight());
    }
};

#endif
