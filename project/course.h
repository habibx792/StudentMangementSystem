#define _HAS_STD_BYTE 0
#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <string>
using namespace std;

class Course
{
private:
    int courseId;
    string courseTitle;
    string teacherName;

public:
    // Default constructor
    Course()
    {
        this->courseId = -1;
        this->courseTitle = "";
        this->teacherName = "";
    }

    Course(string title, string Tname)
    {
        this->courseTitle = title;
        this->teacherName = Tname;
        this->courseId = -1;
    }

    Course(int id, string title, string tName)
    {
        this->teacherName = tName;
        this->courseId = id;
        this->courseTitle = title;
    }

    void setId(int id)
    {
        this->courseId = id;
    }

    void setCourseTitle(const string &title)
    {
        this->courseTitle = title;
    }

    void setTeacherName(const string &teacher)
    {
        this->teacherName = teacher;
    }

    void print() const
    {
        cout << "courseID : " << courseId << endl;
        cout << "Course Title : " << courseTitle << endl;
        cout << "Teacher Name : " << teacherName << endl;
    }

    int getId() const
    {
        return courseId;
    }

    string getCourseTitle() const
    {
        return courseTitle;
    }

    string getTeacherName() const
    {
        return teacherName;
    }
};

#endif // COURSE_H