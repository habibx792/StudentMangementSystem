#ifndef SMSSYSTEM_H
#define SMSSYSTEM_H

// ================= Standard Library =================
#include <string>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <locale>
#include <algorithm>

// ================= Core Data Structures =================
#include "stdBst.h"
#include "opQueue.h"
#include "updationQueue.h"
#include "delNod.h"
#include "deleteQueues.h"

// #include "delNod.h"
// #include "opNode.h"
// #include "InsertionQueue.h"
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

#include "db.h"

// ================= System Class =================
class SMSSystem
{
private:
    DB db;                   // Database connection handler                  // User interface handler
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
    opQueue<Admin> newAdminOpQueue;              // Queue for new admin records
    upQueue updateQueue;                         // Queue for tracking updates
    deleteQueue delQueue;                        // Queue for deletions

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
        cout << "Test  don --->\n";

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
            std::cout << "Student inserted hghghjghj  successfully." << std::endl;
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
        const char *query = "INSERT INTO fieldStudy (fieldName) VALUES (?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        std::string fieldName = truncateToSize(sanitizeForSQL(field.getFieldName()), 40);
        SQLLEN nameLen = fieldName.empty() ? SQL_NULL_DATA : SQL_NTS;

        // Bind only one parameter now (fieldName)
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
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
        const char *query = "INSERT INTO Attendance (stdId, courseId, attendanceDate, isPresent) VALUES (?, ?, ?, ?)";
        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        SQLINTEGER stdId = attendance.getStdId();
        SQLINTEGER courseId = attendance.getCourseId();
        std::string attendanceDate = truncateToSize(sanitizeForSQL(attendance.getAttendanceDate()), 10);
        SQLSMALLINT isPresent = attendance.getIsPresent() ? 1 : 0;

        SQLLEN dateLen = attendanceDate.empty() ? SQL_NULL_DATA : SQL_NTS;
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &courseId, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         10, 0, (SQLPOINTER)attendanceDate.c_str(), 0, &dateLen);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT,
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

        const char *query = "INSERT INTO StudentFees ( stdId, amount, paymentDate, status) VALUES ( ?, ?, ?, ?)";

        if (SQLPrepare(hStmt, (SQLCHAR *)query, SQL_NTS) != SQL_SUCCESS)
        {
            std::cout << "Failed to prepare statement!" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        SQLINTEGER stdId = fee.getStdId();
        SQLDOUBLE amount = fee.getAmount();
        std::string paymentDate = truncateToSize(sanitizeForSQL(fee.getPaymentDate()), 10);
        std::string status = truncateToSize(sanitizeForSQL(fee.getStatus()), 20);

        SQLLEN dateLen = paymentDate.empty() ? SQL_NULL_DATA : SQL_NTS;
        SQLLEN statusLen = status.empty() ? SQL_NULL_DATA : SQL_NTS;
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
                         0, 0, &amount, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         10, 0, (SQLPOINTER)paymentDate.c_str(), 0, &dateLen);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
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

        const char *query = "INSERT INTO result (stdId, courseId, gotNumber, grade, isPass) VALUES (?, ?, ?, ?, ?)";

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
        SQLCHAR isPass = result.isPass();

        SQLLEN gradeLen = grade.empty() ? SQL_NULL_DATA : SQL_NTS;

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &stdId, 0, nullptr);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                         0, 0, &courseId, 0, nullptr);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
                         0, 0, &gotNumber, 0, nullptr);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         2, 0, (SQLPOINTER)grade.c_str(), 0, &gradeLen);
        SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                         1, 0, &isPass, 0, nullptr);

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
        insertAdminToDB(admin);
        adminBST.insert(admin);
        newAdminOpQueue.enqueueNewStd(admin);
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

            std::pair<pair<int, int>, std::string> meta = nod->getMetaData();
            int id = meta.first.first;
            std::string table = meta.second;

            std::cout << "\n--------------------------------------" << std::endl;
            std::cout << "Processing: " << table << " (ID: " << id << ")" << std::endl;
            std::cout << "--------------------------------------" << std::endl;

            if (table == "student")
            {
                int choice;
                std::cout << "\nSelect field to update:" << std::endl;
                std::cout << "1.Student  Name (VARCHAR(40))" << std::endl;
                std::cout << "2.Student FatherName (VARCHAR(40))" << std::endl;
                std::cout << "3.Student UserName (VARCHAR(40))" << std::endl;
                std::cout << "4.Student Age (INT)" << std::endl;
                std::cout << "5.Student fieldId (INT)" << std::endl;
                std::cout << "6. Student stdId (INT - Update ID itself)" << std::endl;
                std::cout << "Enter choice: ";
                std::cin >> choice;
                std::cin.ignore();

                std::string newValue;
                bool success = false;

                if (choice == 1)
                {
                    std::cout << "Enter new Student Name (max 40 chars): ";
                    std::getline(std::cin, newValue);
                    success = updateStudentField(id, "stdName", newValue);
                }
                else if (choice == 2)
                {
                    std::cout << "Enter  Student new FatherName (max 40 chars): ";
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
                    std::cout << "Enter  Student new Age: ";
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
                    std::cout << "Enter Student new fieldId: ";
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
                std::cout << "1. course Title (VARCHAR(40))" << std::endl;
                std::cout << "2. teacher Name (VARCHAR(40))" << std::endl;
                std::cout << "3. courseId (INT - Update ID itself)" << std::endl;
                std::cout << "Enter choice: ";
                std::cin >> choice;
                std::cin.ignore();

                std::string newValue;
                bool success = false;

                if (choice == 1)
                {
                    std::cout << "Enter new course Title (max 40 chars): ";
                    std::getline(std::cin, newValue);
                    success = updateCourseField(id, "courseTitle", newValue);
                }
                else if (choice == 2)
                {
                    std::cout << "Enter new teacher Name (max 40 chars): ";
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
            else if (table == "fieldStudy")
            {
                int choice;
                std::cout << "\nSelect field to update:" << std::endl;
                std::cout << "1. fieldName (VARCHAR(40))" << std::endl;
                std::cout << "Enter choice: ";
                std::cin >> choice;
                std::cin.ignore();

                std::string newValue;
                bool success = false;

                if (choice == 1)
                {
                    std::cout << "Enter new field name (max 40 chars): ";
                    std::getline(std::cin, newValue);
                    std::string cleanValue = truncateToSize(sanitizeForSQL(newValue), 40);
                    if (db.executeUpdate("UPDATE fieldStudy SET fieldName = ? WHERE fieldId = ?", cleanValue, id))
                    {
                        success = true;
                        std::cout << "✓ Field name updated successfully." << std::endl;
                    }
                }
                else
                {
                    std::cout << "Invalid choice!";
                }
            }
            else if (table == "result")
            {
                int choice;
                std::cout << "Updating Student Result Information \n";
                cout << "Enter Your Student  Id: \n";
                int resid;
                cin >> resid;
                cin.ignore();
                cout << "Enter Your Course ID : \n";
                int courseId;
                cin >> courseId;
                cin.ignore();
                cout << "1-> To Update Got Number \n";
                cout << "2-> To Update isPass \n";
                cout << "Enter Your Choice: \n";
                cin >> choice;
                cin.ignore();
                if (choice == 1)
                {
                    int marksObtained;
                    cout << "Enter New Marks Obtained: \n";
                    cin >> marksObtained;
                    cin.ignore();
                    if (db.executeUpdate("UPDATE result SET marksObtained = ? WHERE stdId = ? AND courseId = ?", marksObtained, resid, courseId))
                    {

                        std::cout << "✓ Marks obtained updated successfully." << std::endl;
                    }
                }
                else if (choice == 2)
                {
                    bool isPass;
                    cout << "Enter New isPass (0 or 1): \n";
                    cin >> isPass;
                    cin.ignore();
                    if (db.executeUpdate("UPDATE result SET isPass = ? WHERE stdId = ? AND courseId = ?", isPass, resid, courseId))
                    {

                        std::cout << "✓ Is pass updated successfully." << std::endl;
                    }
                }
            }
            else if (table == "attendance")
            {
                int choice;
                int stdId;
                int courseId;
                cout << "Enter Student Id: \n";
                cin >> stdId;
                cin.ignore();
                cout << "Enter Course Id: \n";
                cin >> courseId;
                cin.ignore();
                cout << "1-> To Update Attendance Status \n";
                cout << "2-> To Update Attendance Date \n";
                cout << "Enter Your Choice: \n";
                cin >> choice;
                cin.ignore();
                if (choice == 1)
                {
                    bool isPresent;
                    cout << "Enter New Attendance Status (0 or 1): \n";
                    cin >> isPresent;
                    cin.ignore();
                    if (db.executeUpdate("UPDATE attendance SET isPresent = ? WHERE stdId = ? AND courseId = ?", isPresent, stdId, courseId))
                    {
                        std::cout << "✓ Attendance status updated successfully." << std::endl;
                    }
                }
                else if (choice == 2)
                {
                    string date;
                    cout << "Enter New Attendance Date (YYYY-MM-DD): \n";
                    getline(cin, date);
                    if (db.executeUpdate("UPDATE attendance SET date = ? WHERE stdId = ? AND courseId = ?", date, stdId, courseId))
                    {
                        std::cout << "✓ Attendance date updated successfully." << std::endl;
                    }
                }
            }
            else if (table == "StudentFees")
            {
                int choice;
                int stdId;
                cout << "Enter Student Id \n";
                cin >> stdId;
                cin.ignore();
                int feeId;
                cout << "Enter Fee Id \n";
                cin >> feeId;
                cin.ignore();
                cout << "1-> To Update Fee Amount \n";
                cout << "2-> To Update Fee Date \n";
                cout << "Enter Your Choice: \n";
                cin >> choice;
                cin.ignore();
                if (choice == 1)
                {
                    double feeAmount;
                    cout << "Enter New Fee Amount: \n";
                    cin >> feeAmount;
                    cin.ignore();
                    if (db.executeUpdate("UPDATE StudentFees SET feeAmount = ? WHERE stdId = ? AND feeId = ?", feeAmount, stdId, feeId))
                    {
                        std::cout << "✓ Fee amount updated successfully." << std::endl;
                    }
                }
                else if (choice == 2)
                {
                    string date;
                    cout << "Enter New Fee Date (YYYY-MM-DD): \n";
                    getline(cin, date);
                    if (db.executeUpdate("UPDATE StudentFees SET date = ? WHERE stdId = ? AND feeId = ?", date, stdId, feeId))
                    {
                        std::cout << "✓ Fee date updated successfully." << std::endl;
                    }
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

    // ================= Insertion UI Methods =================
    void addNewStudent()
    {
        int stdId;
        string stdName;
        string fatherName;
        string userName;
        int fieldId;

        int age;
        cout << "Enter A random Student Id Between 10000 t to 999999999 \n";
        cin >> stdId;
        cin.ignore();

        cout << "Enter Student Name: ";
        getline(cin, stdName);
        cout << "Enter Student Age: ";
        cin >> age;
        cin.ignore();
        cout << "Enter Student Father Name: ";
        getline(cin, fatherName);
        cout << "Enter Student User Name: ";
        getline(cin, userName);
        cout << "Enter Student Field Id: ";
        cin >> fieldId;
        cin.ignore();
        //     Student(int id, string name, string userName, int age, int fId, string fatherName)

        Student student(stdId, stdName, userName, age, fieldId, fatherName);
        registerNewStudent(student);
    }
    void addNewCourse()
    {
        string courseTitle;
        string teacherName;
        int id;
        cout << "Enter A randdom Course Id betweeen 10000 to 99999\n";
        cin >> id;
        cin.ignore();
        cout << "Enter Course Title: \n";
        getline(cin, courseTitle);
        cout << "Enter Teacher Name: \n";
        getline(cin, teacherName);
        Course course(id, courseTitle, teacherName);
        registerNewCourse(course);
    }
    void addNewField()
    {
        string fieldName;
        int fieldId;
        cout << "Enter Field Id: \n";
        cin >> fieldId;
        cin.ignore();
        cout << "Enter Field Name: \n";
        getline(cin, fieldName);
        FieldStudy field(fieldId, fieldName);

        registerNewField(field);
    }
    void addNewAttendance()
    {
        int attendanceId;
        int stdId;
        int courseId;
        string attendanceDate;
        bool isPresent;
        cout << "Enter Attendance Id: \n";
        cin >> attendanceId;
        cin.ignore();
        cout << "Enter Student Id: \n";
        cin >> stdId;
        cin.ignore();
        cout << "Enter Course Id: \n";
        cin >> courseId;
        cin.ignore();
        cout << "Enter Attendance Date (YYYY-MM-DD): \n";
        getline(cin, attendanceDate);
        cout << "Enter Present Status (1 for Present, 0 for Absent): \n";
        cin >> isPresent;
        cin.ignore();
        Attendance attendance(attendanceId, stdId, courseId, attendanceDate, isPresent);
        registerNewAttendance(attendance);
    }
    void addNewStdCourse()
    {
        int stdId;
        int courseId;
        string regDate;
        cout << "Enter Student Id: \n";
        cin >> stdId;
        cin.ignore();
        cout << "Enter Course Id: \n";
        cin >> courseId;
        cin.ignore();
        cout << "Enter Registration Date (YYYY-MM-DD): \n";
        getline(cin, regDate);
        StdCourse stdCourse(stdId, courseId, regDate);
        registerNewStdCourse(stdCourse);
    }
    void addNewStudentFees()
    {
        int feeId;
        int stdId;
        double amount;
        string paymentDate;
        string status;
        cout << "Enter Fee Id: \n";
        cin >> feeId;
        cin.ignore();
        cout << "Enter Student Id: \n";
        cin >> stdId;
        cin.ignore();
        cout << "Enter Amount: \n";
        cin >> amount;
        cin.ignore();
        cout << "Enter Payment Date (YYYY-MM-DD): \n";
        getline(cin, paymentDate);
        cout << "Enter Status:->Pending<->Paid \n";
        cout << "1 For Pending,\n 2 For Paid \n";
        int st;
        cin >> st;
        cin.ignore();
        if (st == 1)
        {
            status = "Pending";
        }
        else
        {
            status = "Paid";
        }
        StudentFees stdFee(feeId, stdId, amount, paymentDate, status);
        registerNewStudentFees(stdFee);
    }
    void addNewResult()
    {
        int stdId;
        int courseId;
        double gotNumber;
        string grade;
        cout << "Enter Student Id: \n";
        cin >> stdId;
        cin.ignore();
        cout << "Enter Course Id: \n";
        cin >> courseId;
        cin.ignore();
        cout << "Enter Got Number: \n";
        cin >> gotNumber;
        cin.ignore();
        cout << "Enter Grade:->A->A+ \n";
        getline(cin, grade);
        Result stdResult(stdId, courseId, gotNumber, grade);
        registerNewResult(stdResult);
    }
    void addNewAdmin()
    {
        int adminId;
        string adminName;
        string passWord;
        cout << "Enter Admin Id: \n";
        cin >> adminId;
        cin.ignore();
        cout << "Enter Admin Name: \n";
        getline(cin, adminName);
        cout << "Enter Admin Password: \n";
        getline(cin, passWord);
        Admin admin(adminId, adminName, passWord);
        registerNewAdmin(admin);
    }
    void UniVersalInsertionMethod()
    {
        cout << "Welcome to Student Management System Insertion ";
        int insetChoise = 0;
        while (insetChoise != 9)
        {
            cout << "\n=== INSERTION MENU ===" << std::endl;
            cout << "1. Add Student" << std::endl;
            cout << "2. Add Course" << std::endl;
            cout << "3. Add Field Study" << std::endl;
            cout << "4. Add Attendance" << std::endl;
            cout << "5. Add Student Course Registration" << std::endl;
            cout << "6. Add Student Fee Information" << std::endl;
            cout << "7. Add Result Information" << std::endl;
            cout << "8. Add Admin Information" << std::endl;
            cout << "9. Back to Main Menu" << std::endl;
            insetChoise = getChoice(1, 9);

            switch (insetChoise)
            {
            case 1:
                addNewStudent();
                break;
            case 2:
                addNewCourse();
                break;
            case 3:
                addNewField();
                break;
            case 4:
                addNewAttendance();
                break;
            case 5:
                addNewStdCourse();
                break;
            case 6:
                addNewStudentFees();
                break;
            case 7:
                addNewResult();
                break;
            case 8:
                addNewAdmin();
                break;
            default:
                break; // No action needed for default
            }
        }
    }

    // ================= Deletion UI Methods =================
    void deleteStudent()
    {
        int stdId;
        string table = "student"; // corrected
        cout << "Enter Student ID to delete: ";
        cin >> stdId;
        cin.ignore();
        deleteNode *delNode = new deleteNode(stdId, table);
        delQueue.enqueue(delNode);
    }
    void deleteCourse()
    {
        int courseId;
        string table = "course"; // corrected
        cout << "Enter Course ID to delete: ";
        cin >> courseId;
        cin.ignore();
        deleteNode *delNode = new deleteNode(courseId, table);
        delQueue.enqueue(delNode);
    }
    void deleteField()
    {
        int fieldId;
        string table = "fieldStudy"; // corrected
        cout << "Enter Field Study ID to delete: ";
        cin >> fieldId;
        cin.ignore();
        deleteNode *delNode = new deleteNode(fieldId, table);
        delQueue.enqueue(delNode);
    }
    void deleteResult()
    {
        int stdId;
        int courseId;
        string table = "result"; // already correct
        cout << "Enter Student ID to delete result information: ";
        cin >> stdId;
        cin.ignore();
        cout << "Enter Course ID to delete result information: ";
        cin >> courseId;
        deleteNode *delNode = new deleteNode(stdId, courseId, table);
        delQueue.enqueue(delNode);
    }
    void deleteStdFee()
    {
        int stdId;
        int feeId;
        string table = "StudentFees"; // already correct
        cout << "Enter Student ID to delete fee information: ";
        cin >> stdId;
        cin.ignore();
        cout << "Enter Fee ID to delete fee information: ";

        cin >> feeId;
        deleteNode *delNode = new deleteNode(stdId, feeId, table);
        delQueue.enqueue(delNode);
    }
    void deleteAdmin()
    {
        int adminId;
        string table = "adminTab"; // corrected
        cout << "Enter Admin ID to delete: ";
        cin >> adminId;
        cin.ignore();
        deleteNode *delNode = new deleteNode(adminId, table);
        delQueue.enqueue(delNode);
    }
    void deleteCourseStdReg()
    {
        int stdId;
        int courseId;
        string table = "courseRegStd"; // already correct
        cout << "Enter Student ID to delete course registration: ";
        cin >> stdId;
        cin.ignore();
        cout << "Enter Course ID to delete course registration: ";
        cin >> courseId;
        deleteNode *delNode = new deleteNode(stdId, courseId, table);
        delQueue.enqueue(delNode);
    }
    void deleteAttendance()
    {
        int stdId;
        int courseId;
        string table = "Attendance"; // already correct
        cout << "Enter Student ID to delete attendance: ";
        cin >> stdId;
        cin.ignore();
        cout << "Enter Course ID to delete attendance: ";

        cin >> courseId;
        deleteNode *delNode = new deleteNode(stdId, courseId, table);
        delQueue.enqueue(delNode);
    }
    void UniversalDeletions()
    {
        cout << "Welcome to Student Management System Deletion ";
        int deleteChoice = 0;
        while (deleteChoice != 9)
        {
            cout << "\n=== DELETION MENU ===" << std::endl;
            cout << "1. Delete Student" << std::endl;
            cout << "2. Delete Course" << std::endl;
            cout << "3. Delete Field Study" << std::endl;
            cout << "4. Delete Attendance" << std::endl;
            cout << "5. Delete Student Course Registration" << std::endl;
            cout << "6. Delete Student Fee Information" << std::endl;
            cout << "7. Delete Result Information" << std::endl;
            cout << "8. Delete Admin Information" << std::endl;
            cout << "9. Back to Main Menu" << std::endl;
            deleteChoice = getChoice(1, 9);
            switch (deleteChoice)
            {
            case 1:
                deleteStudent();
                break;
            case 2:
                deleteCourse();
                break;
            case 3:
                deleteField();
                break;
            case 4:
                deleteAttendance();
                break;
            case 5:
                deleteCourseStdReg();
                break;
            case 6:
                deleteStdFee();
                break;
            case 7:
                deleteResult();
                break;
            case 8:
                deleteAdmin();
                break;
            default:
                break;
            }
        }
    }

    // ================= Database Deletion Methods =================
    void deleteStudentFromDB(int stdId)
    {
        if (!db.connect())
        {
            cout << "Database connection failed." << endl;
            return;
        }
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to prepare statement." << endl;
            return;
        }
        string quer = "DELETE FROM student WHERE stdId = ?";
        if (SQLPrepare(hStmt, (SQLCHAR *)quer.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            cout << "Failed to prepare statement." << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &stdId, 0, NULL);
        ret = SQLExecute(hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to execute statement." << endl;
            return;
        }
        else
        {
            cout << "Student deleted successfully." << endl;
        }
    }
    void deleteCourseFromDB(int courseId)
    {
        if (!db.connect())
        {
            cout << "Database connection failed." << endl;
            return;
        }
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to prepare statement." << endl;
            return;
        }
        string quer = "DELETE FROM course WHERE courseId = ?";
        if (SQLPrepare(hStmt, (SQLCHAR *)quer.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            cout << "Failed to prepare statement." << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        cin.ignore();
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &courseId, 0, NULL);
        ret = SQLExecute(hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to execute statement." << endl;
            return;
        }
        else
        {
            cout << "Course deleted successfully." << endl;
        }
    }
    void deleteCourseRegFromDB(int std, int courseId)
    {
        if (!db.connect())
        {
            cout << "Database connection failed." << endl;
            return;
        }
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to prepare statement." << endl;
            return;
        }
        string quer = "DELETE FROM StudentCourse WHERE stdId = ? AND courseId = ?";
        if (SQLPrepare(hStmt, (SQLCHAR *)quer.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            cout << "Failed to prepare statement." << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &std, 0, NULL);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &courseId, 0, NULL);
        ret = SQLExecute(hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to execute statement." << endl;
            return;
        }
        else
        {
            cout << "Course registration deleted successfully." << endl;
        }
    }
    void deleteFieldFromDB(int fieldId)
    {
        if (!db.connect())
        {
            cout << "Database connection failed." << endl;
            return;
        }
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to prepare statement." << endl;
            return;
        }
        string quer = "DELETE FROM fieldStudy WHERE fieldId = ?";
        if (SQLPrepare(hStmt, (SQLCHAR *)quer.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            cout << "Failed to prepare statement." << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }

        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &fieldId, 0, NULL);
        ret = SQLExecute(hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to execute statement." << endl;
            return;
        }
        else
        {
            cout << "Field Study deleted successfully." << endl;
        }
    }
    void deleteAttendanceFromDB(int stdId, int courseId)
    {
        if (!db.connect())
        {
            cout << "Database connection failed." << endl;
            return;
        }
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to prepare statement." << endl;
            return;
        }
        string quer = "DELETE FROM Attendance WHERE stdId = ? AND courseId = ?";
        if (SQLPrepare(hStmt, (SQLCHAR *)quer.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            cout << "Failed to prepare statement." << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &stdId, 0, NULL);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &courseId, 0, NULL);
        ret = SQLExecute(hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to execute statement." << endl;
            return;
        }
        else
        {
            cout << "Attendance deleted successfully." << endl;
        }
    }
    void deleteStdFeeFromDB(int stdId, int feeId)
    {
        if (!db.connect())
        {
            cout << "Database connection failed." << endl;
            return;
        }
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to prepare statement." << endl;
            return;
        }
        string quer = "DELETE FROM StudentFees WHERE stdId = ? AND feeId = ?";
        if (SQLPrepare(hStmt, (SQLCHAR *)quer.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            cout << "Failed to prepare statement." << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &stdId, 0, NULL);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &feeId, 0, NULL);
        ret = SQLExecute(hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to execute statement." << endl;
            return;
        }
        else
        {
            cout << "Student Fee deleted successfully." << endl;
        }
    }
    void deleteResultFromDB(int stdId, int courseId)
    {
        if (!db.connect())
        {
            cout << "Database connection failed." << endl;
            return;
        }
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to prepare statement." << endl;
            return;
        }
        string quer = "DELETE FROM Result WHERE stdId = ? AND courseId = ?";
        if (SQLPrepare(hStmt, (SQLCHAR *)quer.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            cout << "Failed to prepare statement." << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &stdId, 0, NULL);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &courseId, 0, NULL);
        ret = SQLExecute(hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to execute statement." << endl;
            return;
        }
        else
        {
            cout << "Result deleted successfully." << endl;
        }
    }
    void deleteAdminFromDB(int adminId)
    {
        if (!db.connect())
        {
            cout << "Database connection failed." << endl;
            return;
        }
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, db.returnDb(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to prepare statement." << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        string quer = "DELETE FROM adminTab WHERE adminId = ?";
        if (SQLPrepare(hStmt, (SQLCHAR *)quer.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            cout << "Failed to prepare statement." << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return;
        }
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 10, 0, &adminId, 0, NULL);
        ret = SQLExecute(hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Failed to execute statement." << endl;
            return;
        }
        else
        {
            cout << "Admin deleted successfully." << endl;
        }
    }
    void restartSystemToDeleteData()
    {
        char choice;
        cout << "For deletion, do you want to restart the system? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            // Restart the system
            cout << "Restarting system..." << endl;
            while (delQueue.isEmpty())
            {
                deleteNode *nod = delQueue.dequeue();
                pair<pair<int, int>, string> metaData = nod->getMetaData();
                int delId = metaData.first.first;
                int delIdTwo = metaData.first.second;
                string table = metaData.second;
                if (table == "Student")
                {
                    deleteStudentFromDB(delId);
                }
                else if (table == "Course")
                {
                    deleteCourseFromDB(delId);
                }
                else if (table == "FieldStudy")
                {
                    deleteFieldFromDB(delId);
                }
                else if (table == "Attendance")
                {
                    deleteAttendanceFromDB(delId, delIdTwo);
                }
                else if (table == "courseRegStd")
                {
                    deleteCourseRegFromDB(delId, delIdTwo);
                }
                else if (table == "StudentFees")
                {
                    deleteStdFeeFromDB(delId, delIdTwo);
                }
                else if (table == "Result")
                {
                    deleteResultFromDB(delId, delIdTwo);
                }
                else if (table == "Admin")
                {
                    deleteAdminFromDB(delId);
                }
            }
        }
    }

public:
    // Static method to get the single instance
    static SMSSystem &getInstance()
    {
        static SMSSystem instance;
        return instance;
    }

    // Main method to run the system
    void clearScreen()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    // Pause execution until Enter is pressed
    void pause()
    {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    // Get validated integer input within a range
    int getChoice(int min, int max)
    {
        int choice;
        while (true)
        {
            cout << "Enter choice (" << min << "-" << max << "): ";
            cin >> choice;
            cin.ignore();

            if (cin.fail() || choice < min || choice > max)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Try again.\n";
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
        }
    }
    void run()
    {
        clearScreen();
        std::cout << "\n=== Student Management System ===" << std::endl;

        if (!db.connect())
        {
            std::cout << "Failed to connect to database. Exiting..." << std::endl;
            pause();
            return;
        }
        db.printStatus();
        loadAllDataFromDB();
        pause();

        int choice = 0;
        while (choice != 11) // Changed to 8 for exit
        {
            clearScreen();
            cout << "\n=== MAIN MENU ===" << std::endl;
            cout << "1. Student Information" << std::endl;
            cout << "2. Course Information" << std::endl;
            cout << "3. Admin Information" << std::endl;
            cout << "4. Fee Information" << std::endl;
            cout << "5. Field Information" << std::endl;
            cout << "6. Search Engine System" << std::endl;
            cout << "7. Update Data" << std::endl;
            cout << "8. Delete Data" << std::endl;
            cout << "9. Print Engine" << std::endl;
            cout << "10 To Insetion Operations \n";
            cout << "11. Exit" << std::endl;

            choice = getChoice(1, 11);

            if (choice == 1)
            {
                // Student Information Menu
                int stdChoice = 0;
                while (stdChoice != 7)
                {
                    clearScreen();
                    cout << "\n=== STUDENT MENU ===" << std::endl;
                    cout << "1. Add Student" << std::endl;
                    cout << "2. Update Student" << std::endl;
                    cout << "3. Delete Student" << std::endl;
                    cout << "4. Search Student" << std::endl;
                    cout << "5. Check Student Fee Information" << std::endl;
                    cout << "6. View All Students" << std::endl;
                    cout << "7. Back to Main Menu" << std::endl;

                    stdChoice = getChoice(1, 7);

                    if (stdChoice == 1)
                    {
                        clearScreen();
                        cout << "=== ADD STUDENT ===" << endl;
                        // Add Student
                        addnewStudent();
                        cout << "\nStudent Added Successfully!" << std::endl;
                        pause();
                    }
                    else if (stdChoice == 2)
                    {
                        clearScreen();
                        cout << "=== UPDATE STUDENT ===" << endl;
                        // Update Student
                        int stdId;
                        cout << "Enter Student ID to update: ";
                        cin >> stdId;
                        // Add update logic here
                        pause();
                    }
                    else if (stdChoice == 3)
                    {
                        clearScreen();
                        cout << "=== DELETE STUDENT ===" << endl;
                        // Delete Student
                        int stdId;
                        cout << "Enter Student ID to delete: ";
                        cin >> stdId;
                        // Add delete logic here
                        pause();
                    }
                    else if (stdChoice == 4)
                    {
                        clearScreen();
                        cout << "=== SEARCH STUDENT ===" << endl;
                        // Search Student
                        int stdId;
                        cout << "Enter Student ID to search: ";
                        cin >> stdId;
                        StdNode<Student> *node = studentBST.search(stdId);
                        if (node)
                        {
                            Student std = node->getData();
                            printEngine.printStudentById(studentBST, stdId);
                        }
                        else
                        {
                            cout << "Student not found!" << std::endl;
                        }
                        pause();
                    }
                    else if (stdChoice == 5)
                    {
                        clearScreen();
                        cout << "=== STUDENT FEE INFORMATION ===" << endl;
                        // Check Student Fee Information
                        int stdId;
                        cout << "Enter Student ID to check fee: ";
                        cin >> stdId;
                        // Add fee check logic here
                        pause();
                    }
                    else if (stdChoice == 6)
                    {
                        clearScreen();
                        cout << "=== ALL STUDENTS ===" << endl;
                        // View All Students
                        printEngine.printAllStudents(studentBST);
                        pause();
                    }
                    else if (stdChoice == 7)
                    {
                        cout << "Returning to Main Menu..." << std::endl;
                    }
                }
            }
            else if (choice == 2)
            {
                // Course Information Menu
                int courseChoice = 0;
                while (courseChoice != 6)
                {
                    clearScreen();
                    cout << "\n=== COURSE MENU ===" << std::endl;
                    cout << "1. Add Course" << std::endl;
                    cout << "2. Update Course" << std::endl;
                    cout << "3. Delete Course" << std::endl;
                    cout << "4. Search Course" << std::endl;
                    cout << "5. View All Courses" << std::endl;
                    cout << "6. Back to Main Menu" << std::endl;

                    courseChoice = getChoice(1, 6);

                    if (courseChoice == 1)
                    {
                        clearScreen();
                        cout << "=== ADD COURSE ===" << endl;
                        // Add Course
                        int courseId;
                        string courseName, teacherName;

                        cout << "Enter Course ID: ";
                        cin >> courseId;
                        cin.ignore();
                        cout << "Enter Course Title: ";
                        getline(cin, courseName);
                        cout << "Enter Teacher Name: ";
                        getline(cin, teacherName);

                        Course course(courseId, courseName, teacherName);
                        newRegCourseOpQueue.enqueueNewStd(course);
                        cout << "\nCourse Added Successfully!" << std::endl;
                        pause();
                    }
                    else if (courseChoice == 2)
                    {
                        clearScreen();
                        cout << "=== UPDATE COURSE ===" << endl;
                        // Update Course
                        int courseId;
                        cout << "Enter Course ID to update: ";
                        cin >> courseId;
                        // Add update logic here
                        pause();
                    }
                    else if (courseChoice == 3)
                    {
                        clearScreen();
                        cout << "=== DELETE COURSE ===" << endl;
                        // Delete Course
                        int courseId;
                        cout << "Enter Course ID to delete: ";
                        cin >> courseId;
                        // Add delete logic here
                        pause();
                    }
                    else if (courseChoice == 4)
                    {
                        clearScreen();
                        cout << "=== SEARCH COURSE ===" << endl;
                        // Search Course
                        int courseId;
                        cout << "Enter Course ID to search: ";
                        cin >> courseId;
                        StdNode<Course> *node = courseBST.search(courseId);
                        if (node)
                        {
                            Course course = node->getData();
                            printEngine.printCourseById(courseBST, courseId);
                        }
                        else
                        {
                            cout << "Course not found!" << std::endl;
                        }
                        pause();
                    }
                    else if (courseChoice == 5)
                    {
                        clearScreen();
                        cout << "=== ALL COURSES ===" << endl;
                        // View All Courses
                        printEngine.printAllCourses(courseBST);
                        pause();
                    }
                    else if (courseChoice == 6)
                    {
                        cout << "Returning to Main Menu..." << std::endl;
                    }
                }
            }
            else if (choice == 3)
            {
                // Admin Information Menu
                int adminChoice = 0;
                while (adminChoice != 6)
                {
                    clearScreen();
                    cout << "\n=== ADMIN MENU ===" << std::endl;
                    cout << "1. Add Admin" << std::endl;
                    cout << "2. Update Admin" << std::endl;
                    cout << "3. Delete Admin" << std::endl;
                    cout << "4. Search Admin" << std::endl;
                    cout << "5. View All Admins" << std::endl;
                    cout << "6. Back to Main Menu" << std::endl;

                    adminChoice = getChoice(1, 6);

                    if (adminChoice == 1)
                    {
                        clearScreen();
                        cout << "=== ADD ADMIN ===" << endl;
                        // Add Admin
                        int adminId;
                        string adminName, adminPassword;

                        cout << "Enter Admin ID: ";
                        cin >> adminId;
                        cin.ignore();
                        cout << "Enter Admin Name: ";
                        getline(cin, adminName);
                        cout << "Enter Admin Password: ";
                        getline(cin, adminPassword);

                        Admin admin(adminId, adminName, adminPassword);
                        newAdminOpQueue.enqueueNewStd(admin);
                        cout << "\nAdmin Added Successfully!" << std::endl;
                        pause();
                    }
                    else if (adminChoice == 2)
                    {
                        clearScreen();
                        cout << "=== UPDATE ADMIN ===" << endl;
                        // Update Admin
                        int adminId;
                        cout << "Enter Admin ID to update: ";
                        cin >> adminId;
                        // Add update logic here
                        pause();
                    }
                    else if (adminChoice == 3)
                    {
                        clearScreen();
                        cout << "=== DELETE ADMIN ===" << endl;
                        // Delete Admin
                        int adminId;
                        cout << "Enter Admin ID to delete: ";
                        cin >> adminId;
                        // Add delete logic here
                        pause();
                    }
                    else if (adminChoice == 4)
                    {
                        clearScreen();
                        cout << "=== SEARCH ADMIN ===" << endl;
                        // Search Admin
                        int adminId;
                        cout << "Enter Admin ID to search: ";
                        cin >> adminId;
                        StdNode<Admin> *node = adminBST.search(adminId);
                        if (node)
                        {
                            Admin searched = node->getData();
                            cout << "Admin Found!" << endl;
                            cout << "Admin ID: " << searched.getAdminId() << endl;
                            cout << "Admin Name: " << searched.getAdminName() << endl;
                        }
                        // Add search logic here
                        pause();
                    }
                    else if (adminChoice == 5)
                    {
                        clearScreen();
                        cout << "=== ALL ADMINS ===" << endl;
                        // View All Admins
                        printEngine.printAllAdmins(adminBST);
                        pause();
                    }
                    else if (adminChoice == 6)
                    {
                        cout << "Returning to Main Menu..." << std::endl;
                    }
                }
            }
            else if (choice == 4)
            {
                // Fee Information Menu
                int feeChoice = 0;
                while (feeChoice != 5)
                {
                    clearScreen();
                    cout << "\n=== FEE MENU ===" << std::endl;
                    cout << "1. Add Fee Record" << std::endl;
                    cout << "2. Update Fee Record" << std::endl;
                    cout << "3. Delete Fee Record" << std::endl;
                    cout << "4. View All Fee Records" << std::endl;
                    cout << "5. Back to Main Menu" << std::endl;

                    feeChoice = getChoice(1, 5);

                    if (feeChoice == 1)
                    {
                        clearScreen();
                        cout << "=== ADD FEE RECORD ===" << endl;
                        // Add Fee Record
                        cout << "Add Fee Record functionality" << std::endl;
                        // Add fee record logic here
                        pause();
                    }
                    else if (feeChoice == 2)
                    {
                        clearScreen();
                        cout << "=== UPDATE FEE RECORD ===" << endl;
                        // Update Fee Record
                        cout << "Update Fee Record functionality" << std::endl;
                        // Add update logic here
                        pause();
                    }
                    else if (feeChoice == 3)
                    {
                        clearScreen();
                        cout << "=== DELETE FEE RECORD ===" << endl;
                        // Delete Fee Record
                        cout << "Delete Fee Record functionality" << std::endl;
                        // Add delete logic here
                        pause();
                    }
                    else if (feeChoice == 4)
                    {
                        clearScreen();
                        cout << "=== ALL FEE RECORDS ===" << endl;
                        // View All Fee Records
                        printEngine.printAllStudentFees(stdFeeBST);
                        pause();
                    }
                    else if (feeChoice == 5)
                    {
                        cout << "Returning to Main Menu..." << std::endl;
                    }
                }
            }
            else if (choice == 6)
            {
                // Search System Menu
                int searchChoice = 0;
                while (searchChoice != 5)
                {
                    clearScreen();
                    cout << "\n=== SEARCH SYSTEM ===" << std::endl;
                    cout << "1. Search Student" << std::endl;
                    cout << "2. Search Course" << std::endl;
                    cout << "3. Search Admin" << std::endl;
                    cout << "4. Search Field Study" << std::endl;
                    cout << "5. Back to Main Menu" << std::endl;

                    searchChoice = getChoice(1, 5);

                    if (searchChoice == 1)
                    {
                        clearScreen();
                        cout << "=== SEARCH STUDENT ===" << endl;
                        // Search Student
                        int searchType = 0;
                        cout << "\nSearch Student by:" << endl;
                        cout << "1. By ID" << endl;
                        cout << "2. By Name" << endl;
                        cout << "Enter choice: ";
                        searchType = getChoice(1, 2);

                        if (searchType == 1)
                        {
                            int stdId;
                            cout << "Enter Student ID: ";
                            cin >> stdId;
                            StdNode<Student> *node = studentBST.search(stdId);
                            if (node)
                            {
                                printEngine.printStudentById(studentBST, stdId);
                            }
                            else
                            {
                                cout << "Student not found!" << std::endl;
                            }
                        }
                        else if (searchType == 2)
                        {
                            string stdName;
                            cin.ignore();
                            cout << "Enter Student Name: ";
                            getline(cin, stdName);
                            printEngine.printStudentByName(studentBST, stdName);
                        }
                        pause();
                    }
                    else if (searchChoice == 2)
                    {
                        clearScreen();
                        cout << "=== SEARCH COURSE ===" << endl;
                        // Search Course
                        int courseId;
                        cout << "Enter Course ID: ";
                        cin >> courseId;
                        printEngine.printCourseById(courseBST, courseId);
                        pause();
                    }
                    else if (searchChoice == 3)
                    {
                        clearScreen();
                        cout << "=== SEARCH ADMIN ===" << endl;
                        // Search Admin
                        int adminId;
                        cout << "Enter Admin ID: ";
                        cin >> adminId;
                        // Add admin search logic here
                        pause();
                    }
                    else if (searchChoice == 4)
                    {
                        clearScreen();
                        cout << "=== SEARCH FIELD STUDY ===" << endl;
                        // Search Field Study
                        int fieldId;
                        cout << "Enter Field ID: ";
                        cin >> fieldId;
                        printEngine.printFieldById(fieldBST, fieldId);
                        pause();
                    }
                    else if (searchChoice == 5)
                    {
                        cout << "Returning to Main Menu..." << std::endl;
                    }
                }
            }
            else if (choice == 7)
            {
                // Update Data Menu
                int updateChoice = 0;
                while (updateChoice != 6)
                {
                    clearScreen();
                    cout << "\n=== UPDATE DATA ===" << std::endl;
                    cout << "1. Update Student" << std::endl;
                    cout << "2. Update Course" << std::endl;
                    cout << "3. Update Admin" << std::endl;
                    cout << "4. Update Fee Record" << std::endl;
                    cout << "5. View Pending Updates" << std::endl;
                    cout << "6. Back to Main Menu" << std::endl;

                    updateChoice = getChoice(1, 6);

                    // Add update functionality here
                    if (updateChoice == 1)
                    {
                        clearScreen();
                        cout << "Update Student functionality" << std::endl;
                        pause();
                    }
                    else if (updateChoice == 2)
                    {
                        clearScreen();
                        cout << "Update Course functionality" << std::endl;
                        pause();
                    }
                    else if (updateChoice == 3)
                    {
                        clearScreen();
                        cout << "Update Admin functionality" << std::endl;
                        pause();
                    }
                    else if (updateChoice == 4)
                    {
                        clearScreen();
                        cout << "Update Fee Record functionality" << std::endl;
                        pause();
                    }
                    else if (updateChoice == 5)
                    {
                        clearScreen();
                        cout << "View Pending Updates functionality" << std::endl;
                        pause();
                    }
                    else if (updateChoice == 6)
                    {
                        cout << "Returning to Main Menu..." << std::endl;
                    }
                }
            }
            else if (choice == 8)
            {
                // Delete Data Menu
                int deleteChoice = 0;
                while (deleteChoice != 9)
                {
                    clearScreen();
                    cout << "\n=== DELETE DATA ===" << std::endl;
                    cout << "1. Delete Student Record" << std::endl;
                    cout << "2. Delete Course Record" << std::endl;
                    cout << "3. Delete Student Course Record" << std::endl;
                    cout << "4. Delete Admin Record" << std::endl;
                    cout << "5. Delete Fee Record" << std::endl;
                    cout << "6. Delete Attendance Record" << std::endl;
                    cout << "7. Delete Result Record" << std::endl;
                    cout << "8. View Deletion Queue" << std::endl;
                    cout << "9. Back to Main Menu" << std::endl;

                    deleteChoice = getChoice(1, 9);

                    switch (deleteChoice)
                    {
                    case 1: // Delete Student Record
                        clearScreen();
                        cout << "=== DELETE STUDENT RECORD ===" << std::endl;
                        deleteStudent();
                        char restartChoice;
                        cout << "\nDo you want to restart the system? (y/n): ";
                        cin >> restartChoice;
                        if (restartChoice == 'y' || restartChoice == 'Y')
                        {
                            restartSystemToDeleteData();
                        }
                        pause();
                        break;

                    case 2: // Delete Course Record
                        clearScreen();
                        cout << "=== DELETE COURSE RECORD ===" << std::endl;
                        deleteCourse();
                        cout << "\nDo you want to restart the system? (y/n): ";
                        cin >> restartChoice;
                        if (restartChoice == 'y' || restartChoice == 'Y')
                        {
                            restartSystemToDeleteData();
                        }
                        pause();
                        break;

                    case 3: // Delete Student Course Record
                        clearScreen();
                        cout << "=== DELETE STUDENT COURSE ENROLLMENT ===" << std::endl;
                        // deleteStudentCourse();
                        cout << "\nDo you want to restart the system? (y/n): ";
                        cin >> restartChoice;
                        if (restartChoice == 'y' || restartChoice == 'Y')
                        {
                            restartSystemToDeleteData();
                        }
                        pause();
                        break;

                    case 4: // Delete Admin Record
                        clearScreen();
                        cout << "=== DELETE ADMIN RECORD ===" << std::endl;
                        deleteAdmin();
                        cout << "\nDo you want to restart the system? (y/n): ";
                        cin >> restartChoice;
                        if (restartChoice == 'y' || restartChoice == 'Y')
                        {
                            restartSystemToDeleteData();
                        }
                        pause();
                        break;

                    case 5: // Delete Fee Record
                        clearScreen();
                        cout << "=== DELETE FEE RECORD ===" << std::endl;
                        deleteStdFee();
                        cout << "\nDo you want to restart the system? (y/n): ";
                        cin >> restartChoice;
                        if (restartChoice == 'y' || restartChoice == 'Y')
                        {
                            restartSystemToDeleteData();
                        }
                        pause();
                        break;

                    case 6: // Delete Attendance Record
                        clearScreen();
                        cout << "=== DELETE ATTENDANCE RECORD ===" << std::endl;
                        deleteAttendance();
                        cout << "\nDo you want to restart the system? (y/n): ";
                        cin >> restartChoice;
                        if (restartChoice == 'y' || restartChoice == 'Y')
                        {
                            restartSystemToDeleteData();
                        }
                        pause();
                        break;

                    case 7: // Delete Result Record
                        clearScreen();
                        cout << "=== DELETE RESULT RECORD ===" << std::endl;
                        deleteResult();
                        cout << "\nDo you want to restart the system? (y/n): ";
                        cin >> restartChoice;
                        if (restartChoice == 'y' || restartChoice == 'Y')
                        {
                            restartSystemToDeleteData();
                        }
                        pause();
                        break;

                    case 8: // View Deletion Queue
                        clearScreen();
                        cout << "=== DELETION QUEUE ===" << std::endl;
                        // viewDeletionQueue();
                        pause();
                        break;

                    case 9: // Back to Main Menu
                        cout << "Returning to Main Menu..." << std::endl;
                        break;

                    default:
                        cout << "Invalid choice! Please try again." << std::endl;
                        pause();
                    }
                }
            }
            else if (choice == 9)
            {
                clearScreen();
                // Print All Data
                std::cout << "\n======================================" << std::endl;
                std::cout << "        PRINTING ALL DATA" << std::endl;
                std::cout << "======================================" << std::endl;
                int printChoice = 0;
                while (printChoice != 9)
                {
                    cout << "1 For Print all Students\n";
                    cout << "2 For Print all Courses\n";
                    cout << "3 For Print all Fields\n";
                    cout << "4 For Print all Fee Records\n";
                    cout << "5 For Print all Attendance\n";
                    cout << "6 For Print all Student Courses\n";
                    cout << "7 For Print all Results\n";
                    cout << "8 For Print all Admins\n";
                    cout << "9 Return Back to Main Menu\n";
                    printChoice = getChoice(1, 9);
                    if (printChoice == 1)
                    {
                        clearScreen();
                        std::cout << "\n=== All Students ===" << std::endl;
                        printEngine.printAllStudents(studentBST);
                        pause();
                    }
                    else if (printChoice == 2)
                    {
                        clearScreen();
                        std::cout << "\n=== All Courses ===" << std::endl;
                        printEngine.printAllCourses(courseBST);
                        pause();
                    }
                    else if (printChoice == 3)
                    {
                        clearScreen();
                        std::cout << "\n=== All Fields ===" << std::endl;
                        printEngine.printAllFields(fieldBST);
                        pause();
                    }
                    else if (printChoice == 4)
                    {
                        std::cout << "\n=== All Fee Records ===" << std::endl;
                        printEngine.printAllStudentFees(stdFeeBST);
                    }
                    else if (printChoice == 5)
                    {
                        std::cout << "\n=== All Attendance ===" << std::endl;
                        printEngine.printAllAttendance(attendanceBST);
                    }
                    else if (printChoice == 6)
                    {
                        std::cout << "\n=== All Student Courses ===" << std::endl;
                        printEngine.printAllStdCourses(stdCourseBST);
                    }
                    else if (printChoice == 7)
                    {
                        std::cout << "\n=== All Results ===" << std::endl;
                        printEngine.printAllResults(stdResultBST);
                    }
                    else if (printChoice == 8)
                    {
                        std::cout << "\n=== All Admins ===" << std::endl;
                        printEngine.printAllAdmins(adminBST);
                    }
                    else if (printChoice == 9)
                    {
                        cout << "Return Back to Main \n";
                    }
                }

                pause();
            }
            else if (choice == 10)
            {
                clearScreen();
                cout << "\n === Insertion Operations ===\n"
                     << std::endl;
                cin.ignore();
                UniVersalInsertionMethod();
                cout << "Search New insrtion Informainon \n";
                int newInsetIonChoich = 0;
                while (newInsetIonChoich != 9)
                {
                    cout << "1 For Search New Inseted Students\n";
                    cout << "2 For Search New Inseted Courses\n";
                    cout << "3 For Search New Inseted Fields\n";
                    cout << "4 For Search New Inseted Fee Records\n";
                    cout << "5 For Search New Inseted Attendance\n";
                    cout << "6 For Search New Inseted Student Courses\n";
                    cout << "7 For Search New Inseted Results\n";
                    cout << "8 For Search New Inseted Admins\n";
                    cout << "9 Return Back to Main Menu\n";
                    newInsetIonChoich = getChoice(1, 9);
                    if (newInsetIonChoich == 1)
                    {
                        std::cout << "\n=== Search New Inserted Students ===" << std::endl;
                        int stdId;
                        cout << "Enter New Registrion Student Id \n";
                        cin >> stdId;
                        cin.ignore();
                        StdNode<Student> *node = studentBST.search(stdId);
                        if (node)
                        {
                            Student std = node->getData();
                            std.print();
                        }
                    }
                    else if (newInsetIonChoich == 2)
                    {
                        std::cout << "\n=== Search New Inserted Courses ===" << std::endl;
                        int courseId;
                        cout << "Enter New Registrion Course Id \n";
                        cin >> courseId;
                        cin.ignore();
                        StdNode<Course> *node = courseBST.search(courseId);
                        if (node)
                        {
                            Course course = node->getData();
                            course.print();
                        }
                    }
                    else if (newInsetIonChoich == 3)
                    {
                        std::cout << "\n=== Search New Inserted Fields ===" << std::endl;
                        int fieldId;
                        cout << "Enter New Registrion Field Id \n";
                        cin >> fieldId;
                        cin.ignore();
                        StdNode<FieldStudy> *node = fieldBST.search(fieldId);
                        if (node)
                        {
                            FieldStudy field = node->getData();
                            field.print();
                        }
                    }
                    else if (newInsetIonChoich == 4)
                    {
                        std::cout << "\n=== Search New Inserted Fee Records ===" << std::endl;
                        int stdId;
                        cout << "Enter New Registrion Student Id \n";
                        cin >> stdId;
                        cin.ignore();
                        StdNode<StudentFees> *node = stdFeeBST.search(stdId);
                        if (node)
                        {
                            StudentFees fees = node->getData();
                            fees.print();
                        }
                    }
                    else if (newInsetIonChoich == 5)
                    {
                        std::cout << "\n=== Search New Inserted Attendance ===" << std::endl;
                        int stdId;
                        cout << "Enter New Registrion Student Id \n";
                        cin >> stdId;
                        cin.ignore();
                        StdNode<Attendance> *node = attendanceBST.search(stdId);
                        if (node)
                        {
                            Attendance att = node->getData();
                            att.print();
                        }
                    }
                    else if (newInsetIonChoich == 6)
                    {
                        std::cout << "\n=== Search New Inserted Student Courses ===" << std::endl;
                        int stdId;
                        cout << "Enter New Registrion Student Id \n";
                        cin >> stdId;
                        cin.ignore();
                        StdNode<StdCourse> *node = stdCourseBST.search(stdId);
                        if (node)
                        {
                            StdCourse course = node->getData();
                            course.print();
                        }
                    }
                    else if (newInsetIonChoich == 7)
                    {
                        std::cout << "\n=== Search New Inserted Results ===" << std::endl;
                        int stdId;
                        cout << "Enter New Registrion Student Id \n";
                        cin >> stdId;
                        cin.ignore();
                        StdNode<Result> *node = stdResultBST.search(stdId);
                        if (node)
                        {
                            Result result = node->getData();
                            result.print();
                        }
                    }
                    else if (newInsetIonChoich == 8)
                    {
                        std::cout << "\n=== Search New Inserted Admin ===" << std::endl;
                        int stdId;
                        cout << "Enter New Registrion Admin Id \n";
                        cin >> stdId;
                        cin.ignore();
                        StdNode<Admin> *node = adminBST.search(stdId);
                        if (node)
                        {
                            Admin admin = node->getData();
                            admin.print();
                        }
                    }
                }
            }

            // Process any pending updates before exiting

            cout << "\nThank you for using Student Management System!" << endl;
            pause();
        }
        db.printStatus();
        loadAllDataFromDB();
        restartSystemToDeleteData();
        universalUPdation(updateQueue);
        UniVersalInsertionMethod();
    }
    // // Public getter methods
    // BST<Student> &getStudentBST()
    // {
    //     return studentBST;
    // }
    // BST<Admin> &getAdminBST() { return adminBST; }
    // BST<Course> &getCourseBST() { return courseBST; }
    // BST<FieldStudy> &getFieldBST() { return fieldBST; }
    // BST<Attendance> &getAttendanceBST() { return attendanceBST; }
    // BST<StdCourse> &getStdCourseBST() { return stdCourseBST; }
    // BST<StudentFees> &getStdFeeBST() { return stdFeeBST; }
    // BST<Result> &getStdResultBST() { return stdResultBST; }

    // DB &getDB() { return db; }
    // PrintEngine &getPrintEngine() { return printEngine; }
};

#endif // SMSSYSTEM_H