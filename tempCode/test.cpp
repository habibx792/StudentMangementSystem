#include <iostream>
#include "classList.h"
#include "stdNode.h"
#include "student.h"
#include <string>
using namespace std;
stdNode *registerStd()
{
    string name;
    int age;
    stdNode * n;
    cout << "enter Name \n";
    getline(cin, name);
    cout << "Enter Your Age \n";
    cin >> age;
    cin.ignore();
    Student std(name, age, 1, "wali");
    n=new stdNode(std);
    return n;
}
class A
{
};

int main()
{
    queue q;
    int count;
    cout << "How many std \n";
    cin >> count;
    cin.ignore();
    while (count > 0)
    {
       stdNode* node = registerStd();
    q.enQue(node);
    count--;
    }
    q.printQueue();
    return 0;
}