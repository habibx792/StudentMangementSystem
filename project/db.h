#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stireg.h>
#include <sstream>
#include <vector>
#include <limits>
#ifndef DB_H
#define DB_H

#include <string>
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

#endif // DB_H