#ifndef SMSSYSTEM_H
#define SMSSYSTEM_H

// ================= Standard Library =================
#include <string>
#include <iostream>

// ================= Core Data Structures =================
#include "stdBst.h"
#include "opQueue.h"
#include "updationQueue.h"
#include "deleteQueues.h"

// ================= Domain Models =================
#include "student.h"
#include "course.h"
#include "field.h"
#include "admin.h"
#include "stdCourse.h"
#include "stdFee.h"
#include "stdResult.h"
#include "attendance.h"

// ================= Engines / Services =================
#include "searchEngine.h"
#include "printEngine.h"
#include "UI.h"
#include "db.h"

// ================= System Class =================
class SmsSytem
{
private:
    DB db;
    UI ui;
    BST<Student> studentBST;
    BST<Student> newRegStdBst;
    BST<Course> courseBST;
    BST<FieldStudy> fieldBST;
    BST<Attendance> attendanceBST;
    BST<StdCourse> StdCourseBST;
    BST<StudentFees> stdFeeBST;
    BST<Result> stdResultBST;
    opQueue<Student> newstudentOpQueue;
    opQueue<Course> newRegcourseOpQueue;
    opQueue<FieldStudy> newfieldOpQueue;
    opQueue<Attendance> newStdattendanceOpQueue;
    opQueue<StdCourse> newStdCourseOpQueue;
    opQueue<StudentFees> stdFeeOpQueue;
    opQueue<Result> stdResultOpQueue;
    // contaain update metData
    upQueue upDateQueue;
    // contain detion table data
    deleteQueue deleteQueue;
    SmsSytem()
    {
    }
    void registeNewStudent(Student &student)
    {

        newstudentOpQueue.enqueueNewStd(student);
        studentBST.insert(student);
    }
    void registeNewCourse(Course &course)
    {
        newRegcourseOpQueue.enqueueNewStd(course);
        courseBST.insert(course);
    }
    void registeNewField(FieldStudy &field)
    {

        newfieldOpQueue.enqueueNewStd(field);
        fieldBST.insert(field);
    }
    void registeNewAttendance(Attendance &attendance)
    {
        newStdattendanceOpQueue.enqueueNewStd(attendance);
        attendanceBST.insert(attendance);
    }
    void registeNewStdCourse(StdCourse &stdCourse)
    {
        newStdCourseOpQueue.enqueueNewStd(stdCourse);
        StdCourseBST.insert(stdCourse);
    }
    void registeNewStudentFees(StudentFees &stdFee)
    {
        stdFeeBST.insert(stdFee);
    }
    void registeNewResult(Result &stdResult)
    {
        stdResultBST.insert(stdResult);
    }

    SmsSytem(const SmsSytem &) = delete;
    SmsSytem &operator=(const SmsSytem &) = delete;
    // main working of system

public:
    static SmsSytem &getInstance()
    {
        static SmsSytem instance;
        return instance;
    }

    void run()
    {
        db.connect();
        ui.displayAdminMenu();
    }
};

#endif // SMSSYSTEM_H
