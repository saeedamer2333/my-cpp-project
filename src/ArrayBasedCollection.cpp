#include <string>
#include <iostream>
#include <chrono>
using namespace std;
#include "../include/ArrayBasedCollection.hpp"

ArrayBasedCollection::ArrayBasedCollection(string &searchKey, int numTransactions, Transaction *transactions)
    : searchKey(searchKey), numTransactions(numTransactions), transactions(transactions), 
      searchTime(chrono::milliseconds::zero()), sortTime(chrono::milliseconds::zero())
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

    // Track sorting time for payment channel
    auto sortStart = chrono::high_resolution_clock::now();
    mergeSortByPaymentChannel(arr, 0, numTransactions - 1);
    auto sortEnd = chrono::high_resolution_clock::now();
    auto channelSortTime = chrono::duration_cast<chrono::milliseconds>(sortEnd - sortStart);

    int i = 0;
    int totalResults = 0;
    chrono::milliseconds totalSearchTime = chrono::milliseconds::zero();
    chrono::milliseconds totalAmountSortTime = chrono::milliseconds::zero();
    cout << "\n========================================" << endl;
    cout << "Grouped Transactions by Payment Channel" << endl;
    while (i < numTransactions)
    {
        string currentChannel = arr[i].getPaymentChannel();

        // Find the end of this payment channel group
        int j = i;
        while (j < numTransactions && arr[j].getPaymentChannel() == currentChannel)
        {
            j++;
        }

        // Search by transaction type using the new method with timing
        Transaction *group = new Transaction[j - i];
        auto searchStart = chrono::high_resolution_clock::now();
        int groupSize = searchbyTransactionType(arr, i, j, searchKey, group);
        auto searchEndTime = chrono::high_resolution_clock::now();
        totalSearchTime += chrono::duration_cast<chrono::milliseconds>(searchEndTime - searchStart);

        // Sort this group by amount (highest first), then by location (alphabetically)
        if (groupSize > 0)
        {
            auto amountSortStart = chrono::high_resolution_clock::now();
            mergeSortByAmountThenLocation(group, 0, groupSize - 1);
            auto amountSortEnd = chrono::high_resolution_clock::now();
            totalAmountSortTime += chrono::duration_cast<chrono::milliseconds>(amountSortEnd - amountSortStart);

            // Print payment channel header with column names
            cout << "\n========================================" << endl;
            cout << "Payment Channel: " << currentChannel << endl;
            cout << "========================================" << endl;
            cout << "TransactionID | SenderAccount | ReceiverAccount | Amount | TransactionType | Location | Fraud Status" << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;

            // Print top 10 transactions with formatted columns
            int displayCount = min(groupSize, 10);
            for (int k = 0; k < displayCount; ++k)
            {
                cout << group[k].getTransactionID() << " | "
                     << group[k].getSenderAccount() << " | "
                     << group[k].getReceiverAccount() << " | "
                     << group[k].getAmount() << " | "
                     << group[k].getTransactionType() << " | "
                     << group[k].getLocation() << " | "
                     << (group[k].getIsFraud() ? "Fraud" : "Not Fraud") << endl;
            }
            totalResults += displayCount;
        }

        delete[] group;
        i = j;
    }

    // Store total timing metrics
    searchTime = totalSearchTime;
    sortTime = channelSortTime + totalAmountSortTime;

      // Add clear and short performance summary
    cout << "\n========================================" << endl;
    cout << "ARRAY PERFORMANCE SUMMARY" << endl;
    cout << "========================================" << endl;
    cout << "Processing Time:" << endl;
    cout << "  Sorting: " << (channelSortTime + totalAmountSortTime).count() << " ms" << endl;
    cout << "  Searching: " << totalSearchTime.count() << " ms" << endl;
    cout << "  Total: " << (channelSortTime + totalAmountSortTime + totalSearchTime).count() << " ms" << endl;
    
    cout << "Memory Usage:" << endl;
    double memoryMB = (numTransactions * sizeof(Transaction)) / (1024.0 * 1024.0);
    cout << "   Dataset: " << memoryMB << " MB (" << numTransactions << " transactions)" << endl;
    cout << "   Results Displayed: " << totalResults << " transactions" << endl;
    cout << "========================================" << endl;
    
}

// Process the array structure silently without printing
void ArrayBasedCollection::processSilently(Transaction arr[], int numTransactions, string &searchKey)
{
    if (numTransactions == 0 || arr == nullptr) {
        return;
    }

    // Sort by payment channel first with timing
    auto channelSortStart = chrono::high_resolution_clock::now();
    mergeSortByPaymentChannel(arr, 0, numTransactions - 1);
    auto channelSortEnd = chrono::high_resolution_clock::now();
    auto channelSortTime = chrono::duration_cast<chrono::milliseconds>(channelSortEnd - channelSortStart);

    chrono::milliseconds totalSearchTime = chrono::milliseconds::zero();
    chrono::milliseconds totalAmountSortTime = chrono::milliseconds::zero();
    
    int currentIndex = 0;
    
    while (currentIndex < numTransactions) {
        string currentChannel = transactions[currentIndex].getPaymentChannel();
        
        // Find the range of transactions for this channel
        int channelStart = currentIndex;
        int channelEnd = currentIndex;
        while (channelEnd < numTransactions && 
               transactions[channelEnd].getPaymentChannel() == currentChannel) {
            channelEnd++;
        }
        int channelSize = channelEnd - channelStart;

        // Search for matching transactions with timing (SILENTLY)
        auto searchStart = chrono::high_resolution_clock::now();
        
        // Create temporary array for matching transactions
        Transaction* matchingTransactions = new Transaction[channelSize];
        int matchingCount = 0;
        
        // Search in the channel range
        for (int i = channelStart; i < channelEnd; i++) {
            if (transactions[i].getTransactionType() == searchKey) {
                matchingTransactions[matchingCount] = transactions[i];
                matchingCount++;
            }
        }
        
        auto searchEnd = chrono::high_resolution_clock::now();
        totalSearchTime += chrono::duration_cast<chrono::milliseconds>(searchEnd - searchStart);

        if (matchingCount > 0) {
            // Sort the matching group with timing (SILENTLY)
            auto amountSortStart = chrono::high_resolution_clock::now();
            mergeSortByAmountThenLocation(matchingTransactions, 0, matchingCount - 1);
            auto amountSortEnd = chrono::high_resolution_clock::now();
            totalAmountSortTime += chrono::duration_cast<chrono::milliseconds>(amountSortEnd - amountSortStart);

            // NO PRINTING - just process silently
        }

        // Clean up temporary array
        delete[] matchingTransactions;
        
        // Move to next channel
        currentIndex = channelEnd;
    }

    // Store total timing metrics
    searchTime = totalSearchTime;
    sortTime = channelSortTime + totalAmountSortTime;
}
// Helper function to translate internal type names to user-friendly names

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