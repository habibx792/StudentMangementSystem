#include <iostream>
#include <windows.h>
#include <string>
#include <sqlext.h>

using namespace std;

// Robust ODBC diagnostics
void showDiag(SQLHANDLE handle, SQLSMALLINT type)
{
    SQLCHAR state[6];
    SQLCHAR msg[512];
    SQLINTEGER native;
    SQLSMALLINT len;
    SQLSMALLINT i = 1;

    while (true)
    {
        SQLRETURN ret = SQLGetDiagRec(type, handle, i, state, &native, msg, sizeof(msg), &len);
        if (ret == SQL_NO_DATA)
            break;
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            cout << "SQLSTATE: " << state
                 << " | Native Error: " << native
                 << " | Message: " << msg << endl;
        }
        i++;
    }
}

// Helper to check success or success-with-info
bool ok(SQLRETURN ret)
{
    return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

int main()
{
    SQLHENV hEnv = NULL;
    SQLHDBC hDbc = NULL;
    SQLHSTMT hStmt = NULL;

    // 1. Allocate environment
    if (!ok(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv)))
    {
        cout << "Failed to allocate environment handle.\n";
        return 1;
    }
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);

    // 2. Allocate connection
    if (!ok(SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc)))
    {
        cout << "Failed to allocate connection handle.\n";
        return 1;
    }

    // 3. Connect to SQL Server master DB
    SQLCHAR connStr[] =
        "Driver={ODBC Driver 17 for SQL Server};"
        "Server=localhost;"
        "Database=master;"
        "Trusted_Connection=yes;";

    SQLRETURN ret = SQLDriverConnect(
        hDbc,
        NULL,
        connStr,
        SQL_NTS,
        NULL,
        0,
        NULL,
        SQL_DRIVER_COMPLETE);

    if (!ok(ret))
    {
        cout << "Connection failed:\n";
        showDiag(hDbc, SQL_HANDLE_DBC);
        return 1;
    }

    cout << "Connected to master database.\n";

    // 4. Allocate statement
    if (!ok(SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt)))
    {
        cout << "Failed to allocate statement handle.\n";
        return 1;
    }

    // 5. Create table if it doesn't exist
    SQLCHAR createSQL[] =
        "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name='CppTest') "
        "CREATE TABLE CppTest ("
        "id INT IDENTITY PRIMARY KEY, "
        "name VARCHAR(50), "
        "age INT)";

    ret = SQLExecDirect(hStmt, createSQL, SQL_NTS);
    if (!ok(ret))
    {
        cout << "Table creation failed:\n";
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    cout << "Table ready.\n";

    // 6. Input
    string name;
    int age;

    cout << "Enter name: ";
    cin >> name;
    cout << "Enter age: ";
    cin >> age;

    // 7. Insert data
    string insertSQL = "INSERT INTO CppTest (name, age) VALUES ('" + name + "', " + to_string(age) + ")";

    ret = SQLExecDirect(hStmt, (SQLCHAR *)insertSQL.c_str(), SQL_NTS);
    if (!ok(ret))
    {
        cout << "Insert failed:\n";
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    cout << "Data inserted successfully.\n";

    // 8. Select & display all rows
    ret = SQLExecDirect(hStmt, (SQLCHAR *)"SELECT id, name, age FROM CppTest", SQL_NTS);
    if (!ok(ret))
    {
        cout << "Select failed:\n";
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    int id, dbAge;
    char dbName[50];

    cout << "\nID | Name | Age\n";
    cout << "------------------\n";

    while (SQLFetch(hStmt) == SQL_SUCCESS)
    {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, dbName, sizeof(dbName), NULL);
        SQLGetData(hStmt, 3, SQL_C_LONG, &dbAge, 0, NULL);

        cout << id << " | " << dbName << " | " << dbAge << endl;
    }

    // 9. Cleanup
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
