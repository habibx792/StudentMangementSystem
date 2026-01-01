#ifndef SMSSYSTEM_H
#define SMSSYSTEM_H

// ================= Standard Library =================
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <locale>
#include <algorithm>

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

    // ================= Helper Functions =================
    // Simple string sanitizer for SQL Server compatibility
    std::string sanitizeForSQL(const std::string &input)
    {
        std::string result;
        for (char c : input)
        {
            // Allow ASCII 32-126 (printable characters)
            if (c >= 32 && c <= 126)
                result += c;
            else if (c == '\t' || c == '\n' || c == '\r')
                result += ' '; // Replace whitespace with space
            // Skip other characters (non-ASCII)
        }
        return result;
    }

    // Truncate string to fit database column size
    std::string truncateToSize(const std::string &input, size_t maxSize)
    {
        if (input.length() <= maxSize)
            return input;
        return input.substr(0, maxSize);
    }

    void printSQLError(SQLHSTMT hStmt)
    {
        SQLCHAR sqlState[6];
        SQLINTEGER nativeError;
        SQLCHAR errMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLSMALLINT errMsgLen;

        while (SQLError(SQL_NULL_HENV, SQL_NULL_HDBC, hStmt, sqlState, &nativeError,
                        errMsg, SQL_MAX_MESSAGE_LENGTH, &errMsgLen) == SQL_SUCCESS)
        {
            std::cout << "SQL State: " << sqlState << ", Error: " << errMsg << std::endl;
        }
    }

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

        const char *query = "INSERT INTO adminTab (adminName, passWord) VALUES (?, ?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        // Prepare data according to database schema
        std::string adminName = truncateToSize(sanitizeForSQL(admin.getAdminName()), 40);
        std::string password = truncateToSize(sanitizeForSQL(admin.getPassWord()), 255);

        SQLLEN nameLen = adminName.empty() ? SQL_NULL_DATA : SQL_NTS;
        SQLLEN passLen = password.empty() ? SQL_NULL_DATA : SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         adminName.length(), 0, (SQLPOINTER)adminName.c_str(), 0, &nameLen);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         password.length(), 0, (SQLPOINTER)password.c_str(), 0, &passLen);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Admin inserted successfully." << std::endl;
        else
        {
            std::cout << "Failed to insert admin. Error: ";
            printSQLError(hStmt);
        }

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

        // Prepare data according to database schema
        SQLINTEGER stdId = student.getStdId();
        SQLINTEGER stdAge = student.getStdAge();
        SQLINTEGER fieldId = student.getFieldId();
        std::string stdName = truncateToSize(sanitizeForSQL(student.getStdName()), 40);
        std::string stdUserName = truncateToSize(sanitizeForSQL(student.getStdUserName()), 40);
        std::string stdFatherName = truncateToSize(sanitizeForSQL(student.getStdFatherName()), 40);

        SQLLEN nameLen = stdName.empty() ? SQL_NULL_DATA : SQL_NTS;
        SQLLEN userLen = stdUserName.empty() ? SQL_NULL_DATA : SQL_NTS;
        SQLLEN fatherLen = stdFatherName.empty() ? SQL_NULL_DATA : SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         40, 0, (SQLPOINTER)stdName.c_str(), 0, &nameLen);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         40, 0, (SQLPOINTER)stdUserName.c_str(), 0, &userLen);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdAge, 0, nullptr);
        SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &fieldId, 0, nullptr);
        SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         40, 0, (SQLPOINTER)stdFatherName.c_str(), 0, &fatherLen);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Student inserted successfully." << std::endl;
        else
        {
            std::cout << "Failed to insert student. Error: ";
            printSQLError(hStmt);
        }

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

        // Prepare data according to database schema
        SQLINTEGER courseId = course.getId();
        std::string courseTitle = truncateToSize(sanitizeForSQL(course.getCourseTitle()), 40);
        std::string teacherName = truncateToSize(sanitizeForSQL(course.getTeacherName()), 40);

        SQLLEN titleLen = courseTitle.empty() ? SQL_NULL_DATA : SQL_NTS;
        SQLLEN teacherLen = teacherName.empty() ? SQL_NULL_DATA : SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &courseId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         40, 0, (SQLPOINTER)courseTitle.c_str(), 0, &titleLen);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         40, 0, (SQLPOINTER)teacherName.c_str(), 0, &teacherLen);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Course inserted successfully." << std::endl;
        else
        {
            std::cout << "Failed to insert course. Error: ";
            printSQLError(hStmt);
        }

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
        std::string fieldName = truncateToSize(sanitizeForSQL(field.getFieldName()), 40);

        SQLLEN nameLen = fieldName.empty() ? SQL_NULL_DATA : SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &fieldId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         40, 0, (SQLPOINTER)fieldName.c_str(), 0, &nameLen);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Field inserted successfully." << std::endl;
        else
        {
            std::cout << "Failed to insert field. Error: ";
            printSQLError(hStmt);
        }

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
        std::string attendanceDate = truncateToSize(sanitizeForSQL(attendance.getAttendanceDate()), 10);
        SQLSMALLINT isPresent = attendance.getIsPresent() ? 1 : 0;

        SQLLEN dateLen = attendanceDate.empty() ? SQL_NULL_DATA : SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &attendanceId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &courseId, 0, nullptr);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         10, 0, (SQLPOINTER)attendanceDate.c_str(), 0, &dateLen);
        SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT,
                         0, 0, &isPresent, 0, nullptr);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Attendance record inserted successfully." << std::endl;
        else
        {
            std::cout << "Failed to insert attendance record. Error: ";
            printSQLError(hStmt);
        }

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
        std::string regDate = truncateToSize(sanitizeForSQL(stdCourse.getRegDate()), 19);

        SQLLEN dateLen = regDate.empty() ? SQL_NULL_DATA : SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &courseId, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         19, 0, (SQLPOINTER)regDate.c_str(), 0, &dateLen);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Student course registration inserted successfully." << std::endl;
        else
        {
            std::cout << "Failed to insert student course registration. Error: ";
            printSQLError(hStmt);
        }

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
        std::string paymentDate = truncateToSize(sanitizeForSQL(fee.getPaymentDate()), 10);
        std::string status = truncateToSize(sanitizeForSQL(fee.getStatus()), 20);

        SQLLEN dateLen = paymentDate.empty() ? SQL_NULL_DATA : SQL_NTS;
        SQLLEN statusLen = status.empty() ? SQL_NULL_DATA : SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &feeId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
                         0, 0, &amount, 0, nullptr);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         10, 0, (SQLPOINTER)paymentDate.c_str(), 0, &dateLen);
        SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         20, 0, (SQLPOINTER)status.c_str(), 0, &statusLen);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Student fee record inserted successfully." << std::endl;
        else
        {
            std::cout << "Failed to insert student fee record. Error: ";
            printSQLError(hStmt);
        }

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
        std::string grade = truncateToSize(sanitizeForSQL(result.getGrade()), 2);

        SQLLEN gradeLen = grade.empty() ? SQL_NULL_DATA : SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &courseId, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
                         0, 0, &gotNumber, 0, nullptr);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         2, 0, (SQLPOINTER)grade.c_str(), 0, &gradeLen);

        if (SQLExecute(hStmt) == SQL_SUCCESS)
            std::cout << "Result inserted successfully." << std::endl;
        else
        {
            std::cout << "Failed to insert result. Error: ";
            printSQLError(hStmt);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    // ================= Registration Methods =================
    void registerNewStudent(const Student &student)
    {
        studentBST.insert(student);
        newStudentOpQueue.enqueueNewStd(student);
        insertStudentToDB(student);
    }

    void registerNewCourse(const Course &course)
    {
        courseBST.insert(course);
        newRegCourseOpQueue.enqueueNewStd(course);
        insertCourseToDB(course);
    }

    void registerNewField(const FieldStudy &field)
    {
        fieldBST.insert(field);
        newFieldOpQueue.enqueueNewStd(field);
        insertFieldToDB(field);
    }

    void registerNewAttendance(const Attendance &attendance)
    {
        attendanceBST.insert(attendance);
        newStdAttendanceOpQueue.enqueueNewStd(attendance);
        insertAttendanceToDB(attendance);
    }

    void registerNewStdCourse(const StdCourse &stdCourse)
    {
        stdCourseBST.insert(stdCourse);
        newStdCourseOpQueue.enqueueNewStd(stdCourse);
        insertStdCourseToDB(stdCourse);
    }

    void registerNewStudentFees(const StudentFees &fee)
    {
        stdFeeBST.insert(fee);
        newStdFeeOpQueue.enqueueNewStd(fee);
        insertStudentFeesToDB(fee);
    }

    void registerNewResult(const Result &result)
    {
        stdResultBST.insert(result);
        newStdResultOpQueue.enqueueNewStd(result);
        insertResultToDB(result);
    }

    void registerNewAdmin(const Admin &admin)
    {
        adminBST.insert(admin);
        insertAdminToDB(admin);
    }

    // ================= Data Loading Method =================
    void loadAllDataFromDB()
    {
        std::cout << "\n=== Loading data from database ===" << std::endl;

        // Clear existing BSTs by creating new ones
        BST<Student> newStudentBST;
        BST<Course> newCourseBST;
        BST<FieldStudy> newFieldBST;
        BST<Attendance> newAttendanceBST;
        BST<StdCourse> newStdCourseBST;
        BST<StudentFees> newStdFeeBST;
        BST<Result> newStdResultBST;
        BST<Admin> newAdminBST;

        SQLHSTMT hstmt;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hstmt);

        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            // Load fieldStudy
            ret = SQLExecDirect(hstmt, (SQLCHAR *)"SELECT * FROM fieldStudy ORDER BY fieldId", SQL_NTS);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
            {
                SQLINTEGER fieldId;
                SQLCHAR fieldName[100];
                SQLLEN indicator;
                while (SQLFetch(hstmt) == SQL_SUCCESS)
                {
                    SQLGetData(hstmt, 1, SQL_C_LONG, &fieldId, 0, NULL);
                    SQLGetData(hstmt, 2, SQL_C_CHAR, fieldName, sizeof(fieldName), &indicator);
                    if (indicator != SQL_NULL_DATA)
                    {
                        std::string cleanFieldName = sanitizeForSQL((char *)fieldName);
                        FieldStudy field(fieldId, (char *)cleanFieldName.c_str());
                        newFieldBST.insert(field);
                    }
                }
                std::cout << "Loaded fields from database into BST" << std::endl;
            }
            else
            {
                std::cout << "Error loading fields: ";
                printSQLError(hstmt);
            }
            SQLFreeStmt(hstmt, SQL_CLOSE);
        }

        // Load students
        ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hstmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            ret = SQLExecDirect(hstmt, (SQLCHAR *)"SELECT * FROM student ORDER BY stdId", SQL_NTS);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
            {
                SQLINTEGER stdId, stdAge, fieldId;
                SQLCHAR stdName[100], stdUserName[100], stdFatherName[100];
                SQLLEN nameInd, userInd, fatherInd;
                while (SQLFetch(hstmt) == SQL_SUCCESS)
                {
                    SQLGetData(hstmt, 1, SQL_C_LONG, &stdId, 0, NULL);
                    SQLGetData(hstmt, 2, SQL_C_CHAR, stdName, sizeof(stdName), &nameInd);
                    SQLGetData(hstmt, 3, SQL_C_CHAR, stdUserName, sizeof(stdUserName), &userInd);
                    SQLGetData(hstmt, 4, SQL_C_LONG, &stdAge, 0, NULL);
                    SQLGetData(hstmt, 5, SQL_C_LONG, &fieldId, 0, NULL);
                    SQLGetData(hstmt, 6, SQL_C_CHAR, stdFatherName, sizeof(stdFatherName), &fatherInd);

                    Student student;
                    student.setStdId(stdId);
                    if (nameInd != SQL_NULL_DATA)
                        student.setStdName(sanitizeForSQL((char *)stdName).c_str());
                    if (userInd != SQL_NULL_DATA)
                        student.setStdUserName(sanitizeForSQL((char *)stdUserName).c_str());
                    student.setStdAge(stdAge);
                    student.setFieldId(fieldId);
                    if (fatherInd != SQL_NULL_DATA)
                        student.setStdFatherName(sanitizeForSQL((char *)stdFatherName).c_str());
                    newStudentBST.insert(student);
                }
                std::cout << "Loaded students from database into BST" << std::endl;
            }
            else
            {
                std::cout << "Error loading students: ";
                printSQLError(hstmt);
            }
            SQLFreeStmt(hstmt, SQL_CLOSE);
        }

        // Load courses
        ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hstmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            ret = SQLExecDirect(hstmt, (SQLCHAR *)"SELECT * FROM course ORDER BY courseId", SQL_NTS);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
            {
                SQLINTEGER courseId;
                SQLCHAR courseTitle[100], teacherName[100];
                SQLLEN titleInd, teacherInd;
                while (SQLFetch(hstmt) == SQL_SUCCESS)
                {
                    SQLGetData(hstmt, 1, SQL_C_LONG, &courseId, 0, NULL);
                    SQLGetData(hstmt, 2, SQL_C_CHAR, courseTitle, sizeof(courseTitle), &titleInd);
                    SQLGetData(hstmt, 3, SQL_C_CHAR, teacherName, sizeof(teacherName), &teacherInd);

                    Course course;
                    course.setId(courseId);
                    if (titleInd != SQL_NULL_DATA)
                        course.setCourseTitle(sanitizeForSQL((char *)courseTitle).c_str());
                    if (teacherInd != SQL_NULL_DATA)
                        course.setTeacherName(sanitizeForSQL((char *)teacherName).c_str());
                    newCourseBST.insert(course);
                }
                std::cout << "Loaded courses from database into BST" << std::endl;
            }
            else
            {
                std::cout << "Error loading courses: ";
                printSQLError(hstmt);
            }
            SQLFreeStmt(hstmt, SQL_CLOSE);
        }

        // Load course registrations
        ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hstmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            ret = SQLExecDirect(hstmt, (SQLCHAR *)"SELECT * FROM courseRegStd ORDER BY stdId, courseId", SQL_NTS);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
            {
                SQLINTEGER stdId, courseId;
                SQLCHAR regDate[100];
                SQLLEN dateInd;
                while (SQLFetch(hstmt) == SQL_SUCCESS)
                {
                    SQLGetData(hstmt, 1, SQL_C_LONG, &stdId, 0, NULL);
                    SQLGetData(hstmt, 2, SQL_C_LONG, &courseId, 0, NULL);
                    SQLGetData(hstmt, 3, SQL_C_CHAR, regDate, sizeof(regDate), &dateInd);

                    StdCourse stdCourse(stdId, courseId, "");
                    if (dateInd != SQL_NULL_DATA)
                        stdCourse.setRegDate((char *)regDate);
                    newStdCourseBST.insert(stdCourse);
                }
                std::cout << "Loaded course registrations from database into BST" << std::endl;
            }
            else
            {
                std::cout << "Error loading course registrations: ";
                printSQLError(hstmt);
            }
            SQLFreeStmt(hstmt, SQL_CLOSE);
        }

        // Load student fees
        ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hstmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            ret = SQLExecDirect(hstmt, (SQLCHAR *)"SELECT * FROM StudentFees ORDER BY feeId", SQL_NTS);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
            {
                SQLINTEGER feeId, stdId;
                SQLDOUBLE amount;
                SQLCHAR paymentDate[100], status[100];
                SQLLEN dateInd, statusInd;
                while (SQLFetch(hstmt) == SQL_SUCCESS)
                {
                    SQLGetData(hstmt, 1, SQL_C_LONG, &feeId, 0, NULL);
                    SQLGetData(hstmt, 2, SQL_C_LONG, &stdId, 0, NULL);
                    SQLGetData(hstmt, 3, SQL_C_DOUBLE, &amount, 0, NULL);
                    SQLGetData(hstmt, 4, SQL_C_CHAR, paymentDate, sizeof(paymentDate), &dateInd);
                    SQLGetData(hstmt, 5, SQL_C_CHAR, status, sizeof(status), &statusInd);

                    StudentFees fee;
                    fee.setFeeId(feeId);
                    fee.setStdId(stdId);
                    fee.setAmount(amount);
                    if (dateInd != SQL_NULL_DATA)
                        fee.setPaymentDate((char *)paymentDate);
                    if (statusInd != SQL_NULL_DATA)
                        fee.setStatus(sanitizeForSQL((char *)status).c_str());
                    newStdFeeBST.insert(fee);
                }
                std::cout << "Loaded student fees from database into BST" << std::endl;
            }
            else
            {
                std::cout << "Error loading student fees: ";
                printSQLError(hstmt);
            }
            SQLFreeStmt(hstmt, SQL_CLOSE);
        }

        // Load attendance
        ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hstmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            ret = SQLExecDirect(hstmt, (SQLCHAR *)"SELECT * FROM Attendance ORDER BY attendanceId", SQL_NTS);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
            {
                SQLINTEGER attendanceId, stdId, courseId;
                SQLCHAR attendanceDate[100];
                SQLSMALLINT isPresent;
                SQLLEN dateInd;
                while (SQLFetch(hstmt) == SQL_SUCCESS)
                {
                    SQLGetData(hstmt, 1, SQL_C_LONG, &attendanceId, 0, NULL);
                    SQLGetData(hstmt, 2, SQL_C_LONG, &stdId, 0, NULL);
                    SQLGetData(hstmt, 3, SQL_C_LONG, &courseId, 0, NULL);
                    SQLGetData(hstmt, 4, SQL_C_CHAR, attendanceDate, sizeof(attendanceDate), &dateInd);
                    SQLGetData(hstmt, 5, SQL_C_SSHORT, &isPresent, 0, NULL);

                    Attendance attendance;
                    attendance.setAttendanceId(attendanceId);
                    attendance.setStdId(stdId);
                    attendance.setCourseId(courseId);
                    if (dateInd != SQL_NULL_DATA)
                        attendance.setAttendanceDate((char *)attendanceDate);
                    attendance.setIsPresent(isPresent == 1);
                    newAttendanceBST.insert(attendance);
                }
                std::cout << "Loaded attendance from database into BST" << std::endl;
            }
            else
            {
                std::cout << "Error loading attendance: ";
                printSQLError(hstmt);
            }
            SQLFreeStmt(hstmt, SQL_CLOSE);
        }

        // Load results
        ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hstmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            ret = SQLExecDirect(hstmt, (SQLCHAR *)"SELECT * FROM result ORDER BY stdId, courseId", SQL_NTS);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
            {
                SQLINTEGER stdId, courseId;
                SQLDOUBLE gotNumber;
                SQLCHAR grade[10];
                SQLLEN gradeInd;
                while (SQLFetch(hstmt) == SQL_SUCCESS)
                {
                    SQLGetData(hstmt, 1, SQL_C_LONG, &stdId, 0, NULL);
                    SQLGetData(hstmt, 2, SQL_C_LONG, &courseId, 0, NULL);
                    SQLGetData(hstmt, 3, SQL_C_DOUBLE, &gotNumber, 0, NULL);
                    SQLGetData(hstmt, 4, SQL_C_CHAR, grade, sizeof(grade), &gradeInd);

                    Result result;
                    result.setStdId(stdId);
                    result.setCourseId(courseId);
                    result.setGotNumber(gotNumber);
                    if (gradeInd != SQL_NULL_DATA)
                        result.setGrade(sanitizeForSQL((char *)grade).c_str());
                    newStdResultBST.insert(result);
                }
                std::cout << "Loaded results from database into BST" << std::endl;
            }
            else
            {
                std::cout << "Error loading results: ";
                printSQLError(hstmt);
            }
            SQLFreeStmt(hstmt, SQL_CLOSE);
        }

        // Load admins
        ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hstmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            ret = SQLExecDirect(hstmt, (SQLCHAR *)"SELECT * FROM adminTab ORDER BY adminId", SQL_NTS);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
            {
                SQLINTEGER adminId;
                SQLCHAR adminName[100], passWord[100];
                SQLLEN nameInd, passInd;
                while (SQLFetch(hstmt) == SQL_SUCCESS)
                {
                    SQLGetData(hstmt, 1, SQL_C_LONG, &adminId, 0, NULL);
                    SQLGetData(hstmt, 2, SQL_C_CHAR, adminName, sizeof(adminName), &nameInd);
                    SQLGetData(hstmt, 3, SQL_C_CHAR, passWord, sizeof(passWord), &passInd);

                    Admin admin;
                    admin.setAdminId(adminId);
                    if (nameInd != SQL_NULL_DATA)
                        admin.setAdminName(sanitizeForSQL((char *)adminName).c_str());
                    if (passInd != SQL_NULL_DATA)
                        admin.setPassWord(sanitizeForSQL((char *)passWord).c_str());
                    newAdminBST.insert(admin);
                }
                std::cout << "Loaded admins from database into BST" << std::endl;
            }
            else
            {
                std::cout << "Error loading admins: ";
                printSQLError(hstmt);
            }
            SQLFreeStmt(hstmt, SQL_CLOSE);
        }

        // Assign new BSTs to replace old ones
        studentBST = newStudentBST;
        courseBST = newCourseBST;
        fieldBST = newFieldBST;
        attendanceBST = newAttendanceBST;
        stdCourseBST = newStdCourseBST;
        stdFeeBST = newStdFeeBST;
        stdResultBST = newStdResultBST;
        adminBST = newAdminBST;

        std::cout << "=== All data loaded successfully ===" << std::endl;
    }

    // ================= Queue Operations =================
    bool updateStudentField(int id, const std::string &fieldName, const std::string &newValue)
    {
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return false;
        }

        std::string sql = "UPDATE student SET " + fieldName + " = ? WHERE stdId = ?";

        if (SQLPrepare(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }

        std::string cleanValue = truncateToSize(sanitizeForSQL(newValue), 40);
        SQLLEN valueLen = SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         cleanValue.length(), 0, (SQLPOINTER)cleanValue.c_str(), 0, &valueLen);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &id, 0, nullptr);

        bool success = (SQLExecute(hStmt) == SQL_SUCCESS);

        if (!success)
        {
            std::cout << "Update failed: ";
            printSQLError(hStmt);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return success;
    }

    bool updateCourseField(int id, const std::string &fieldName, const std::string &newValue)
    {
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Failed to allocate statement handle!" << std::endl;
            return false;
        }

        std::string sql = "UPDATE course SET " + fieldName + " = ? WHERE courseId = ?";

        if (SQLPrepare(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }

        std::string cleanValue = truncateToSize(sanitizeForSQL(newValue), 40);
        SQLLEN valueLen = SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         cleanValue.length(), 0, (SQLPOINTER)cleanValue.c_str(), 0, &valueLen);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &id, 0, nullptr);

        bool success = (SQLExecute(hStmt) == SQL_SUCCESS);

        if (!success)
        {
            std::cout << "Update failed: ";
            printSQLError(hStmt);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return success;
    }

    void universalUPdation(upQueue &que)
    {
        if (!db.connected())
        {
            std::cout << "Database is not connected. Cannot process updates." << std::endl;
            return;
        }

        std::cout << "\nStarting update processing..." << std::endl;

        while (!que.isEmpty())
        {
            upNode *nod = que.dequeue();
            if (!nod)
            {
                std::cerr << "Error: Null node dequeued" << std::endl;
                continue;
            }

            std::pair<int, std::string> meta = nod->getMetaData();
            int id = meta.first;
            std::string table = meta.second;

            std::cout << "\n--------------------------------------" << std::endl;
            std::cout << "Processing: " << table << " (ID: " << id << ")" << std::endl;
            std::cout << "--------------------------------------" << std::endl;

            if (table == "student")
            {
                int choice;
                std::cout << "\nSelect field to update:" << std::endl;
                std::cout << "1. stdName (VARCHAR(40))" << std::endl;
                std::cout << "2. stdFatherName (VARCHAR(40))" << std::endl;
                std::cout << "3. stdUserName (VARCHAR(40))" << std::endl;
                std::cout << "4. stdAge (INT)" << std::endl;
                std::cout << "5. fieldId (INT)" << std::endl;
                std::cout << "6. stdId (INT - Update ID itself)" << std::endl;
                std::cout << "Enter choice: ";
                std::cin >> choice;
                std::cin.ignore();

                std::string newValue;
                bool success = false;

                if (choice == 1)
                {
                    std::cout << "Enter new stdName (max 40 chars): ";
                    std::getline(std::cin, newValue);
                    success = updateStudentField(id, "stdName", newValue);
                }
                else if (choice == 2)
                {
                    std::cout << "Enter new stdFatherName (max 40 chars): ";
                    std::getline(std::cin, newValue);
                    success = updateStudentField(id, "stdFatherName", newValue);
                }
                else if (choice == 3)
                {
                    std::cout << "Enter new stdUserName (max 40 chars): ";
                    std::getline(std::cin, newValue);
                    success = updateStudentField(id, "stdUserName", newValue);
                }
                else if (choice == 4)
                {
                    std::cout << "Enter new stdAge: ";
                    std::getline(std::cin, newValue);
                    try
                    {
                        int age = std::stoi(newValue);
                        if (age > 0)
                        {
                            if (db.executeUpdate("UPDATE student SET stdAge = ? WHERE stdId = ?", age, id))
                            {
                                success = true;
                                std::cout << "✓ Student age updated successfully." << std::endl;
                            }
                        }
                        else
                            std::cout << "Age must be greater than 0!" << std::endl;
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "Invalid age! Please enter a valid number." << std::endl;
                    }
                }
                else if (choice == 5)
                {
                    std::cout << "Enter new fieldId: ";
                    std::getline(std::cin, newValue);
                    try
                    {
                        int fieldId = std::stoi(newValue);
                        if (db.executeUpdate("UPDATE student SET fieldId = ? WHERE stdId = ?", fieldId, id))
                        {
                            success = true;
                            std::cout << "✓ Student field ID updated successfully." << std::endl;
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "Invalid field ID! Please enter a valid number." << std::endl;
                    }
                }
                else if (choice == 6)
                {
                    std::cout << "Enter new stdId: ";
                    std::getline(std::cin, newValue);
                    try
                    {
                        int newId = std::stoi(newValue);
                        if (db.executeUpdate("UPDATE student SET stdId = ? WHERE stdId = ?", newId, id))
                        {
                            success = true;
                            std::cout << "✓ Student ID updated successfully." << std::endl;
                            id = newId;
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "Invalid student ID! Please enter a valid number." << std::endl;
                    }
                }
                else
                {
                    std::cout << "Invalid choice! Skipping this update." << std::endl;
                }

                if (success && choice >= 1 && choice <= 3)
                    std::cout << "✓ Student field updated successfully." << std::endl;
            }
            else if (table == "course")
            {
                int choice;
                std::cout << "\nSelect field to update:" << std::endl;
                std::cout << "1. courseTitle (VARCHAR(40))" << std::endl;
                std::cout << "2. teacherName (VARCHAR(40))" << std::endl;
                std::cout << "3. courseId (INT - Update ID itself)" << std::endl;
                std::cout << "Enter choice: ";
                std::cin >> choice;
                std::cin.ignore();

                std::string newValue;
                bool success = false;

                if (choice == 1)
                {
                    std::cout << "Enter new courseTitle (max 40 chars): ";
                    std::getline(std::cin, newValue);
                    success = updateCourseField(id, "courseTitle", newValue);
                }
                else if (choice == 2)
                {
                    std::cout << "Enter new teacherName (max 40 chars): ";
                    std::getline(std::cin, newValue);
                    success = updateCourseField(id, "teacherName", newValue);
                }
                else if (choice == 3)
                {
                    std::cout << "Enter new courseId: ";
                    std::getline(std::cin, newValue);
                    try
                    {
                        int newId = std::stoi(newValue);
                        if (db.executeUpdate("UPDATE course SET courseId = ? WHERE courseId = ?", newId, id))
                        {
                            success = true;
                            std::cout << "✓ Course ID updated successfully." << std::endl;
                            id = newId;
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "Invalid course ID! Please enter a valid number." << std::endl;
                    }
                }
                else
                {
                    std::cout << "Invalid choice! Skipping this update." << std::endl;
                }

                if (success && (choice == 1 || choice == 2))
                    std::cout << "✓ Course field updated successfully." << std::endl;
            }
            else if (table == "adminTab")
            {
                int choice;
                std::cout << "\nSelect field to update:" << std::endl;
                std::cout << "1. adminName (VARCHAR(40))" << std::endl;
                std::cout << "2. passWord (VARCHAR(255))" << std::endl;
                std::cout << "Enter choice: ";
                std::cin >> choice;
                std::cin.ignore();

                std::string newValue;
                bool success = false;

                if (choice == 1)
                {
                    std::cout << "Enter new adminName (max 40 chars): ";
                    std::getline(std::cin, newValue);
                    std::string cleanValue = truncateToSize(sanitizeForSQL(newValue), 40);
                    if (db.executeUpdate("UPDATE adminTab SET adminName = ? WHERE adminId = ?", cleanValue, id))
                    {
                        success = true;
                        std::cout << "✓ Admin name updated successfully." << std::endl;
                    }
                }
                else if (choice == 2)
                {
                    std::cout << "Enter new passWord (max 255 chars): ";
                    std::getline(std::cin, newValue);
                    std::string cleanValue = truncateToSize(sanitizeForSQL(newValue), 255);
                    if (db.executeUpdate("UPDATE adminTab SET passWord = ? WHERE adminId = ?", cleanValue, id))
                    {
                        success = true;
                        std::cout << "✓ Admin password updated successfully." << std::endl;
                    }
                }
                else
                {
                    std::cout << "Invalid choice! Skipping this update." << std::endl;
                }
            }
            else
            {
                std::cout << "Table '" << table << "' update not implemented yet." << std::endl;
            }

            delete nod;
            std::cout << "--------------------------------------" << std::endl;
        }

        // Reload data after all updates
        loadAllDataFromDB();

        std::cout << "\n======================================" << std::endl;
        std::cout << "All updates processed successfully!" << std::endl;
        std::cout << "======================================" << std::endl;
    }

    void fixCorruptedData()
    {
        std::cout << "\n=== Fixing Corrupted Data ===" << std::endl;

        if (!db.connected())
        {
            std::cout << "Database not connected!" << std::endl;
            return;
        }

        // Fix corrupted student names
        db.executeUpdate("UPDATE student SET stdName = 'Fatima Raza' WHERE stdId = 1004 AND stdName LIKE '%�%'");
        db.executeUpdate("UPDATE student SET stdName = 'Zainab Akhtar' WHERE stdId = 1008 AND stdName LIKE '%�%'");

        // Fix course titles
        db.executeUpdate("UPDATE course SET courseTitle = 'Artificial Intelligence' WHERE courseId = 2009");
        db.executeUpdate("UPDATE course SET courseTitle = 'Cyber Security' WHERE courseId = 2010");

        std::cout << "✓ Attempted to fix corrupted data" << std::endl;

        // Reload data
        loadAllDataFromDB();
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

        int mainChoice = 0;
        while (mainChoice != 6)
        {
            std::cout << "\n======================================" << std::endl;
            std::cout << "        MAIN MENU" << std::endl;
            std::cout << "======================================" << std::endl;
            std::cout << "1. Update System Records" << std::endl;
            std::cout << "2. Process Update Queue (" << updateQueue.size() << " pending)" << std::endl;
            std::cout << "3. Reload Data from Database" << std::endl;
            std::cout << "4. Print All Data" << std::endl;
            std::cout << "5. Fix Corrupted Data" << std::endl;
            std::cout << "6. Exit" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> mainChoice;
            std::cin.ignore();

            switch (mainChoice)
            {
            case 1: // Update System Records
            {
                std::cout << "\n======================================" << std::endl;
                std::cout << "    SELECT TABLE TO UPDATE" << std::endl;
                std::cout << "======================================" << std::endl;
                std::cout << "1. student" << std::endl;
                std::cout << "2. course" << std::endl;
                std::cout << "3. adminTab" << std::endl;
                std::cout << "Enter table number: ";

                int tableChoice;
                std::cin >> tableChoice;
                std::cin.ignore();

                std::string tableName;
                switch (tableChoice)
                {
                case 1:
                    tableName = "student";
                    break;
                case 2:
                    tableName = "course";
                    break;
                case 3:
                    tableName = "adminTab";
                    break;
                default:
                    std::cout << "Invalid choice! Returning to main menu." << std::endl;
                    continue;
                }

                int recordId;
                std::cout << "Enter record ID for " << tableName << ": ";
                std::cin >> recordId;
                std::cin.ignore();

                // Create and add to queue
                upNode *updateNode = new upNode(recordId, tableName);
                updateQueue.enqueue(updateNode);

                std::cout << "\n✓ Update request added to queue!" << std::endl;
                std::cout << "Table: " << tableName << ", ID: " << recordId << std::endl;

                // Ask if user wants to add more
                char more;
                std::cout << "\nAdd another update request? (y/n): ";
                std::cin >> more;
                std::cin.ignore();

                if (more == 'y' || more == 'Y')
                {
                    mainChoice = 1; // Stay in update mode
                }
                break;
            }

            case 2: // Process Update Queue
            {
                if (updateQueue.isEmpty())
                {
                    std::cout << "\nUpdate queue is empty. Nothing to process." << std::endl;
                }
                else
                {
                    std::cout << "\n======================================" << std::endl;
                    std::cout << "    PROCESSING UPDATE QUEUE" << std::endl;
                    std::cout << "======================================" << std::endl;
                    std::cout << "Queue size: " << updateQueue.size() << " requests" << std::endl;

                    std::cout << "\nProcessing update requests..." << std::endl;
                    universalUPdation(updateQueue);
                }
                break;
            }

            case 3: // Reload Data from Database
            {
                std::cout << "\n======================================" << std::endl;
                std::cout << "    RELOADING DATA FROM DATABASE" << std::endl;
                std::cout << "======================================" << std::endl;
                loadAllDataFromDB();
                break;
            }

            case 4: // Print All Data
            {
                std::cout << "\n======================================" << std::endl;
                std::cout << "        PRINTING ALL DATA" << std::endl;
                std::cout << "======================================" << std::endl;

                std::cout << "\n=== All Students ===" << std::endl;
                printEngine.printAllStudents(studentBST);

                std::cout << "\n=== All Courses ===" << std::endl;
                printEngine.printAllCourses(courseBST);

                std::cout << "\n=== All Fields ===" << std::endl;
                printEngine.printAllFields(fieldBST);

                break;
            }

            case 5: // Fix Corrupted Data
            {
                fixCorruptedData();
                break;
            }

            case 6: // Exit
            {
                // Clean up any remaining nodes in the queue
                while (!updateQueue.isEmpty())
                {
                    upNode *node = updateQueue.dequeue();
                    delete node;
                }

                std::cout << "\nExiting Student Management System..." << std::endl;
                db.disconnect();
                return;
            }

            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
            }
        }
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