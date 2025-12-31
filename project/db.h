#define _HAS_STD_BYTE 0
#ifndef DB_H
#define DB_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <string>

using namespace std;

class DB
{
private:
    SQLHENV henv;
    SQLHDBC hdbc;
    bool isConnected;

public:
    DB()
    {
        this->henv = NULL;
        this->hdbc = NULL;
        this->isConnected = false;
    }

    ~DB()
    {
        this->disconnect();
    }

    bool connect()
    {
        SQLRETURN retcode;

        // Allocate environment handle
        retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->henv);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Error allocating environment handle" << endl;
            return false;
        }

        // Set the ODBC version
        retcode = SQLSetEnvAttr(this->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Error setting ODBC version" << endl;
            SQLFreeHandle(SQL_HANDLE_ENV, this->henv);
            return false;
        }

        // Allocate connection handle
        retcode = SQLAllocHandle(SQL_HANDLE_DBC, this->henv, &this->hdbc);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Error allocating connection handle" << endl;
            SQLFreeHandle(SQL_HANDLE_ENV, this->henv);
            return false;
        }

        // Connect to SM_System database using direct ODBC
        SQLCHAR *connStr = (SQLCHAR *)"DRIVER={ODBC Driver 17 for SQL Server};SERVER=localhost\\SQLEXPRESS;DATABASE=SM_System;Trusted_Connection=yes;TrustServerCertificate=yes;";

        SQLCHAR outConnStr[1024];
        SQLSMALLINT outConnStrLen;

        retcode = SQLDriverConnect(this->hdbc, NULL, connStr, SQL_NTS,
                                   outConnStr, sizeof(outConnStr),
                                   &outConnStrLen, SQL_DRIVER_NOPROMPT);

        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Error connecting to SM_System database" << endl;
            SQLFreeHandle(SQL_HANDLE_DBC, this->hdbc);
            SQLFreeHandle(SQL_HANDLE_ENV, this->henv);
            return false;
        }

        this->isConnected = true;
        cout << "Connected to SM_System database successfully" << endl;
        return true;
    }

    void disconnect()
    {
        if (this->hdbc != NULL)
        {
            SQLDisconnect(this->hdbc);
            SQLFreeHandle(SQL_HANDLE_DBC, this->hdbc);
            this->hdbc = NULL;
        }
        if (this->henv != NULL)
        {
            SQLFreeHandle(SQL_HANDLE_ENV, this->henv);
            this->henv = NULL;
        }
        this->isConnected = false;
        cout << "Disconnected from database" << endl;
    }

    SQLHDBC returnDb() const
    {
        return this->hdbc;
    }

    bool connected() const
    {
        return this->isConnected;
    }

    // ADD THIS METHOD - printStatus
    void printStatus() const
    {
        cout << "Database Status: " << (this->isConnected ? "Connected to SM_System" : "Disconnected") << endl;
    }
};

#endif // DB_H