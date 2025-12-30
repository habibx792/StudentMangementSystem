create database mydb;
use  mydb;
create table std
(
	stdId int not null primary key,
	stdName varchar(40),
	age int,
	class int
);
insert into std(stdId,stdName,age,class)
values(1,'habib',20,10);
select * from std;
EXEC sp_rename 'std.class', 'grade', 'COLUMN';
