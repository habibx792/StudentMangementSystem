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

class CLASSNAME {
private:
    // member variables
    string memberName;

public:
    // constructors
    CLASSNAME();
    CLASSNAME(params);

    // getters
    string getmemberName() const;

    // setters
    void setmemberName(const string& value);
};

#endif // CLASSNAME_H