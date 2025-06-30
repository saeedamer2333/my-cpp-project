#pragma once
#include <string>
using namespace std;
#include "../include/Transaction.hpp"
// declartion of LinkedListBasedCollection class
class ArrayBasedCollection
{
private:
    Transaction *transactions;
    int numTransactions;
    string searchKey;

public:
    ArrayBasedCollection(string &searchKey, int numTransactions, Transaction arr[]);

    ~ArrayBasedCollection();

    void printGroupedByPaymentChannel(Transaction arr[], int numTransactions, string &searchKey);
    void mergeSortByPaymentChannel(Transaction arr[], int left, int right);
    void mergeByPaymentChannel(Transaction arr[], int left, int mid, int right);
    void mergeSortByAmountThenLocation(Transaction arr[], int left, int right);
    void mergeByAmountThenLocation(Transaction arr[], int left, int mid, int right);
    int searchbyTransactionType(Transaction arr[], int start, int end, string &searchKey, Transaction *group);
};
