#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
using namespace std;

class Admin
{
private:
    int adminId;
    string adminName;
    string passWord;

public:
    // Constructors
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

    // Getters
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

    // Setters
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

    // Print
    void print() const
    {
        cout << "Admin ID: " << this->adminId
             << " | Name: " << this->adminName
             << " | Password: " << this->passWord << endl;
    }

    // For BST compatibility
    int getId() const
    {
        return this->adminId;
    }
};

#endif // ADMIN_H