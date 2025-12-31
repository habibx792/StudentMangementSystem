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
#include "deleteQueueS.h"

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
class SMSSystem
{
private:
    DB db;
    UI ui;
    BST<Student> studentBST;
    BST<Admin> adminBST;
    BST<Student> newRegStdBst;
    BST<Course> courseBST;
    BST<FieldStudy> fieldBST;
    BST<Attendance> attendanceBST;
    BST<StdCourse> stdCourseBST;
    BST<StudentFees> stdFeeBST;
    BST<Result> stdResultBST;

    opQueue<Student> newStudentOpQueue;
    opQueue<Course> newRegCourseOpQueue;
    opQueue<FieldStudy> newFieldOpQueue;
    opQueue<Attendance> newStdAttendanceOpQueue;
    opQueue<StdCourse> newStdCourseOpQueue;
    opQueue<StudentFees> newStdFeeOpQueue;
    opQueue<Result> newStdResultOpQueue;

    upQueue updateQueue;
    deleteQueue delQueue;

    // Private constructor for Singleton pattern
    SMSSystem() = default;

    // Delete copy constructor and assignment operator
    SMSSystem(const SMSSystem &) = delete;
    SMSSystem &operator=(const SMSSystem &) = delete;

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
    }

    void registerNewCourse(Course &course)
    {
        newRegCourseOpQueue.enqueueNewStd(course);
        courseBST.insert(course);
    }

    void registerNewField(FieldStudy &field)
    {
        newFieldOpQueue.enqueueNewStd(field);
        fieldBST.insert(field);
    }

    void registerNewAttendance(Attendance &attendance)
    {
        newStdAttendanceOpQueue.enqueueNewStd(attendance);
        attendanceBST.insert(attendance);
    }

    void registerNewStdCourse(StdCourse &stdCourse)
    {
        newStdCourseOpQueue.enqueueNewStd(stdCourse);
        stdCourseBST.insert(stdCourse);
    }

    void registerNewStudentFees(StudentFees &stdFee)
    {
        newStdFeeOpQueue.enqueueNewStd(stdFee);
        stdFeeBST.insert(stdFee);
    }

    void registerNewResult(Result &stdResult)
    {
        newStdResultOpQueue.enqueueNewStd(stdResult);
        stdResultBST.insert(stdResult);
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
        ui.displayAdminMenu();
    }

    // Public getter methods for accessing components
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

    // Public queue operation methods
    void updateStudent(upNode *node) { updateStudentInfo(node); }
    void deleteStudent(deleteNode *node) { deleteStudentInformation(node); }

    void addNewStudent(Student &student) { registerNewStudent(student); }
    void addNewCourse(Course &course) { registerNewCourse(course); }
    void addNewField(FieldStudy &field) { registerNewField(field); }
    void addNewAttendance(Attendance &attendance) { registerNewAttendance(attendance); }
    void addNewStdCourse(StdCourse &stdCourse) { registerNewStdCourse(stdCourse); }
    void addNewStudentFees(StudentFees &stdFee) { registerNewStudentFees(stdFee); }
    void addNewResult(Result &stdResult) { registerNewResult(stdResult); }
};

#endif // SMSSYSTEM_H