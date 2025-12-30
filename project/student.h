#ifndef CLASSNAME_H
#define CLASSNAME_H
#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stireg.h>
#include <sstream>
#include <vector>
#include <limits>

using namespace std;

class Student
{
private:
    // member variables
    string name;
    int age;
    int fieldId;
    string fatherName;
    string userName;
    int id;
    string field;

public:
    // constructors
    Student()
    {
    }

    Student(string name, string userName, int age, int fieldId, string fatherName)
    {
        this->name = name;
        this->age = age;
        this->id = -1;
        this->field="NULL";
        this->fatherName = fatherName;
        this->userName = userName;
        this->fieldId = fieldId;
    }

    // ===== Getters =====
    void setField(string field)
    {
        this->field=field;
    }
    string getField()
    {
        return field;
    }
    int getId()const
    {
        return id;
    }
    void setId(int id)
    {
        this->id = id;
    }
    string getName()const
    {
        return name;
    }

    string getUserName()const
    {
        return userName;
    }

    int getAge()const
    {
        return age;
    }

    int getFieldId()const
    {
        return fieldId;
    }

    string getFatherName()const
    {
        return fatherName;
    }

    // ===== Setters =====
    void setName(string name)
    {
        this->name = name;
    }

    void setUserName(string userName)
    {
        this->userName = userName;
    }

    void setAge(int age)
    {
        this->age = age;
    }

    void setFieldId(int fieldId)
    {
        this->fieldId = fieldId;
    }

    void setFatherName(string fatherName)
    {
        this->fatherName = fatherName;
    }
    void print()
    {
        cout<<"Name : "<<name<<endl;
        cout<<"Age : "<<age<<endl;
        cout<<"UserName : "<<userName<<endl;
        cout<<"FatherName : "<<fatherName<<endl;
        cout<<"Field Id : "<<endl;
    }
};

#endif // CLASSNAME_H
