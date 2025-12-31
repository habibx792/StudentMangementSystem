#ifndef CLASSNAME_H
#define CLASSNAME_H
#define _HAS_STD_BYTE 0
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