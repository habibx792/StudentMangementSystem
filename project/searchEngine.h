#ifndef BSTSEARCH_H
#define BSTSEARCH_H
#include "stdNode.h"
#include "student.h"
#include "course.h"
#include "stdResult.h"
#include "attendance.h"
#include "stdCourse.h"
#include "admin.h"
#include "field.h"
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

    if (node->getData().getId() == id)
        return node;

    StdNode<Admin> *left = searchAdminById(node->getLeft(), id);
    if (left)
        return left;

    return searchAdminById(node->getRight(), id);
}

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

// Search by Course ID
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

// Search by Date
inline StdNode<Attendance> *searchAttendanceByDate(StdNode<Attendance> *node, const string &date)
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

// Search by Student ID
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

// Search by Course ID
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

// ------------------- Student Searches -------------------
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

#endif // BSTSEARCH_H
