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
    Result() : stdId(-1), courseId(-1), gotNumber(0), grade("F") {}

    Result(int stdId, int courseId, double gotNumber, string grade)
        : stdId(stdId), courseId(courseId), gotNumber(gotNumber), grade(grade) {}

    // Getters
    int getStdId() const { return stdId; }
    int getCourseId() const { return courseId; }
    double getGotNumber() const { return gotNumber; }
    string getGrade() const { return grade; }
    bool isPass() const { return gotNumber >= 50; }

    // Setters
    void setStdId(int id) { stdId = id; }
    void setCourseId(int id) { courseId = id; }
    void setGotNumber(double num) { gotNumber = num; }
    void setGrade(const string &g) { grade = g; }
    void print() const
    {
        cout << "Student ID: " << stdId
             << " | Course ID: " << courseId
             << " | Score: " << gotNumber
             << " | Grade: " << grade
             << " | Pass: " << (isPass() ? "Yes" : "No") << endl;
    }
    int getId() const { return stdId * 10000 + courseId; } 
};

#endif // RESULT_H
