#define _HAS_STD_BYTE 0
#ifndef RESULT_H
#define RESULT_H

#include <iostream>
#include <string>
using namespace std;

class Result
{
private:
    int stdId;
    int courseId;
    double gotNumber;
    string grade;

public:
    Result()
    {
        this->stdId = -1;
        this->courseId = -1;
        this->gotNumber = 0.0;
        this->grade = "F";
    }

    Result(int stdId, int courseId, double gotNumber, const string &grade)
    {
        this->stdId = stdId;
        this->courseId = courseId;
        this->gotNumber = gotNumber;
        this->grade = grade;
    }

    int getStdId() const
    {
        return this->stdId;
    }

    int getCourseId() const
    {
        return this->courseId;
    }

    double getGotNumber() const
    {
        return this->gotNumber;
    }

    string getGrade() const
    {
        return this->grade;
    }

    bool isPass() const
    {
        return this->gotNumber >= 50;
    }

    void setStdId(int id)
    {
        this->stdId = id;
    }

    void setCourseId(int id)
    {
        this->courseId = id;
    }

    void setGotNumber(double num)
    {
        this->gotNumber = num;
    }

    void setGrade(const string &g)
    {
        this->grade = g;
    }

    void print() const
    {
        cout << "Student ID: " << this->stdId
             << " | Course ID: " << this->courseId
             << " | Score: " << this->gotNumber
             << " | Grade: " << this->grade
             << " | Pass: " << (this->isPass() ? "Yes" : "No") << endl;
    }

    int getId() const
    {
        // Create unique ID for BST (stdId * 1000 + courseId)
        return (this->stdId * 1000) + this->courseId;
    }
};

#endif // RESULT_H