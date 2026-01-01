#define _HAS_STD_BYTE 0
#ifndef PRINTENGINE_H
#define PRINTENGINE_H

#include <iostream>
#include <string>
using namespace std;

#include "stdBst.h"
#include "stdNode.h"
#include "deleteQueues.h"
#include "updationQueue.h"
#include "opQueue.h"

#include "admin.h"
#include "student.h"
#include "course.h"
#include "stdResult.h"
#include "attendance.h"
#include "stdCourse.h"
#include "field.h"
#include "stdFee.h"

#include "searchEngine.h"

class PrintEngine
{
private:
    template <class T>
    void printAllNodes(StdNode<T> *node)
    {
        if (!node)
            return;
        printAllNodes(node->getLeft());
        node->getData().print();
        printAllNodes(node->getRight());
    }

public:
    PrintEngine() {}

    // =========================
    // Generic Template Printers
    // =========================

    template <class T>
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

    template <class T>
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

    template <class T>
    void printNewInsertions(const opQueue<T> &q)
    {
        opNode<T> *curr = q.getFront();
        while (curr != nullptr)
        {
            curr->getData().print();
            curr = curr->getNext();
        }
    }

    template <class T>
    void printAllBST(BST<T> &tree, const string &title = "")
    {
        if (!title.empty())
            cout << "\n=== " << title << " ===" << endl;
        else
            cout << "\n=== All Records ===" << endl;
        printAllNodes(tree.getRoot());
    }

    // =========================
    // Admin Printers
    // =========================

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

    void printAllAdmins(BST<Admin> &tree)
    {
        printAllBST(tree, "All Admins");
    }

    // =========================
    // Student Printers
    // =========================

    void printStudentByName(BST<Student> &tree, const string &name)
    {
        StdNode<Student> *node = searchStudentByName(tree.getRoot(), name);
        if (node)
            node->getData().print();
        else
            cout << "Student not found: " << name << endl;
    }

    void printStudentById(BST<Student> &tree, int id)
    {
        StdNode<Student> *node = searchStudentById(tree.getRoot(), id);
        if (node)
            node->getData().print();
        else
            cout << "Student not found with ID: " << id << endl;
    }

    void printAllStudents(BST<Student> &tree)
    {
        printAllBST(tree, "All Students");
    }

    // =========================
    // Course Printers
    // =========================

    void printCourseByTitle(BST<Course> &tree, const string &title)
    {
        StdNode<Course> *node = searchCourseByTitle(tree.getRoot(), title);
        if (node)
            node->getData().print();
        else
            cout << "Course not found: " << title << endl;
    }

    void printCourseById(BST<Course> &tree, int id)
    {
        StdNode<Course> *node = searchCourseById(tree.getRoot(), id);
        if (node)
            node->getData().print();
        else
            cout << "Course not found with ID: " << id << endl;
    }

    void printAllCourses(BST<Course> &tree)
    {
        printAllBST(tree, "All Courses");
    }

    // =========================
    // Field Printers
    // =========================

    void printFieldByName(BST<FieldStudy> &tree, const string &name)
    {
        StdNode<FieldStudy> *node = searchFieldByName(tree.getRoot(), name);
        if (node)
            node->getData().print();
        else
            cout << "Field not found: " << name << endl;
    }

    void printFieldById(BST<FieldStudy> &tree, int id)
    {
        StdNode<FieldStudy> *node = searchFieldById(tree.getRoot(), id);
        if (node)
            node->getData().print();
        else
            cout << "Field not found with ID: " << id << endl;
    }

    void printAllFields(BST<FieldStudy> &tree)
    {
        printAllBST(tree, "All Fields");
    }

    // =========================
    // StdCourse Printers
    // =========================

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

    void printAllStdCourses(BST<StdCourse> &tree)
    {
        printAllBST(tree, "All Student Course Registrations");
    }

    // =========================
    // Attendance Printers
    // =========================

    void printAttendanceByStdId(BST<Attendance> &tree, int stdId)
    {
        StdNode<Attendance> *node = searchAttendanceByStdId(tree.getRoot(), stdId);
        if (node)
            node->getData().print();
        else
            cout << "No attendance records for Student ID: " << stdId << endl;
    }
    void printAllAttendance(BST<Attendance> &tree)
    {
        printAllBST(tree, "All Attendance Records");
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

    // =========================
    // Result Printers
    // =========================

    void printPassResults(BST<Result> &tree)
    {
        StdNode<Result> *node = searchPassRec(tree.getRoot());
        if (node)
        {
            cout << "\n=== Passing Results ===" << endl;
            node->getData().print();
        }
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
    void printAllResults(BST<Result> &tree)
    {
        printAllBST(tree, "All Results");
    }

    // =========================
    // Student Fees Printers
    // =========================

    void printStudentFeesByStdId(BST<StudentFees> &tree, int stdId)
    {
        StdNode<StudentFees> *node = searchStdFeeByStdId(tree.getRoot(), stdId);
        if (node)
            node->getData().print();
        else
            cout << "No fee records found for Student ID: " << stdId << endl;
    }
    void printAllStudentFees(BST<StudentFees> &tree)
    {
        printAllBST(tree, "All Student Fees");
    }

    // =========================
    // Utility Printers
    // =========================

    void printSeparator(const string &title = "")
    {
        cout << "\n========================================" << endl;
        if (!title.empty())
            cout << title << endl;
        cout << "========================================\n"
             << endl;
    }

    void printMessage(const string &message)
    {
        cout << ">> " << message << endl;
    }

    void printError(const string &error)
    {
        cout << "ERROR: " << error << endl;
    }

    void printSuccess(const string &message)
    {
        cout << "SUCCESS: " << message << endl;
    }
};

#endif // PRINTENGINE_H
