use stdMangementDB;
INSERT INTO adminTab (adminName, passWord)
VALUES
('admin','123asd');
INSERT INTO fieldStudy (fieldName)
VALUES
('BSSC'),
('BSSE'),
('BSAI')
select * from fieldStudy;
INSERT INTO student (stdName, stdUserName, stdAge, fieldId, stdFatherName)
VALUES
('Ali','ali01',20,11,'Ahmed'),
('Sara','sara01',21,12,'Khan'),
('Usman','usman01',22,13,'Iqbal'),
('Ayesha','ayesha01',19,12,'Hassan'),
('Bilal','bilal01',23,11,'Rashid'),
('Hina','hina01',20,13,'Nadeem'),
('Zain','zain01',24,12,'Tariq'),
('Fatima','fatima01',21,11,'Yousaf'),
('Hamza','hamza01',22,13,'Akram'),
('Noor','noor01',20,12,'Aslam');
select * from student;

INSERT INTO StudentFees (stdId, amount, status)
VALUES
(18,50000,'Paid'),
(19,48000,'Paid'),
(20,45000,'Pending'),
(21,45000,'Pending'),
(22,47000,'Paid'),
(23,52000,'Pending'),
(24,46000,'Paid'),
(25,53000,'Paid'),
(26,44000,'Pending'),
(27,49000,'Paid');
select * from StudentFees;
INSERT INTO course (courseTitle, teacherName)
VALUES
('Database Systems','Dr. Khan'),
('Data Structures','Prof. Ali'),
('Operating Systems','Dr. Ahmed'),
('Artificial Intelligence','Dr. Sara'),
('Computer Networks','Prof. Usman'),
('Web Development','Ms. Hina'),
('Machine Learning','Dr. Bilal'),
('Cyber Security','Dr. Zain'),
('Discrete Math','Prof. Fatima'),
('Software Engineering','Dr. Hamza');
select * from course;
INSERT INTO courseRegStd (stdId, courseId)
VALUES
(18,21),
(19,22),
(20,23),
(21,24),
(22,25),
(23,26),
(24,27),
(25,28),
(26,29),
(27,30);


INSERT INTO Attendance (stdId, courseId, isPresent)
VALUES
(18,21,1),
(19,22,1),
(20,23,1),
(21,24,1),
(22,25,1),
(23,26,0),
(24,27,0),
(25,28,1),
(26,29,1),
(27,30,0);

INSERT INTO result (stdId, courseId, gotNumber, grade)
VALUES
 (18,21,78,'A'),
    (19,22,82,'A'),
    (20,23,69,'B'),
    (21,24,91,'A'),
    (22,25,65,'B'),
    (23,26,40,'F'),
    (24,27,45,'F'),
    (25,28,88,'A'),
    (26,29,73,'B'),
    (27,30,50,'C');
