#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>
#include <memory>
#include <mutex>
#include <thread>
#include <sstream>
#include <iomanip>

using namespace std;

// Error checking function
void checkError(SQLRETURN ret, SQLHANDLE handle, SQLSMALLINT type, const string& msg) {
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return;

    SQLCHAR state[6], text[256];
    SQLINTEGER native;
    SQLSMALLINT len;

    SQLGetDiagRec(type, handle, 1, state, &native, text, sizeof(text), &len);
    cerr << msg << "\nSQLSTATE: " << state << "\nERROR: " << text << "\n";
    exit(1);
}

// Forward declarations
class OperationQueue;
class DatabaseManager;
class SystemManager;

// BST Node for Student
class StudentNode {
public:
    int stdId;
    string stdName;
    string stdUserName;
    int stdAge;
    int fieldId;
    string stdFatherName;
    bool modified;
    bool markedForDelete;
    
    StudentNode* left;
    StudentNode* right;
    
    StudentNode(int id, const string& name, const string& username, int age, 
                int field, const string& father)
        : stdId(id), stdName(name), stdUserName(username), stdAge(age),
          fieldId(field), stdFatherName(father), left(nullptr), right(nullptr),
          modified(false), markedForDelete(false) {}
    
    ~StudentNode() {
        delete left;
        delete right;
    }
    
    void display() const {
        cout << "ID: " << stdId 
             << " | Name: " << stdName 
             << " | Username: " << stdUserName
             << " | Age: " << stdAge
             << " | Field ID: " << fieldId
             << " | Father: " << stdFatherName << endl;
    }
};

// BST for Students
class StudentBST {
private:
    StudentNode* root;
    mutex bst_mutex;
    
    StudentNode* insertHelper(StudentNode* node, StudentNode* new_node) {
        if (!node) return new_node;
        
        if (new_node->stdId < node->stdId) {
            node->left = insertHelper(node->left, new_node);
        } else if (new_node->stdId > node->stdId) {
            node->right = insertHelper(node->right, new_node);
        }
        return node;
    }
    
    StudentNode* searchHelper(StudentNode* node, int id) {
        if (!node || node->stdId == id) return node;
        
        if (id < node->stdId) {
            return searchHelper(node->left, id);
        } else {
            return searchHelper(node->right, id);
        }
    }
    
    StudentNode* findMin(StudentNode* node) {
        while (node && node->left) node = node->left;
        return node;
    }
    
    StudentNode* deleteHelper(StudentNode* node, int id) {
        if (!node) return nullptr;
        
        if (id < node->stdId) {
            node->left = deleteHelper(node->left, id);
        } else if (id > node->stdId) {
            node->right = deleteHelper(node->right, id);
        } else {
            if (!node->left) {
                StudentNode* temp = node->right;
                node->right = nullptr;
                delete node;
                return temp;
            } else if (!node->right) {
                StudentNode* temp = node->left;
                node->left = nullptr;
                delete node;
                return temp;
            }
            
            StudentNode* temp = findMin(node->right);
            node->stdId = temp->stdId;
            node->stdName = temp->stdName;
            node->stdUserName = temp->stdUserName;
            node->stdAge = temp->stdAge;
            node->fieldId = temp->fieldId;
            node->stdFatherName = temp->stdFatherName;
            node->right = deleteHelper(node->right, temp->stdId);
        }
        return node;
    }
    
    void inorderHelper(StudentNode* node, vector<StudentNode*>& result) {
        if (!node) return;
        inorderHelper(node->left, result);
        result.push_back(node);
        inorderHelper(node->right, result);
    }
    
    StudentNode* searchByUsernameHelper(StudentNode* node, const string& username) {
        if (!node) return nullptr;
        
        if (node->stdUserName == username) return node;
        
        StudentNode* leftResult = searchByUsernameHelper(node->left, username);
        if (leftResult) return leftResult;
        
        return searchByUsernameHelper(node->right, username);
    }
    
public:
    StudentBST() : root(nullptr) {}
    ~StudentBST() { delete root; }
    
    StudentNode* insert(int id, const string& name, const string& username, 
                       int age, int field, const string& father) {
        lock_guard<mutex> lock(bst_mutex);
        StudentNode* new_node = new StudentNode(id, name, username, age, field, father);
        new_node->modified = true;
        root = insertHelper(root, new_node);
        return new_node;
    }
    
    StudentNode* search(int id) {
        lock_guard<mutex> lock(bst_mutex);
        return searchHelper(root, id);
    }
    
    StudentNode* searchByUsername(const string& username) {
        lock_guard<mutex> lock(bst_mutex);
        return searchByUsernameHelper(root, username);
    }
    
    bool update(int id, const string& name, const string& username, 
                int age, int field, const string& father) {
        lock_guard<mutex> lock(bst_mutex);
        StudentNode* node = searchHelper(root, id);
        if (node) {
            node->stdName = name;
            node->stdUserName = username;
            node->stdAge = age;
            node->fieldId = field;
            node->stdFatherName = father;
            node->modified = true;
            return true;
        }
        return false;
    }
    
    bool markForDeletion(int id) {
        lock_guard<mutex> lock(bst_mutex);
        StudentNode* node = searchHelper(root, id);
        if (node) {
            node->markedForDelete = true;
            node->modified = true;
            return true;
        }
        return false;
    }
    
    bool remove(int id) {
        lock_guard<mutex> lock(bst_mutex);
        root = deleteHelper(root, id);
        return true;
    }
    
    vector<StudentNode*> getAllStudents() {
        lock_guard<mutex> lock(bst_mutex);
        vector<StudentNode*> students;
        inorderHelper(root, students);
        return students;
    }
    
    vector<StudentNode*> searchByField(int fieldId) {
        lock_guard<mutex> lock(bst_mutex);
        vector<StudentNode*> students;
        vector<StudentNode*> all = getAllStudents();
        for (auto node : all) {
            if (node->fieldId == fieldId) {
                students.push_back(node);
            }
        }
        return students;
    }
    
    void displayAll() {
        vector<StudentNode*> students = getAllStudents();
        cout << "\n=== All Students (" << students.size() << ") ===" << endl;
        cout << "ID\tName\t\tUsername\tAge\tField\tFather" << endl;
        cout << "----------------------------------------------------------------" << endl;
        for (auto student : students) {
            cout << student->stdId << "\t" 
                 << student->stdName << "\t\t"
                 << student->stdUserName << "\t\t"
                 << student->stdAge << "\t"
                 << student->fieldId << "\t"
                 << student->stdFatherName << endl;
        }
    }
};

// Course Node
class CourseNode {
public:
    int courseId;
    string courseTitle;
    string teacherName;
    bool modified;
    bool markedForDelete;
    
    CourseNode(int id, const string& title, const string& teacher)
        : courseId(id), courseTitle(title), teacherName(teacher),
          modified(false), markedForDelete(false) {}
    
    void display() const {
        cout << "Course ID: " << courseId 
             << " | Title: " << courseTitle
             << " | Teacher: " << teacherName << endl;
    }
};

// Fee Node
class FeeNode {
public:
    int feeId;
    int stdId;
    double amount;
    string paymentDate;
    string status;
    bool modified;
    bool markedForDelete;
    
    FeeNode(int id, int studentId, double amt, const string& date, const string& stat)
        : feeId(id), stdId(studentId), amount(amt), paymentDate(date), status(stat),
          modified(false), markedForDelete(false) {}
    
    void display() const {
        cout << "Fee ID: " << feeId 
             << " | Student ID: " << stdId
             << " | Amount: " << amount
             << " | Date: " << paymentDate
             << " | Status: " << status << endl;
    }
};

// Operation Queues
class OperationQueue {
private:
    queue<StudentNode*> studentInsertQueue;
    queue<StudentNode*> studentUpdateQueue;
    queue<StudentNode*> studentDeleteQueue;
    
    queue<CourseNode*> courseInsertQueue;
    queue<CourseNode*> courseUpdateQueue;
    queue<CourseNode*> courseDeleteQueue;
    
    queue<FeeNode*> feeInsertQueue;
    queue<FeeNode*> feeUpdateQueue;
    queue<FeeNode*> feeDeleteQueue;
    
    mutex queue_mutex;
    
public:
    // Student operations
    void addStudentInsert(StudentNode* node) {
        lock_guard<mutex> lock(queue_mutex);
        studentInsertQueue.push(node);
    }
    
    void addStudentUpdate(StudentNode* node) {
        lock_guard<mutex> lock(queue_mutex);
        studentUpdateQueue.push(node);
    }
    
    void addStudentDelete(StudentNode* node) {
        lock_guard<mutex> lock(queue_mutex);
        studentDeleteQueue.push(node);
    }
    
    // Course operations
    void addCourseInsert(CourseNode* node) {
        lock_guard<mutex> lock(queue_mutex);
        courseInsertQueue.push(node);
    }
    
    void addCourseUpdate(CourseNode* node) {
        lock_guard<mutex> lock(queue_mutex);
        courseUpdateQueue.push(node);
    }
    
    void addCourseDelete(CourseNode* node) {
        lock_guard<mutex> lock(queue_mutex);
        courseDeleteQueue.push(node);
    }
    
    // Fee operations
    void addFeeInsert(FeeNode* node) {
        lock_guard<mutex> lock(queue_mutex);
        feeInsertQueue.push(node);
    }
    
    void addFeeUpdate(FeeNode* node) {
        lock_guard<mutex> lock(queue_mutex);
        feeUpdateQueue.push(node);
    }
    
    void addFeeDelete(FeeNode* node) {
        lock_guard<mutex> lock(queue_mutex);
        feeDeleteQueue.push(node);
    }
    
    // Get all operations for processing
    vector<StudentNode*> getStudentInserts() {
        lock_guard<mutex> lock(queue_mutex);
        vector<StudentNode*> result;
        while (!studentInsertQueue.empty()) {
            result.push_back(studentInsertQueue.front());
            studentInsertQueue.pop();
        }
        return result;
    }
    
    vector<StudentNode*> getStudentUpdates() {
        lock_guard<mutex> lock(queue_mutex);
        vector<StudentNode*> result;
        while (!studentUpdateQueue.empty()) {
            result.push_back(studentUpdateQueue.front());
            studentUpdateQueue.pop();
        }
        return result;
    }
    
    vector<StudentNode*> getStudentDeletes() {
        lock_guard<mutex> lock(queue_mutex);
        vector<StudentNode*> result;
        while (!studentDeleteQueue.empty()) {
            result.push_back(studentDeleteQueue.front());
            studentDeleteQueue.pop();
        }
        return result;
    }
    
    vector<CourseNode*> getCourseInserts() {
        lock_guard<mutex> lock(queue_mutex);
        vector<CourseNode*> result;
        while (!courseInsertQueue.empty()) {
            result.push_back(courseInsertQueue.front());
            courseInsertQueue.pop();
        }
        return result;
    }
    
    vector<CourseNode*> getCourseUpdates() {
        lock_guard<mutex> lock(queue_mutex);
        vector<CourseNode*> result;
        while (!courseUpdateQueue.empty()) {
            result.push_back(courseUpdateQueue.front());
            courseUpdateQueue.pop();
        }
        return result;
    }
    
    vector<CourseNode*> getCourseDeletes() {
        lock_guard<mutex> lock(queue_mutex);
        vector<CourseNode*> result;
        while (!courseDeleteQueue.empty()) {
            result.push_back(courseDeleteQueue.front());
            courseDeleteQueue.pop();
        }
        return result;
    }
    
    vector<FeeNode*> getFeeInserts() {
        lock_guard<mutex> lock(queue_mutex);
        vector<FeeNode*> result;
        while (!feeInsertQueue.empty()) {
            result.push_back(feeInsertQueue.front());
            feeInsertQueue.pop();
        }
        return result;
    }
    
    vector<FeeNode*> getFeeUpdates() {
        lock_guard<mutex> lock(queue_mutex);
        vector<FeeNode*> result;
        while (!feeUpdateQueue.empty()) {
            result.push_back(feeUpdateQueue.front());
            feeUpdateQueue.pop();
        }
        return result;
    }
    
    vector<FeeNode*> getFeeDeletes() {
        lock_guard<mutex> lock(queue_mutex);
        vector<FeeNode*> result;
        while (!feeDeleteQueue.empty()) {
            result.push_back(feeDeleteQueue.front());
            feeDeleteQueue.pop();
        }
        return result;
    }
    
    void displayQueueStatus() {
        lock_guard<mutex> lock(queue_mutex);
        cout << "\n=== Operation Queue Status ===" << endl;
        cout << "Students - Insert: " << studentInsertQueue.size()
             << " | Update: " << studentUpdateQueue.size()
             << " | Delete: " << studentDeleteQueue.size() << endl;
        cout << "Courses  - Insert: " << courseInsertQueue.size()
             << " | Update: " << courseUpdateQueue.size()
             << " | Delete: " << courseDeleteQueue.size() << endl;
        cout << "Fees     - Insert: " << feeInsertQueue.size()
             << " | Update: " << feeUpdateQueue.size()
             << " | Delete: " << feeDeleteQueue.size() << endl;
        cout << "================================\n" << endl;
    }
};

// Database Manager
class DatabaseManager {
private:
    SQLHENV env;
    SQLHDBC dbc;
    string connectionString;
    
    bool connect() {
        SQLRETURN ret;
        
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
        SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        
        SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
        
        SQLCHAR connStr[] =
            "DRIVER={ODBC Driver 18 for SQL Server};"
            "SERVER=localhost;"
            "DATABASE=stdMangementDB2;"
            "Trusted_Connection=Yes;"
            "Encrypt=No;";
        
        ret = SQLDriverConnect(dbc, NULL, connStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
            return false;
        }
        return true;
    }
    
    void disconnect() {
        if (dbc) {
            SQLDisconnect(dbc);
            SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        }
        if (env) {
            SQLFreeHandle(SQL_HANDLE_ENV, env);
        }
    }
    
public:
    DatabaseManager() : env(NULL), dbc(NULL) {
        if (!connect()) {
            cerr << "Failed to connect to database!" << endl;
            exit(1);
        }
        cout << "Connected to SQL Server Database: stdMangementDB2" << endl;
    }
    
    ~DatabaseManager() {
        disconnect();
    }
    
    // Load operations
    vector<shared_ptr<StudentNode>> loadStudents() {
        vector<shared_ptr<StudentNode>> students;
        SQLHSTMT stmt;
        SQLRETURN ret;
        
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        ret = SQLExecDirect(stmt, 
            (SQLCHAR*)"SELECT stdId, stdName, stdUserName, stdAge, fieldId, stdFatherName FROM student",
            SQL_NTS);
        
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            SQLINTEGER stdId, stdAge, fieldId;
            SQLCHAR stdName[50], stdUserName[50], stdFatherName[50];
            SQLLEN indicator;
            
            while (SQLFetch(stmt) == SQL_SUCCESS) {
                SQLGetData(stmt, 1, SQL_C_LONG, &stdId, 0, NULL);
                SQLGetData(stmt, 2, SQL_C_CHAR, stdName, sizeof(stdName), NULL);
                SQLGetData(stmt, 3, SQL_C_CHAR, stdUserName, sizeof(stdUserName), NULL);
                SQLGetData(stmt, 4, SQL_C_LONG, &stdAge, 0, NULL);
                SQLGetData(stmt, 5, SQL_C_LONG, &fieldId, 0, NULL);
                SQLGetData(stmt, 6, SQL_C_CHAR, stdFatherName, sizeof(stdFatherName), &indicator);
                
                string fatherName = (indicator == SQL_NULL_DATA) ? "" : (char*)stdFatherName;
                
                auto student = make_shared<StudentNode>(
                    stdId, (char*)stdName, (char*)stdUserName, stdAge, fieldId, fatherName);
                student->modified = false;
                students.push_back(student);
            }
        }
        
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return students;
    }
    
    vector<shared_ptr<CourseNode>> loadCourses() {
        vector<shared_ptr<CourseNode>> courses;
        SQLHSTMT stmt;
        
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        SQLExecDirect(stmt, 
            (SQLCHAR*)"SELECT courseId, courseTitle, teacherName FROM course",
            SQL_NTS);
        
        SQLINTEGER courseId;
        SQLCHAR courseTitle[50], teacherName[50];
        
        while (SQLFetch(stmt) == SQL_SUCCESS) {
            SQLGetData(stmt, 1, SQL_C_LONG, &courseId, 0, NULL);
            SQLGetData(stmt, 2, SQL_C_CHAR, courseTitle, sizeof(courseTitle), NULL);
            SQLGetData(stmt, 3, SQL_C_CHAR, teacherName, sizeof(teacherName), NULL);
            
            auto course = make_shared<CourseNode>(courseId, (char*)courseTitle, (char*)teacherName);
            course->modified = false;
            courses.push_back(course);
        }
        
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return courses;
    }
    
    vector<shared_ptr<FeeNode>> loadFees() {
        vector<shared_ptr<FeeNode>> fees;
        SQLHSTMT stmt;
        
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        SQLExecDirect(stmt, 
            (SQLCHAR*)"SELECT feeId, stdId, amount, CONVERT(VARCHAR, paymentDate, 23), status FROM StudentFees",
            SQL_NTS);
        
        SQLINTEGER feeId, stdId;
        SQLDOUBLE amount;
        SQLCHAR paymentDate[20], status[20];
        
        while (SQLFetch(stmt) == SQL_SUCCESS) {
            SQLGetData(stmt, 1, SQL_C_LONG, &feeId, 0, NULL);
            SQLGetData(stmt, 2, SQL_C_LONG, &stdId, 0, NULL);
            SQLGetData(stmt, 3, SQL_C_DOUBLE, &amount, 0, NULL);
            SQLGetData(stmt, 4, SQL_C_CHAR, paymentDate, sizeof(paymentDate), NULL);
            SQLGetData(stmt, 5, SQL_C_CHAR, status, sizeof(status), NULL);
            
            auto fee = make_shared<FeeNode>(feeId, stdId, amount, (char*)paymentDate, (char*)status);
            fee->modified = false;
            fees.push_back(fee);
        }
        
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return fees;
    }
    
    // Sync operations
    bool insertStudent(StudentNode* student) {
        SQLHSTMT stmt;
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        SQLPrepare(stmt, 
            (SQLCHAR*)"INSERT INTO student (stdId, stdName, stdUserName, stdAge, fieldId, stdFatherName) VALUES (?, ?, ?, ?, ?, ?)",
            SQL_NTS);
        
        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &student->stdId, 0, NULL);
        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, 
                        (SQLPOINTER)student->stdName.c_str(), 0, NULL);
        SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, 
                        (SQLPOINTER)student->stdUserName.c_str(), 0, NULL);
        SQLBindParameter(stmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &student->stdAge, 0, NULL);
        SQLBindParameter(stmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &student->fieldId, 0, NULL);
        SQLBindParameter(stmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, 
                        (SQLPOINTER)student->stdFatherName.c_str(), 0, NULL);
        
        SQLRETURN ret = SQLExecute(stmt);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        
        return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
    }
    
    bool updateStudent(StudentNode* student) {
        SQLHSTMT stmt;
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        SQLPrepare(stmt, 
            (SQLCHAR*)"UPDATE student SET stdName = ?, stdUserName = ?, stdAge = ?, fieldId = ?, stdFatherName = ? WHERE stdId = ?",
            SQL_NTS);
        
        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, 
                        (SQLPOINTER)student->stdName.c_str(), 0, NULL);
        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, 
                        (SQLPOINTER)student->stdUserName.c_str(), 0, NULL);
        SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &student->stdAge, 0, NULL);
        SQLBindParameter(stmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &student->fieldId, 0, NULL);
        SQLBindParameter(stmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, 
                        (SQLPOINTER)student->stdFatherName.c_str(), 0, NULL);
        SQLBindParameter(stmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &student->stdId, 0, NULL);
        
        SQLRETURN ret = SQLExecute(stmt);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        
        return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
    }
    
    bool deleteStudent(int stdId) {
        SQLHSTMT stmt;
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        SQLPrepare(stmt, 
            (SQLCHAR*)"DELETE FROM student WHERE stdId = ?",
            SQL_NTS);
        
        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &stdId, 0, NULL);
        
        SQLRETURN ret = SQLExecute(stmt);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        
        return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
    }
    
    bool insertCourse(CourseNode* course) {
        SQLHSTMT stmt;
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        SQLPrepare(stmt, 
            (SQLCHAR*)"INSERT INTO course (courseId, courseTitle, teacherName) VALUES (?, ?, ?)",
            SQL_NTS);
        
        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &course->courseId, 0, NULL);
        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, 
                        (SQLPOINTER)course->courseTitle.c_str(), 0, NULL);
        SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, 
                        (SQLPOINTER)course->teacherName.c_str(), 0, NULL);
        
        SQLRETURN ret = SQLExecute(stmt);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        
        return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
    }
    
    bool insertFee(FeeNode* fee) {
        SQLHSTMT stmt;
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        SQLPrepare(stmt, 
            (SQLCHAR*)"INSERT INTO StudentFees (feeId, stdId, amount, paymentDate, status) VALUES (?, ?, ?, ?, ?)",
            SQL_NTS);
        
        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &fee->feeId, 0, NULL);
        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &fee->stdId, 0, NULL);
        SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DECIMAL, 10, 2, &fee->amount, 0, NULL);
        SQLBindParameter(stmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_DATE, 20, 0, 
                        (SQLPOINTER)fee->paymentDate.c_str(), 0, NULL);
        SQLBindParameter(stmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, 
                        (SQLPOINTER)fee->status.c_str(), 0, NULL);
        
        SQLRETURN ret = SQLExecute(stmt);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        
        return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
    }
    
    bool checkAdmin(const string& username, const string& password) {
        SQLHSTMT stmt;
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        SQLPrepare(stmt, 
            (SQLCHAR*)"SELECT COUNT(*) FROM adminTab WHERE adminName = ? AND passWord = ?",
            SQL_NTS);
        
        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 40, 0, 
                        (SQLPOINTER)username.c_str(), 0, NULL);
        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, 
                        (SQLPOINTER)password.c_str(), 0, NULL);
        
        SQLExecute(stmt);
        SQLINTEGER count = 0;
        
        if (SQLFetch(stmt) == SQL_SUCCESS) {
            SQLGetData(stmt, 1, SQL_C_LONG, &count, 0, NULL);
        }
        
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return (count > 0);
    }
    
    int getMaxStudentId() {
        SQLHSTMT stmt;
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        SQLExecDirect(stmt, (SQLCHAR*)"SELECT MAX(stdId) FROM student", SQL_NTS);
        
        SQLINTEGER maxId = 0;
        if (SQLFetch(stmt) == SQL_SUCCESS) {
            SQLGetData(stmt, 1, SQL_C_LONG, &maxId, 0, NULL);
        }
        
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return maxId;
    }
    
    vector<pair<int, string>> getFieldStudies() {
        vector<pair<int, string>> fields;
        SQLHSTMT stmt;
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        
        SQLExecDirect(stmt, (SQLCHAR*)"SELECT fieldId, fieldName FROM fieldStudy", SQL_NTS);
        
        SQLINTEGER fieldId;
        SQLCHAR fieldName[50];
        
        while (SQLFetch(stmt) == SQL_SUCCESS) {
            SQLGetData(stmt, 1, SQL_C_LONG, &fieldId, 0, NULL);
            SQLGetData(stmt, 2, SQL_C_CHAR, fieldName, sizeof(fieldName), NULL);
            fields.push_back({fieldId, (char*)fieldName});
        }
        
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return fields;
    }
};

// Main System Manager
class SystemManager {
private:
    unique_ptr<StudentBST> studentBST;
    unique_ptr<OperationQueue> opQueue;
    unique_ptr<DatabaseManager> dbManager;
    
    vector<shared_ptr<CourseNode>> courses;
    vector<shared_ptr<FeeNode>> fees;
    map<int, string> fieldStudies;
    
    bool running;
    int nextStudentId;
    int nextFeeId;
    int nextCourseId;
    
    void loadInitialData() {
        cout << "Loading data from database..." << endl;
        
        // Load field studies
        auto fields = dbManager->getFieldStudies();
        for (const auto& field : fields) {
            fieldStudies[field.first] = field.second;
        }
        
        // Load students
        auto students = dbManager->loadStudents();
        for (const auto& student : students) {
            studentBST->insert(student->stdId, student->stdName, student->stdUserName,
                             student->stdAge, student->fieldId, student->stdFatherName);
        }
        
        // Load courses
        courses = dbManager->loadCourses();
        
        // Load fees
        fees = dbManager->loadFees();
        
        // Set next IDs
        nextStudentId = dbManager->getMaxStudentId() + 1;
        nextFeeId = fees.empty() ? 1 : fees.back()->feeId + 1;
        nextCourseId = courses.empty() ? 1 : courses.back()->courseId + 1;
        
        cout << "Initial data loaded successfully!" << endl;
        cout << "Next Student ID: " << nextStudentId << endl;
        cout << "Available Fields: " << fieldStudies.size() << endl;
    }
    
    void displayFieldStudies() {
        cout << "\n=== Available Field Studies ===" << endl;
        for (const auto& field : fieldStudies) {
            cout << field.first << ". " << field.second << endl;
        }
    }
    
public:
    SystemManager() : running(true), nextStudentId(1), nextFeeId(1), nextCourseId(1) {
        studentBST = make_unique<StudentBST>();
        opQueue = make_unique<OperationQueue>();
        dbManager = make_unique<DatabaseManager>();
        
        loadInitialData();
    }
    
    ~SystemManager() {
        shutdown();
    }
    
    bool login() {
        string username, password;
        cout << "\n=== ADMIN LOGIN ===" << endl;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        
        if (dbManager->checkAdmin(username, password)) {
            cout << "Login successful!" << endl;
            return true;
        } else {
            cout << "Invalid credentials!" << endl;
            return false;
        }
    }
    
    void addStudent() {
        string name, username, father;
        int age, fieldId;
        
        cin.ignore();
        cout << "\n=== ADD NEW STUDENT ===" << endl;
        cout << "Enter student name: ";
        getline(cin, name);
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter age: ";
        cin >> age;
        
        displayFieldStudies();
        cout << "Select field ID: ";
        cin >> fieldId;
        
        cin.ignore();
        cout << "Enter father's name: ";
        getline(cin, father);
        
        StudentNode* newStudent = studentBST->insert(nextStudentId++, name, username, age, fieldId, father);
        opQueue->addStudentInsert(newStudent);
        
        cout << "Student added successfully with ID: " << newStudent->stdId << endl;
    }
    
    void searchStudent() {
        int choice;
        cout << "\n=== SEARCH STUDENT ===" << endl;
        cout << "1. Search by ID" << endl;
        cout << "2. Search by Username" << endl;
        cout << "3. Search by Field" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        if (choice == 1) {
            int id;
            cout << "Enter student ID: ";
            cin >> id;
            
            StudentNode* student = studentBST->search(id);
            if (student) {
                student->display();
            } else {
                cout << "Student not found!" << endl;
            }
        } else if (choice == 2) {
            string username;
            cin.ignore();
            cout << "Enter username: ";
            getline(cin, username);
            
            StudentNode* student = studentBST->searchByUsername(username);
            if (student) {
                student->display();
            } else {
                cout << "Student not found!" << endl;
            }
        } else if (choice == 3) {
            displayFieldStudies();
            int fieldId;
            cout << "Enter field ID: ";
            cin >> fieldId;
            
            auto students = studentBST->searchByField(fieldId);
            cout << "\nFound " << students.size() << " students in field " << fieldId << endl;
            for (auto student : students) {
                student->display();
            }
        }
    }
    
    void updateStudent() {
        int id;
        cout << "\n=== UPDATE STUDENT ===" << endl;
        cout << "Enter student ID to update: ";
        cin >> id;
        
        StudentNode* student = studentBST->search(id);
        if (!student) {
            cout << "Student not found!" << endl;
            return;
        }
        
        cout << "Current details:" << endl;
        student->display();
        
        string name, username, father;
        int age, fieldId;
        
        cin.ignore();
        cout << "\nEnter new name (press enter to keep current): ";
        getline(cin, name);
        if (name.empty()) name = student->stdName;
        
        cout << "Enter new username (press enter to keep current): ";
        getline(cin, username);
        if (username.empty()) username = student->stdUserName;
        
        cout << "Enter new age (0 to keep current): ";
        cin >> age;
        if (age == 0) age = student->stdAge;
        
        displayFieldStudies();
        cout << "Enter new field ID (0 to keep current): ";
        cin >> fieldId;
        if (fieldId == 0) fieldId = student->fieldId;
        
        cin.ignore();
        cout << "Enter father's name (press enter to keep current): ";
        getline(cin, father);
        if (father.empty()) father = student->stdFatherName;
        
        if (studentBST->update(id, name, username, age, fieldId, father)) {
            StudentNode* updated = studentBST->search(id);
            opQueue->addStudentUpdate(updated);
            cout << "Student updated successfully!" << endl;
        }
    }
    
    void deleteStudent() {
        int id;
        cout << "\n=== DELETE STUDENT ===" << endl;
        cout << "Enter student ID to delete: ";
        cin >> id;
        
        if (studentBST->markForDeletion(id)) {
            StudentNode* student = studentBST->search(id);
            opQueue->addStudentDelete(student);
            cout << "Student marked for deletion!" << endl;
        } else {
            cout << "Student not found!" << endl;
        }
    }
    
    void addCourse() {
        string title, teacher;
        int id;
        
        cin.ignore();
        cout << "\n=== ADD NEW COURSE ===" << endl;
        cout << "Enter course ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter course title: ";
        getline(cin, title);
        cout << "Enter teacher name: ";
        getline(cin, teacher);
        
        auto course = make_shared<CourseNode>(id, title, teacher);
        course->modified = true;
        courses.push_back(course);
        opQueue->addCourseInsert(course.get());
        
        cout << "Course added successfully!" << endl;
    }
    
    void addFee() {
        int stdId, feeId;
        double amount;
        string status;
        
        cout << "\n=== ADD FEE PAYMENT ===" << endl;
        cout << "Enter fee ID: ";
        cin >> feeId;
        cout << "Enter student ID: ";
        cin >> stdId;
        
        StudentNode* student = studentBST->search(stdId);
        if (!student) {
            cout << "Student not found!" << endl;
            return;
        }
        
        cout << "Enter amount: ";
        cin >> amount;
        cin.ignore();
        cout << "Enter status (Paid/Pending): ";
        getline(cin, status);
        
        // Get current date
        time_t now = time(nullptr);
        tm* tm = localtime(&now);
        stringstream date;
        date << (tm->tm_year + 1900) << "-" 
             << setw(2) << setfill('0') << (tm->tm_mon + 1) << "-"
             << setw(2) << setfill('0') << tm->tm_mday;
        
        auto fee = make_shared<FeeNode>(feeId, stdId, amount, date.str(), status);
        fee->modified = true;
        fees.push_back(fee);
        opQueue->addFeeInsert(fee.get());
        
        cout << "Fee record added successfully!" << endl;
    }
    
    void syncToDatabase() {
        cout << "\n=== SYNCING TO DATABASE ===" << endl;
        
        // Sync students
        auto studentInserts = opQueue->getStudentInserts();
        for (auto student : studentInserts) {
            if (dbManager->insertStudent(student)) {
                student->modified = false;
                cout << "Inserted student ID: " << student->stdId << endl;
            }
        }
        
        auto studentUpdates = opQueue->getStudentUpdates();
        for (auto student : studentUpdates) {
            if (dbManager->updateStudent(student)) {
                student->modified = false;
                cout << "Updated student ID: " << student->stdId << endl;
            }
        }
        
        auto studentDeletes = opQueue->getStudentDeletes();
        for (auto student : studentDeletes) {
            if (dbManager->deleteStudent(student->stdId)) {
                studentBST->remove(student->stdId);
                cout << "Deleted student ID: " << student->stdId << endl;
            }
        }
        
        // Sync courses
        auto courseInserts = opQueue->getCourseInserts();
        for (auto course : courseInserts) {
            if (dbManager->insertCourse(course)) {
                course->modified = false;
                cout << "Inserted course ID: " << course->courseId << endl;
            }
        }
        
        // Sync fees
        auto feeInserts = opQueue->getFeeInserts();
        for (auto fee : feeInserts) {
            if (dbManager->insertFee(fee)) {
                fee->modified = false;
                cout << "Inserted fee ID: " << fee->feeId << endl;
            }
        }
        
        cout << "=== SYNC COMPLETE ===" << endl;
    }
    
    void displayMenu() {
        cout << "\n=== STUDENT MANAGEMENT SYSTEM ===" << endl;
        cout << "1. Add New Student" << endl;
        cout << "2. Search Student" << endl;
        cout << "3. Update Student" << endl;
        cout << "4. Delete Student" << endl;
        cout << "5. View All Students" << endl;
        cout << "6. Add Course" << endl;
        cout << "7. Add Fee Payment" << endl;
        cout << "8. View Pending Operations" << endl;
        cout << "9. Sync to Database" << endl;
        cout << "10. Exit" << endl;
        cout << "Enter choice: ";
    }
    
    void shutdown() {
        if (running) {
            cout << "\n=== SYSTEM SHUTDOWN ===" << endl;
            cout << "Syncing all changes to database..." << endl;
            syncToDatabase();
            running = false;
            cout << "System shutdown complete!" << endl;
        }
    }
    
    void run() {
        if (!login()) {
            cout << "Login failed! Exiting..." << endl;
            return;
        }
        
        while (running) {
            displayMenu();
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: addStudent(); break;
                case 2: searchStudent(); break;
                case 3: updateStudent(); break;
                case 4: deleteStudent(); break;
                case 5: studentBST->displayAll(); break;
                case 6: addCourse(); break;
                case 7: addFee(); break;
                case 8: opQueue->displayQueueStatus(); break;
                case 9: syncToDatabase(); break;
                case 10: shutdown(); break;
                default: cout << "Invalid choice!" << endl;
            }
        }
    }
};

// Main function
int main() {
    try {
        SystemManager system;
        system.run();
    } catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}