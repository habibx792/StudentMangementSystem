#define _HAS_STD_BYTE 0
#ifndef ATTENDANCE_H
#define ATTENDANCE_H
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

class Attendance
{
private:
    int attendanceId;
    int stdId;
    int courseId;
    string attendanceDate;
    bool isPresent;

public:
    Attendance()
    {
        this->attendanceId = -1;
        this->stdId = -1;
        this->courseId = -1;
        this->attendanceDate = "1970-01-01";
        this->isPresent = false;
    }
    Attendance(int attendanceId, int stdId, int courseId, const string &date, bool present)
    {
        this->attendanceId = attendanceId;
        this->stdId = stdId;
        this->courseId = courseId;
        this->attendanceDate = date;
        this->isPresent = present;
    }

    Attendance(int stdId, int courseId, const string &date, bool present)
    {
        this->attendanceId = -1;
        this->stdId = stdId;
        this->courseId = courseId;
        this->attendanceDate = date;
        this->isPresent = present;
    }

    int getAttendanceId() const
    {
        return this->attendanceId;
    }

    int getStdId() const
    {
        return this->stdId;
    }

    int getCourseId() const
    {
        return this->courseId;
    }

    string getAttendanceDate() const
    {
        return this->attendanceDate;
    }

    bool getIsPresent() const
    {
        return this->isPresent;
    }

    void setAttendanceId(int id)
    {
        this->attendanceId = id;
    }

    void setStdId(int id)
    {
        this->stdId = id;
    }

    void setCourseId(int id)
    {
        this->courseId = id;
    }

    void setAttendanceDate(const string &date)
    {
        this->attendanceDate = date;
    }

    void setIsPresent(bool present)
    {
        this->isPresent = present;
    }

    void print() const
    {
        cout<<"===========================================\n";
        cout << left
             << setw(15) << "Attendance ID:" << attendanceId << endl
             << setw(15) << "Student ID:" << stdId << endl
             << setw(15) << "Course ID:" << courseId << endl
             << setw(15) << "Date:" << attendanceDate << endl
             << setw(15) << "Present:" << (isPresent ? "Yes" : "No") << endl;
        cout<<"===========================================\n";
    }

    int getId() const
    {
        return this->attendanceId;
    }
};

#endif // ATTENDANCE_H