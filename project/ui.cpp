#include "UI.h"
#include "sm_system.h" // Now include this in the cpp file
#include "stdNode.h"
#include "student.h"
#include "course.h"
#include "field.h"
#include "admin.h"

// ================== Display Functions ==================
void UI::displayHeader(const string &title)
{
    cout << "\n";
    cout << "====================================================\n";
    cout << "             " << title << "\n";
    cout << "====================================================\n";
}

void UI::displaySuccess(const string &message)
{
    cout << "\n✓ " << message << "\n";
}

void UI::displayError(const string &message)
{
    cout << "\n✗ ERROR: " << message << "\n";
}

void UI::displayInfo(const string &message)
{
    cout << "\nℹ " << message << "\n";
}

void UI::clearScreen()
{
    system("cls");
}

void UI::sleep(int milliseconds)
{
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void UI::pressEnterToContinue()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ================== Input Helper Functions ==================
int UI::getChoice(const string &prompt, int min, int max)
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

string UI::getStringInput(const string &prompt)
{
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int UI::getIntInput(const string &prompt)
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

double UI::getDoubleInput(const string &prompt)
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

bool UI::getBoolInput(const string &prompt)
{
    char choice;
    while (true)
    {
        cout << prompt << " (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 'y' || choice == 'Y')
            return true;
        if (choice == 'n' || choice == 'N')
            return false;
        displayError("Please enter 'y' or 'n'.");
    }
}

// ================== Login Function ==================
bool UI::login()
{
    clearScreen();
    displayHeader("LOGIN TO STUDENT MANAGEMENT SYSTEM");

    int adminId;
    string password;

    cout << "\nAdmin Login\n";
    cout << "------------\n";

    adminId = getIntInput("Enter Admin ID: ");
    password = getStringInput("Enter Password: ");

    StdNode<Admin> *adminNode = smsSystem->getAdminBST().search(adminId);
    if (adminNode && adminNode->getData().getPassWord() == password)
    {
        displaySuccess("Login successful! Welcome " + adminNode->getData().getAdminName());
        sleep(1000);
        return true;
    }

    displayError("Invalid admin ID or password!");
    sleep(2000);
    return false;
}

// ================== Display Menus ==================
void UI::displayMainMenu()
{
    clearScreen();
    displayHeader("STUDENT MANAGEMENT SYSTEM - MAIN MENU");
    cout << "\n";
    cout << "1.  Student Operations\n";
    cout << "2.  Course Operations\n";
    cout << "3.  Field Operations\n";
    cout << "4.  Attendance Operations\n";
    cout << "5.  Course Registration\n";
    cout << "6.  Fee Operations\n";
    cout << "7.  Result Operations\n";
    cout << "8.  Admin Operations\n";
    cout << "9.  Search Operations\n";
    cout << "10. Print/View Operations\n";
    cout << "11. Queue Operations\n";
    cout << "12. Process All Updates & Deletions\n";
    cout << "0.  Exit System\n";
    cout << "====================================================\n";
}

void UI::displayStudentMenu()
{
    displayHeader("STUDENT OPERATIONS");
    cout << "\n";
    cout << "1. Add New Student\n";
    cout << "2. Search Student\n";
    cout << "3. Update Student\n";
    cout << "4. Delete Student\n";
    cout << "5. View All Students\n";
    cout << "0. Back to Main Menu\n";
}

void UI::displayCourseMenu()
{
    displayHeader("COURSE OPERATIONS");
    cout << "\n";
    cout << "1. Add New Course\n";
    cout << "2. Search Course\n";
    cout << "3. Update Course\n";
    cout << "4. Delete Course\n";
    cout << "5. View All Courses\n";
    cout << "0. Back to Main Menu\n";
}

void UI::displayFieldMenu()
{
    displayHeader("FIELD OPERATIONS");
    cout << "\n";
    cout << "1. Add New Field\n";
    cout << "2. Search Field\n";
    cout << "3. Update Field\n";
    cout << "4. Delete Field\n";
    cout << "5. View All Fields\n";
    cout << "0. Back to Main Menu\n";
}

// ================== Main System Functions ==================
void UI::runMainSystem()
{
    bool loggedIn = false;

    while (!loggedIn)
    {
        loggedIn = login();
        if (!loggedIn)
        {
            cout << "\n1. Try again\n";
            cout << "2. Exit\n";
            int choice = getChoice("Enter choice: ", 1, 2);
            if (choice == 2)
            {
                cout << "\nExiting system...\n";
                exit(0);
            }
        }
    }

    while (true)
    {
        displayMainMenu();
        int choice = getChoice("Enter your choice: ", 0, 12);

        switch (choice)
        {
        case 1:
            handleStudentOperations();
            break;
        case 2:
            handleCourseOperations();
            break;
        case 3:
            handleFieldOperations();
            break;
        case 4:
            handleAttendanceOperations();
            break;
        case 5:
            handleCourseRegistration();
            break;
        case 6:
            handleFeeOperations();
            break;
        case 7:
            handleResultOperations();
            break;
        case 8:
            handleAdminOperations();
            break;
        case 9:
            handleSearchOperations();
            break;
        case 10:
            handlePrintOperations();
            break;
        case 11:
            handleQueueOperations();
            break;
        case 12:
            smsSystem->processAllUpdates();
            smsSystem->processAllDeletions();
            pressEnterToContinue();
            break;
        case 0:
            if (confirmAction("exit the system"))
            {
                clearScreen();
                displaySuccess("Thank you for using Student Management System!");
                sleep(2000);
                exit(0);
            }
            break;
        default:
            displayError("Invalid choice!");
        }
    }
}

// ================== Operation Handlers ==================
void UI::handleStudentOperations()
{
    while (true)
    {
        displayStudentMenu();
        int choice = getChoice();

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            searchStudent();
            break;
        case 3:
            updateStudent();
            break;
        case 4:
            deleteStudent();
            break;
        case 5:
            viewAllStudents();
            pressEnterToContinue();
            break;
        case 0:
            return;
        default:
            displayError("Invalid choice!");
        }
    }
}

void UI::handleCourseOperations()
{
    while (true)
    {
        displayCourseMenu();
        int choice = getChoice();

        switch (choice)
        {
        case 1:
            addCourse();
            break;
        case 2:
            searchCourse();
            break;
        case 3:
            updateCourse();
            break;
        case 4:
            deleteCourse();
            break;
        case 5:
            viewAllCourses();
            pressEnterToContinue();
            break;
        case 0:
            return;
        default:
            displayError("Invalid choice!");
        }
    }
}

void UI::handleFieldOperations()
{
    while (true)
    {
        displayFieldMenu();
        int choice = getChoice();

        switch (choice)
        {
        case 1:
            addField();
            break;
        case 2:
            searchField();
            break;
        case 3:
            updateField();
            break;
        case 4:
            deleteField();
            break;
        case 5:
            viewAllFields();
            pressEnterToContinue();
            break;
        case 0:
            return;
        default:
            displayError("Invalid choice!");
        }
    }
}

// ================== CRUD Operations ==================
void UI::addStudent()
{
    displayHeader("ADD NEW STUDENT");

    Student student;
    int id = getIntInput("Enter Student ID: ");
    student.setStdId(id);

    string name = getStringInput("Enter Student Name: ");
    student.setStdName(name);

    string username = getStringInput("Enter Username: ");
    student.setStdUserName(username);

    int age = getIntInput("Enter Age: ");
    student.setStdAge(age);

    int fieldId = getIntInput("Enter Field ID: ");
    student.setFieldId(fieldId);

    string fatherName = getStringInput("Enter Father's Name: ");
    student.setStdFatherName(fatherName);

    smsSystem->addNewStudent(student);
    displaySuccess("Student added successfully!");
    pressEnterToContinue();
}

void UI::searchStudent()
{
    displayHeader("SEARCH STUDENT");
    int id = getIntInput("Enter Student ID to search: ");

    StdNode<Student> *node = smsSystem->getStudentBST().search(id);
    if (node)
    {
        cout << "\n--- Student Found ---\n";
        node->getData().print();
    }
    else
    {
        displayError("Student not found!");
    }
    pressEnterToContinue();
}

void UI::updateStudent()
{
    displayHeader("UPDATE STUDENT");
    int id = getIntInput("Enter Student ID to update: ");

    StdNode<Student> *node = smsSystem->getStudentBST().search(id);
    if (!node)
    {
        displayError("Student not found!");
        pressEnterToContinue();
        return;
    }

    cout << "\nCurrent Student Information:\n";
    node->getData().print();

    cout << "\nEnter new values (leave empty to keep current value):\n";

    map<string, string> updateFields;

    string newName = getStringInput("New Name [" + node->getData().getStdName() + "]: ");
    if (!newName.empty())
        updateFields["stdName"] = newName;

    string newUsername = getStringInput("New Username [" + node->getData().getStdUserName() + "]: ");
    if (!newUsername.empty())
        updateFields["stdUserName"] = newUsername;

    string newAge = getStringInput("New Age [" + to_string(node->getData().getStdAge()) + "]: ");
    if (!newAge.empty())
        updateFields["stdAge"] = newAge;

    string newFieldId = getStringInput("New Field ID [" + to_string(node->getData().getFieldId()) + "]: ");
    if (!newFieldId.empty())
        updateFields["fieldId"] = newFieldId;

    string newFatherName = getStringInput("New Father's Name [" + node->getData().getStdFatherName() + "]: ");
    if (!newFatherName.empty())
        updateFields["stdFatherName"] = newFatherName;

    if (!updateFields.empty())
    {
        smsSystem->addUpdate(id, "student", updateFields);
        displaySuccess("Update queued for student ID: " + to_string(id));
    }
    else
    {
        displayInfo("No changes made.");
    }
    pressEnterToContinue();
}

void UI::deleteStudent()
{
    displayHeader("DELETE STUDENT");
    int id = getIntInput("Enter Student ID to delete: ");

    StdNode<Student> *node = smsSystem->getStudentBST().search(id);
    if (!node)
    {
        displayError("Student not found!");
        pressEnterToContinue();
        return;
    }

    cout << "\nStudent to delete:\n";
    node->getData().print();

    if (confirmAction("delete this student"))
    {
        smsSystem->addDeletion(id, "student");
        displaySuccess("Delete request queued for student ID: " + to_string(id));
    }
    pressEnterToContinue();
}

void UI::viewAllStudents()
{
    displayHeader("ALL STUDENTS");
    smsSystem->getPrintEngine().printAllStudents(smsSystem->getStudentBST());
}

void UI::addCourse()
{
    displayHeader("ADD NEW COURSE");

    Course course;
    int id = getIntInput("Enter Course ID: ");
    course.setId(id);

    string title = getStringInput("Enter Course Title: ");
    course.setCourseTitle(title);

    string teacher = getStringInput("Enter Teacher Name: ");
    course.setTeacherName(teacher);

    smsSystem->addNewCourse(course);
    displaySuccess("Course added successfully!");
    pressEnterToContinue();
}

void UI::searchCourse()
{
    displayHeader("SEARCH COURSE");
    int id = getIntInput("Enter Course ID to search: ");

    StdNode<Course> *node = smsSystem->getCourseBST().search(id);
    if (node)
    {
        cout << "\n--- Course Found ---\n";
        node->getData().print();
    }
    else
    {
        displayError("Course not found!");
    }
    pressEnterToContinue();
}

void UI::updateCourse()
{
    displayHeader("UPDATE COURSE");
    int id = getIntInput("Enter Course ID to update: ");

    StdNode<Course> *node = smsSystem->getCourseBST().search(id);
    if (!node)
    {
        displayError("Course not found!");
        pressEnterToContinue();
        return;
    }

    map<string, string> updateFields;

    cout << "\nEnter new values (leave empty to keep current):\n";

    string newTitle = getStringInput("New Course Title [" + node->getData().getCourseTitle() + "]: ");
    if (!newTitle.empty())
        updateFields["courseTitle"] = newTitle;

    string newTeacher = getStringInput("New Teacher Name [" + node->getData().getTeacherName() + "]: ");
    if (!newTeacher.empty())
        updateFields["teacherName"] = newTeacher;

    if (!updateFields.empty())
    {
        smsSystem->addUpdate(id, "course", updateFields);
        displaySuccess("Update queued for course ID: " + to_string(id));
    }
    else
    {
        displayInfo("No changes made.");
    }
    pressEnterToContinue();
}

void UI::deleteCourse()
{
    displayHeader("DELETE COURSE");
    int id = getIntInput("Enter Course ID to delete: ");

    StdNode<Course> *node = smsSystem->getCourseBST().search(id);
    if (!node)
    {
        displayError("Course not found!");
        pressEnterToContinue();
        return;
    }

    if (confirmAction("delete this course"))
    {
        smsSystem->addDeletion(id, "course");
        displaySuccess("Delete request queued for course ID: " + to_string(id));
    }
    pressEnterToContinue();
}

void UI::viewAllCourses()
{
    displayHeader("ALL COURSES");
    smsSystem->getPrintEngine().printAllCourses(smsSystem->getCourseBST());
}

void UI::addField()
{
    displayHeader("ADD NEW FIELD");

    FieldStudy field;
    int id = getIntInput("Enter Field ID: ");
    field.setFieldId(id);

    string name = getStringInput("Enter Field Name: ");
    field.setFieldName(name);

    smsSystem->addNewField(field);
    displaySuccess("Field added successfully!");
    pressEnterToContinue();
}

void UI::searchField()
{
    displayHeader("SEARCH FIELD");
    int id = getIntInput("Enter Field ID to search: ");

    StdNode<FieldStudy> *node = smsSystem->getFieldBST().search(id);
    if (node)
    {
        cout << "\n--- Field Found ---\n";
        node->getData().print();
    }
    else
    {
        displayError("Field not found!");
    }
    pressEnterToContinue();
}

void UI::updateField()
{
    displayHeader("UPDATE FIELD");
    int id = getIntInput("Enter Field ID to update: ");

    StdNode<FieldStudy> *node = smsSystem->getFieldBST().search(id);
    if (!node)
    {
        displayError("Field not found!");
        pressEnterToContinue();
        return;
    }

    map<string, string> updateFields;

    string newName = getStringInput("New Field Name [" + node->getData().getFieldName() + "]: ");
    if (!newName.empty())
    {
        updateFields["fieldName"] = newName;
        smsSystem->addUpdate(id, "fieldStudy", updateFields);
        displaySuccess("Update queued for field ID: " + to_string(id));
    }
    else
    {
        displayInfo("No changes made.");
    }
    pressEnterToContinue();
}

void UI::deleteField()
{
    displayHeader("DELETE FIELD");
    int id = getIntInput("Enter Field ID to delete: ");

    StdNode<FieldStudy> *node = smsSystem->getFieldBST().search(id);
    if (!node)
    {
        displayError("Field not found!");
        pressEnterToContinue();
        return;
    }

    if (confirmAction("delete this field"))
    {
        smsSystem->addDeletion(id, "fieldStudy");
        displaySuccess("Delete request queued for field ID: " + to_string(id));
    }
    pressEnterToContinue();
}

void UI::viewAllFields()
{
    displayHeader("ALL FIELDS");
    smsSystem->getPrintEngine().printAllFields(smsSystem->getFieldBST());
}

// ================== Other Operations ==================
void UI::handleAttendanceOperations()
{
    displayHeader("ATTENDANCE OPERATIONS");
    displayInfo("Attendance operations module coming soon!");
    pressEnterToContinue();
}

void UI::handleCourseRegistration()
{
    displayHeader("COURSE REGISTRATION");
    displayInfo("Course registration module coming soon!");
    pressEnterToContinue();
}

void UI::handleFeeOperations()
{
    displayHeader("FEE OPERATIONS");
    displayInfo("Fee operations module coming soon!");
    pressEnterToContinue();
}

void UI::handleResultOperations()
{
    displayHeader("RESULT OPERATIONS");
    displayInfo("Result operations module coming soon!");
    pressEnterToContinue();
}

void UI::handleAdminOperations()
{
    displayHeader("ADMIN OPERATIONS");
    displayInfo("Admin operations module coming soon!");
    pressEnterToContinue();
}

void UI::handleSearchOperations()
{
    displayHeader("SEARCH OPERATIONS");
    displayInfo("Search operations module coming soon!");
    pressEnterToContinue();
}

void UI::handlePrintOperations()
{
    while (true)
    {
        displayHeader("PRINT/VIEW OPERATIONS");
        cout << "\n1. View All Students\n";
        cout << "2. View All Courses\n";
        cout << "3. View All Fields\n";
        cout << "4. View All Admins\n";
        cout << "0. Back to Main Menu\n";

        int choice = getChoice();

        switch (choice)
        {
        case 1:
            viewAllStudents();
            break;
        case 2:
            viewAllCourses();
            break;
        case 3:
            viewAllFields();
            break;
        case 4:
            smsSystem->getPrintEngine().printAllAdmins(smsSystem->getAdminBST());
            break;
        case 0:
            return;
        default:
            displayError("Invalid choice!");
        }
        pressEnterToContinue();
    }
}

void UI::handleQueueOperations()
{
    displayHeader("QUEUE OPERATIONS");
    cout << "\n1. Show Update Queue Status\n";
    cout << "2. Show Delete Queue Status\n";
    cout << "3. Clear Update Queue\n";
    cout << "4. Clear Delete Queue\n";
    cout << "0. Back\n";

    int choice = getChoice();

    switch (choice)
    {
    case 1:
        smsSystem->showUpdateQueueStatus();
        break;
    case 2:
        smsSystem->showDeleteQueueStatus();
        break;
    case 3:
        smsSystem->clearAllUpdates();
        break;
    case 4:
        smsSystem->clearAllDeletions();
        break;
    case 0:
        return;
    }
    pressEnterToContinue();
}

// ================== Utility Functions ==================
bool UI::confirmAction(const string &action)
{
    cout << "\nAre you sure you want to " << action << "? (y/n): ";
    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return (choice == 'y' || choice == 'Y');
}

void UI::displayWelcome()
{
    clearScreen();
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════════╗\n";
    cout << "║         STUDENT MANAGEMENT SYSTEM v1.0                  ║\n";
    cout << "║                Console Edition                          ║\n";
    cout << "║                                                          ║\n";
    cout << "║  Features:                                               ║\n";
    cout << "║  • Student Management                                    ║\n";
    cout << "║  • Course Management                                     ║\n";
    cout << "║  • Field Management                                      ║\n";
    cout << "║  • Database Integration                                  ║\n";
    cout << "║  • Queue-based Updates                                   ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n";
    cout << "\n";
}