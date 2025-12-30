#include <iostream>
#include "classList.h"
#include "student.h"
#include "course.h"
#include "stdNode.h"

using namespace std;

Student registerStd()
{
    string name;
    int age;

    cout << "Enter name: ";
    getline(cin, name);

    cout << "Enter age: ";
    cin >> age;
    cin.ignore();

    return Student(name, age, 1, "wali");
}
Course registerCourse()
{
    string title;
    int id;
    string teacherName;

    cout << "Enter Course Title: ";
    getline(cin, title);

    cout << "Enter age: ";
    cin >> id;
    cin.ignore();
    cout << "Enter teacher Name \n";
    
    getline(cin, teacherName);
    return Course(title, id, teacherName);
}
int main()
{
    Queue<Student> q;
    Queue<Course> courses;
    int count;
    cout << "How many students? ";
    cin >> count;
    cin.ignore();
    while (count--)
        q.enQueue(registerStd());
    courses.enQueue(registerCourse());
    Node<Student>* std = q.getFront();
    std->getValue().printStd();
    Node<Course>* co=courses.getFront();
    co->getValue().coursePrint();
}
