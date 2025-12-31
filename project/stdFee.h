#ifndef STUDENTFEES_H
#define STUDENTFEES_H

#include <iostream>
#include <string>
using namespace std;

class StudentFees
{
private:
    int feeId;
    int stdId;
    double amount;
    string paymentDate;
    string status;

public:
    // Constructors
    StudentFees()
    {
        this->feeId = -1;
        this->stdId = -1;
        this->amount = 0.0;
        this->paymentDate = "1970-01-01";
        this->status = "Pending";
    }

    StudentFees(int fId, int sId, double amt, const string &date, const string &stat)
    {
        this->feeId = fId;
        this->stdId = sId;
        this->amount = amt;
        this->paymentDate = date;
        this->status = stat;
    }

    // Getters
    int getFeeId() const
    {
        return this->feeId;
    }

    int getStdId() const
    {
        return this->stdId;
    }

    double getAmount() const
    {
        return this->amount;
    }

    string getPaymentDate() const
    {
        return this->paymentDate;
    }

    string getStatus() const
    {
        return this->status;
    }

    // Setters
    void setFeeId(int id)
    {
        this->feeId = id;
    }

    void setStdId(int id)
    {
        this->stdId = id;
    }

    void setAmount(double amt)
    {
        this->amount = amt;
    }

    void setPaymentDate(const string &date)
    {
        this->paymentDate = date;
    }

    void setStatus(const string &stat)
    {
        this->status = stat;
    }

    // Print
    void print() const
    {
        cout << "Fee ID: " << this->feeId
             << " | Student ID: " << this->stdId
             << " | Amount: " << this->amount
             << " | Payment Date: " << this->paymentDate
             << " | Status: " << this->status << endl;
    }

    // For BST 
    int getId() const
    {
        return this->feeId;
    }
};

#endif // STUDENTFEES_H