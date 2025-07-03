#pragma once
#include <string>
#include <chrono>
#include <iostream>
using namespace std;
#include "Transaction.hpp"
#include "ArrayBasedCollection.hpp"
#include "LinkedListBasedCollection.hpp"

class DataStructureComparator
{
private:
    Transaction *transactions;
    int numTransactions;
    string searchKey;

    // Performance metrics
    struct PerformanceMetrics
    {
        chrono::microseconds creationTime;
        chrono::microseconds sortingTime;
        chrono::microseconds processingTime;
        chrono::microseconds totalTime;
        size_t memoryUsage;
        int resultsDisplayed;
        int channelsProcessed;
    };

    PerformanceMetrics arrayMetrics;
    PerformanceMetrics linkedListMetrics;

public:
    DataStructureComparator(Transaction *transactions, int numTransactions, const string &searchKey);
    ~DataStructureComparator();

    // Main comparison function

    void processLinkedListStructureSilent();

    void displayFinalSummary();
    void processArrayStructureSilent();

    void setLinkedListTime(long long timeInMicroseconds)
    {
        linkedListMetrics.totalTime = chrono::microseconds(timeInMicroseconds);
    }

    void setArrayTime(long long timeInMicroseconds)
    {
        arrayMetrics.totalTime = chrono::microseconds(timeInMicroseconds);
    }

    void setLinkedListSearchTime(long long timeInMicroseconds)
    {
        linkedListMetrics.processingTime = chrono::microseconds(timeInMicroseconds);
    }

    void setLinkedListSortTime(long long timeInMicroseconds)
    {
        linkedListMetrics.sortingTime = chrono::microseconds(timeInMicroseconds);
    }

    void setArraySearchTime(long long timeInMicroseconds)
    {
        arrayMetrics.processingTime = chrono::microseconds(timeInMicroseconds);
    }

    void setArraySortTime(long long timeInMicroseconds)
    {
        arrayMetrics.sortingTime = chrono::microseconds(timeInMicroseconds);
    }

private:
    // Helper methods

    void calculateMemoryUsage();
};
