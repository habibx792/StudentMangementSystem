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
     BST<Course> courseBST;
     BST<Field> fieldBST;
    BST<Attendance> attendanceBST; 
    Bst<att
    SmsSytem() {}

    SmsSytem(const SmsSytem &) = delete;
    SmsSytem &operator=(const SmsSytem &) = delete;
//main working of system 

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
