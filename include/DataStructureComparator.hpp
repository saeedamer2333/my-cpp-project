#pragma once
#include <string>
#include <chrono>
#include <iostream>
using namespace std;
#include "Transaction.hpp"
#include "ArrayBasedCollection.hpp"
#include "LinkedListBasedCollection.hpp"

class DataStructureComparator {
private:
    Transaction* transactions;
    int numTransactions;
    string searchKey;
    
    // Performance metrics
    struct PerformanceMetrics {
        chrono::milliseconds creationTime;
        chrono::milliseconds sortingTime;
        chrono::milliseconds processingTime;
        chrono::milliseconds totalTime;
        size_t memoryUsage;
        int resultsDisplayed;
        int channelsProcessed;
    };
    
    PerformanceMetrics arrayMetrics;
    PerformanceMetrics linkedListMetrics;
    
public:
    DataStructureComparator(Transaction* transactions, int numTransactions, const string& searchKey);
    ~DataStructureComparator();
    
    // Main comparison function
    void performComparison();
    
private:
    // Helper methods
    void displayInitialComparison();
    void processArrayStructure();
    void processLinkedListStructureSilent();
    void displayCreationComparison();
    void displayFinalSummary();
    void calculateMemoryUsage();
    void displayPerformanceInsights();
    
};
