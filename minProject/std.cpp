#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stireg.h>
#include <sstream>
#include <vector>
#include <limits>
using namespace std;

class DB
{
    SQLHENV hEnv;
    SQLHDBC hDbc;

public:
    DB()
    {
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
        SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    }
    ~DB()
    {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        cout << "DB resources released\n";
    }
    SQLHDBC &returnDb() { return hDbc; }
    void connect()
    {
        SQLCHAR connStr[] =
            "DRIVER={ODBC Driver 17 for SQL Server};"
            "SERVER=localhost;"
            "DATABASE=mydb;"
            "Trusted_Connection=yes;";

        SQLDriverConnect(
            hDbc,
            NULL,
            connStr,
            SQL_NTS,
            NULL,
            0,
            NULL,
            SQL_DRIVER_COMPLETE);

        cout << "Connected to database\n";
    }
};

template <class T>
class Node
{
public:
    T data;
    Node<T> *left;
    Node<T> *right;
    Node(T d) : data(d), left(NULL), right(NULL) {}
    T &getData() const
    {
        return data;
    }
    void setData(T dat)
    {
        this->data = dat;
    }
    void setLeft(Node<T> node)
    {
        this->left = node;
    }
    Node<T> *GetLeft() const
    {
        return left;
    }
    Node<T> *GetRight() const
    {
        return right;
    }
};
template <class T>
class bst
{
private:
    Node<T> *logSearch(Node<T> *node, int id)
    {
        if (node == NULL)
            return NULL;

        if (node->data.getId() == id)
            return node;

        if (id < node->data.getId())
            return logSearch(node->left, id);
        else
            return logSearch(node->right, id);
    }
    Node<T> *insertRec(Node<T> *node, T value)
    {
        if (node == NULL)
            return new Node<T>(value);

        if (value.getId() < node->data.getId())
            node->left = insertRec(node->left, value);
        else if (value.getId() > node->data.getId())
            node->right = insertRec(node->right, value);

        return node;
    }

public:
    Node<T> *root;
    bst()
    {
        this->root = NULL;
    }
    bst(Node<T> *nod)
    {
        this->root = nod;
    }
    void buildBst(T value)
    {
        root = insertRec(root, value);
    }
    Node<T> *search(int id)
    {
        return logSearch(root, id);
    }
};
class Student
{
    int Id;
    string Name;
    int Grade;
    int Age;

public:
    Student(int id, string name, int age, int grade)
    {
        this->Id = id;
        this->Name = name;
        this->Age = age;
        this->Grade = grade;
    }
    int getId() { return Id; }
    string getName() { return Name; }
    int getClass() { return Grade; }
    int getAge() { return Age; }
    void printStd()
    {
        // Fixed: now prints Id
        cout << "Id : " << Id << endl;
        cout << "Name : " << Name << endl;
        cout << "Age : " << Age << endl;
        cout << "Class : " << Grade << endl;
        cout << "-------------------\n";
    }
};
class operationQueue
{
    
}
class System
{
    DB d;

    // Minimal helper to see SQL errors
    void printError(SQLHANDLE handle)
    {
        SQLCHAR sqlState[1024], message[1024];
        SQLINTEGER nativeError;
        SQLSMALLINT textLength;
        if (SQLGetDiagRec(SQL_HANDLE_STMT, handle, 1, sqlState, &nativeError, message, sizeof(message), &textLength) == SQL_SUCCESS)
            cout << "ODBC Error: " << message << " (SQLState: " << sqlState << ")\n";
    }

public:
    void connect() { d.connect(); }

    void addStd(Student s)
    {
        int id = s.getId();
        int age = s.getAge();
        string name = s.getName();
        int grade = s.getClass();

        string query =
            "INSERT INTO std (stdId, stdName, age, grade) VALUES (" +
            to_string(id) + ", '" +
            name + "', " +
            to_string(age) + ", " +
            to_string(grade) + ")";

        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, d.returnDb(), &hStmt);

        SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR *)query.c_str(), SQL_NTS);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Insert failed!" << endl;
            printError(hStmt); // Show why
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    vector<Student> displayStd()
    {
        SQLHSTMT hStmt;
        string query = "SELECT * FROM std";
        SQLAllocHandle(SQL_HANDLE_STMT, d.returnDb(), &hStmt);
        SQLExecDirect(hStmt, (SQLCHAR *)query.c_str(), SQL_NTS);

        vector<Student> stds;
        int id, age, grade;
        char name[50];

        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
            SQLGetData(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
            SQLGetData(hStmt, 3, SQL_C_LONG, &age, 0, NULL);
            SQLGetData(hStmt, 4, SQL_C_LONG, &grade, 0, NULL);

            Student s(id, name, age, grade);
            stds.push_back(s);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return stds;
    }
};
class ui
{

    int getput()
    {
        int choice;
        while (true)
        {
            cin >> choice;

            if (cin.fail())
            {
                cin.clear(); // remove fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter an integer: ";
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
        }
    }
    void stdOpeartion()
    {
        cout << "1->TO regitser a student \n";
        cout << "2->To search a studnet \n";
        cout << "3->To update student Information \n";

    }
    void diplay()
    {
        cout << "********************************************************** Welcome to Student Mangement System ************************************************************\n";
        cout << "1-> for std Operation \n";
        cout << "1-> for course Operatrion \n";
        cout << "3->Fee opeatrion \n";
        cout << "4->For admin system Setting \n";
    }
};

int main()
{
    System sys;
    sys.connect();
    Student s(132, "habib", 20, 10);
    sys.addStd(s);

    vector<Student> stdList = sys.displayStd();
    bst<Student> bs;
    for (auto &stu : stdList)
    {
        bs.buildBst(stu); // ✅ correct
    }

    int id;
    cout << "Enter search id : \n";
    cin >> id;
    Node<Student> *found = bs.search(id);
    if (found)
        found->data.printStd();
    else
        cout << "Student not found\n";
    return 0;
}
