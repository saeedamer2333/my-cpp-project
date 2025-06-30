#include <string>
#include <iostream>
using namespace std;
#include "../include/Transaction.hpp"

Transaction::Transaction(const string &transactionID,
                        const string &senderAccount,
                        const string &receiverAccount,
                        double amount,
                        const string &transactionType,
                        const string &location,
                        const string &paymentChannel,
                        bool isFraud)
    : transactionID(transactionID),
      senderAccount(senderAccount),
      receiverAccount(receiverAccount),
      amount(amount),
      transactionType(transactionType),
      location(location),
      paymentChannel(paymentChannel),
      isFraud(isFraud)
{
    // Constructor body (if needed)
}

// Getter implementations
string Transaction::getTransactionID() const {
    return transactionID;
}

string Transaction::getSenderAccount() const {
    return senderAccount;
}

string Transaction::getReceiverAccount() const {
    return receiverAccount;
}

double Transaction::getAmount() const {
    return amount;
}

string Transaction::getTransactionType() const {
    return transactionType;
}

string Transaction::getLocation() const {
    return location;
}

string Transaction::getPaymentChannel() const {
    return paymentChannel;
}

bool Transaction::getIsFraud() const {
    return isFraud;
}



// Static utility method that works with passed arrays
void Transaction::showUniqueTransactionTypes(Transaction* transactions, int count) {
    if (transactions == nullptr || count <= 0) {
        cout << "No transactions available.\n";
        return;
    }

    // Use a simple array to store unique types (max 50 types should be enough)
    string uniqueTypes[50];
    int uniqueCount = 0;

    for (int i = 0; i < count; i++) {
        string type = transactions[i].getTransactionType();
        
        // Check if this type is already in our unique types array
        bool found = false;
        for (int j = 0; j < uniqueCount; j++) {
            if (uniqueTypes[j] == type) {
                found = true;
                break;
            }
        }
        
        // If not found and we have space, add it
        if (!found && uniqueCount < 50) {
            uniqueTypes[uniqueCount] = type;
            uniqueCount++;
        }
    }

    cout << "\nAvailable Transaction Types:\n";
    for (int i = 0; i < uniqueCount; i++) {
        cout << (i + 1) << ". " << uniqueTypes[i] << endl;
    }
}