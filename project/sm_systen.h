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
    void universalUPdation(upQueue &que)
    {
        // Use the existing database connection from db object
        if (!db.isConnected())
        {
            cout << "Database is not connected. Cannot process updates." << endl;
            return;
        }

        cout << "\nStarting update processing..." << endl;

        // Implementation for updating student data
        while (!que.isEmpty())
        {
            upNode *nod = que.dequeue();
            if (!nod)
            {
                cerr << "Error: Null node dequeued" << endl;
                continue;
            }

            pair<int, string> meta = nod->getMetaData();
            int id = meta.first;
            string table = meta.second;

            cout << "\n--------------------------------------" << endl;
            cout << "Processing: " << table << " (ID: " << id << ")" << endl;
            cout << "--------------------------------------" << endl;

            if (table == "student")
            {
                int choice;
                cout << "\nSelect field to update:" << endl;
                cout << "1. stdName" << endl;
                cout << "2. stdFatherName" << endl;
                cout << "3. stdUserName" << endl;
                cout << "4. stdAge" << endl;
                cout << "5. fieldId" << endl;
                cout << "6. stdId (Update ID itself)" << endl;
                cout << "Enter choice: ";
                cin >> choice;
                cin.ignore();

                string newValue;
                string sqlQuery;
                bool validChoice = true;

                if (choice == 1)
                {
                    cout << "Enter new stdName: ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE student SET stdName = ? WHERE stdId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else if (choice == 2)
                {
                    cout << "Enter new stdFatherName: ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE student SET stdFatherName = ? WHERE stdId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else if (choice == 3)
                {
                    cout << "Enter new stdUserName: ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE student SET stdUserName = ? WHERE stdId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else if (choice == 4)
                {
                    cout << "Enter new stdAge: ";
                    getline(cin, newValue);
                    int age = stoi(newValue);
                    sqlQuery = "UPDATE student SET stdAge = ? WHERE stdId = ?";
                    db.executeUpdate(sqlQuery, age, id);
                }
                else if (choice == 5)
                {
                    cout << "Enter new fieldId: ";
                    getline(cin, newValue);
                    int fieldId = stoi(newValue);
                    sqlQuery = "UPDATE student SET fieldId = ? WHERE stdId = ?";
                    db.executeUpdate(sqlQuery, fieldId, id);
                }
                else if (choice == 6)
                {
                    cout << "Enter new stdId: ";
                    getline(cin, newValue);
                    int newId = stoi(newValue);
                    sqlQuery = "UPDATE student SET stdId = ? WHERE stdId = ?";
                    db.executeUpdate(sqlQuery, newId, id);
                }
                else
                {
                    cout << "Invalid choice! Skipping this update." << endl;
                    validChoice = false;
                }

                if (validChoice)
                {
                    cout << "✓ Student update processed for ID: " << id << endl;
                }
            }
            else if (table == "adminTab")
            {
                int choice;
                cout << "\nSelect field to update:" << endl;
                cout << "1. adminName" << endl;
                cout << "2. passWord" << endl;
                cout << "Enter choice: ";
                cin >> choice;
                cin.ignore();

                string newValue;
                string sqlQuery;
                bool validChoice = true;

                if (choice == 1)
                {
                    cout << "Enter new adminName: ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE adminTab SET adminName = ? WHERE adminId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else if (choice == 2)
                {
                    cout << "Enter new passWord: ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE adminTab SET passWord = ? WHERE adminId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else
                {
                    cout << "Invalid choice! Skipping this update." << endl;
                    validChoice = false;
                }

                if (validChoice)
                {
                    cout << "✓ Admin update processed for ID: " << id << endl;
                }
            }
            else if (table == "StudentFees")
            {
                int choice;
                cout << "\nSelect field to update:" << endl;
                cout << "1. amount" << endl;
                cout << "2. status" << endl;
                cout << "3. paymentDate" << endl;
                cout << "4. stdId" << endl;
                cout << "Enter choice: ";
                cin >> choice;
                cin.ignore();

                string newValue;
                string sqlQuery;
                bool validChoice = true;

                if (choice == 1)
                {
                    cout << "Enter new amount: ";
                    getline(cin, newValue);
                    double amount = stod(newValue);
                    sqlQuery = "UPDATE StudentFees SET amount = ? WHERE feeId = ?";
                    db.executeUpdate(sqlQuery, amount, id);
                }
                else if (choice == 2)
                {
                    cout << "Enter new status (Paid/Pending): ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE StudentFees SET status = ? WHERE feeId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else if (choice == 3)
                {
                    cout << "Enter new paymentDate (YYYY-MM-DD): ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE StudentFees SET paymentDate = ? WHERE feeId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else if (choice == 4)
                {
                    cout << "Enter new stdId: ";
                    getline(cin, newValue);
                    int stdId = stoi(newValue);
                    sqlQuery = "UPDATE StudentFees SET stdId = ? WHERE feeId = ?";
                    db.executeUpdate(sqlQuery, stdId, id);
                }
                else
                {
                    cout << "Invalid choice! Skipping this update." << endl;
                    validChoice = false;
                }

                if (validChoice)
                {
                    cout << "✓ StudentFees update processed for ID: " << id << endl;
                }
            }
            else if (table == "Attendance")
            {
                int choice;
                cout << "\nSelect field to update:" << endl;
                cout << "1. isPresent (1=Present, 0=Absent)" << endl;
                cout << "2. attendanceDate" << endl;
                cout << "3. stdId" << endl;
                cout << "4. courseId" << endl;
                cout << "Enter choice: ";
                cin >> choice;
                cin.ignore();

                string newValue;
                string sqlQuery;
                bool validChoice = true;

                if (choice == 1)
                {
                    cout << "Enter new isPresent (1 for Present, 0 for Absent): ";
                    getline(cin, newValue);
                    int isPresent = stoi(newValue);
                    sqlQuery = "UPDATE Attendance SET isPresent = ? WHERE attendanceId = ?";
                    db.executeUpdate(sqlQuery, isPresent, id);
                }
                else if (choice == 2)
                {
                    cout << "Enter new attendanceDate (YYYY-MM-DD): ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE Attendance SET attendanceDate = ? WHERE attendanceId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else if (choice == 3)
                {
                    cout << "Enter new stdId: ";
                    getline(cin, newValue);
                    int stdId = stoi(newValue);
                    sqlQuery = "UPDATE Attendance SET stdId = ? WHERE attendanceId = ?";
                    db.executeUpdate(sqlQuery, stdId, id);
                }
                else if (choice == 4)
                {
                    cout << "Enter new courseId: ";
                    getline(cin, newValue);
                    int courseId = stoi(newValue);
                    sqlQuery = "UPDATE Attendance SET courseId = ? WHERE attendanceId = ?";
                    db.executeUpdate(sqlQuery, courseId, id);
                }
                else
                {
                    cout << "Invalid choice! Skipping this update." << endl;
                    validChoice = false;
                }

                if (validChoice)
                {
                    cout << "✓ Attendance update processed for ID: " << id << endl;
                }
            }
            else if (table == "fieldStudy")
            {
                int choice;
                cout << "\nSelect field to update:" << endl;
                cout << "1. fieldName" << endl;
                cout << "Enter choice: ";
                cin >> choice;
                cin.ignore();

                string newValue;
                string sqlQuery;
                bool validChoice = true;

                if (choice == 1)
                {
                    cout << "Enter new fieldName: ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE fieldStudy SET fieldName = ? WHERE fieldId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else
                {
                    cout << "Invalid choice! Skipping this update." << endl;
                    validChoice = false;
                }

                if (validChoice)
                {
                    cout << "✓ FieldStudy update processed for ID: " << id << endl;
                }
            }
            else if (table == "course")
            {
                int choice;
                cout << "\nSelect field to update:" << endl;
                cout << "1. courseTitle" << endl;
                cout << "2. teacherName" << endl;
                cout << "3. courseId (Update ID itself)" << endl;
                cout << "Enter choice: ";
                cin >> choice;
                cin.ignore();

                string newValue;
                string sqlQuery;
                bool validChoice = true;

                if (choice == 1)
                {
                    cout << "Enter new courseTitle: ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE course SET courseTitle = ? WHERE courseId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else if (choice == 2)
                {
                    cout << "Enter new teacherName: ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE course SET teacherName = ? WHERE courseId = ?";
                    db.executeUpdate(sqlQuery, newValue, id);
                }
                else if (choice == 3)
                {
                    cout << "Enter new courseId: ";
                    getline(cin, newValue);
                    int newId = stoi(newValue);
                    sqlQuery = "UPDATE course SET courseId = ? WHERE courseId = ?";
                    db.executeUpdate(sqlQuery, newId, id);
                }
                else
                {
                    cout << "Invalid choice! Skipping this update." << endl;
                    validChoice = false;
                }

                if (validChoice)
                {
                    cout << "✓ Course update processed for ID: " << id << endl;
                }
            }
            else if (table == "result")
            {
                int currentCourseId;
                cout << "Enter current courseId for this result: ";
                cin >> currentCourseId;
                cin.ignore();

                int choice;
                cout << "\nSelect field to update:" << endl;
                cout << "1. gotNumber" << endl;
                cout << "2. grade" << endl;
                cout << "3. courseId" << endl;
                cout << "Enter choice: ";
                cin >> choice;
                cin.ignore();

                string newValue;
                string sqlQuery;
                bool validChoice = true;

                if (choice == 1)
                {
                    cout << "Enter new gotNumber: ";
                    getline(cin, newValue);
                    double gotNumber = stod(newValue);
                    sqlQuery = "UPDATE result SET gotNumber = ? WHERE stdId = ? AND courseId = ?";
                    db.executeUpdate(sqlQuery, gotNumber, id, currentCourseId);
                }
                else if (choice == 2)
                {
                    cout << "Enter new grade: ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE result SET grade = ? WHERE stdId = ? AND courseId = ?";
                    db.executeUpdate(sqlQuery, newValue, id, currentCourseId);
                }
                else if (choice == 3)
                {
                    cout << "Enter new courseId: ";
                    getline(cin, newValue);
                    int newCourseId = stoi(newValue);
                    sqlQuery = "UPDATE result SET courseId = ? WHERE stdId = ? AND courseId = ?";
                    db.executeUpdate(sqlQuery, newCourseId, id, currentCourseId);
                }
                else
                {
                    cout << "Invalid choice! Skipping this update." << endl;
                    validChoice = false;
                }

                if (validChoice)
                {
                    cout << "✓ Result update processed for Student ID: " << id << endl;
                }
            }
            else if (table == "courseRegStd")
            {
                int currentCourseId;
                cout << "Enter current courseId for this registration: ";
                cin >> currentCourseId;
                cin.ignore();

                int choice;
                cout << "\nSelect field to update:" << endl;
                cout << "1. regDate" << endl;
                cout << "2. courseId" << endl;
                cout << "Enter choice: ";
                cin >> choice;
                cin.ignore();

                string newValue;
                string sqlQuery;
                bool validChoice = true;

                if (choice == 1)
                {
                    cout << "Enter new regDate (YYYY-MM-DD HH:MM:SS): ";
                    getline(cin, newValue);
                    sqlQuery = "UPDATE courseRegStd SET regDate = ? WHERE stdId = ? AND courseId = ?";
                    db.executeUpdate(sqlQuery, newValue, id, currentCourseId);
                }
                else if (choice == 2)
                {
                    cout << "Enter new courseId: ";
                    getline(cin, newValue);
                    int newCourseId = stoi(newValue);
                    sqlQuery = "UPDATE courseRegStd SET courseId = ? WHERE stdId = ? AND courseId = ?";
                    db.executeUpdate(sqlQuery, newCourseId, id, currentCourseId);
                }
                else
                {
                    cout << "Invalid choice! Skipping this update." << endl;
                    validChoice = false;
                }

                if (validChoice)
                {
                    cout << "✓ Course registration update processed for Student ID: " << id << endl;
                }
            }
            else
            {
                cout << "Unknown table: " << table << ". Skipping this update." << endl;
            }

            // Delete the node after processing
            delete nod;

            cout << "--------------------------------------" << endl;
        }

        cout << "\n======================================" << endl;
        cout << "All updates processed successfully!" << endl;
        cout << "======================================" << endl;
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

        // Create an update queue
        upQueue updateQueue;

        int mainChoice = 0;
        while (mainChoice != 3)
        {
            cout << "\n======================================" << endl;
            cout << "        MAIN MENU" << endl;
            cout << "======================================" << endl;
            cout << "1. Update System Records" << endl;
            cout << "2. Process Update Queue (" << updateQueue.size() << " pending)" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> mainChoice;
            cin.ignore();

            switch (mainChoice)
            {
            case 1: // Update System Records
            {
                cout << "\n======================================" << endl;
                cout << "    SELECT TABLE TO UPDATE" << endl;
                cout << "======================================" << endl;
                cout << "1. student" << endl;
                cout << "2. adminTab" << endl;
                cout << "3. StudentFees" << endl;
                cout << "4. Attendance" << endl;
                cout << "5. fieldStudy" << endl;
                cout << "6. course" << endl;
                cout << "7. result" << endl;
                cout << "8. courseRegStd" << endl;
                cout << "Enter table number: ";

                int tableChoice;
                cin >> tableChoice;
                cin.ignore();

                string tableName;
                switch (tableChoice)
                {
                case 1:
                    tableName = "student";
                    break;
                case 2:
                    tableName = "adminTab";
                    break;
                case 3:
                    tableName = "StudentFees";
                    break;
                case 4:
                    tableName = "Attendance";
                    break;
                case 5:
                    tableName = "fieldStudy";
                    break;
                case 6:
                    tableName = "course";
                    break;
                case 7:
                    tableName = "result";
                    break;
                case 8:
                    tableName = "courseRegStd";
                    break;
                default:
                    cout << "Invalid choice! Returning to main menu." << endl;
                    continue;
                }

                int recordId;
                cout << "Enter record ID for " << tableName << ": ";
                cin >> recordId;
                cin.ignore();

                // Create and add to queue
                upNode *updateNode = new upNode(recordId, tableName);
                updateQueue.enqueue(updateNode);

                cout << "\n✓ Update request added to queue!" << endl;
                cout << "Table: " << tableName << ", ID: " << recordId << endl;

                // Ask if user wants to add more
                char more;
                cout << "\nAdd another update request? (y/n): ";
                cin >> more;
                cin.ignore();

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
                    cout << "\nUpdate queue is empty. Nothing to process." << endl;
                }
                else
                {
                    cout << "\n======================================" << endl;
                    cout << "    PROCESSING UPDATE QUEUE" << endl;
                    cout << "======================================" << endl;
                    cout << "Queue size: " << updateQueue.size() << " requests" << endl;

                    cout << "\nProcessing update requests..." << endl;
                    universalUPdation(updateQueue);
                }
                break;
            }

            case 3: // Exit
            {
                // Clean up any remaining nodes in the queue
                while (!updateQueue.isEmpty())
                {
                    upNode *node = updateQueue.dequeue();
                    delete node;
                }

                cout << "\nExiting Student Management System..." << endl;
                break;
            }

            default:
                cout << "Invalid choice! Please try again." << endl;
            }
        }

        // Display admin menu at the end
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