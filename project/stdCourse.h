#define _HAS_STD_BYTE 0
#ifndef STDCOURSE_H
#define STDCOURSE_H

#include <iostream>
#include <string>
using namespace std;

class StdCourse
{
private:
    int stdId;
    int courseId;
    string regDate;

public:
    StdCourse()
    {
        this->stdId = -1;
        this->courseId = -1;
        this->regDate = "1970-01-01";
    }

    StdCourse(int stdId, int courseId, const string &regDate)
    {
        this->stdId = stdId;
        this->courseId = courseId;
        this->regDate = regDate;
    }

    int getStdId() const
    {
        return this->stdId;
    }

    int getCourseId() const
    {
        return this->courseId;
    }

    string getRegDate() const
    {
        return this->regDate;
    }

    void setStdId(int id)
    {
        this->stdId = id;
    }

    void setCourseId(int id)
    {
        this->courseId = id;
    }

    void setRegDate(const string &date)
    {
        this->regDate = date;
    }

    void print() const
    {
        cout << "Student ID: " << this->stdId
             << " | Course ID: " << this->courseId
             << " | Registration Date: " << this->regDate << endl;
    }

    int getId() const
    {
        // Create unique ID for BST (stdId * 1000 + courseId)
        return (this->stdId * 1000) + this->courseId;
    }
};

#endif // STDCOURSE_H