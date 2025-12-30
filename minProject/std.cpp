#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <cstring>

using namespace std;

// ================== DATABASE CLASS ==================
class Database
{
private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    bool connected;

    void printError(SQLSMALLINT handleType, SQLHANDLE handle, const string &prefix)
    {
        SQLCHAR sqlState[1024], message[1024];
        SQLINTEGER nativeError;
        SQLSMALLINT textLength;
        SQLSMALLINT recNum = 1;

        while (SQLGetDiagRec(handleType, handle, recNum, sqlState, &nativeError,
                             message, sizeof(message), &textLength) == SQL_SUCCESS)
        {
            cout << prefix << " - Error " << recNum << ": " << message
                 << " (SQLState: " << sqlState << ")\n";
            recNum++;
        }
    }

public:
    Database() : hEnv(NULL), hDbc(NULL), connected(false) {}

    ~Database()
    {
        disconnect();
    }

    bool connect()
    {
        // Allocate environment handle
        if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
        {
            cout << "Failed to allocate environment handle\n";
            return false;
        }

        // Set ODBC version
        if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0) != SQL_SUCCESS)
        {
            cout << "Failed to set ODBC version\n";
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
            return false;
        }

        // Allocate connection handle
        if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
        {
            cout << "Failed to allocate connection handle\n";
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
            return false;
        }

        // Connection string with SSL bypass for development
        SQLCHAR connStr[] =
            "DRIVER={ODBC Driver 17 for SQL Server};"
            "SERVER=localhost\\SQLEXPRESS;" // Use your SQL Server instance
            "DATABASE=mydb;"
            "Trusted_Connection=yes;"
            "TrustServerCertificate=yes;"; // Bypass SSL certificate validation

        SQLCHAR outConnStr[1024];
        SQLSMALLINT outConnStrLen;

        // Connect to database
        SQLRETURN ret = SQLDriverConnect(hDbc, NULL, connStr, SQL_NTS,
                                         outConnStr, sizeof(outConnStr),
                                         &outConnStrLen, SQL_DRIVER_NOPROMPT);

        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            printError(SQL_HANDLE_DBC, hDbc, "Connection failed");
            return false;
        }

        connected = true;
        cout << "Connected to database successfully!\n";
        return true;
    }

    void disconnect()
    {
        if (connected)
        {
            SQLDisconnect(hDbc);
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
            cout << "Disconnected from database\n";
            connected = false;
        }
    }

    bool isConnected() const { return connected; }

    SQLHDBC getConnection() const { return hDbc; }

    // Execute any SQL command
    bool executeSQL(const string &sql)
    {
        if (!connected)
        {
            cout << "Not connected to database\n";
            return false;
        }

        SQLHSTMT hStmt;
        if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        {
            printError(SQL_HANDLE_DBC, hDbc, "Failed to allocate statement handle");
            return false;
        }

        SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS);
        bool success = (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);

        if (!success)
        {
            printError(SQL_HANDLE_STMT, hStmt, "SQL execution failed");
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return success;
    }

    // Initialize database with your SQL
    bool initializeDatabase()
    {
        cout << "Initializing database...\n";

        // Create database if not exists (may require admin privileges)
        // executeSQL("CREATE DATABASE mydb;");

        // Use the database
        if (!executeSQL("USE mydb;"))
        {
            cout << "Note: USE mydb command may fail in some ODBC configurations\n";
            cout << "Make sure you're connected to 'mydb' database\n";
        }

        // Create table
        string createTableSQL =
            "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'std') "
            "CREATE TABLE std ("
            "    stdId INT NOT NULL PRIMARY KEY,"
            "    stdName VARCHAR(40),"
            "    age INT,"
            "    grade INT"
            ");";

        if (!executeSQL(createTableSQL))
        {
            cout << "Failed to create table\n";
            return false;
        }

        // Check if table has data, if not insert sample
        string checkDataSQL = "SELECT COUNT(*) FROM std;";
        SQLHSTMT hStmt;
        if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        {
            return false;
        }

        if (SQLExecDirect(hStmt, (SQLCHAR *)checkDataSQL.c_str(), SQL_NTS) == SQL_SUCCESS)
        {
            SQLINTEGER count;
            SQLFetch(hStmt);
            SQLGetData(hStmt, 1, SQL_C_LONG, &count, 0, NULL);
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

            if (count == 0)
            {
                // Insert sample data
                string insertSQL =
                    "INSERT INTO std(stdId, stdName, age, grade) VALUES "
                    "(1, 'habib', 20, 10), "
                    "(2, 'ali', 21, 11), "
                    "(3, 'sara', 19, 9);";

                if (!executeSQL(insertSQL))
                {
                    cout << "Failed to insert sample data\n";
                    return false;
                }
                cout << "Sample data inserted\n";
            }
        }

        cout << "Database initialized successfully\n";
        return true;
    }
};

// ================== STUDENT CLASS ==================
class Student
{
private:
    int id;
    string name;
    int age;
    int grade;

public:
    Student() : id(0), age(0), grade(0) {}

    Student(int id, const string &name, int age, int grade)
        : id(id), name(name), age(age), grade(grade) {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    int getGrade() const { return grade; }

    // Setters
    void setId(int id) { this->id = id; }
    void setName(const string &name) { this->name = name; }
    void setAge(int age) { this->age = age; }
    void setGrade(int grade) { this->grade = grade; }

    void display() const
    {
        cout << left << setw(6) << id
             << setw(20) << name
             << setw(8) << age
             << setw(8) << grade << endl;
    }
};

// ================== BST NODE ==================
template <class T>
class BSTNode
{
public:
    T data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(T data) : data(data), left(nullptr), right(nullptr) {}
};

// ================== BST CLASS ==================
template <class T>
class StudentBST
{
private:
    BSTNode<T> *root;

    BSTNode<T> *insertNode(BSTNode<T> *node, T data)
    {
        if (!node)
            return new BSTNode<T>(data);

        if (data.getId() < node->data.getId())
            node->left = insertNode(node->left, data);
        else if (data.getId() > node->data.getId())
            node->right = insertNode(node->right, data);

        return node;
    }

    BSTNode<T> *searchNode(BSTNode<T> *node, int id)
    {
        if (!node || node->data.getId() == id)
            return node;

        if (id < node->data.getId())
            return searchNode(node->left, id);
        else
            return searchNode(node->right, id);
    }

    void inOrderTraversal(BSTNode<T> *node, vector<T> &result)
    {
        if (node)
        {
            inOrderTraversal(node->left, result);
            result.push_back(node->data);
            inOrderTraversal(node->right, result);
        }
    }

    void deleteTree(BSTNode<T> *node)
    {
        if (node)
        {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    StudentBST() : root(nullptr) {}

    ~StudentBST()
    {
        deleteTree(root);
    }

    void insert(T data)
    {
        root = insertNode(root, data);
    }

    T *search(int id)
    {
        BSTNode<T> *result = searchNode(root, id);
        return result ? &(result->data) : nullptr;
    }

    vector<T> getAllStudents()
    {
        vector<T> result;
        inOrderTraversal(root, result);
        return result;
    }

    void displayAll()
    {
        vector<T> students = getAllStudents();
        if (students.empty())
        {
            cout << "No students in BST\n";
            return;
        }

        cout << "\nStudents in BST (sorted by ID):\n";
        cout << string(42, '-') << endl;
        cout << left << setw(6) << "ID"
             << setw(20) << "Name"
             << setw(8) << "Age"
             << setw(8) << "Grade" << endl;
        cout << string(42, '-') << endl;

        for (const auto &student : students)
        {
            student.display();
        }
        cout << string(42, '-') << endl;
    }

    void clear()
    {
        deleteTree(root);
        root = nullptr;
    }
};

// ================== STUDENT MANAGER CLASS ==================
class StudentManager
{
private:
    Database db;
    StudentBST<Student> bst;

    vector<Student> fetchAllFromDB()
    {
        vector<Student> students;

        if (!db.isConnected())
        {
            cout << "Not connected to database\n";
            return students;
        }

        SQLHSTMT hStmt;
        if (SQLAllocHandle(SQL_HANDLE_STMT, db.getConnection(), &hStmt) != SQL_SUCCESS)
        {
            return students;
        }

        string sql = "SELECT stdId, stdName, age, grade FROM std ORDER BY stdId;";
        if (SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return students;
        }

        SQLINTEGER id, age, grade;
        SQLLEN id_ind, age_ind, grade_ind, name_ind;
        char name[100];

        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, &id_ind);
            SQLGetData(hStmt, 2, SQL_C_CHAR, name, sizeof(name), &name_ind);
            SQLGetData(hStmt, 3, SQL_C_LONG, &age, 0, &age_ind);
            SQLGetData(hStmt, 4, SQL_C_LONG, &grade, 0, &grade_ind);

            if (id_ind != SQL_NULL_DATA)
            {
                Student student(id,
                                name_ind == SQL_NULL_DATA ? "Unknown" : name,
                                age_ind == SQL_NULL_DATA ? 0 : age,
                                grade_ind == SQL_NULL_DATA ? 0 : grade);
                students.push_back(student);
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return students;
    }

public:
    StudentManager() {}

    bool initialize()
    {
        if (!db.connect())
        {
            cout << "Failed to connect to database\n";
            return false;
        }

        if (!db.initializeDatabase())
        {
            cout << "Failed to initialize database\n";
            return false;
        }

        // Load students from database to BST
        loadStudentsToBST();
        return true;
    }

    void loadStudentsToBST()
    {
        vector<Student> students = fetchAllFromDB();
        bst.clear();

        for (const auto &student : students)
        {
            bst.insert(student);
        }

        cout << "Loaded " << students.size() << " students into BST\n";
    }

    void displayAllFromDB()
    {
        vector<Student> students = fetchAllFromDB();

        if (students.empty())
        {
            cout << "No students found in database\n";
            return;
        }

        cout << "\nStudents in Database:\n";
        cout << string(42, '-') << endl;
        cout << left << setw(6) << "ID"
             << setw(20) << "Name"
             << setw(8) << "Age"
             << setw(8) << "Grade" << endl;
        cout << string(42, '-') << endl;

        for (const auto &student : students)
        {
            student.display();
        }
        cout << string(42, '-') << endl;
        cout << "Total: " << students.size() << " students\n";
    }

    void displayBST()
    {
        bst.displayAll();
    }

    void searchInBST(int id)
    {
        Student *result = bst.search(id);
        if (result)
        {
            cout << "\nStudent found in BST:\n";
            cout << string(30, '-') << endl;
            cout << "ID: " << result->getId() << endl;
            cout << "Name: " << result->getName() << endl;
            cout << "Age: " << result->getAge() << endl;
            cout << "Grade: " << result->getGrade() << endl;
            cout << string(30, '-') << endl;
        }
        else
        {
            cout << "Student with ID " << id << " not found in BST\n";
        }
    }

void addStudent(int id, const string& name, int age, int grade) {
    if (!db.isConnected()) {
        cout << "Not connected to database\n";
        return;
    }
    
    // Simple INSERT without IDENTITY_INSERT (table doesn't have identity property)
    string sql = "INSERT INTO std(stdId, stdName, age, grade) VALUES (" +
                to_string(id) + ", '" + name + "', " +
                to_string(age) + ", " + to_string(grade) + ");";
    
    if (db.executeSQL(sql)) {
        cout << "✓ Student added to database successfully\n";
        
        // Also add to BST
        Student newStudent(id, name, age, grade);
        bst.insert(newStudent);
        cout << "✓ Student also added to BST\n";
    } else {
        cout << "✗ Failed to add student. ID might already exist.\n";
    }
}

    void searchInDB(int id)
    {
        if (!db.isConnected())
        {
            cout << "Not connected to database\n";
            return;
        }

        SQLHSTMT hStmt;
        if (SQLAllocHandle(SQL_HANDLE_STMT, db.getConnection(), &hStmt) != SQL_SUCCESS)
        {
            return;
        }

        string sql = "SELECT stdId, stdName, age, grade FROM std WHERE stdId = " + to_string(id) + ";";
        if (SQLExecDirect(hStmt, (SQLCHAR *)sql.c_str(), SQL_NTS) != SQL_SUCCESS)
        {
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            cout << "Error executing search query\n";
            return;
        }

        SQLINTEGER db_id, age, grade;
        SQLLEN id_ind, age_ind, grade_ind, name_ind;
        char name[100];

        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_LONG, &db_id, 0, &id_ind);
            SQLGetData(hStmt, 2, SQL_C_CHAR, name, sizeof(name), &name_ind);
            SQLGetData(hStmt, 3, SQL_C_LONG, &age, 0, &age_ind);
            SQLGetData(hStmt, 4, SQL_C_LONG, &grade, 0, &grade_ind);

            cout << "\nStudent found in Database:\n";
            cout << string(30, '-') << endl;
            cout << "ID: " << db_id << endl;
            cout << "Name: " << (name_ind == SQL_NULL_DATA ? "Unknown" : name) << endl;
            cout << "Age: " << (age_ind == SQL_NULL_DATA ? 0 : age) << endl;
            cout << "Grade: " << (grade_ind == SQL_NULL_DATA ? 0 : grade) << endl;
            cout << string(30, '-') << endl;
        }
        else
        {
            cout << "Student with ID " << id << " not found in database\n";
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
};

// ================== UI FUNCTIONS ==================
void displayMenu()
{
    cout << "\n=== STUDENT MANAGEMENT SYSTEM ===\n";
    cout << "1. Display all students (from Database)\n";
    cout << "2. Display students in BST\n";
    cout << "3. Search student by ID (in BST)\n";
    cout << "4. Search student by ID (in Database)\n";
    cout << "5. Add new student\n";
    cout << "6. Refresh BST from Database\n";
    cout << "7. Exit\n";
    cout << "Enter your choice (1-7): ";
}

void addStudentMenu(StudentManager &manager)
{
    int id, age, grade;
    string name;

    cout << "\n--- Add New Student ---\n";

    cout << "Enter student ID: ";
    while (!(cin >> id))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number for ID: ";
    }
    cin.ignore();

    cout << "Enter student name: ";
    getline(cin, name);

    cout << "Enter student age: ";
    while (!(cin >> age) || age <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid age. Enter a positive number: ";
    }

    cout << "Enter student grade: ";
    while (!(cin >> grade) || grade <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid grade. Enter a positive number: ";
    }

    manager.addStudent(id, name, age, grade);
}

// ================== MAIN FUNCTION ==================
int main()
{
    cout << "========================================\n";
    cout << "   STUDENT MANAGEMENT SYSTEM\n";
    cout << "   Using SQL Server + ODBC + BST\n";
    cout << "========================================\n";

    StudentManager manager;

    if (!manager.initialize())
    {
        cout << "\nFailed to initialize system. Exiting...\n";
        return 1;
    }

    int choice;
    bool exitProgram = false;

    while (!exitProgram)
    {
        displayMenu();

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number 1-7.\n";
            continue;
        }
        cin.ignore(); // Clear newline

        switch (choice)
        {
        case 1:
            manager.displayAllFromDB();
            break;
        case 2:
            manager.displayBST();
            break;
        case 3:
        {
            int searchId;
            cout << "Enter student ID to search in BST: ";
            if (!(cin >> searchId))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid ID\n";
            }
            else
            {
                manager.searchInBST(searchId);
            }
            break;
        }
        case 4:
        {
            int searchId;
            cout << "Enter student ID to search in Database: ";
            if (!(cin >> searchId))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid ID\n";
            }
            else
            {
                manager.searchInDB(searchId);
            }
            break;
        }
        case 5:
            addStudentMenu(manager);
            break;
        case 6:
            manager.loadStudentsToBST();
            break;
        case 7:
            exitProgram = true;
            cout << "\nExiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please enter a number 1-7.\n";
            break;
        }
    }

    return 0;
}