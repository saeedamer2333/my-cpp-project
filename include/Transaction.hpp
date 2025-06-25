#pragma once
#include <string>
#include <iostream>
using namespace std;

class Transaction
{
private:
    string transactionID;
    string customerID;
    string transactionType;
    double amount;
    string timestamp;
    string location;
    string paymentChannel;
    bool isFraud;

public:
    // Constructor with all parameters
    Transaction(const string &transactionID,
                const string &customerID,
                const string &transactionType,
                double amount,
                const string &timestamp,
                const string &location,
                const string &paymentChannel,
                bool isFraud);

    // Getter methods
    string getTransactionID() const;
    string getCustomerID() const;
    string getTransactionType() const;
    double getAmount() const;
    string getTimestamp() const;
    string getLocation() const;
    string getPaymentChannel() const;
    bool getIsFraud() const;

    // Display transaction details
    void display() const;

    // Comparison operator for sorting by location
    bool operator<(const Transaction &other) const;
};