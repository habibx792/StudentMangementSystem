// main.cpp - CORRECT ORDER
#define _HAS_STD_BYTE 0 // Define it first
#include <windows.h>    // Include Windows headers second
// Now include C++ standard and your project headers
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
    SMSSystem &system = SMSSystem::getInstance(); // Changed from SmsSytem to SMSSystem
    system.run();
    cout << "//COMPLETION DATA: 04/01/2026 \n";
    return 0;
}