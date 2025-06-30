#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <iostream>
using namespace std;

class Transaction
{
private:
    string transactionID;
    string senderAccount;
    string receiverAccount;
    double amount;
    string transactionType;
    string location;
    string paymentChannel;
    bool isFraud;
    int transactionNumber;

public:
    // Constructor declaration
    Transaction(const string &transactionID,
                const string &senderAccount,
                const string &receiverAccount,
                double amount,
                const string &transactionType,
                const string &location,
                const string &paymentChannel,
                bool isFraud);
    Transaction() = default; // Default constructor

    string getTransactionID() const;
    string getSenderAccount() const;
    string getReceiverAccount() const;
    double getAmount() const;
    string getTransactionType() const;
    string getLocation() const;
    string getPaymentChannel() const;
    bool getIsFraud() const;
    // show all avalibe Transaction types
    static void showUniqueTransactionTypes(Transaction arr[], int numTransactions) ;
    

};

#endif
