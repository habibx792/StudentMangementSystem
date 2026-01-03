#define _HAS_STD_BYTE 0
#ifndef SEARCHENGINE_H // Fixed: Changed from searchEngine_H to SEARCHENGINE_H
#define SEARCHENGINE_H

#include "stdNode.h"
#include "student.h"
#include "course.h"
#include "stdResult.h"
#include "attendance.h"
#include "stdCourse.h"
#include "stdBst.h"
#include "admin.h"
#include "field.h"
#include "stdFee.h"
#include <string>

// ------------------- Result Searches -------------------
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

inline StdNode<Result> *searchResultByStdId(StdNode<Result> *node, int stdId)
{
    if (!node)
        return nullptr;
    if (node->getData().getStdId() == stdId)
        return node;
    StdNode<Result> *left = searchResultByStdId(node->getLeft(), stdId);
    if (left)
        return left;
    return searchResultByStdId(node->getRight(), stdId);
}

inline StdNode<Result> *searchResultByCourseId(StdNode<Result> *node, int courseId)
{
    if (!node)
        return nullptr;
    if (node->getData().getCourseId() == courseId)
        return node;
    StdNode<Result> *left = searchResultByCourseId(node->getLeft(), courseId);
    if (left)
        return left;
    return searchResultByCourseId(node->getRight(), courseId);
}

// ------------------- Admin Searches -------------------
inline StdNode<Admin> *searchAdminByName(StdNode<Admin> *node, const std::string &name)
{
    if (!node)
        return nullptr;
    if (node->getData().getAdminName() == name)
        return node;
    StdNode<Admin> *left = searchAdminByName(node->getLeft(), name);
    if (left)
        return left;
    return searchAdminByName(node->getRight(), name);
}

inline StdNode<Admin> *searchAdminById(StdNode<Admin> *node, int id)
{
    if (!node)
        return nullptr;
    if (node->getData().getAdminId() == id) // Fixed: Changed getId() to getAdminId()
        return node;
    StdNode<Admin> *left = searchAdminById(node->getLeft(), id);
    if (left)
        return left;
    return searchAdminById(node->getRight(), id);
}

// ------------------- Field Searches -------------------
inline StdNode<FieldStudy> *searchFieldStudyById(StdNode<FieldStudy> *node, int fieldId)
{
    if (!node)
        return nullptr;
    if (node->getData().getFieldId() == fieldId)
        return node;
    StdNode<FieldStudy> *left = searchFieldStudyById(node->getLeft(), fieldId);
    if (left)
        return left;
    return searchFieldStudyById(node->getRight(), fieldId);
}

inline StdNode<FieldStudy> *searchFieldById(StdNode<FieldStudy> *node, int id)
{
    return searchFieldStudyById(node, id);
}

inline StdNode<FieldStudy> *searchFieldByName(StdNode<FieldStudy> *node, const std::string &name)
{
    if (!node)
        return nullptr;
    if (node->getData().getFieldName() == name)
        return node;
    StdNode<FieldStudy> *left = searchFieldByName(node->getLeft(), name);
    if (left)
        return left;
    return searchFieldByName(node->getRight(), name);
}

// ------------------- Student Searches -------------------
inline StdNode<Student> *searchStudentByName(StdNode<Student> *node, const std::string &name)
{
    if (!node)
        return nullptr;
    if (node->getData().getStdName() == name)
        return node;
    StdNode<Student> *left = searchStudentByName(node->getLeft(), name);
    if (left)
        return left;
    return searchStudentByName(node->getRight(), name);
}

inline StdNode<Student> *searchStudentById(StdNode<Student> *node, int id)
{
    if (!node)
        return nullptr;
    if (node->getData().getStdId() == id) // Fixed: Changed getId() to getStdId()
        return node;
    StdNode<Student> *left = searchStudentById(node->getLeft(), id);
    if (left)
        return left;
    return searchStudentById(node->getRight(), id);
}

// ------------------- Course Searches -------------------
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

inline StdNode<Course> *searchCourseById(StdNode<Course> *node, int id)
{
    if (!node)
        return nullptr;
    if (node->getData().getId() == id)
        return node;
    StdNode<Course> *left = searchCourseById(node->getLeft(), id);
    if (left)
        return left;
    return searchCourseById(node->getRight(), id);
}

// ------------------- StdCourse Searches -------------------
inline StdNode<StdCourse> *searchStdCourseByStdId(StdNode<StdCourse> *node, int stdId)
{
    if (!node)
        return nullptr;
    if (node->getData().getStdId() == stdId)
        return node;
    StdNode<StdCourse> *left = searchStdCourseByStdId(node->getLeft(), stdId);
    if (left)
        return left;
    return searchStdCourseByStdId(node->getRight(), stdId);
}

inline StdNode<StdCourse> *searchStdCourseByCourseId(StdNode<StdCourse> *node, int courseId)
{
    if (!node)
        return nullptr;
    if (node->getData().getCourseId() == courseId)
        return node;
    StdNode<StdCourse> *left = searchStdCourseByCourseId(node->getLeft(), courseId);
    if (left)
        return left;
    return searchStdCourseByCourseId(node->getRight(), courseId);
}

// ------------------- Attendance Searches -------------------
inline StdNode<Attendance> *searchAttendanceByStdId(StdNode<Attendance> *node, int stdId)
{
    if (!node)
        return nullptr;
    if (node->getData().getStdId() == stdId)
        return node;
    StdNode<Attendance> *left = searchAttendanceByStdId(node->getLeft(), stdId);
    if (left)
        return left;
    return searchAttendanceByStdId(node->getRight(), stdId);
}

inline StdNode<Attendance> *searchAttendanceByCourseId(StdNode<Attendance> *node, int courseId)
{
    if (!node)
        return nullptr;
    if (node->getData().getCourseId() == courseId)
        return node;
    StdNode<Attendance> *left = searchAttendanceByCourseId(node->getLeft(), courseId);
    if (left)
        return left;
    return searchAttendanceByCourseId(node->getRight(), courseId);
}

inline StdNode<Attendance> *searchAttendanceByDate(StdNode<Attendance> *node, const std::string &date)
{
    if (!node)
        return nullptr;
    if (node->getData().getAttendanceDate() == date)
        return node;
    StdNode<Attendance> *left = searchAttendanceByDate(node->getLeft(), date);
    if (left)
        return left;
    return searchAttendanceByDate(node->getRight(), date);
}

// ------------------- StudentFees Searches -------------------
inline StdNode<StudentFees> *searchStdFeeByStdId(StdNode<StudentFees> *node, int stdId)
{
    if (!node)
        return nullptr;
    if (node->getData().getStdId() == stdId)
        return node;
    StdNode<StudentFees> *left = searchStdFeeByStdId(node->getLeft(), stdId);
    if (left)
        return left;
    return searchStdFeeByStdId(node->getRight(), stdId);
}
template<typename T>
inline StdNode<T> *searchStdFeeByStdId(BST<T> &feeBST, int stdId)
{
    if (!feeBST.getRoot())
    {
        return NULL;
    }
    if (feeBST.getRoot().getData().getStdId() == stdId)
    {
        return feeBST.getRoot();
    }
    return searchStdFeeByStdId(feeBST.getRoot(), stdId);
}

#endif // SEARCHENGINE_H