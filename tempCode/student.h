#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

class Student
{
private:
    string stdName;
    int stdAge;
    int fieldId;
    string stdFatherName;

public:
    Student(string name, int age, int fieldId, string fatherName)
        : stdName(name), stdAge(age), fieldId(fieldId), stdFatherName(fatherName) {}

    Student()
        : stdName(""), stdFatherName(""), stdAge(0), fieldId(0) {}
    ~Student() = default;
    void setName(const string &name) { stdName = name; }
    void setAge(int age) { stdAge = age; }
    void setFieldId(int id) { fieldId = id; }
    void setFatherName(const string &fatherName) { stdFatherName = fatherName; }

    string getName() const { return stdName; }
    int getAge() const { return stdAge; }
    int getFieldId() const { return fieldId; }
    string getFatherName() const { return stdFatherName; }
};

#endif
