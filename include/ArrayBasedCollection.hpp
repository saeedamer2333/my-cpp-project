#pragma once
#include <string>
#include <chrono>
using namespace std;
#include "../include/Transaction.hpp"

// declartion of ArrayBasedCollection class
class ArrayBasedCollection
{
private:
    Transaction *transactions;
    int numTransactions;
    string searchKey;

    void mergeSortByPaymentChannel(Transaction arr[], int left, int right);
    void mergeByPaymentChannel(Transaction arr[], int left, int mid, int right);
    void mergeSortByAmountThenLocation(Transaction arr[], int left, int right);
    void mergeByAmountThenLocation(Transaction arr[], int left, int mid, int right);
    int searchbyTransactionType(Transaction arr[], int start, int end, string &searchKey, Transaction *group);

public:
    // Timing metrics for algorithm performance
    chrono::milliseconds searchTime;
    chrono::milliseconds sortTime;

    ArrayBasedCollection(string &searchKey, int numTransactions, Transaction arr[]);

    ~ArrayBasedCollection();

    void printGroupedByPaymentChannel(Transaction arr[], int numTransactions, string &searchKey);
    
    
    // Getters for performance metrics
    chrono::milliseconds getSearchTime() const { return searchTime; }
    chrono::milliseconds getSortTime() const { return sortTime; }
};
