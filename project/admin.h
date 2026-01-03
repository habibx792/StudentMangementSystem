#ifndef ADMIN_H
#define ADMIN_H
#define _HAS_STD_BYTE 0
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Admin
{
private:
    int adminId;
    string adminName;
    string passWord;

public:
    Admin()
    {
        this->adminId = -1;
        this->adminName = "";
        this->passWord = "";
    }

    Admin(int id, const string &name, const string &pass)
    {
        this->adminId = id;
        this->adminName = name;
        this->passWord = pass;
    }

    int getAdminId() const
    {
        return this->adminId;
    }

    string getAdminName() const
    {
        return this->adminName;
    }

    string getPassWord() const
    {
        return this->passWord;
    }

    void setAdminId(int id)
    {
        this->adminId = id;
    }

    void setAdminName(const string &name)
    {
        this->adminName = name;
    }

    void setPassWord(const string &pass)
    {
        this->passWord = pass;
    }

    void print() const
    {
        cout << left
             << setw(12) << "Admin ID:" << adminId << endl
             << setw(12) << "Name:" << adminName << endl
             << setw(12) << "Password:" << passWord << endl
             << "-----------------------------" << endl;
    }

    int getId() const
    {
        return this->adminId;
    }
};

#endif // ADMIN_H