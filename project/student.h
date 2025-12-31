#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
using namespace std;

class Student
{
private:
    int stdId;
    string stdName;
    string stdUserName;
    int stdAge;
    int fieldId;
    string stdFatherName;

public:
    // Constructors
    Student()
    {
        this->stdId = -1;
        this->stdName = "";
        this->stdUserName = "";
        this->stdAge = 0;
        this->fieldId = -1;
        this->stdFatherName = "";
    }

    Student(int id, string name, string userName, int age, int fId, string fatherName)
    {
        this->stdId = id;
        this->stdName = name;
        this->stdUserName = userName;
        this->stdAge = age;
        this->fieldId = fId;
        this->stdFatherName = fatherName;
    }

    // ===== Getters =====
    int getStdId() const
    {
        return this->stdId;
    }

    string getStdName() const
    {
        return this->stdName;
    }

    string getStdUserName() const
    {
        return this->stdUserName;
    }

    int getStdAge() const
    {
        return this->stdAge;
    }

    int getFieldId() const
    {
        return this->fieldId;
    }

    string getStdFatherName() const
    {
        return this->stdFatherName;
    }

    // ===== Setters =====
    void setStdId(int id)
    {
        this->stdId = id;
    }

    void setStdName(const string &name)
    {
        this->stdName = name;
    }

    void setStdUserName(const string &userName)
    {
        this->stdUserName = userName;
    }

    void setStdAge(int age)
    {
        this->stdAge = age;
    }

    void setFieldId(int fId)
    {
        this->fieldId = fId;
    }

    void setStdFatherName(const string &fatherName)
    {
        this->stdFatherName = fatherName;
    }

    // ===== Print =====
    void print() const
    {
        cout << "Student ID: " << this->stdId << endl;
        cout << "Name: " << this->stdName << endl;
        cout << "Username: " << this->stdUserName << endl;
        cout << "Age: " << this->stdAge << endl;
        cout << "Field ID: " << this->fieldId << endl;
        cout << "Father's Name: " << this->stdFatherName << endl;
    }

    // For BST compatibility
    int getId() const
    {
        return this->stdId;
    }

    // For BST sorting
    bool operator<(const Student &other) const
    {
        return this->stdId < other.stdId;
    }

    bool operator>(const Student &other) const
    {
        return this->stdId > other.stdId;
    }
};

#endif // STUDENT_H