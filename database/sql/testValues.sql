-- Insert Field of Study data
-- ===example data ====== --
use SM_System
INSERT INTO fieldStudy
   (fieldName)
VALUES
   ('Computer Science'),
   ('Electrical Engineering'),
   ('Business Administration'),
   ('Medicine'),
   ('Law'),
   ('Arts and Humanities'),
   ('Mechanical Engineering'),
   ('Civil Engineering'),
   ('Biotechnology'),
   ('Psychology');
GO

-- Insert Admin data
INSERT INTO adminTab
   (adminName, passWord)
VALUES
   ('admin1', 'password123'),
   ('admin2', 'securepass456'),
   ('admin3', 'admin@2024'),
   ('superadmin', 'super@secure789');
GO

-- Insert Student data (stdId starts from 1000)
INSERT INTO student
   (stdId, stdName, stdUserName, stdAge, fieldId, stdFatherName)
VALUES
   (1001, 'Ali Khan', 'alikhan01', 20, 1, 'Ahmed Khan'),
   (1002, 'Sara Ahmed', 'saraahmed02', 21, 2, 'Rashid Ahmed'),
   (1003, 'Mohammad Hassan', 'mhassan03', 19, 1, 'Hassan Ali'),
   (1004, 'Fatima Raza', 'fatimaraza04', 22, 3, 'Raza Ahmed'),
   (1005, 'Usman Malik', 'umalik05', 20, 4, 'Malik Shah'),
   (1006, 'Ayesha Bibi', 'ayeshab06', 21, 5, 'Bibi Khan'),
   (1007, 'Bilal Arshad', 'bilala07', 19, 6, 'Arshad Mehmood'),
   (1008, 'Zainab Akhtar', 'zainaba08', 22, 7, 'Akhtar Hussain'),
   (1009, 'Hamza Yousaf', 'hamzay09', 20, 8, 'Yousaf Ali'),
   (1010, 'Hina Shah', 'hinashah10', 21, 9, 'Shah Rukh');
GO

-- Insert Course data (courseId starts from 2000)
INSERT INTO course
   (courseId, courseTitle, teacherName)
VALUES
   (2001, 'Introduction to Programming', 'Dr. Ahmed Raza'),
   (2002, 'Database Systems', 'Prof. Sara Khan'),
   (2003, 'Data Structures', 'Dr. Mohammad Ali'),
   (2004, 'Operating Systems', 'Prof. Fatima Shah'),
   (2005, 'Computer Networks', 'Dr. Usman Malik'),
   (2006, 'Software Engineering', 'Prof. Hina Bibi'),
   (2007, 'Web Development', 'Dr. Bilal Arshad'),
   (2008, 'Mobile App Development', 'Prof. Zainab Akhtar'),
   (2009, 'Artificial Intelligence', 'Dr. Hamza Yousaf'),
   (2010, 'Cyber Security', 'Prof. Hina Shah');
GO

-- Insert Course Registration data
INSERT INTO courseRegStd
   (stdId, courseId, regDate)
VALUES
   (1001, 2001, '2024-01-15'),
   (1001, 2002, '2024-01-15'),
   (1001, 2003, '2024-01-15'),
   (1002, 2001, '2024-01-16'),
   (1002, 2004, '2024-01-16'),
   (1003, 2002, '2024-01-17'),
   (1003, 2005, '2024-01-17'),
   (1004, 2003, '2024-01-18'),
   (1004, 2006, '2024-01-18'),
   (1005, 2004, '2024-01-19'),
   (1005, 2007, '2024-01-19'),
   (1006, 2005, '2024-01-20'),
   (1006, 2008, '2024-01-20'),
   (1007, 2006, '2024-01-21'),
   (1007, 2009, '2024-01-21'),
   (1008, 2007, '2024-01-22'),
   (1008, 2010, '2024-01-22'),
   (1009, 2008, '2024-01-23'),
   (1009, 2001, '2024-01-23'),
   (1010, 2009, '2024-01-24'),
   (1010, 2002, '2024-01-24');
GO

-- Insert Attendance data
INSERT INTO Attendance
   (stdId, courseId, attendanceDate, isPresent)
VALUES
   (1001, 2001, '2024-02-01', 1),
   (1001, 2001, '2024-02-08', 1),
   (1001, 2001, '2024-02-15', 0),
   (1001, 2002, '2024-02-02', 1),
   (1001, 2002, '2024-02-09', 1),
   (1002, 2001, '2024-02-01', 1),
   (1002, 2001, '2024-02-08', 0),
   (1002, 2004, '2024-02-03', 1),
   (1003, 2002, '2024-02-02', 1),
   (1003, 2005, '2024-02-04', 1),
   (1004, 2003, '2024-02-05', 1),
   (1004, 2006, '2024-02-06', 0),
   (1005, 2004, '2024-02-03', 1),
   (1005, 2007, '2024-02-07', 1),
   (1006, 2005, '2024-02-04', 1),
   (1006, 2008, '2024-02-08', 1),
   (1007, 2006, '2024-02-06', 0),
   (1007, 2009, '2024-02-09', 1),
   (1008, 2007, '2024-02-07', 1),
   (1008, 2010, '2024-02-10', 1);
GO

-- Insert Result data
INSERT INTO result
   (stdId, courseId, gotNumber, grade)
VALUES
   (1001, 2001, 85.5, 'A'),
   (1001, 2002, 78.0, 'B+'),
   (1001, 2003, 92.5, 'A+'),
   (1002, 2001, 65.0, 'B'),
   (1002, 2004, 45.0, 'F'),
   (1003, 2002, 88.0, 'A'),
   (1003, 2005, 76.5, 'B+'),
   (1004, 2003, 52.0, 'C'),
   (1004, 2006, 48.5, 'F'),
   (1005, 2004, 67.5, 'B'),
   (1005, 2007, 81.0, 'A-'),
   (1006, 2005, 73.0, 'B'),
   (1006, 2008, 90.5, 'A+'),
   (1007, 2006, 55.0, 'C+'),
   (1007, 2009, 83.5, 'A'),
   (1008, 2007, 79.0, 'B+'),
   (1008, 2010, 91.0, 'A+'),
   (1009, 2008, 62.5, 'C+'),
   (1009, 2001, 87.0, 'A'),
   (1010, 2009, 74.5, 'B'),
   (1010, 2002, 56.0, 'C+');
GO

-- Insert Student Fees data
INSERT INTO StudentFees
   (stdId, amount, paymentDate, status)
VALUES
   (1001, 50000.00, '2024-01-20', 'Paid'),
   (1001, 25000.00, '2024-02-20', 'Paid'),
   (1002, 50000.00, '2024-01-21', 'Paid'),
   (1002, 25000.00, '2024-02-21', 'Pending'),
   (1003, 50000.00, '2024-01-22', 'Paid'),
   (1003, 25000.00, '2024-02-22', 'Paid'),
   (1004, 50000.00, '2024-01-23', 'Paid'),
   (1004, 25000.00, '2024-02-23', 'Pending'),
   (1005, 50000.00, '2024-01-24', 'Paid'),
   (1005, 25000.00, '2024-02-24', 'Paid'),
   (1006, 50000.00, '2024-01-25', 'Paid'),
   (1006, 25000.00, '2024-02-25', 'Pending'),
   (1007, 50000.00, '2024-01-26', 'Paid'),
   (1007, 25000.00, '2024-02-26', 'Paid'),
   (1008, 50000.00, '2024-01-27', 'Paid'),
   (1008, 25000.00, '2024-02-27', 'Pending'),
   (1009, 50000.00, '2024-01-28', 'Paid'),
   (1009, 25000.00, '2024-02-28', 'Paid'),
   (1010, 50000.00, '2024-01-29', 'Paid'),
   (1010, 25000.00, '2024-02-29', 'Pending');
GO

-- Verification queries
   SELECT 'Field of Study' as TableName, COUNT(*) as RecordCount
   FROM fieldStudy
UNION ALL
   SELECT 'Admin', COUNT(*)
   FROM adminTab
UNION ALL
   SELECT 'Student', COUNT(*)
   FROM student
UNION ALL
   SELECT 'Course', COUNT(*)
   FROM course
UNION ALL
   SELECT 'Course Registration', COUNT(*)
   FROM courseRegStd
UNION ALL
   SELECT 'Attendance', COUNT(*)
   FROM Attendance
UNION ALL
   SELECT 'Result', COUNT(*)
   FROM result
UNION ALL
   SELECT 'Student Fees', COUNT(*)
   FROM StudentFees;
GO