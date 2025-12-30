#ifndef Course_H
#define Course_H
#include <iostream>
#include <string>
using namespace std;

class Course
{
private:
    // member variables
    string courseTitle;
    int courseId;
    string teacherName;

public:
    Course();
    Course(string title, int id, string teachName)
    {
        this->courseTitle = title;
        this->courseId = id;
        this->teacherName = teachName;
    }
    void makeCourse(string title, int id, string teachName)
    {
        this->courseTitle = title;
        this->courseId = id;
        this->teacherName = teachName;
    }
    string getTitle()
    {
        return courseTitle;
    }
    int getId()
    {
        return courseId;
    }
    string getTeacherName()
    {
        return teacherName;
    }
    void coursePrint()
    {
        cout << "Title : " << courseTitle << endl;
        cout << "ID : " << courseId << endl;
        cout << "Teacher : " << teacherName << endl;
    }
};
#endif // Course_H