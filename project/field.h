#define _HAS_STD_BYTE 0
#ifndef FIELDSTUDY_H
#define FIELDSTUDY_H
#include <iostream>
#include <string>
using namespace std;

class FieldStudy
{
private:
    int fieldId;
    string fieldName;

public:
    FieldStudy()
    {
        this->fieldId = -1;
        this->fieldName = "";
    }

    FieldStudy(int id, const string &name)
    {
        this->fieldId = id;
        this->fieldName = name;
    }
    FieldStudy(const string &name)
    {
        this->fieldId = -1;
        this->fieldName = name;
    }

    int getFieldId() const
    {
        return this->fieldId;
    }

    string getFieldName() const
    {
        return this->fieldName;
    }

    void setFieldId(int id)
    {
        this->fieldId = id;
    }

    void setFieldName(const string &name)
    {
        this->fieldName = name;
    }

    void print() const
    {
        cout << "Field ID: " << this->fieldId
             << " | Field Name: " << this->fieldName << endl;
    }

    int getId() const
    {
        return this->fieldId;
    }
};

#endif // FIELDSTUDY_H