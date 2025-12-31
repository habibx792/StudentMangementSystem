#ifndef SMSSYSTEM_H
#define SMSSYSTEM_H

#include <iostream>
#include <string>
#include "db.h"
#include "UI.h"
#include "printEngine.h"
using namespace std;

class SmsSytem
{
private:
    DB db;
    UI ui;

    SmsSytem()
    {
    }

    SmsSytem(const SmsSytem &) = delete;
    SmsSytem &operator=(const SmsSytem &) = delete;

public:
    static SmsSytem &getInstance()
    {
        static SmsSytem instance;
        return instance;
    }

    void run()
    {
        db.connect();
        ui.displayAdminMenu();
    }
};

#endif // SMSSYSTEM_H