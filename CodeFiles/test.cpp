#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <string>

using namespace std;

// Function to display ODBC errors
void showDiag(SQLHANDLE handle, SQLSMALLINT type)
{
    SQLCHAR state[6], msg[256];
    SQLINTEGER native;
    SQLSMALLINT len;
    if (SQLGetDiagRec(type, handle, 1, state, &native, msg, sizeof(msg), &len) == SQL_SUCCESS)
        cout << "ODBC Error: " << msg << endl;
}

// Helper to check return codes
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
        return 1;
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);

    // 2. Allocate connection
    if (!ok(SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc)))
        return 1;

    // 3. Connect to SQL Server Express (named instance SQLEXPRESS)
    SQLCHAR connStr[] =
        "Driver={ODBC Driver 18 for SQL Server};"
        "Server=CODEX\\SQLEXPRESS;" // replace CODEX with your hostname if needed
        "Database=master;"
        "Trusted_Connection=yes;"
        "TrustServerCertificate=yes;";

    SQLRETURN ret = SQLDriverConnect(
        hDbc, NULL, connStr, SQL_NTS,
        NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    if (!ok(ret))
    {
        showDiag(hDbc, SQL_HANDLE_DBC);
        return 1;
    }

    cout << "Connected to SQL Server Express successfully.\n";

    // 4. Allocate statement
    if (!ok(SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt)))
        return 1;

    // 5. Create database HABIB if not exists
    ret = SQLExecDirect(hStmt, (SQLCHAR *)"IF DB_ID('HABIB') IS NULL CREATE DATABASE HABIB;", SQL_NTS);
    if (!ok(ret))
    {
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    // 6. Use HABIB database
    ret = SQLExecDirect(hStmt, (SQLCHAR *)"USE HABIB;", SQL_NTS);
    if (!ok(ret))
    {
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    // 7. Drop old STUDENT table if exists
    ret = SQLExecDirect(hStmt, (SQLCHAR *)"IF OBJECT_ID('STUDENT', 'U') IS NOT NULL DROP TABLE STUDENT;", SQL_NTS);
    if (!ok(ret))
    {
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    // 8. Create new STUDENT table with auto-increment StudentID
    SQLCHAR createTable[] =
        "CREATE TABLE STUDENT ("
        "StudentID INT IDENTITY(1,1) PRIMARY KEY, "
        "FirstName VARCHAR(50), "
        "LastName VARCHAR(50), "
        "Age INT, "
        "EnrollmentDate DATE);";

    ret = SQLExecDirect(hStmt, createTable, SQL_NTS);
    if (!ok(ret))
    {
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    cout << "Table STUDENT is ready.\n";

    // 9. Insert sample data (do NOT insert StudentID, it auto-generates)
    ret = SQLExecDirect(hStmt, (SQLCHAR *)"INSERT INTO STUDENT (FirstName, LastName, Age, EnrollmentDate) VALUES "
                                          "('Ali', 'Khan', 20, '2025-01-15'),"
                                          "('Sara', 'Ahmed', 22, '2025-02-10'),"
                                          "('Omar', 'Hussain', 21, '2025-03-05'),"
                                          "('Ayesha', 'Shah', 19, '2025-04-12'),"
                                          "('Habib', 'Ali', 23, '2025-05-01');",
                        SQL_NTS);

    if (!ok(ret))
    {
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }
    cout << "Sample data inserted.\n";

    // 10. Select and display data
    ret = SQLExecDirect(hStmt, (SQLCHAR *)"SELECT StudentID, FirstName, LastName, Age, EnrollmentDate FROM STUDENT;", SQL_NTS);
    if (!ok(ret))
    {
        showDiag(hStmt, SQL_HANDLE_STMT);
        return 1;
    }

    cout << "\nID | FirstName | LastName | Age | EnrollmentDate\n";
    cout << "-----------------------------------------------\n";

    int id, age;
    char first[51], last[51];
    char enroll[11];

    while (SQLFetch(hStmt) == SQL_SUCCESS)
    {
        SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, first, sizeof(first), NULL);
        SQLGetData(hStmt, 3, SQL_C_CHAR, last, sizeof(last), NULL);
        SQLGetData(hStmt, 4, SQL_C_LONG, &age, 0, NULL);
        SQLGetData(hStmt, 5, SQL_C_CHAR, enroll, sizeof(enroll), NULL);

        cout << id << " | " << first << " | " << last << " | " << age << " | " << enroll << endl;
    }

    // 11. Cleanup
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
