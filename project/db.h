#define _HAS_STD_BYTE 0
#ifndef DB_H
#define DB_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <string>
#include <type_traits>

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

    // ==========================================
    // NEW METHOD: executeUpdate (with parameters)
    // ==========================================
    template<typename... Args>
    bool executeUpdate(const string& query, Args... args)
    {
        if (!this->isConnected)
        {
            cout << "Database not connected!" << endl;
            return false;
        }

        SQLHSTMT hstmt = NULL;
        SQLRETURN retcode;

        // Allocate statement handle
        retcode = SQLAllocHandle(SQL_HANDLE_STMT, this->hdbc, &hstmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Error allocating statement handle" << endl;
            return false;
        }

        // Prepare the SQL statement
        retcode = SQLPrepare(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Error preparing SQL statement" << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
            return false;
        }

        // Helper function to bind parameters
        int paramIndex = 1;
        auto bindParam = [&](auto&& value) {
            using T = std::decay_t<decltype(value)>;
            
            if constexpr (std::is_same_v<T, string> || std::is_same_v<T, char*> || std::is_same_v<T, const char*>)
            {
                string strValue = value;
                SQLLEN strLen = SQL_NTS;
                retcode = SQLBindParameter(hstmt, paramIndex++, SQL_PARAM_INPUT,
                                          SQL_C_CHAR, SQL_VARCHAR,
                                          strValue.length(), 0,
                                          (SQLPOINTER)strValue.c_str(), 0, &strLen);
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                retcode = SQLBindParameter(hstmt, paramIndex++, SQL_PARAM_INPUT,
                                          SQL_C_LONG, SQL_INTEGER,
                                          0, 0, &value, 0, NULL);
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                retcode = SQLBindParameter(hstmt, paramIndex++, SQL_PARAM_INPUT,
                                          SQL_C_DOUBLE, SQL_DOUBLE,
                                          0, 0, &value, 0, NULL);
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                retcode = SQLBindParameter(hstmt, paramIndex++, SQL_PARAM_INPUT,
                                          SQL_C_FLOAT, SQL_REAL,
                                          0, 0, &value, 0, NULL);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                SQLINTEGER boolValue = value ? 1 : 0;
                retcode = SQLBindParameter(hstmt, paramIndex++, SQL_PARAM_INPUT,
                                          SQL_C_LONG, SQL_INTEGER,
                                          0, 0, &boolValue, 0, NULL);
            }
            
            if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
            {
                cout << "Error binding parameter " << (paramIndex - 1) << endl;
            }
        };

        // Bind all parameters using fold expression (C++17)
        (bindParam(args), ...);

        // Execute the statement
        retcode = SQLExecute(hstmt);
        
        bool success = (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO);
        
        if (success)
        {
            SQLLEN rowCount = 0;
            SQLRowCount(hstmt, &rowCount);
            if (rowCount >= 0)
            {
                cout << "✓ " << rowCount << " row(s) affected" << endl;
            }
        }
        else
        {
            SQLCHAR sqlState[6] = "", errorMsg[SQL_MAX_MESSAGE_LENGTH] = "";
            SQLINTEGER nativeError = 0;
            SQLSMALLINT msgLen = 0;
            
            SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, 1, sqlState, &nativeError, 
                         errorMsg, sizeof(errorMsg), &msgLen);
            cout << "SQL Error: " << errorMsg << " (State: " << sqlState << ")" << endl;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        return success;
    }

    // ==========================================
    // NEW METHOD: updateField (helper method)
    // ==========================================
    bool updateField(const string& table, const string& field, const string& newValue, 
                     const string& idField, int id)
    {
        string query = "UPDATE " + table + " SET " + field + " = ? WHERE " + idField + " = ?";
        return executeUpdate(query, newValue, id);
    }

    // ==========================================
    // NEW METHOD: executeUpdate (no parameters)
    // ==========================================
    bool executeUpdate(const string& query)
    {
        if (!this->isConnected)
        {
            cout << "Database not connected!" << endl;
            return false;
        }

        SQLHSTMT hstmt = NULL;
        SQLRETURN retcode;

        // Allocate statement handle
        retcode = SQLAllocHandle(SQL_HANDLE_STMT, this->hdbc, &hstmt);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        {
            cout << "Error allocating statement handle" << endl;
            return false;
        }

        // Execute the SQL statement directly
        retcode = SQLExecDirect(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);
        
        bool success = (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO);
        
        if (success)
        {
            SQLLEN rowCount = 0;
            SQLRowCount(hstmt, &rowCount);
            if (rowCount >= 0)
            {
                cout << "✓ " << rowCount << " row(s) affected" << endl;
            }
        }
        else
        {
            SQLCHAR sqlState[6] = "", errorMsg[SQL_MAX_MESSAGE_LENGTH] = "";
            SQLINTEGER nativeError = 0;
            SQLSMALLINT msgLen = 0;
            
            SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, 1, sqlState, &nativeError, 
                         errorMsg, sizeof(errorMsg), &msgLen);
            cout << "SQL Error: " << errorMsg << " (State: " << sqlState << ")" << endl;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        return success;
    }
};

#endif // DB_H