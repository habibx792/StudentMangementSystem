#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stireg.h>
#include <sstream>
#include <vector>
#include <limits>

#ifndef CLASSNAME_H
#define CLASSNAME_H

#include <string>
using namespace std;

lass ui
{

    int getput()
    {
        int choice;
        while (true)
        {
            cin >> choice;

            if (cin.fail())
            {
                cin.clear(); // remove fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter an integer: ";
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
        }
    }
    void stdOpeartion()
    {
        cout << "1->TO regitser a student \n";
        cout << "2->To search a studnet \n";
        cout << "3->To update student Information \n";

    }
    void diplay()
    {
        cout << "********************************************************** Welcome to Student Mangement System ************************************************************\n";
        cout << "1-> for std Operation \n";
        cout << "1-> for course Operatrion \n";
        cout << "3->Fee opeatrion \n";
        cout << "4->For admin system Setting \n";
    }
};

#endif // CLASSNAME_H