USE SM_System;
GO

-- 1. Insert more field studies
INSERT INTO fieldStudy (fieldName) VALUES 
('Mechanical Engineering'),
('Psychology'),
('Mathematics'),
('Chemistry'),
('Architecture');
GO

---- 2. Insert students (20 students)
--INSERT INTO student (stdId, stdName, stdUserName, stdAge, fieldId, stdFatherName) VALUES
--(1001, 'Ali Ahmed', 'ali.ahmed', 20, 1022, 'Ahmed Khan'),      -- Architecture
--(1002, 'Sara Khan', 'sara.khan', 21, 1019, 'Khalid Khan'),    -- Psychology
--(1003, 'Muhammad Bilal', 'm.bilal', 22, 1021, 'Bilal Ahmed'), -- Chemistry
--(1004, 'Fatima Zara', 'fatima.z', 19, 1020, 'Zafar Ali'),     -- Mathematics
--(1005, 'Omar Farooq', 'omar.f', 23, 1022, 'Farooq Ahmed'),   -- Architecture
--(1006, 'Ayesha Siddiqui', 'ayesha.s', 20, 1018, 'Siddiqui Raza'), -- Mechanical
--(1007, 'Hassan Raza', 'h.raza', 21, 1021, 'Raza Muhammad'),  -- Chemistry
--(1008, 'Zainab Malik', 'zainab.m', 22, 1019, 'Malik Akbar'), -- Psychology
--(1009, 'Usman Ghani', 'usman.g', 20, 1020, 'Ghaniullah'),    -- Mathematics
--(1010, 'Nadia Shah', 'nadia.shah', 21, 1018, 'Shah Jahan'),  -- Mechanical
--(1011, 'Kamran Ali', 'kamran.a', 22, 1022, 'Ali Hassan'),    -- Architecture
--(1012, 'Sana Ullah', 'sana.u', 19, 1021, 'Ullah Khan'),      -- Chemistry
--(1013, 'Bilal Yousaf', 'bilal.y', 23, 1019, 'Yousaf Khan'),  -- Psychology
--(1014, 'Mehwish Akram', 'mehwish.a', 20, 1020, 'Akram Chaudhry'), -- Mathematics
--(1015, 'Imran Hashmi', 'imran.h', 21, 1018, 'Hashmi Raza'),  -- Mechanical
--(1016, 'Rabia Arif', 'rabia.a', 22, 1022, 'Arif Mehmood'),   -- Architecture
--(1017, 'Faisal Iqbal', 'faisal.i', 20, 1021, 'Iqbal Ahmed'), -- Chemistry
--(1018, 'Saima Noor', 'saima.n', 21, 1019, 'Noor Muhammad'),  -- Psychology
--(1019, 'Tariq Javed', 'tariq.j', 23, 1020, 'Javed Iqbal'),   -- Mathematics
--(1020, 'Hina Aslam', 'hina.a', 20, 1018, 'Aslam Khan');      -- Mechanical
--GO



INSERT INTO student (stdId, stdName, stdUserName, stdAge, fieldId, stdFatherName) VALUES
(1001, 'Ali Ahmed', 'ali.ahmed', 20, 17, 'Ahmed Khan'),      -- Architecture
(1002, 'Sara Khan', 'sara.khan', 21, 14, 'Khalid Khan'),    -- Psychology
(1003, 'Muhammad Bilal', 'm.bilal', 22, 16, 'Bilal Ahmed'), -- Chemistry
(1004, 'Fatima Zara', 'fatima.z', 19, 15, 'Zafar Ali'),     -- Mathematics
(1005, 'Omar Farooq', 'omar.f', 23, 17, 'Farooq Ahmed'),   -- Architecture
(1006, 'Ayesha Siddiqui', 'ayesha.s', 20, 13, 'Siddiqui Raza'), -- Mechanical
(1007, 'Hassan Raza', 'h.raza', 21, 16, 'Raza Muhammad'),  -- Chemistry
(1008, 'Zainab Malik', 'zainab.m', 22, 14, 'Malik Akbar'), -- Psychology
(1009, 'Usman Ghani', 'usman.g', 20, 15, 'Ghaniullah'),    -- Mathematics
(1010, 'Nadia Shah', 'nadia.shah', 21, 13, 'Shah Jahan'),  -- Mechanical
(1011, 'Kamran Ali', 'kamran.a', 22, 17, 'Ali Hassan'),    -- Architecture
(1012, 'Sana Ullah', 'sana.u', 19, 16, 'Ullah Khan'),      -- Chemistry
(1013, 'Bilal Yousaf', 'bilal.y', 23, 14, 'Yousaf Khan'),  -- Psychology
(1014, 'Mehwish Akram', 'mehwish.a', 20, 15, 'Akram Chaudhry'), -- Mathematics
(1015, 'Imran Hashmi', 'imran.h', 21, 13, 'Hashmi Raza'),  -- Mechanical
(1016, 'Rabia Arif', 'rabia.a', 22, 17, 'Arif Mehmood'),   -- Architecture
(1017, 'Faisal Iqbal', 'faisal.i', 20, 16, 'Iqbal Ahmed'), -- Chemistry
(1018, 'Saima Noor', 'saima.n', 21, 14, 'Noor Muhammad'),  -- Psychology
(1019, 'Tariq Javed', 'tariq.j', 23, 15, 'Javed Iqbal'),   -- Mathematics
(1020, 'Hina Aslam', 'hina.a', 20, 13, 'Aslam Khan');      -- Mechanical
GO

-- 3. Insert courses (8 courses)
INSERT INTO course (courseId, courseTitle, teacherName) VALUES
(101, 'Database Systems', 'Dr. Ahmed Raza'),
(102, 'Data Structures', 'Prof. Sara Khan'),
(103, 'Calculus I', 'Dr. Muhammad Ali'),
(104, 'Electrical Circuits', 'Prof. Hamid Ullah'),
(105, 'Business Ethics', 'Dr. Fatima Shah'),
(106, 'Organic Chemistry', 'Prof. Yasir Mahmood'),
(107, 'Programming Fundamentals', 'Dr. Bilal Ahmed'),
(108, 'Human Anatomy', 'Prof. Sana Javed');
GO

-- 4. Insert student fees (multiple payments per student)
INSERT INTO StudentFees (stdId, amount, paymentDate, status) VALUES
-- Student 1001
(1001, 5000.00, '2024-01-15', 'Paid'),
(1001, 5000.00, '2024-02-10', 'Paid'),
(1001, 5000.00, '2024-03-12', 'Pending'),

-- Student 1002
(1002, 6000.00, '2024-01-20', 'Paid'),
(1002, 6000.00, '2024-02-15', 'Paid'),

-- Student 1003
(1003, 5500.00, '2024-01-18', 'Paid'),
(1003, 5500.00, '2024-02-20', 'Pending'),

-- Student 1004
(1004, 4500.00, '2024-01-22', 'Paid'),

-- Student 1005
(1005, 5000.00, '2024-01-25', 'Paid'),
(1005, 5000.00, '2024-02-18', 'Paid'),
(1005, 5000.00, '2024-03-20', 'Paid'),

-- Student 1006
(1006, 7000.00, '2024-01-10', 'Paid'),

-- Student 1007
(1007, 5500.00, '2024-01-30', 'Paid'),
(1007, 5500.00, '2024-02-25', 'Pending'),

-- Student 1008
(1008, 6500.00, '2024-01-05', 'Paid'),

-- Student 1009
(1009, 4800.00, '2024-01-28', 'Paid'),

-- Student 1010
(1010, 7200.00, '2024-01-15', 'Paid'),
(1010, 7200.00, '2024-02-12', 'Paid'),

-- Student 1011
(1011, 5000.00, '2024-01-20', 'Paid'),

-- Student 1012
(1012, 5800.00, '2024-01-25', 'Pending'),

-- Student 1013
(1013, 6200.00, '2024-01-18', 'Paid'),

-- Student 1014
(1014, 4900.00, '2024-01-22', 'Paid'),

-- Student 1015
(1015, 7100.00, '2024-01-30', 'Paid'),

-- Student 1016
(1016, 5100.00, '2024-01-05', 'Paid'),

-- Student 1017
(1017, 5600.00, '2024-01-28', 'Pending'),

-- Student 1018
(1018, 6300.00, '2024-01-15', 'Paid'),

-- Student 1019
(1019, 4700.00, '2024-01-20', 'Paid'),

-- Student 1020
(1020, 6900.00, '2024-01-25', 'Paid');
GO

-- 5. Insert course registrations (each student registered for 3-5 courses)
INSERT INTO courseRegStd (stdId, courseId, regDate) VALUES
-- Student 1001 (Computer Science) takes CS courses
(1001, 101, '2024-01-10 09:00:00'),
(1001, 102, '2024-01-10 09:30:00'),
(1001, 107, '2024-01-11 10:00:00'),
(1001, 103, '2024-01-11 11:00:00'),

-- Student 1002 (Computer Science)
(1002, 101, '2024-01-10 10:00:00'),
(1002, 102, '2024-01-10 10:30:00'),
(1002, 107, '2024-01-11 11:00:00'),

-- Student 1003 (Electrical Engineering)
(1003, 104, '2024-01-12 09:00:00'),
(1003, 103, '2024-01-12 10:00:00'),
(1003, 101, '2024-01-12 11:00:00'),

-- Student 1004 (Business Administration)
(1004, 105, '2024-01-13 09:00:00'),
(1004, 101, '2024-01-13 10:00:00'),
(1004, 103, '2024-01-13 11:00:00'),

-- Student 1005 (Computer Science)
(1005, 101, '2024-01-14 09:00:00'),
(1005, 102, '2024-01-14 10:00:00'),
(1005, 107, '2024-01-14 11:00:00'),
(1005, 103, '2024-01-14 12:00:00'),

-- Student 1006 (Medicine)
(1006, 108, '2024-01-15 09:00:00'),
(1006, 103, '2024-01-15 10:00:00'),
(1006, 105, '2024-01-15 11:00:00'),

-- Student 1007 (Electrical Engineering)
(1007, 104, '2024-01-16 09:00:00'),
(1007, 103, '2024-01-16 10:00:00'),
(1007, 102, '2024-01-16 11:00:00'),

-- Student 1008 (Law)
(1008, 105, '2024-01-17 09:00:00'),
(1008, 101, '2024-01-17 10:00:00'),

-- Student 1009 (Business Administration)
(1009, 105, '2024-01-18 09:00:00'),
(1009, 101, '2024-01-18 10:00:00'),
(1009, 103, '2024-01-18 11:00:00'),

-- Student 1010 (Medicine)
(1010, 108, '2024-01-19 09:00:00'),
(1010, 103, '2024-01-19 10:00:00'),
(1010, 105, '2024-01-19 11:00:00'),
(1010, 101, '2024-01-19 12:00:00'),

-- Add more registrations for other students (similarly)
(1011, 101, '2024-01-20 09:00:00'),
(1011, 102, '2024-01-20 10:00:00'),
(1011, 107, '2024-01-20 11:00:00'),

(1012, 104, '2024-01-21 09:00:00'),
(1012, 103, '2024-01-21 10:00:00'),

(1013, 105, '2024-01-22 09:00:00'),
(1013, 101, '2024-01-22 10:00:00'),
(1013, 103, '2024-01-22 11:00:00'),

(1014, 105, '2024-01-23 09:00:00'),
(1014, 101, '2024-01-23 10:00:00'),

(1015, 108, '2024-01-24 09:00:00'),
(1015, 103, '2024-01-24 10:00:00'),

(1016, 101, '2024-01-25 09:00:00'),
(1016, 102, '2024-01-25 10:00:00'),
(1016, 107, '2024-01-25 11:00:00'),

(1017, 104, '2024-01-26 09:00:00'),
(1017, 103, '2024-01-26 10:00:00'),

(1018, 105, '2024-01-27 09:00:00'),
(1018, 101, '2024-01-27 10:00:00'),

(1019, 105, '2024-01-28 09:00:00'),
(1019, 103, '2024-01-28 10:00:00'),

(1020, 108, '2024-01-29 09:00:00'),
(1020, 103, '2024-01-29 10:00:00'),
(1020, 105, '2024-01-29 11:00:00');
GO

-- 6. Insert attendance records (for January 2024, random attendance)
INSERT INTO Attendance (stdId, courseId, attendanceDate, isPresent) VALUES
-- Sample attendance for first week of January for few students
-- Course 101 (Database Systems)
(1001, 101, '2024-01-02', 1),
(1002, 101, '2024-01-02', 1),
(1003, 101, '2024-01-02', 0),
(1004, 101, '2024-01-02', 1),
(1005, 101, '2024-01-02', 1),

(1001, 101, '2024-01-04', 1),
(1002, 101, '2024-01-04', 0),
(1003, 101, '2024-01-04', 1),
(1004, 101, '2024-01-04', 1),
(1005, 101, '2024-01-04', 1),

(1001, 101, '2024-01-06', 1),
(1002, 101, '2024-01-06', 1),
(1003, 101, '2024-01-06', 1),
(1004, 101, '2024-01-06', 0),
(1005, 101, '2024-01-06', 1),

-- Course 102 (Data Structures)
(1001, 102, '2024-01-03', 1),
(1002, 102, '2024-01-03', 1),
(1005, 102, '2024-01-03', 0),
(1006, 102, '2024-01-03', 1),

(1001, 102, '2024-01-05', 1),
(1002, 102, '2024-01-05', 0),
(1005, 102, '2024-01-05', 1),
(1006, 102, '2024-01-05', 1),

-- Course 104 (Electrical Circuits)
(1003, 104, '2024-01-02', 1),
(1007, 104, '2024-01-02', 1),
(1012, 104, '2024-01-02', 0),

(1003, 104, '2024-01-04', 1),
(1007, 104, '2024-01-04', 0),
(1012, 104, '2024-01-04', 1),

-- Course 105 (Business Ethics)
(1004, 105, '2024-01-03', 1),
(1006, 105, '2024-01-03', 1),
(1008, 105, '2024-01-03', 1),
(1009, 105, '2024-01-03', 0),

(1004, 105, '2024-01-05', 1),
(1006, 105, '2024-01-05', 0),
(1008, 105, '2024-01-05', 1),
(1009, 105, '2024-01-05', 1),

-- Course 108 (Human Anatomy)
(1006, 108, '2024-01-04', 1),
(1010, 108, '2024-01-04', 1),
(1015, 108, '2024-01-04', 1),
(1020, 108, '2024-01-04', 0),

(1006, 108, '2024-01-06', 1),
(1010, 108, '2024-01-06', 1),
(1015, 108, '2024-01-06', 0),
(1020, 108, '2024-01-06', 1);
GO

insert into adminTab (adminName,passWord)
values ('admin','123');
-- 7. Insert results
INSERT INTO result (stdId, courseId, gotNumber, grade) VALUES
-- Course 101 Results
(1001, 101, 85.5, 'A'),
(1002, 101, 78.0, 'B'),
(1003, 101, 92.5, 'A+'),
(1004, 101, 65.0, 'C'),
(1005, 101, 88.0, 'A'),
(1008, 101, 45.0, 'F'),
(1009, 101, 72.5, 'B'),
(1010, 101, 81.0, 'B+'),
(1011, 101, 69.0, 'C+'),
(1013, 101, 53.0, 'D'),

-- Course 102 Results
(1001, 102, 90.0, 'A+'),
(1002, 102, 82.5, 'B+'),
(1005, 102, 76.0, 'B'),
(1006, 102, 89.5, 'A'),
(1007, 102, 94.0, 'A+'),
(1011, 102, 61.0, 'C'),
(1016, 102, 79.0, 'B'),

-- Course 103 Results
(1001, 103, 87.5, 'A'),
(1003, 103, 91.0, 'A+'),
(1004, 103, 73.0, 'B'),
(1005, 103, 68.5, 'C+'),
(1006, 103, 82.0, 'B+'),
(1007, 103, 77.5, 'B'),
(1009, 103, 84.0, 'B+'),
(1010, 103, 58.0, 'D+'),
(1012, 103, 49.0, 'F'),
(1013, 103, 66.0, 'C'),
(1015, 103, 71.0, 'B-'),
(1017, 103, 88.5, 'A'),
(1019, 103, 63.0, 'C'),
(1020, 103, 79.5, 'B'),

-- Course 104 Results
(1003, 104, 95.0, 'A+'),
(1007, 104, 86.0, 'A'),
(1012, 104, 74.0, 'B'),
(1017, 104, 81.5, 'B+'),

-- Course 105 Results
(1004, 105, 89.0, 'A'),
(1006, 105, 92.5, 'A+'),
(1008, 105, 76.5, 'B'),
(1009, 105, 83.0, 'B+'),
(1010, 105, 67.0, 'C+'),
(1013, 105, 71.0, 'B-'),
(1014, 105, 78.5, 'B'),
(1018, 105, 64.0, 'C'),
(1019, 105, 59.5, 'D+'),
(1020, 105, 87.0, 'A'),

-- Course 107 Results
(1001, 107, 93.0, 'A+'),
(1002, 107, 85.5, 'A'),
(1005, 107, 79.0, 'B'),
(1011, 107, 88.0, 'A'),
(1016, 107, 91.5, 'A+'),

-- Course 108 Results
(1006, 108, 84.0, 'B+'),
(1010, 108, 90.5, 'A+'),
(1015, 108, 77.0, 'B'),
(1020, 108, 82.5, 'B+');
GO

