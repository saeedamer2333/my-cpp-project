#include <string>
#include <iostream>
using namespace std;
#include "../include/ArrayBasedCollection.hpp"

ArrayBasedCollection::ArrayBasedCollection(string &searchKey, int numTransactions, Transaction *transactions)
    : searchKey(searchKey), numTransactions(numTransactions), transactions(transactions)
{
    // Constructor body (if needed)
}

ArrayBasedCollection::~ArrayBasedCollection()
{
    // Destructor - no dynamic memory to clean up since we're not owning the transactions array
}

void ArrayBasedCollection::printGroupedByPaymentChannel(Transaction arr[], int numTransactions, string &searchKey)
{
    if (numTransactions == 0)
        return;
    // Sort transactions by Payment Channel
    mergeSortByPaymentChannel(arr, 0, numTransactions - 1);

    int i = 0;
    while (i < numTransactions)
    {
        string currentChannel = arr[i].getPaymentChannel();

        // Find the end of this payment channel group
        int j = i;
        while (j < numTransactions && arr[j].getPaymentChannel() == currentChannel)
        {
            j++;
        }

        // Search by transaction type using the new method
        Transaction *group = new Transaction[j - i];
        int groupSize = searchbyTransactionType(arr, i, j, searchKey, group);

        // Sort this group by amount (highest first), then by location (alphabetically)
        if (groupSize > 0)
        {
            mergeSortByAmountThenLocation(group, 0, groupSize - 1);

            // Print payment channel header with column names
            cout << "\n========================================" << endl;
            cout << "Payment Channel: " << currentChannel << endl;
            cout << "========================================" << endl;
            cout << "TransactionID | SenderAccount | ReceiverAccount | Amount | TransactionType | Location | Fraud Status" << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;

            // Print top 10 transactions with formatted columns
            for (int k = 0; k < groupSize && k < 10; ++k)
            {
                cout << group[k].getTransactionID() << " | "
                     << group[k].getSenderAccount() << " | "
                     << group[k].getReceiverAccount() << " | "
                     << group[k].getAmount() << " | "
                     << group[k].getTransactionType() << " | "
                     << group[k].getLocation() << " | "
                     << (group[k].getIsFraud() ? "Fraud" : "Not Fraud")
                     << endl;
            }
        }

        delete[] group;
        i = j;
    }
}

int ArrayBasedCollection::searchbyTransactionType(Transaction arr[], int start, int end, string &searchKey, Transaction *group)
{

    int groupSize = 0;
    for (int j = start; j < end; ++j)
    {
        if (arr[j].getTransactionType() == searchKey)
        {
            group[groupSize++] = arr[j];
        }
    }
    return groupSize;
}

void ArrayBasedCollection::mergeByPaymentChannel(Transaction arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Transaction *L = new Transaction[n1];
    Transaction *R = new Transaction[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i].getPaymentChannel() <= R[j].getPaymentChannel())
        {
            arr[k++] = L[i++];
        }
        else
        {
            arr[k++] = R[j++];
        }
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void ArrayBasedCollection::mergeSortByPaymentChannel(Transaction arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSortByPaymentChannel(arr, left, mid);
        mergeSortByPaymentChannel(arr, mid + 1, right);
        mergeByPaymentChannel(arr, left, mid, right);
    }
}

void ArrayBasedCollection::mergeByAmountThenLocation(Transaction arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Transaction *L = new Transaction[n1];
    Transaction *R = new Transaction[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        // Primary sort: by amount (descending - highest first)
        if (L[i].getAmount() > R[j].getAmount())
        {
            arr[k++] = L[i++];
        }
        else if (L[i].getAmount() < R[j].getAmount())
        {
            arr[k++] = R[j++];
        }
        else
        {
            // Secondary sort: by location (ascending - alphabetical)
            if (L[i].getLocation() <= R[j].getLocation())
            {
                arr[k++] = L[i++];
            }
            else
            {
                arr[k++] = R[j++];
            }
        }
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void ArrayBasedCollection::mergeSortByAmountThenLocation(Transaction arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSortByAmountThenLocation(arr, left, mid);
        mergeSortByAmountThenLocation(arr, mid + 1, right);
        mergeByAmountThenLocation(arr, left, mid, right);
    }
}