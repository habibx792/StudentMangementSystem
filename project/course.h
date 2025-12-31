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

    void print()
    {
        cout << "courseID : " << courseId << endl;
        cout << "Course Title : " << courseTitle << endl;
        cout << "Teacher Name : " << teacherName << endl;
    }

    int getId()
    {
        return courseId;
    }

    string getCourseTitle()
    {
        return courseTitle;
    }

    string getTeacherName()
    {
        return teacherName;
    }
};

#endif // COURSE_H