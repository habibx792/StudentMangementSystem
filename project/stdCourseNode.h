#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stireg.h>
#include <sstream>
#include <vector>
#include <limits>
#include <string>
using namespace std;
#ifndef Student_H
#define Student_H

class Student {


private:
    // member variables
    string name;
    int age;
    int fieldId;
    string fatherName;
    string userName;

public:
    // constructors
    Student()
    {

    }
    Student(string name,string userName,int age,int fieldId,string fatherName)
    {
        
    }

    // getters
    string getmemberName() const;

    // setters
    void setmemberName(const string& value);
};

#endif // CLASSNAME_H