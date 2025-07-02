#pragma once
#include <string>
#include <chrono>
using namespace std;
#include "Transaction.hpp"

//declaration of LinkedListBasedCollection class
class LinkedListBasedCollection { 
private:
    struct TransactionNode {
        Transaction transaction;
        TransactionNode* next;
    };
    
    TransactionNode* head;
    string searchKey;
    int numTransactions;
    
    // Helper methods for linked list operations
    void convertArrayToLinkedList(Transaction *transactions, int numTransactions);
    void insertTransaction(const Transaction &transaction);
    void clearLinkedList();
    void clearGroupList(TransactionNode* groupHead);
    
    // Search methods
    int searchByTransactionTypeInChannel(TransactionNode* channelStart, const string &channelName, 
                                       const string &searchKey, TransactionNode* &groupHead);
    
    // Sorting methods for payment channel
    TransactionNode* mergeSortByPaymentChannel(TransactionNode* head);
    TransactionNode* getMiddle(TransactionNode* head);
    TransactionNode* mergeByPaymentChannel(TransactionNode* left, TransactionNode* right);
    
    // Sorting methods for amount and location
    TransactionNode* mergeSortByAmountThenLocation(TransactionNode* head);
    TransactionNode* mergeByAmountThenLocation(TransactionNode* left, TransactionNode* right);

public:
    // Timing metrics for algorithm performance
    chrono::milliseconds searchTime;
    chrono::milliseconds sortTime;

    LinkedListBasedCollection(string &searchKey, int numTransactions, Transaction *transactions);
    ~LinkedListBasedCollection();
    void printGroupedByPaymentChannel(string &searchKey);
    void processSilently(string &searchKey);  // Process without printing
    
    // Getters for performance metrics
    chrono::milliseconds getSearchTime() const { return searchTime; }
    chrono::milliseconds getSortTime() const { return sortTime; }
};
