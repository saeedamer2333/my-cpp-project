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

public:
    // Constructor with all parameters
    Transaction(const string &transactionID,
                const string &senderAccount,
                const string &receiverAccount,
                double amount,
                const string &transactionType,
                const string &location,
                bool isFraud,
                const string &paymentChannel)
    {
       this->transactionID = transactionID;
        this->senderAccount = senderAccount;
        this->receiverAccount = receiverAccount;
        this->amount = amount;
        this->transactionType = transactionType;
        this->location = location;
        this->paymentChannel = paymentChannel;
        this->isFraud = isFraud;
    }

    // Getter methods
    string getTransactionID() const { return transactionID; }
    string getSenderAccount() const { return senderAccount; }
    string getReceiverAccount() const { return receiverAccount; }
    double getAmount() const { return amount; }
    string getTransactionType() const { return transactionType; }
    string getLocation() const { return location; }
    string getPaymentChannel() const { return paymentChannel; }
    bool getIsFraud() const { return isFraud; }

    
    // Comparison operator for sorting by location
    bool operator<(const Transaction &other) const
    {
        return this->location < other.location;
    }
};