use stdMangementDB2;
INSERT INTO adminTab (adminName, passWord)
VALUES
('admin','123asd');
INSERT INTO fieldStudy (fieldId,fieldName)
VALUES
(1,'BSSC'),
(2,'BSSE'),
(3,'BSAI')
select * from fieldStudy;
INSERT INTO student (stdId,stdName, stdUserName, stdAge, fieldId, stdFatherName)
VALUES
(1,'Ali','ali01',20,1,'Ahmed'),
(2,'Sara','sara01',21,2,'Khan'),
(3,'Usman','usman01',22,3,'Iqbal');
select * from student;

INSERT INTO StudentFees (feeId,stdId, amount, status)
VALUES
(12,1,50000,'Paid'),
(13,2,48000,'Paid'),
(14,3,45000,'Pending')
;
select * from StudentFees;
INSERT INTO course (courseId,courseTitle, teacherName)
VALUES
(14,'Database Systems','Dr. Khan'),
(15,'Data Structures','Prof. Ali')
;
select * from course;
INSERT INTO courseRegStd (stdId, courseId)
VALUES
(1,12),
(2,13);


INSERT INTO Attendance (stdId, courseId, isPresent)
VALUES
(1,12,1),
(2,13,1);

INSERT INTO result (stdId, courseId, gotNumber, grade)
VALUES
 (1,12,78,'A'),
    (2,13,82,'A');
