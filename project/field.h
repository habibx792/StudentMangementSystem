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
    // Constructors
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

    // Getters
    int getFieldId() const
    {
        return this->fieldId;
    }

    string getFieldName() const
    {
        return this->fieldName;
    }

    // Setters
    void setFieldId(int id)
    {
        this->fieldId = id;
    }

    void setFieldName(const string &name)
    {
        this->fieldName = name;
    }

    // Print
    void print() const
    {
        cout << "Field ID: " << this->fieldId
             << " | Field Name: " << this->fieldName << endl;
    }

    // For BST compatibility
    int getId() const
    {
        return this->fieldId;
    }
};

#endif // FIELDSTUDY_H