#define _HAS_STD_BYTE 0 
#include <windows.h>    // Include Windows headers second
#include <iostream>
#include "sm_systen.h"
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
int main()
{
    clearScreen();
    SMSSystem &system = SMSSystem::getInstance(); 
    SMSSystem &system = SMSSystem::getInstance(); // Changed from SmsSytem to SMSSystem

    system.run();    
    return 0;
   
}
