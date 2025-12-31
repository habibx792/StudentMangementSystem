#ifndef BST_H
#define BST_H

#include "stdNode.h"
#include "student.h"
#include "course.h"
#include "stdResult.h"
#include "stdBst.h"
inline StdNode<Result> *searchPassRec(StdNode<Result> *node)
{
    if (!node)
        return nullptr;

    if (node->getData().isPass())
        return node;

    StdNode<Result> *left = searchPassRec(node->getLeft());
    if (left)
        return left;

    return searchPassRec(node->getRight());
}

// Search Student by name
inline StdNode<Student> *searchStudentByName(StdNode<Student> *node, const std::string &name)
{
    if (!node)
        return nullptr;

    if (node->getData().getName() == name)
        return node;

    StdNode<Student> *left = searchStudentByName(node->getLeft(), name);
    if (left)
        return left;

    return searchStudentByName(node->getRight(), name);
}

// Search Course by title
inline StdNode<Course> *searchCourseByTitle(StdNode<Course> *node, const std::string &title)
{
    if (!node)
        return nullptr;

    if (node->getData().getCourseTitle() == title)
        return node;

    StdNode<Course> *left = searchCourseByTitle(node->getLeft(), title);
    if (left)
        return left;

    return searchCourseByTitle(node->getRight(), title);
}

template <class T>
class BST
{
private:
    StdNode<T> *root;

    // Insert recursively based on getId()
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

    // Search recursively by ID
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
    // Search Result that is Pass
    StdNode<Result> *searchPassRec(StdNode<Result> *node)
    {
        if (!node)
            return nullptr;

        if (node->getData().isPass())
            return node;

        StdNode<Result> *left = searchPassRec(node->getLeft());
        if (left)
            return left;

        return searchPassRec(node->getRight());
    }

    // Search Student by name
    StdNode<Student> *searchStudentByName(StdNode<Student> *node, const std::string &name)
    {
        if (!node)
            return nullptr;

        if (node->getData().getName() == name)
            return node;

        StdNode<Student> *left = searchStudentByName(node->getLeft(), name);
        if (left)
            return left;

        return searchStudentByName(node->getRight(), name);
    }

    // Search Course by title
    StdNode<Course> *searchCourseByTitle(StdNode<Course> *node, const std::string &title)
    {
        if (!node)
            return nullptr;

        if (node->getData().getCourseTitle() == title)
            return node;

        StdNode<Course> *left = searchCourseByTitle(node->getLeft(), title);
        if (left)
            return left;

        return searchCourseByTitle(node->getRight(), title);
    }
};

#endif // BST_H
