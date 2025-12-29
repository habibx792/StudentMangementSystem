#include <iostream>
#include <windows.h>
#include <sqlext.h>

using namespace std;

void showDiag(SQLHANDLE handle, SQLSMALLINT type) {
    SQLCHAR state[6];
    SQLCHAR msg[256];
    SQLINTEGER native;
    SQLSMALLINT len;

    if (SQLGetDiagRec(type, handle, 1, state, &native, msg, sizeof(msg), &len) == SQL_SUCCESS) {
        cout << msg << endl;
    }
}

bool ok(SQLRETURN ret) {
    return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

int main() {
    SQLHENV hEnv = NULL;
    SQLHDBC hDbc = NULL;
    SQLHSTMT hStmt = NULL;

    // 1. Environment
    if (!ok(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv))) return 1;
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

    // 2. Connection
    if (!ok(SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc))) return 1;

    SQLCHAR connStr[] =
        "Driver={SQL Server};"
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
        SQL_DRIVER_COMPLETE
    );

    if (!ok(ret)) {
        showDiag(hDbc, SQL_HANDLE_DBC);
        return 1;
    }

    cout << "Connected to master.\n";

    // 3. Statement
    if (!ok(SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt))) return 1;

    // 4. Create table
    SQLCHAR createSQL[] =
        "IF NOT EXISTS (SELECT * FROM sys.tables WHERE name='CppTest') "
        "CREATE TABLE CppTest ("
        "id INT IDENTITY PRIMARY KEY, "
        "name VARCHAR(50), "
        "age INT)";

    ret = SQLExecDirect(hStmt, createSQL, SQL_NTS);
    if (!ok(ret)) {
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    cout << "Table ready.\n";

    // 5. Input
    string name;
    int age;

    cout << "Enter name: ";
    cin >> name;
    cout << "Enter age: ";
    cin >> age;

    // 6. Insert
    string insertSQL =
        "INSERT INTO CppTest (name, age) VALUES ('" +
        name + "', " + to_string(age) + ")";

    ret = SQLExecDirect(hStmt, (SQLCHAR*)insertSQL.c_str(), SQL_NTS);
    if (!ok(ret)) {
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    cout << "Data inserted.\n";

    // 7. Select
    ret = SQLExecDirect(
        hStmt,
        (SQLCHAR*)"SELECT id, name, age FROM CppTest",
        SQL_NTS
    );

    if (!ok(ret)) {
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    int id, dbAge;
    char dbName[50];

    cout << "\nID | Name | Age\n";
    cout << "------------------\n";

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, dbName, sizeof(dbName), NULL);
        SQLGetData(hStmt, 3, SQL_C_LONG, &dbAge, 0, NULL);

        cout << id << " | " << dbName << " | " << dbAge << endl;
    }

    // 8. Cleanup
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
