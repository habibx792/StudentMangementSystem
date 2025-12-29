
CREATE DATABASE stdMangementDB;
GO

USE stdMangementDB;
GO

CREATE TABLE adminTab
(
    adminId INT IDENTITY(1,1) PRIMARY KEY,
    adminName VARCHAR(40) NOT NULL,
    passWord VARCHAR(255) NOT NULL
);
GO


CREATE TABLE fieldStudy
(
    fieldId INT IDENTITY(1,1) PRIMARY KEY,
    fieldName VARCHAR(40) NOT NULL UNIQUE
);
GO

CREATE TABLE student
(
    stdId INT IDENTITY(1,1) PRIMARY KEY,
    stdName VARCHAR(40) NOT NULL,
    stdUserName VARCHAR(40) NOT NULL UNIQUE,
    stdAge INT CHECK (stdAge > 0),
    fieldId INT NOT NULL,
    stdFatherName VARCHAR(40),
    FOREIGN KEY (fieldId) REFERENCES fieldStudy(fieldId)
);
GO

CREATE TABLE StudentFees
(
    feeId INT IDENTITY(1,1) PRIMARY KEY,
    stdId INT NOT NULL,
    amount DECIMAL(10,2) CHECK (amount >= 0),
    paymentDate DATE DEFAULT GETDATE(),
    status VARCHAR(20) DEFAULT 'Pending',
    FOREIGN KEY (stdId) REFERENCES student(stdId)
);
GO


CREATE TABLE course
(
    courseId INT IDENTITY(1,1) PRIMARY KEY,
    courseTitle VARCHAR(40) NOT NULL,
    teacherName VARCHAR(40) NOT NULL
);
GO
CREATE TABLE courseRegStd
(
    stdId INT NOT NULL,
    courseId INT NOT NULL,
    regDate DATETIME DEFAULT GETDATE(),
    PRIMARY KEY (stdId, courseId),
    FOREIGN KEY (stdId) REFERENCES student(stdId),
    FOREIGN KEY (courseId) REFERENCES course(courseId)
);
GO
CREATE TABLE Attendance 
(
    attendanceId INT IDENTITY(1,1) PRIMARY KEY,
    stdId INT NOT NULL,
    courseId INT NOT NULL,
    attendanceDate DATE DEFAULT CAST(GETDATE() AS DATE),
    isPresent BIT DEFAULT 0,
    FOREIGN KEY (stdId) REFERENCES student(stdId),
    FOREIGN KEY (courseId) REFERENCES course(courseId),
    CONSTRAINT UC_Attendance UNIQUE (stdId, courseId, attendanceDate)
);
GO

CREATE TABLE result
(
    stdId INT NOT NULL,
    courseId INT NOT NULL,
    gotNumber DECIMAL(5,2) CHECK (gotNumber >= 0),
    grade VARCHAR(2),
    isPass AS (CASE WHEN gotNumber >= 50 THEN 1 ELSE 0 END),
    PRIMARY KEY (stdId, courseId),
    FOREIGN KEY (stdId) REFERENCES student(stdId),
    FOREIGN KEY (courseId) REFERENCES course(courseId)
);
GO
