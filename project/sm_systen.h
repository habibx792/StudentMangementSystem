#define _HAS_STD_BYTE 0
#ifndef SMSSYSTEM_H
#define SMSSYSTEM_H

// ================= Standard Library =================
#include <string>
#include <iostream>
#include <memory>

// ================= Core Data Structures =================
#include "stdBst.h"
#include "opQueue.h"
#include "updationQueue.h"
#include "deleteQueues.h" // FIXED: Changed from deleteQueues.h

// ================= Domain Models =================
#include "student.h"
#include "course.h"
#include "field.h"
#include "admin.h"
#include "stdCourse.h"
#include "stdFee.h"
#include "stdResult.h"
#include "attendance.h"
#include "stdNode.h"

// ================= Engines / Services =================
#include "searchEngine.h"
#include "printEngine.h"
#include "UI.h"
#include "db.h"

// ================= System Class =================
class SMSSystem
{
private:
    DB db;                   // Database connection handler
    UI ui;                   // User interface handler
    PrintEngine printEngine; // Printing engine

    BST<Student> studentBST;       // Binary Search Tree for storing students
    BST<Admin> adminBST;           // Binary Search Tree for storing admins
    BST<Student> newRegStdBst;     // BST for newly registered students
    BST<Course> courseBST;         // BST for storing courses
    BST<FieldStudy> fieldBST;      // BST for storing fields of study
    BST<Attendance> attendanceBST; // BST for storing attendance records
    BST<StdCourse> stdCourseBST;   // BST for student-course registrations
    BST<StudentFees> stdFeeBST;    // BST for storing student fees
    BST<Result> stdResultBST;      // BST for storing student results

    opQueue<Student> newStudentOpQueue;          // Queue for new student registrations
    opQueue<Course> newRegCourseOpQueue;         // Queue for new course registrations
    opQueue<FieldStudy> newFieldOpQueue;         // Queue for new field registrations
    opQueue<Attendance> newStdAttendanceOpQueue; // Queue for new attendance records
    opQueue<StdCourse> newStdCourseOpQueue;      // Queue for new course registrations
    opQueue<StudentFees> newStdFeeOpQueue;       // Queue for new fee records
    opQueue<Result> newStdResultOpQueue;         // Queue for new result records

    upQueue updateQueue;  // Queue for tracking updates
    deleteQueue delQueue; // Queue for deletions

    // Private constructor for Singleton pattern
    SMSSystem() = default;

    // Delete copy constructor and assignment operator
    SMSSystem(const SMSSystem &) = delete;
    SMSSystem &operator=(const SMSSystem &) = delete;

    // ================= Database Insertion Methods =================
    void insertAdminToDB(const Admin &admin)
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        // If adminId is auto-increment, don't include it in INSERT
        const char *query = "INSERT INTO adminTab (adminName, passWord) VALUES (?, ?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        // Bind parameters
        std::string adminName = admin.getAdminName();
        std::string password = admin.getPassWord();

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         adminName.length(), 0, (SQLPOINTER)adminName.c_str(), 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         password.length(), 0, (SQLPOINTER)password.c_str(), 0, nullptr);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Admin inserted successfully." << std::endl;
        else
            std::cout << "Failed to insert admin." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    void insertStudentToDB(const Student &student)
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "INSERT INTO student (stdId, stdName, stdUserName, stdAge, fieldId, stdFatherName) VALUES (?, ?, ?, ?, ?, ?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        // Get values
        SQLINTEGER stdId = student.getStdId();
        SQLINTEGER stdAge = student.getStdAge();
        SQLINTEGER fieldId = student.getFieldId();
        std::string stdName = student.getStdName();
        std::string stdUserName = student.getStdUserName();
        std::string stdFatherName = student.getStdFatherName();

        // Bind parameters
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         stdName.length(), 0, (SQLPOINTER)stdName.c_str(), 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         stdUserName.length(), 0, (SQLPOINTER)stdUserName.c_str(), 0, nullptr);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdAge, 0, nullptr);
        SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &fieldId, 0, nullptr);
        SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         stdFatherName.length(), 0, (SQLPOINTER)stdFatherName.c_str(), 0, nullptr);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Student inserted successfully." << std::endl;
        else
            std::cout << "Failed to insert student." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    void insertCourseToDB(const Course &course)
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "INSERT INTO course (courseId, courseTitle, teacherName) VALUES (?, ?, ?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        SQLINTEGER courseId = course.getId();
        std::string courseTitle = course.getCourseTitle();
        std::string teacherName = course.getTeacherName();

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &courseId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         courseTitle.length(), 0, (SQLPOINTER)courseTitle.c_str(), 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         teacherName.length(), 0, (SQLPOINTER)teacherName.c_str(), 0, nullptr);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Course inserted successfully." << std::endl;
        else
            std::cout << "Failed to insert course." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    void insertFieldToDB(const FieldStudy &field)
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "INSERT INTO fieldStudy (fieldId, fieldName) VALUES (?, ?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        SQLINTEGER fieldId = field.getFieldId();
        std::string fieldName = field.getFieldName();

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &fieldId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         fieldName.length(), 0, (SQLPOINTER)fieldName.c_str(), 0, nullptr);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Field inserted successfully." << std::endl;
        else
            std::cout << "Failed to insert field." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    void insertAttendanceToDB(const Attendance &attendance)
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "INSERT INTO Attendance (attendanceId, stdId, courseId, attendanceDate, isPresent) VALUES (?, ?, ?, ?, ?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        SQLINTEGER attendanceId = attendance.getAttendanceId();
        SQLINTEGER stdId = attendance.getStdId();
        SQLINTEGER courseId = attendance.getCourseId();
        std::string attendanceDate = attendance.getAttendanceDate();
        SQLSMALLINT isPresent = attendance.getIsPresent() ? 1 : 0;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &attendanceId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &courseId, 0, nullptr);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         attendanceDate.length(), 0, (SQLPOINTER)attendanceDate.c_str(), 0, nullptr);
        SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT,
                         0, 0, &isPresent, 0, nullptr);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Attendance record inserted successfully." << std::endl;
        else
            std::cout << "Failed to insert attendance record." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    void insertStdCourseToDB(const StdCourse &stdCourse)
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "INSERT INTO courseRegStd (stdId, courseId, regDate) VALUES (?, ?, ?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        SQLINTEGER stdId = stdCourse.getStdId();
        SQLINTEGER courseId = stdCourse.getCourseId();
        std::string regDate = stdCourse.getRegDate();

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &courseId, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         regDate.length(), 0, (SQLPOINTER)regDate.c_str(), 0, nullptr);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Student course registration inserted successfully." << std::endl;
        else
            std::cout << "Failed to insert student course registration." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    void insertStudentFeesToDB(const StudentFees &fee)
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "INSERT INTO StudentFees (feeId, stdId, amount, paymentDate, status) VALUES (?, ?, ?, ?, ?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        SQLINTEGER feeId = fee.getFeeId();
        SQLINTEGER stdId = fee.getStdId();
        SQLDOUBLE amount = fee.getAmount();
        std::string paymentDate = fee.getPaymentDate();
        std::string status = fee.getStatus();

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &feeId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
                         0, 0, &amount, 0, nullptr);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         paymentDate.length(), 0, (SQLPOINTER)paymentDate.c_str(), 0, nullptr);
        SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         status.length(), 0, (SQLPOINTER)status.c_str(), 0, nullptr);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Student fee record inserted successfully." << std::endl;
        else
            std::cout << "Failed to insert student fee record." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    void insertResultToDB(const Result &result)
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "INSERT INTO result (stdId, courseId, gotNumber, grade) VALUES (?, ?, ?, ?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        SQLINTEGER stdId = result.getStdId();
        SQLINTEGER courseId = result.getCourseId();
        SQLDOUBLE gotNumber = result.getGotNumber();
        std::string grade = result.getGrade();

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &courseId, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
                         0, 0, &gotNumber, 0, nullptr);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         grade.length(), 0, (SQLPOINTER)grade.c_str(), 0, nullptr);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Result inserted successfully." << std::endl;
        else
            std::cout << "Failed to insert result." << std::endl;

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    // ================= Queue Operations =================
    void updateStudentInfo(upNode *node)
    {
        updateQueue.enqueue(node);
    }

    void deleteStudentInformation(deleteNode *node)
    {
        delQueue.enqueue(node);
    }

    // ================= Registration Functions =================
    void registerNewStudent(Student &student)
    {
        newStudentOpQueue.enqueueNewStd(student);
        studentBST.insert(student);
        insertStudentToDB(student); // Save to database
    }

    void registerNewCourse(Course &course)
    {
        newRegCourseOpQueue.enqueueNewStd(course);
        courseBST.insert(course);
        insertCourseToDB(course); // Save to database
    }

    void registerNewField(FieldStudy &field)
    {
        newFieldOpQueue.enqueueNewStd(field);
        fieldBST.insert(field);
        insertFieldToDB(field); // Save to database
    }

    void registerNewAttendance(Attendance &attendance)
    {
        newStdAttendanceOpQueue.enqueueNewStd(attendance);
        attendanceBST.insert(attendance);
        insertAttendanceToDB(attendance); // Save to database
    }

    void registerNewStdCourse(StdCourse &stdCourse)
    {
        newStdCourseOpQueue.enqueueNewStd(stdCourse);
        stdCourseBST.insert(stdCourse);
        insertStdCourseToDB(stdCourse); // Save to database
    }

    void registerNewStudentFees(StudentFees &stdFee)
    {
        newStdFeeOpQueue.enqueueNewStd(stdFee);
        stdFeeBST.insert(stdFee);
        insertStudentFeesToDB(stdFee); // Save to database
    }

    void registerNewResult(Result &stdResult)
    {
        newStdResultOpQueue.enqueueNewStd(stdResult);
        stdResultBST.insert(stdResult);
        insertResultToDB(stdResult); // Save to database
    }

    void registerNewAdmin(Admin &admin)
    {
        adminBST.insert(admin);
        insertAdminToDB(admin); // Save to database
    }

    // ================= Database Loading Functions =================
    void loadAdminsFromDB()
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "SELECT adminId, adminName, passWord FROM adminTab";

        if (SQLExecDirect(hStmt, (SQLCHAR *)query, SQL_NTS) == SQL_SUCCESS)
        {
            SQLINTEGER adminId;
            SQLCHAR name[41];
            SQLCHAR password[256];
            SQLLEN idInd = 0, nameInd = 0, passInd = 0;

            while (SQLFetch(hStmt) == SQL_SUCCESS)
            {
                SQLGetData(hStmt, 1, SQL_C_SLONG, &adminId, 0, &idInd);
                SQLGetData(hStmt, 2, SQL_C_CHAR, name, sizeof(name), &nameInd);
                SQLGetData(hStmt, 3, SQL_C_CHAR, password, sizeof(password), &passInd);

                Admin admin;
                admin.setAdminId(adminId);
                admin.setAdminName(std::string(reinterpret_cast<char *>(name)));
                admin.setPassWord(std::string(reinterpret_cast<char *>(password)));

                adminBST.insert(admin);
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout << "Loaded admins from database into BST" << std::endl;
    }

    void loadFieldsFromDB()
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "SELECT fieldId, fieldName FROM fieldStudy";

        if (SQLExecDirect(hStmt, (SQLCHAR *)query, SQL_NTS) == SQL_SUCCESS)
        {
            SQLINTEGER fieldId;
            SQLCHAR name[41];
            SQLLEN idInd = 0, nameInd = 0;

            while (SQLFetch(hStmt) == SQL_SUCCESS)
            {
                SQLGetData(hStmt, 1, SQL_C_SLONG, &fieldId, 0, &idInd);
                SQLGetData(hStmt, 2, SQL_C_CHAR, name, sizeof(name), &nameInd);

                FieldStudy field;
                field.setFieldId(fieldId);
                field.setFieldName(std::string(reinterpret_cast<char *>(name)));

                fieldBST.insert(field);
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout << "Loaded fields from database into BST" << std::endl;
    }

    void loadStudentsFromDB()
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "SELECT stdId, stdName, stdUserName, stdAge, fieldId, stdFatherName FROM student";

        if (SQLExecDirect(hStmt, (SQLCHAR *)query, SQL_NTS) == SQL_SUCCESS)
        {
            SQLINTEGER stdId, stdAge, fieldId;
            SQLCHAR stdName[41], stdUserName[41], stdFatherName[41];
            SQLLEN idInd = 0, nameInd = 0, userInd = 0, ageInd = 0, fieldInd = 0, fatherInd = 0;

            while (SQLFetch(hStmt) == SQL_SUCCESS)
            {
                SQLGetData(hStmt, 1, SQL_C_SLONG, &stdId, 0, &idInd);
                SQLGetData(hStmt, 2, SQL_C_CHAR, stdName, sizeof(stdName), &nameInd);
                SQLGetData(hStmt, 3, SQL_C_CHAR, stdUserName, sizeof(stdUserName), &userInd);
                SQLGetData(hStmt, 4, SQL_C_SLONG, &stdAge, 0, &ageInd);
                SQLGetData(hStmt, 5, SQL_C_SLONG, &fieldId, 0, &fieldInd);
                SQLGetData(hStmt, 6, SQL_C_CHAR, stdFatherName, sizeof(stdFatherName), &fatherInd);

                Student student;
                student.setStdId(stdId);
                student.setStdName(std::string(reinterpret_cast<char *>(stdName)));
                student.setStdUserName(std::string(reinterpret_cast<char *>(stdUserName)));
                student.setStdAge(stdAge);
                student.setFieldId(fieldId);
                student.setStdFatherName(std::string(reinterpret_cast<char *>(stdFatherName)));

                studentBST.insert(student);
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout << "Loaded students from database into BST" << std::endl;
    }

    void loadCoursesFromDB()
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "SELECT courseId, courseTitle, teacherName FROM course";

        if (SQLExecDirect(hStmt, (SQLCHAR *)query, SQL_NTS) == SQL_SUCCESS)
        {
            SQLINTEGER courseId;
            SQLCHAR courseTitle[41], teacherName[41];
            SQLLEN idInd = 0, titleInd = 0, teacherInd = 0;

            while (SQLFetch(hStmt) == SQL_SUCCESS)
            {
                SQLGetData(hStmt, 1, SQL_C_SLONG, &courseId, 0, &idInd);
                SQLGetData(hStmt, 2, SQL_C_CHAR, courseTitle, sizeof(courseTitle), &titleInd);
                SQLGetData(hStmt, 3, SQL_C_CHAR, teacherName, sizeof(teacherName), &teacherInd);

                Course course;
                course.setId(courseId);
                course.setCourseTitle(std::string(reinterpret_cast<char *>(courseTitle)));
                course.setTeacherName(std::string(reinterpret_cast<char *>(teacherName)));

                courseBST.insert(course);
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout << "Loaded courses from database into BST" << std::endl;
    }

    void loadCourseRegistrationsFromDB()
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "SELECT stdId, courseId, regDate FROM courseRegStd";

        if (SQLExecDirect(hStmt, (SQLCHAR *)query, SQL_NTS) == SQL_SUCCESS)
        {
            SQLINTEGER stdId, courseId;
            SQLCHAR regDate[20];
            SQLLEN stdInd = 0, courseInd = 0, dateInd = 0;

            while (SQLFetch(hStmt) == SQL_SUCCESS)
            {
                SQLGetData(hStmt, 1, SQL_C_SLONG, &stdId, 0, &stdInd);
                SQLGetData(hStmt, 2, SQL_C_SLONG, &courseId, 0, &courseInd);
                SQLGetData(hStmt, 3, SQL_C_CHAR, regDate, sizeof(regDate), &dateInd);

                StdCourse reg;
                reg.setStdId(stdId);
                reg.setCourseId(courseId);
                reg.setRegDate(std::string(reinterpret_cast<char *>(regDate)));

                stdCourseBST.insert(reg);
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout << "Loaded course registrations from database into BST" << std::endl;
    }

    void loadAttendanceFromDB()
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "SELECT attendanceId, stdId, courseId, attendanceDate, isPresent FROM Attendance";

        if (SQLExecDirect(hStmt, (SQLCHAR *)query, SQL_NTS) == SQL_SUCCESS)
        {
            SQLINTEGER attendanceId, stdId, courseId;
            SQLCHAR attendanceDate[11];
            SQLSMALLINT isPresent;
            SQLLEN idInd = 0, stdInd = 0, courseInd = 0, dateInd = 0, presentInd = 0;

            while (SQLFetch(hStmt) == SQL_SUCCESS)
            {
                SQLGetData(hStmt, 1, SQL_C_SLONG, &attendanceId, 0, &idInd);
                SQLGetData(hStmt, 2, SQL_C_SLONG, &stdId, 0, &stdInd);
                SQLGetData(hStmt, 3, SQL_C_SLONG, &courseId, 0, &courseInd);
                SQLGetData(hStmt, 4, SQL_C_CHAR, attendanceDate, sizeof(attendanceDate), &dateInd);
                SQLGetData(hStmt, 5, SQL_C_SSHORT, &isPresent, 0, &presentInd);

                Attendance attendance;
                attendance.setAttendanceId(attendanceId);
                attendance.setStdId(stdId);
                attendance.setCourseId(courseId);
                attendance.setAttendanceDate(std::string(reinterpret_cast<char *>(attendanceDate)));
                attendance.setIsPresent(isPresent == 1);

                attendanceBST.insert(attendance);
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout << "Loaded attendance from database into BST" << std::endl;
    }

    void loadStudentFeesFromDB()
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "SELECT feeId, stdId, amount, paymentDate, status FROM StudentFees";

        if (SQLExecDirect(hStmt, (SQLCHAR *)query, SQL_NTS) == SQL_SUCCESS)
        {
            SQLINTEGER feeId, stdId;
            SQLDOUBLE amount;
            SQLCHAR paymentDate[11], status[21];
            SQLLEN idInd = 0, stdInd = 0, amountInd = 0, dateInd = 0, statusInd = 0;

            while (SQLFetch(hStmt) == SQL_SUCCESS)
            {
                SQLGetData(hStmt, 1, SQL_C_SLONG, &feeId, 0, &idInd);
                SQLGetData(hStmt, 2, SQL_C_SLONG, &stdId, 0, &stdInd);
                SQLGetData(hStmt, 3, SQL_C_DOUBLE, &amount, 0, &amountInd);
                SQLGetData(hStmt, 4, SQL_C_CHAR, paymentDate, sizeof(paymentDate), &dateInd);
                SQLGetData(hStmt, 5, SQL_C_CHAR, status, sizeof(status), &statusInd);

                StudentFees fee;
                fee.setFeeId(feeId);
                fee.setStdId(stdId);
                fee.setAmount(amount);
                fee.setPaymentDate(std::string(reinterpret_cast<char *>(paymentDate)));
                fee.setStatus(std::string(reinterpret_cast<char *>(status)));

                stdFeeBST.insert(fee);
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout << "Loaded student fees from database into BST" << std::endl;
    }

    void loadResultsFromDB()
    {
        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return;
        }

        const char *query = "SELECT stdId, courseId, gotNumber, grade FROM result";

        if (SQLExecDirect(hStmt, (SQLCHAR *)query, SQL_NTS) == SQL_SUCCESS)
        {
            SQLINTEGER stdId, courseId;
            SQLDOUBLE gotNumber;
            SQLCHAR grade[3];
            SQLLEN stdInd = 0, courseInd = 0, numberInd = 0, gradeInd = 0;

            while (SQLFetch(hStmt) == SQL_SUCCESS)
            {
                SQLGetData(hStmt, 1, SQL_C_SLONG, &stdId, 0, &stdInd);
                SQLGetData(hStmt, 2, SQL_C_SLONG, &courseId, 0, &courseInd);
                SQLGetData(hStmt, 3, SQL_C_DOUBLE, &gotNumber, 0, &numberInd);
                SQLGetData(hStmt, 4, SQL_C_CHAR, grade, sizeof(grade), &gradeInd);

                Result result;
                result.setStdId(stdId);
                result.setCourseId(courseId);
                result.setGotNumber(gotNumber);
                result.setGrade(std::string(reinterpret_cast<char *>(grade)));

                stdResultBST.insert(result);
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout << "Loaded results from database into BST" << std::endl;
    }

    void loadAllDataFromDB()
    {
        std::cout << "\n=== Loading data from database ===" << std::endl;

        if (!db.connected())
        {
            std::cout << "ERROR: Database not connected! Cannot load data." << std::endl;
            return;
        }

        loadFieldsFromDB();
        loadStudentsFromDB();
        loadCoursesFromDB();
        loadCourseRegistrationsFromDB();
        loadStudentFeesFromDB();
        loadAttendanceFromDB();
        loadResultsFromDB();
        loadAdminsFromDB();

        std::cout << "=== All data loaded successfully ===" << std::endl;
    }

public:
    // Static method to get the single instance
    static SMSSystem &getInstance()
    {
        static SMSSystem instance;
        return instance;
    }

    // Main method to run the system
    void run()
    {
        std::cout << "\n=== Student Management System ===" << std::endl;

        if (!db.connect())
        {
            std::cout << "Failed to connect to database. Exiting..." << std::endl;
            return;
        }

        db.printStatus();
        loadAllDataFromDB();

        // Test print engine
        printEngine.printAllStudents(studentBST);
        printEngine.printAllCourses(courseBST);
        int id;
        cout<<"Enter Your student id";
        cin>>id;
        StdNode<Student> *node = studentBST.search(id);
            if (node)
        {
            std::cout << "Student Found: ";
            node->getData().print();
        }
      else
      {
        std::cout<<"Student not found with ID: "<<id<<std::endl;
      }

        ui.displayAdminMenu();
    }

    // Public getter methods
    BST<Student> &getStudentBST() { return studentBST; }
    BST<Admin> &getAdminBST() { return adminBST; }
    BST<Course> &getCourseBST() { return courseBST; }
    BST<FieldStudy> &getFieldBST() { return fieldBST; }
    BST<Attendance> &getAttendanceBST() { return attendanceBST; }
    BST<StdCourse> &getStdCourseBST() { return stdCourseBST; }
    BST<StudentFees> &getStdFeeBST() { return stdFeeBST; }
    BST<Result> &getStdResultBST() { return stdResultBST; }

    DB &getDB() { return db; }
    UI &getUI() { return ui; }
    PrintEngine &getPrintEngine() { return printEngine; }

    // Public registration methods
    void addNewStudent(Student &student) { registerNewStudent(student); }
    void addNewCourse(Course &course) { registerNewCourse(course); }
    void addNewField(FieldStudy &field) { registerNewField(field); }
    void addNewAttendance(Attendance &attendance) { registerNewAttendance(attendance); }
    void addNewStdCourse(StdCourse &stdCourse) { registerNewStdCourse(stdCourse); }
    void addNewStudentFees(StudentFees &stdFee) { registerNewStudentFees(stdFee); }
    void addNewResult(Result &stdResult) { registerNewResult(stdResult); }
    void addNewAdmin(Admin &admin) { registerNewAdmin(admin); }
};

#endif // SMSSYSTEM_H