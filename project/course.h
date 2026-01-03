#define _HAS_STD_BYTE 0
#ifndef COURSE_H
#define COURSE_H
#include <iomanip>
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
        cout<<"===========================================\n";
        cout << left
             << setw(15) << "Course ID:" << courseId << endl
             << setw(15) << "Course Title:" << courseTitle << endl
             << setw(15) << "Teacher Name:" << teacherName << endl;
        cout<<"===========================================\n";
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