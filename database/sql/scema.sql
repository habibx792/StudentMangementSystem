-- SQL Server Database Setup Script
USE master;
GO

IF EXISTS(SELECT * FROM sys.databases WHERE name = 'SM_System')
    DROP DATABASE SM_System;
GO

CREATE DATABASE SM_System;
GO

USE SM_System;
GO

-- Admin Table
CREATE TABLE adminTab
(
    adminId INT IDENTITY(1,1) PRIMARY KEY,
    adminName VARCHAR(40) NOT NULL,
    passWord VARCHAR(255) NOT NULL
);
GO

-- Field of Study
CREATE TABLE fieldStudy
(
    fieldId INT IDENTITY(1,1) PRIMARY KEY,
    fieldName VARCHAR(40) NOT NULL UNIQUE
);
GO

-- Student Table
CREATE TABLE student
(
    stdId INT NOT NULL PRIMARY KEY,
    stdName VARCHAR(40) NOT NULL,
    stdUserName VARCHAR(40) NOT NULL UNIQUE,
    stdAge INT CHECK (stdAge > 0),
    fieldId INT NOT NULL,
    stdFatherName VARCHAR(40),
    FOREIGN KEY (fieldId) REFERENCES fieldStudy(fieldId) ON DELETE RESTRICT
);
GO

-- Course Table
CREATE TABLE course
(
    courseId INT NOT NULL PRIMARY KEY,
    courseTitle VARCHAR(40) NOT NULL,
    teacherName VARCHAR(40) NOT NULL
);
GO

-- Student Fees
CREATE TABLE StudentFees
(
    feeId INT IDENTITY(1,1) PRIMARY KEY,
    stdId INT NOT NULL,
    amount DECIMAL(10,2) CHECK (amount >= 0),
    paymentDate DATE DEFAULT GETDATE(),
    status VARCHAR(20) DEFAULT 'Pending',
    FOREIGN KEY (stdId) REFERENCES student(stdId) ON DELETE CASCADE
);
GO

-- Course Registration
CREATE TABLE courseRegStd
(
    stdId INT NOT NULL,
    courseId INT NOT NULL,
    regDate DATETIME DEFAULT GETDATE(),
    PRIMARY KEY (stdId, courseId),
    FOREIGN KEY (stdId) REFERENCES student(stdId) ON DELETE CASCADE,
    FOREIGN KEY (courseId) REFERENCES course(courseId) ON DELETE CASCADE
);
GO

-- Attendance
CREATE TABLE Attendance 
(
    attendanceId INT IDENTITY(1,1) PRIMARY KEY,
    stdId INT NOT NULL,
    courseId INT NOT NULL,
    attendanceDate DATE DEFAULT CAST(GETDATE() AS DATE),
    isPresent BIT DEFAULT 0,
    FOREIGN KEY (stdId) REFERENCES student(stdId) ON DELETE CASCADE,
    FOREIGN KEY (courseId) REFERENCES course(courseId) ON DELETE CASCADE,
    CONSTRAINT UC_Attendance UNIQUE (stdId, courseId, attendanceDate)
);
GO

-- Results
CREATE TABLE result
(
    stdId INT NOT NULL,
    courseId INT NOT NULL,
    gotNumber DECIMAL(5,2) CHECK (gotNumber >= 0 AND gotNumber <= 100),
    grade VARCHAR(2),
    isPass AS (CASE WHEN gotNumber >= 50 THEN 1 ELSE 0 END) PERSISTED,
    PRIMARY KEY (stdId, courseId),
    FOREIGN KEY (stdId) REFERENCES student(stdId) ON DELETE CASCADE,
    FOREIGN KEY (courseId) REFERENCES course(courseId) ON DELETE CASCADE
);
GO

-- Insert Default Data
INSERT INTO fieldStudy (fieldName) VALUES 
('Computer Science'),
('Electrical Engineering'),
('Business Administration'),
('Medicine'),
('Law');
GO

INSERT INTO adminTab (adminName, passWord) VALUES 
('admin', 'admin123'),
('supervisor', 'sup123');
GO

-- Create Indexes for Performance
CREATE INDEX idx_student_field ON student(fieldId);
CREATE INDEX idx_fees_student ON StudentFees(stdId);
CREATE INDEX idx_attendance_student ON Attendance(stdId);
CREATE INDEX idx_attendance_course ON Attendance(courseId);
CREATE INDEX idx_result_student ON result(stdId);
GO

PRINT 'Database SM_System created successfully with all tables!';
GO