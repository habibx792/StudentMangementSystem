#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

class Student
{
private:
    int id;
    string stdName;
    int stdAge;
    int fieldId;
    string stdFatherName;

public:
    Student(int id, string name, int age, int fieldId, string fatherName)
    {
        this->id = id;
        this->stdName = name;
        this->stdAge = age;
        this->fieldId = fieldId;
        this->stdFatherName = fatherName;
    }

    Student()
        : stdName(""), stdFatherName(""), stdAge(0), fieldId(0) {}
    ~Student() = default;
    void setName(const string &name) { stdName = name; }
    void setAge(int age) { stdAge = age; }
    void setFieldId(int id) { fieldId = id; }
    void setFatherName(const string &fatherName) { stdFatherName = fatherName; }
    void printStd()
    {
        cout << "Name : " << this->stdName << endl;
        cout << "age : " << this->stdAge << endl;
    }
    string getName() const
    {
        return stdName;
    }
    int getAge() const
    {
        return stdAge;
    }
    int getId()
    {
        return id;
    }
    int getFieldId() const
    {
        return fieldId;
    }
    string getFatherName() const
    {
        return stdFatherName;
    }
};

#endif
