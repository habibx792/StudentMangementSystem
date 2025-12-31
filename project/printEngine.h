#ifndef PRINTEINGE_H
#define PRINTEINGE_H

#include <string>
#include "stdQueue.h"
#include "stdBst.h"
#include "deleteQueue.h"
#include "updationQueue.h"
#include "searchEngine.h"
#include "stdCourse.h"
#include "attendance.h"
using namespace std;

template <class T>
class printEinge
{
public:
    printEinge() {}
    // ------------------- Queue Printing -------------------
    void printUpdated(BST<T> &tree, upQueue &q)
    {
        upNode *curr = q.getFront();
        while (curr != nullptr)
        {
            auto [id, table] = curr->getMetaData();
            StdNode<T> *node = tree.search(id);
            if (node)
                node->getData().print();
            curr = curr->getNext();
        }
    }

    void printDeleted(BST<T> &tree, deleteQueue &q)
    {
        deleteNode *curr = q.getFront();
        while (curr != nullptr)
        {
            auto [id, table] = curr->getMetaData();
            StdNode<T> *node = tree.search(id);
            if (node)
                node->getData().print();
            curr = curr->getNext();
        }
    }
    void printNewInsertions(const opQueue<T> &q)
    {
        opNode<T> *curr = q.getFront();
        while (curr != nullptr)
        {
            curr->getData().print();
            curr = curr->getNext();
        }
    }
    // ------------------- Admin Printing -------------------
    void printAdminByName(BST<Admin> &tree, const string &name)
    {
        StdNode<Admin> *node = searchAdminByName(tree.getRoot(), name);
        if (node)
            node->getData().print();
        else
            cout << "Admin not found: " << name << endl;
    }

    void printAdminById(BST<Admin> &tree, int id)
    {
        StdNode<Admin> *node = searchAdminById(tree.getRoot(), id);
        if (node)
            node->getData().print();
        else
            cout << "Admin not found with ID: " << id << endl;
    }
    // Print StdCourse registrations
    void printStdCourseByStdId(BST<StdCourse> &tree, int stdId)
    {
        StdNode<StdCourse> *node = searchStdCourseByStdId(tree.getRoot(), stdId);
        if (node)
            node->getData().print();
        else
            cout << "No course registrations found for Student ID: " << stdId << endl;
    }

    void printStdCourseByCourseId(BST<StdCourse> &tree, int courseId)
    {
        StdNode<StdCourse> *node = searchStdCourseByCourseId(tree.getRoot(), courseId);
        if (node)
            node->getData().print();
        else
            cout << "No course registrations found for Course ID: " << courseId << endl;
    }

    // ------------------- Attendance Printing -------------------
    void printAttendanceByStdId(BST<Attendance> &tree, int stdId)
    {
        StdNode<Attendance> *node = searchAttendanceByStdId(tree.getRoot(), stdId);
        if (node)
            node->getData().print();
        else
            cout << "No attendance records for Student ID: " << stdId << endl;
    }
    void printAllAttenca(BST<Attendance> &tree)
    {
        cout << "Prinint all Attendanse : " << endl;
        printAllBST(tree.getRoot());
    }

    void printAttendanceByCourseId(BST<Attendance> &tree, int courseId)
    {
        StdNode<Attendance> *node = searchAttendanceByCourseId(tree.getRoot(), courseId);
        if (node)
            node->getData().print();
        else
            cout << "No attendance records for Course ID: " << courseId << endl;
    }

    void printAttendanceByDate(BST<Attendance> &tree, const string &date)
    {
        StdNode<Attendance> *node = searchAttendanceByDate(tree.getRoot(), date);
        if (node)
            node->getData().print();
        else
            cout << "No attendance records on Date: " << date << endl;
    }
    void printStudentByName(BST<Student> &tree, const string &name)
    {
        StdNode<Student> *node = searchStudentByName(tree.getRoot(), name);
        if (node)
            node->getData().print();
        else
            cout << "Student not found: " << name << endl;
    }
    // print add filed
    void printAllField(BST<FieldStudy> &tree)
    {
        printAllBST(tree.getRoot());
    }

    void printCourseByTitle(BST<Course> &tree, const string &title)
    {
        StdNode<Course> *node = searchCourseByTitle(tree.getRoot(), title);
        if (node)
            node->getData().print();
        else
            cout << "Course not found: " << title << endl;
    }

    void printPassResults(BST<Result> &tree)
    {
        StdNode<Result> *node = searchPassRec(tree.getRoot());
        if (node)
            node->getData().print();
        else
            cout << "No passing results found." << endl;
    }

    void printResultByStdId(BST<Result> &tree, int stdId)
    {
        StdNode<Result> *node = searchResultByStdId(tree.getRoot(), stdId);
        if (node)
            node->getData().print();
        else
            cout << "No results found for Student ID: " << stdId << endl;
    }

    void printResultByCourseId(BST<Result> &tree, int courseId)
    {
        StdNode<Result> *node = searchResultByCourseId(tree.getRoot(), courseId);
        if (node)
            node->getData().print();
        else
            cout << "No results found for Course ID: " << courseId << endl;
    }

    // ------------------- Generic BST Print -------------------
    void printAllBST(StdNode<T> *node)
    {
        if (!node)
            return;
        printAllBST(node->getLeft());
        node->getData().print();
        printAllBST(node->getRight());
    }
};

#endif // PRINTEINGE_H
