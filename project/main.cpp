#include <iostream>
#include "sm_systen.h"
// #include "searchEngine.h"
using namespace std;

int main()
{
    SmsSytem &s = SmsSytem::getInstance();
    s.run();
    return 0;
}