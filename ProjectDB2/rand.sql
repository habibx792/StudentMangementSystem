-- Create database if it doesn't exist
CREATE DATABASE mydb;
GO

USE mydb;
GO

-- Drop table if exists
IF OBJECT_ID('std', 'U') IS NOT NULL
    DROP TABLE std;
GO

-- Create table with auto-increment ID and correct column names
CREATE TABLE std
(
    stdId INT IDENTITY(1,1) PRIMARY KEY,
    stdName VARCHAR(40),
    age INT,
    grade INT
);
GO

-- Insert initial test data
INSERT INTO std (stdName, age, grade)
VALUES 
    ('habib', 20, 10),
    ('ali', 21, 11),
    ('sara', 19, 9);
GO

-- Display all students
SELECT * FROM std;
GO
-- Create a backup of your data
SELECT * INTO std_backup FROM std;

-- Drop and recreate the table without IDENTITY
DROP TABLE std;

CREATE TABLE std (
    stdId INT NOT NULL PRIMARY KEY,
    stdName VARCHAR(40),
    age INT,
    grade INT
);

-- Restore your data
INSERT INTO std(stdId, stdName, age, grade)
SELECT stdId, stdName, age, grade FROM std_backup;

DROP TABLE std_backup;