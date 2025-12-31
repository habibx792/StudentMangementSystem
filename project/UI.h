#ifndef UI_H
#define UI_H

#include <iostream>
#include <limits>
#include <string>
#include <iomanip>

using namespace std;

class UI
{
public:
    // ------------------- Display Functions -------------------
    void displayHeader(const string &title)
    {
        cout << "\n";
        cout << "╔══════════════════════════════════════════════════════════╗\n";
        cout << "║                     " << setw(30) << left << title << "                     ║\n";
        cout << "╚══════════════════════════════════════════════════════════╝\n";
    }

    void displaySuccess(const string &message)
    {
        cout << "\n✅ " << message << "\n";
    }

    void displayError(const string &message)
    {
        cout << "\n❌ Error: " << message << "\n";
    }

    void displayInfo(const string &message)
    {
        cout << "\nℹ️  " << message << "\n";
    }

    void clearScreen()
    {
        system("cls");
    }

    void pressEnterToContinue()
    {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    // ------------------- Input Helper -------------------
    int getChoice(const string &prompt = "Enter your choice: ", int min = 0, int max = 9)
    {
        int choice;
        while (true)
        {
            cout << "\n"
                 << prompt;
            cin >> choice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                displayError("Invalid input. Please enter a number.");
            }
            else if (choice < min || choice > max)
            {
                displayError("Choice out of range. Please enter between " + to_string(min) + " and " + to_string(max));
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
        }
    }

    string getStringInput(const string &prompt)
    {
        string input;
        cout << prompt;
        getline(cin, input);
        return input;
    }

    int getIntInput(const string &prompt)
    {
        int value;
        while (true)
        {
            cout << prompt;
            cin >> value;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                displayError("Invalid input. Please enter an integer.");
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

    double getDoubleInput(const string &prompt)
    {
        double value;
        while (true)
        {
            cout << prompt;
            cin >> value;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                displayError("Invalid input. Please enter a number.");
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

    // ------------------- Main Menu -------------------
    void displayMainMenu()
    {
        clearScreen();
        displayHeader("STUDENT MANAGEMENT SYSTEM");
        cout << "\n";
        cout << "╔══════════════════════════════════════════════════════════╗\n";
        cout << "║                         MAIN MENU                        ║\n";
        cout << "╠══════════════════════════════════════════════════════════╣\n";
        cout << "║  1. 👨‍🎓 Student Operations                            ║\n";
        cout << "║  2. 📚 Course Operations                                 ║\n";
        cout << "║  3. 💰 Fee Operations                                    ║\n";
        cout << "║  4. 📊 Attendance Operations                             ║\n";
        cout << "║  5. 🏆 Result Operations                                 ║\n";
        cout << "║  6. 🎓 Course Registration                               ║\n";
        cout << "║  7. ⚙️  Admin Settings                                   ║\n";
        cout << "║  8. 📋 Field of Study Management                         ║\n";
        cout << "║  0. 🚪 Exit System                                       ║\n";
        cout << "╚══════════════════════════════════════════════════════════╝\n";
    }

    // ------------------- Student Menu -------------------
    void displayStudentMenu()
    {
        displayHeader("STUDENT OPERATIONS");
        cout << "\n";
        cout << "1. ➕ Register New Student\n";
        cout << "2. 🔍 Search Student\n";
        cout << "3. ✏️  Update Student Information\n";
        cout << "4. 🗑️  Delete Student Record\n";
        cout << "5. 📋 View All Students\n";
        cout << "0. ↩️  Back to Main Menu\n";
    }

    // ------------------- Course Menu -------------------
    void displayCourseMenu()
    {
        displayHeader("COURSE OPERATIONS");
        cout << "\n";
        cout << "1. ➕ Add New Course\n";
        cout << "2. 🔍 Search Course\n";
        cout << "3. ✏️  Update Course Information\n";
        cout << "4. 🗑️  Delete Course\n";
        cout << "5. 📋 View All Courses\n";
        cout << "0. ↩️  Back to Main Menu\n";
    }

    // ------------------- Fee Menu -------------------
    void displayFeeMenu()
    {
        displayHeader("FEE OPERATIONS");
        cout << "\n";
        cout << "1. 💳 Record Payment\n";
        cout << "2. ✏️  Update Payment Status\n";
        cout << "3. 🔍 View Student Fees\n";
        cout << "4. 📋 View All Fee Records\n";
        cout << "0. ↩️  Back to Main Menu\n";
    }

    // ------------------- Attendance Menu -------------------
    void displayAttendanceMenu()
    {
        displayHeader("ATTENDANCE OPERATIONS");
        cout << "\n";
        cout << "1. 📅 Mark Attendance\n";
        cout << "2. ✏️  Update Attendance\n";
        cout << "3. 🔍 Search Attendance Records\n";
        cout << "4. 📋 View All Attendance\n";
        cout << "5. 📊 Generate Attendance Report\n";
        cout << "0. ↩️  Back to Main Menu\n";
    }

    // ------------------- Result Menu -------------------
    void displayResultMenu()
    {
        displayHeader("RESULT OPERATIONS");
        cout << "\n";
        cout << "1. 📝 Enter Student Results\n";
        cout << "2. ✏️  Update Results\n";
        cout << "3. 🔍 Search Results\n";
        cout << "4. 🏆 View Passing Students\n";
        cout << "5. 📋 View All Results\n";
        cout << "0. ↩️  Back to Main Menu\n";
    }

    // ------------------- Course Registration Menu -------------------
    void displayCourseRegistrationMenu()
    {
        displayHeader("COURSE REGISTRATION");
        cout << "\n";
        cout << "1. 📝 Register Student for Course\n";
        cout << "2. 🗑️  Remove Registration\n";
        cout << "3. 🔍 View Student's Registered Courses\n";
        cout << "4. 🔍 View Course's Registered Students\n";
        cout << "0. ↩️  Back to Main Menu\n";
    }

    // ------------------- Admin Menu -------------------
    void displayAdminMenu()
    {
        displayHeader("ADMIN SETTINGS");
        cout << "\n";
        cout << "1. ➕ Add New Admin\n";
        cout << "2. ✏️  Update Admin Password\n";
        cout << "3. 🔍 View Admin List\n";
        cout << "4. 🗑️  Delete Admin Account\n";
        cout << "0. ↩️  Back to Main Menu\n";
    }

    // ------------------- Field Study Menu -------------------
    void displayFieldStudyMenu()
    {
        displayHeader("FIELD OF STUDY MANAGEMENT");
        cout << "\n";
        cout << "1. ➕ Add New Field of Study\n";
        cout << "2. ✏️  Update Field Name\n";
        cout << "3. 🗑️  Delete Field of Study\n";
        cout << "4. 🔍 Search Field of Study\n";
        cout << "5. 📋 View All Fields of Study\n";
        cout << "0. ↩️  Back to Main Menu\n";
    }

    // ------------------- Login Screen -------------------
    void displayLoginScreen()
    {
        clearScreen();
        cout << "\n";
        cout << "╔══════════════════════════════════════════════════════════╗\n";
        cout << "║               STUDENT MANAGEMENT SYSTEM                  ║\n";
        cout << "║                     Login Portal                         ║\n";
        cout << "╚══════════════════════════════════════════════════════════╝\n";
        cout << "\n";
    }

    // ------------------- Search Options -------------------
    void displaySearchOptions(const string &entity)
    {
        cout << "\n--- Search " << entity << " by ---\n";
        cout << "1. By ID\n";
        cout << "2. By Name\n";
        cout << "3. Back\n";
    }

    // ------------------- Report Menu -------------------
    void displayReportMenu()
    {
        displayHeader("REPORTS & ANALYTICS");
        cout << "\n";
        cout << "1. 📊 Student Performance Report\n";
        cout << "2. 📅 Attendance Summary\n";
        cout << "3. 💰 Fee Collection Report\n";
        cout << "4. 🎓 Course Enrollment Statistics\n";
        cout << "0. ↩️  Back to Main Menu\n";
    }

    // ------------------- Confirmation -------------------
    bool confirmAction(const string &action)
    {
        cout << "\n⚠️  Are you sure you want to " << action << "? (y/n): ";
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return (choice == 'y' || choice == 'Y');
    }
};

#endif // UI_H