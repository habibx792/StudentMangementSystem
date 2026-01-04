use dsadb;; 
SELECT * FROM adminTab;
GO

SELECT * FROM fieldStudy;
GO

SELECT * FROM student;
GO

SELECT * FROM StudentFees;
GO

SELECT * FROM course;
GO

SELECT * FROM courseRegStd;
GO
-- Test cascade delete
DELETE FROM student WHERE stdId = 1001;
select * from student;
-- Check if related records were deleted
SELECT * FROM StudentFees WHERE stdId = 1001; -- Should be 0 rows
SELECT * FROM courseRegStd WHERE stdId = 1001; -- Should be 0 rows
SELECT * FROM Attendance WHERE stdId = 1001; -- Should be 0 rows
SELECT * FROM result WHERE stdId = 1001; -- Should be 0 rows
SELECT * FROM Attendance;
GO

SELECT * FROM result;
GO
