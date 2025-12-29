#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

// Function to display ODBC errors
void showDiag(SQLHANDLE handle, SQLSMALLINT type)
{
    SQLCHAR state[6], msg[512];
    SQLINTEGER native;
    SQLSMALLINT len;
    SQLSMALLINT i = 1;
    while (SQLGetDiagRec(type, handle, i, state, &native, msg, sizeof(msg), &len) == SQL_SUCCESS) {
        cout << "ODBC Error [" << state << "] (" << native << "): " << msg << endl;
        i++;
    }
}

// Helper to check return codes
bool ok(SQLRETURN ret)
{
    return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

// Helper to execute SQL query and clean up cursor
bool execQuery(SQLHSTMT stmt, const string& query, bool closeCursor = true)
{
    SQLRETURN ret = SQLExecDirect(stmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    
    // Close cursor if needed (for SELECT queries)
    if (closeCursor && (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
        SQLCloseCursor(stmt);
    }
    
    if (!ok(ret)) {
        if (ret != SQL_NO_DATA) {  // SQL_NO_DATA is not an error
            showDiag(stmt, SQL_HANDLE_STMT);
        }
        return false;
    }
    return true;
}

class DSA_Database {
private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    
    void cleanup() {
        if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        if (hDbc) {
            SQLDisconnect(hDbc);
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        }
        if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }

public:
    DSA_Database() : hEnv(NULL), hDbc(NULL), hStmt(NULL) {}
    
    ~DSA_Database() { cleanup(); }
    
    bool connect() {
        // 1. Allocate environment
        if (!ok(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv))) return false;
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        
        // 2. Allocate connection
        if (!ok(SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc))) return false;
        
        // 3. Connect to SQL Server Express
        SQLCHAR connStr[] =
            "Driver={ODBC Driver 18 for SQL Server};"
            "Server=CODEX\\SQLEXPRESS;"
            "Database=master;"
            "Trusted_Connection=yes;"
            "TrustServerCertificate=yes;";
        
        SQLRETURN connRet = SQLDriverConnect(hDbc, NULL, connStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
        if (!ok(connRet)) {
            showDiag(hDbc, SQL_HANDLE_DBC);
            return false;
        }
        
        cout << "Connected to SQL Server Express successfully.\n";
        
        // 4. Allocate statement
        if (!ok(SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt))) return false;
        
        return true;
    }
    
    bool setupDatabase() {
        // Create DSA database
        if (!execQuery(hStmt, "IF DB_ID('DSA_DB') IS NULL CREATE DATABASE DSA_DB;")) return false;
        if (!execQuery(hStmt, "USE DSA_DB;")) return false;
        
        cout << "DSA Database created/selected.\n";
        return true;
    }
    
    bool createTables() {
        // 1. Stack Table
        string createStack = R"(
            IF OBJECT_ID('Stack', 'U') IS NOT NULL DROP TABLE Stack;
            CREATE TABLE Stack (
                StackID INT IDENTITY(1,1) PRIMARY KEY,
                Value INT NOT NULL,
                OperationTime DATETIME DEFAULT GETDATE(),
                OperationType VARCHAR(10) CHECK(OperationType IN ('PUSH', 'POP'))
            );
        )";
        
        // 2. Queue Table
        string createQueue = R"(
            IF OBJECT_ID('Queue', 'U') IS NOT NULL DROP TABLE Queue;
            CREATE TABLE Queue (
                QueueID INT IDENTITY(1,1) PRIMARY KEY,
                Value INT NOT NULL,
                EnqueueTime DATETIME DEFAULT GETDATE(),
                IsProcessed BIT DEFAULT 0
            );
        )";
        
        // 3. LinkedList Table
        string createLinkedList = R"(
            IF OBJECT_ID('LinkedList', 'U') IS NOT NULL DROP TABLE LinkedList;
            CREATE TABLE LinkedList (
                NodeID INT IDENTITY(1,1) PRIMARY KEY,
                Data INT NOT NULL,
                NextNodeID INT NULL,
                InsertionTime DATETIME DEFAULT GETDATE()
            );
        )";
        
        // 4. Graph Table
        string createGraph = R"(
            IF OBJECT_ID('Graph', 'U') IS NOT NULL DROP TABLE Graph;
            CREATE TABLE Graph (
                GraphID INT IDENTITY(1,1) PRIMARY KEY,
                Vertex INT NOT NULL,
                AdjacentVertex INT NOT NULL,
                Weight INT DEFAULT 1,
                CreatedTime DATETIME DEFAULT GETDATE()
            );
        )";
        
        cout << "Creating DSA tables...\n";
        if (!execQuery(hStmt, createStack)) return false;
        if (!execQuery(hStmt, createQueue)) return false;
        if (!execQuery(hStmt, createLinkedList)) return false;
        if (!execQuery(hStmt, createGraph)) return false;
        
        cout << "All DSA tables created successfully.\n";
        return true;
    }
    
    // Helper function to execute SELECT query and fetch results
    bool executeSelectQuery(const string& query) {
        // Close any existing cursor first
        SQLCloseCursor(hStmt);
        
        SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);
        if (!ok(ret) && ret != SQL_NO_DATA) {
            showDiag(hStmt, SQL_HANDLE_STMT);
            return false;
        }
        return true;
    }
    
    // STACK OPERATIONS
    void stackPush(int value) {
        string query = "INSERT INTO Stack (Value, OperationType) VALUES (" + 
                      to_string(value) + ", 'PUSH');";
        if (execQuery(hStmt, query, false)) {
            cout << "Pushed " << value << " to Stack.\n";
        }
    }
    
    void stackPop() {
        // Close any existing cursor
        SQLCloseCursor(hStmt);
        
        string query = "SELECT TOP 1 StackID, Value FROM Stack WHERE OperationType='PUSH' ORDER BY StackID DESC;";
        if (executeSelectQuery(query)) {
            int stackID = 0, value = 0;
            if (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &stackID, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_LONG, &value, 0, NULL);
                
                // Close cursor after fetch
                SQLCloseCursor(hStmt);
                
                string insertQuery = "INSERT INTO Stack (Value, OperationType) VALUES (" + 
                                    to_string(value) + ", 'POP');";
                if (execQuery(hStmt, insertQuery, false)) {
                    cout << "Popped " << value << " from Stack.\n";
                }
            } else {
                SQLCloseCursor(hStmt);
                cout << "Stack is empty!\n";
            }
        }
    }
    
    void displayStack() {
        cout << "\n=== STACK CONTENTS ===\n";
        cout << "ID | Value | Operation | Time\n";
        cout << "----------------------------\n";
        
        string query = "SELECT StackID, Value, OperationType, CONVERT(VARCHAR, OperationTime, 120) FROM Stack ORDER BY StackID;";
        
        if (executeSelectQuery(query)) {
            int id = 0, value = 0;
            char opType[20], opTime[30];
            
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_LONG, &value, 0, NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, opType, sizeof(opType), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, opTime, sizeof(opTime), NULL);
                
                cout << id << " | " << value << " | " << opType << " | " << opTime << endl;
            }
            SQLCloseCursor(hStmt);
        }
    }
    
    // QUEUE OPERATIONS
    void enqueue(int value) {
        string query = "INSERT INTO Queue (Value) VALUES (" + to_string(value) + ");";
        if (execQuery(hStmt, query, false)) {
            cout << "Enqueued " << value << " to Queue.\n";
        }
    }
    
    void dequeue() {
        // Close any existing cursor
        SQLCloseCursor(hStmt);
        
        string query = "SELECT TOP 1 QueueID, Value FROM Queue WHERE IsProcessed=0 ORDER BY QueueID;";
        if (executeSelectQuery(query)) {
            int queueID = 0, value = 0;
            if (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &queueID, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_LONG, &value, 0, NULL);
                
                SQLCloseCursor(hStmt);
                
                string updateQuery = "UPDATE Queue SET IsProcessed=1 WHERE QueueID=" + to_string(queueID) + ";";
                if (execQuery(hStmt, updateQuery, false)) {
                    cout << "Dequeued " << value << " from Queue.\n";
                }
            } else {
                SQLCloseCursor(hStmt);
                cout << "Queue is empty!\n";
            }
        }
    }
    
    void displayQueue() {
        cout << "\n=== QUEUE CONTENTS ===\n";
        cout << "ID | Value | Status | Time\n";
        cout << "--------------------------\n";
        
        string query = "SELECT QueueID, Value, IsProcessed, CONVERT(VARCHAR, EnqueueTime, 120) FROM Queue ORDER BY QueueID;";
        
        if (executeSelectQuery(query)) {
            int id = 0, value = 0, processed = 0;
            char enqTime[30];
            
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &id, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_LONG, &value, 0, NULL);
                SQLGetData(hStmt, 3, SQL_C_LONG, &processed, 0, NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, enqTime, sizeof(enqTime), NULL);
                
                string status = processed ? "Processed" : "Pending";
                cout << id << " | " << value << " | " << status << " | " << enqTime << endl;
            }
            SQLCloseCursor(hStmt);
        }
    }
    
    // LINKED LIST OPERATIONS
    void insertLinkedList(int data, int afterNode = 0) {
        if (afterNode == 0) {
            // Insert at beginning
            string query = "INSERT INTO LinkedList (Data) OUTPUT INSERTED.NodeID VALUES (" + to_string(data) + ");";
            if (executeSelectQuery(query)) {
                int newID = 0;
                if (SQLFetch(hStmt) == SQL_SUCCESS) {
                    SQLGetData(hStmt, 1, SQL_C_LONG, &newID, 0, NULL);
                    cout << "Inserted " << data << " (NodeID: " << newID << ") at beginning of LinkedList.\n";
                }
                SQLCloseCursor(hStmt);
            }
        } else {
            // Insert after specific node
            string query = "INSERT INTO LinkedList (Data) OUTPUT INSERTED.NodeID VALUES (" + to_string(data) + ");";
            if (executeSelectQuery(query)) {
                int newID = 0;
                if (SQLFetch(hStmt) == SQL_SUCCESS) {
                    SQLGetData(hStmt, 1, SQL_C_LONG, &newID, 0, NULL);
                    
                    SQLCloseCursor(hStmt);
                    
                    // Update previous node's NextNodeID
                    string updateQuery = "UPDATE LinkedList SET NextNodeID = " + to_string(newID) + 
                                        " WHERE NodeID = " + to_string(afterNode) + ";";
                    if (execQuery(hStmt, updateQuery, false)) {
                        cout << "Inserted " << data << " (NodeID: " << newID << ") after node " << afterNode << " in LinkedList.\n";
                    }
                } else {
                    SQLCloseCursor(hStmt);
                }
            }
        }
    }
    
    void displayLinkedList() {
        cout << "\n=== LINKED LIST ===\n";
        cout << "NodeID | Data | NextNode | Insertion Time\n";
        cout << "-----------------------------------------\n";
        
        string query = "SELECT NodeID, Data, NextNodeID, CONVERT(VARCHAR, InsertionTime, 120) FROM LinkedList ORDER BY NodeID;";
        
        if (executeSelectQuery(query)) {
            int nodeID = 0, data = 0, nextID = 0;
            char insTime[30];
            
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &nodeID, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_LONG, &data, 0, NULL);
                SQLGetData(hStmt, 3, SQL_C_LONG, &nextID, 0, NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, insTime, sizeof(insTime), NULL);
                
                cout << nodeID << " | " << data << " | ";
                if (nextID == 0) cout << "NULL";
                else cout << nextID;
                cout << " | " << insTime << endl;
            }
            SQLCloseCursor(hStmt);
        }
    }
    
    // GRAPH OPERATIONS
    void addEdge(int vertex, int adjacent, int weight = 1) {
        string query = "INSERT INTO Graph (Vertex, AdjacentVertex, Weight) VALUES (" +
                      to_string(vertex) + ", " + to_string(adjacent) + ", " + to_string(weight) + ");";
        if (execQuery(hStmt, query, false)) {
            cout << "Added edge: " << vertex << " -> " << adjacent << " (weight: " << weight << ")\n";
        }
    }
    
    void displayGraph() {
        cout << "\n=== GRAPH ADJACENCY LIST ===\n";
        cout << "Vertex -> Adjacent (Weight)\n";
        cout << "----------------------------\n";
        
        string query = "SELECT Vertex, AdjacentVertex, Weight FROM Graph ORDER BY Vertex, AdjacentVertex;";
        
        if (executeSelectQuery(query)) {
            int vertex = 0, adjVertex = 0, weight = 0;
            
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &vertex, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_LONG, &adjVertex, 0, NULL);
                SQLGetData(hStmt, 3, SQL_C_LONG, &weight, 0, NULL);
                
                cout << vertex << " -> " << adjVertex << " (" << weight << ")\n";
            }
            SQLCloseCursor(hStmt);
        }
    }
    
    // DATABASE STATISTICS
    void showStatistics() {
        cout << "\n=== DATABASE STATISTICS ===\n";
        
        string queries[] = {
            "SELECT 'Stack' as TableName, COUNT(*) as Records FROM Stack",
            "SELECT 'Queue' as TableName, COUNT(*) as Records FROM Queue",
            "SELECT 'LinkedList' as TableName, COUNT(*) as Records FROM LinkedList",
            "SELECT 'Graph' as TableName, COUNT(*) as Records FROM Graph"
        };
        
        for (auto& query : queries) {
            if (executeSelectQuery(query)) {
                char tableName[20];
                int records = 0;
                if (SQLFetch(hStmt) == SQL_SUCCESS) {
                    SQLGetData(hStmt, 1, SQL_C_CHAR, tableName, sizeof(tableName), NULL);
                    SQLGetData(hStmt, 2, SQL_C_LONG, &records, 0, NULL);
                    cout << tableName << ": " << records << " records\n";
                }
                SQLCloseCursor(hStmt);
            }
        }
    }
};

// Menu system
void displayMenu() {
    cout << "\n========= DSA DATABASE SYSTEM =========\n";
    cout << "1. Stack Operations\n";
    cout << "2. Queue Operations\n";
    cout << "3. Linked List Operations\n";
    cout << "4. Graph Operations\n";
    cout << "5. View All Data Structures\n";
    cout << "6. Show Database Statistics\n";
    cout << "7. Exit\n";
    cout << "=======================================\n";
    cout << "Enter your choice: ";
}

void stackMenu(DSA_Database& db) {
    int choice, value;
    do {
        cout << "\n=== STACK MENU ===\n";
        cout << "1. Push\n";
        cout << "2. Pop\n";
        cout << "3. View Stack\n";
        cout << "4. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                cout << "Enter value to push: ";
                cin >> value;
                db.stackPush(value);
                break;
            case 2:
                db.stackPop();
                break;
            case 3:
                db.displayStack();
                break;
        }
    } while(choice != 4);
}

void queueMenu(DSA_Database& db) {
    int choice, value;
    do {
        cout << "\n=== QUEUE MENU ===\n";
        cout << "1. Enqueue\n";
        cout << "2. Dequeue\n";
        cout << "3. View Queue\n";
        cout << "4. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                cout << "Enter value to enqueue: ";
                cin >> value;
                db.enqueue(value);
                break;
            case 2:
                db.dequeue();
                break;
            case 3:
                db.displayQueue();
                break;
        }
    } while(choice != 4);
}

int main() {
    DSA_Database db;
    
    // Connect to database
    if (!db.connect()) {
        cout << "Failed to connect to database.\n";
        return 1;
    }
    
    // Setup database and create tables
    if (!db.setupDatabase()) {
        cout << "Failed to setup database.\n";
        return 1;
    }
    
    if (!db.createTables()) {
        cout << "Failed to create tables.\n";
        return 1;
    }
    
    // Insert sample data
    cout << "\nInserting sample data...\n";
    
    // Stack sample data
    db.stackPush(10);
    db.stackPush(20);
    db.stackPush(30);
    
    // Queue sample data
    db.enqueue(100);
    db.enqueue(200);
    db.enqueue(300);
    
    // Linked List sample data
    db.insertLinkedList(1);
    db.insertLinkedList(2);
    db.insertLinkedList(3, 1); // Insert after node 1
    
    // Graph sample data
    db.addEdge(1, 2);
    db.addEdge(1, 3);
    db.addEdge(2, 3, 5);
    db.addEdge(3, 4);
    
    // Main menu loop
    int choice;
    do {
        displayMenu();
        cin >> choice;
        
        switch(choice) {
            case 1:
                stackMenu(db);
                break;
            case 2:
                queueMenu(db);
                break;
            case 3:
                db.displayLinkedList();
                break;
            case 4:
                db.displayGraph();
                break;
            case 5:
                db.displayStack();
                db.displayQueue();
                db.displayLinkedList();
                db.displayGraph();
                break;
            case 6:
                db.showStatistics();
                break;
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while(choice != 7);
    
    return 0;
}