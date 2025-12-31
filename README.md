Student Management System
Overview

The Student Management System (SMS) is a C++ console-based application designed to manage student records, courses, attendance, results, and fees. It provides a visualized structure of data management using Binary Search Trees (BSTs), queues, and other data structures to efficiently handle CRUD operations. This project demonstrates data structure manipulation, database connectivity, and software design principles.

Features
Core Functionalities

Student Management

Register new students

Search for students by name or ID

Update student information

Delete student records

Course Management

Add new courses

Search for courses by title or ID

Update course details

Delete course records

Attendance Management

Record student attendance by course and date

Search attendance by student ID, course ID, or date

Print attendance reports

Result Management

Record student results

Calculate pass/fail automatically

Search results by student or course

Fee Management

Record and update student fee payments

Track pending fees

View fee history per student

Admin Management

Add and manage admin accounts

Secure password storage (basic demonstration)

Admin login for system control

Data Structures Used

Binary Search Trees (BST)

Used for storing students, courses, results, and attendance

Supports efficient searching and traversal operations

Queues

Used for tracking updates, deletions, and new insertions

Singleton Pattern

Ensures only one instance of SMS_SYSTEM exists

C++ Classes & Templates

For modular, reusable code across different data types

Database Design

The system uses SQL Server to persist records. Main tables include:

student – Stores student information

course – Stores course information

Attendance – Tracks student attendance

result – Tracks student results

StudentFees – Manages fee payments

adminTab – Stores admin credentials

fieldStudy – Represents academic fields

courseRegStd – Links students with courses

Sample SQL Table
CREATE TABLE student (
    stdId INT IDENTITY(1,1) PRIMARY KEY,
    stdName VARCHAR(40) NOT NULL,
    stdUserName VARCHAR(40) NOT NULL UNIQUE,
    stdAge INT CHECK (stdAge > 0),
    fieldId INT NOT NULL,
    stdFatherName VARCHAR(40),
    FOREIGN KEY (fieldId) REFERENCES fieldStudy(fieldId)
);

Project Architecture
SMS_SYSTEM (Singleton)
│
├── UI.h             --> User Interface for console menus
├── DB.h             --> SQL Server connection handler
├── stdNode.h        --> Template node class for BST
├── stdBst.h         --> Generic BST implementation
├── bstSearch.h      --> BST search functions for all data types
├── printEngine.h    --> Prints BST nodes and queue contents
├── stdQueue.h       --> Template queue for updates and deletions
├── updationQueue.h  --> Queue for updated records
├── deleteQueue.h    --> Queue for deleted records
├── opQueue.h        --> Queue for new insertions
├── Data Classes     --> Student, Course, Attendance, Result, Admin, FieldStudy

Setup & Installation

Install SQL Server

Create database stdManagementDB2

Execute SQL scripts to create all tables

Configure ODBC Connection

Ensure ODBC Driver 17 for SQL Server is installed

Update DB.h connection string with your server name and database

Compile Project

g++ *.cpp -o StudentManagementSystem -std=c++17 -lodbc32 -lodbccp32


Run Application

./StudentManagementSystem

How to Use

Launch the application

Main menu will appear with options:

Student Operations

Course Operations

Fee Operations

Admin Settings

Navigate menus using integer inputs

Add, search, update, or delete records

View attendance, results, and fees using BST search and print functions

Key Points

BST ensures fast search, insert, and delete operations.

Queues track pending updates or deletions.

Singleton design prevents multiple system instances.

Modular C++ design ensures maintainable and reusable code.

Future Enhancements

Add GUI interface using Qt or SFML

Integrate password hashing for admin security

Implement report generation (PDF/Excel)

Add multi-threading for concurrent database operations

Contributors

[Your Name] – Developer & Project Lead

Semester DSA Project – [University Name]

License

This project is for academic purposes only.
