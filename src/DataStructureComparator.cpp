#include "../include/DataStructureComparator.hpp"


DataStructureComparator::DataStructureComparator(Transaction* transactions, int numTransactions, const string& searchKey)
    : transactions(transactions), numTransactions(numTransactions), searchKey(searchKey)
{
    // Initialize metrics
    arrayMetrics = {};
    linkedListMetrics = {};
}

DataStructureComparator::~DataStructureComparator()
{
    // No dynamic memory to clean up in this class
}

void DataStructureComparator::performComparison()
{
    // Process Array structure WITH printing (to display the payment channel data)
    processArrayStructure();
    
    // Process LinkedList structure SILENTLY (no printing, just timing)
    processLinkedListStructureSilent();
    
    // Show final comparison of both
    displayFinalSummary();
}

void DataStructureComparator::calculateMemoryUsage()
{
    arrayMetrics.memoryUsage = numTransactions * sizeof(Transaction);
    linkedListMetrics.memoryUsage = numTransactions * (sizeof(Transaction) + sizeof(void*));
}

void DataStructureComparator::processArrayStructure()
{
    cout << "\nProcessing Array structure..." << endl;
    auto arrayStartTime = chrono::high_resolution_clock::now();
    ArrayBasedCollection arrayCollection(searchKey, numTransactions, transactions);
    arrayCollection.printGroupedByPaymentChannel(transactions, numTransactions, searchKey);
    auto arrayEndTime = chrono::high_resolution_clock::now();
    
    arrayMetrics.totalTime = chrono::duration_cast<chrono::milliseconds>(arrayEndTime - arrayStartTime);
    arrayMetrics.sortingTime = arrayCollection.getSortTime();
    arrayMetrics.processingTime = arrayCollection.getSearchTime();
    cout << "Array processing completed in " << arrayMetrics.totalTime.count() << " ms" << endl;
}

void DataStructureComparator::processLinkedListStructureSilent()
{
    cout << "\nProcessing LinkedList structure..." << endl;
    cout << "Converting " << numTransactions << " transactions to linked list..." << endl;
    
    auto linkedListStartTime = chrono::high_resolution_clock::now();
    LinkedListBasedCollection linkedListCollection(searchKey, numTransactions, transactions);
    
    cout << "LinkedList conversion completed. Processing data..." << endl;
    
    // Process LinkedList silently - same operations but no cout output
    linkedListCollection.processSilently(searchKey);
    
    auto linkedListEndTime = chrono::high_resolution_clock::now();
    
    linkedListMetrics.totalTime = chrono::duration_cast<chrono::milliseconds>(linkedListEndTime - linkedListStartTime);
    linkedListMetrics.sortingTime = linkedListCollection.getSortTime();
    linkedListMetrics.processingTime = linkedListCollection.getSearchTime();
    
    cout << "LinkedList processing completed in " << linkedListMetrics.totalTime.count() << " ms" << endl;
}

void DataStructureComparator::displayFinalSummary()
{
    calculateMemoryUsage();
    
    cout << "\n========================================" << endl;
    cout << "PERFORMANCE COMPARISON" << endl;
    cout << "========================================" << endl;
    
    cout << "Data Structures Used:" << endl;
    cout << "  Array" << endl;
    cout << "  LinkedList" << endl;
    cout << "\n========================================" << endl;
    cout << "DATA STRUCTURE COMPARISON:" << endl;
  
    cout << "ALGORITHM COMPARISON:" << endl;

    cout << "Search Algorithm:" << endl;
    cout << "  Array: Linear Search O(n)" << endl;
    cout << "  LinkedList: Linear Search O(n)" << endl;
    cout << "Sort Algorithm:" << endl;
    cout << "  Array: Merge Sort O(n log n)" << endl;
    cout << "  LinkedList: Merge Sort O(n log n)" << endl;
  
    cout << "\n========================================" << endl;
    cout << "\nALGORITHM PERFORMANCE TIMINGS:" << endl;

    cout << "Search Time:" << endl;

    cout << "  Array: " << arrayMetrics.processingTime.count() << " ms" << endl;
    cout << "  LinkedList: " << linkedListMetrics.processingTime.count() << " ms" << endl;
    if (arrayMetrics.processingTime.count() > 0 && linkedListMetrics.processingTime.count() > 0) {
        if (arrayMetrics.processingTime < linkedListMetrics.processingTime) {
            double speedup = (double)linkedListMetrics.processingTime.count() / arrayMetrics.processingTime.count();
            cout << "  Winner: Array (" << speedup << "x faster)" << endl;
        } else {
            double speedup = (double)arrayMetrics.processingTime.count() / linkedListMetrics.processingTime.count();
            cout << "  Winner: LinkedList (" << speedup << "x faster)" << endl;
        }
    }
    
    cout << "\nSort Time:" << endl;

    cout << "  Array: " << arrayMetrics.sortingTime.count() << " ms" << endl;
    cout << "  LinkedList: " << linkedListMetrics.sortingTime.count() << " ms" << endl;
    if (arrayMetrics.sortingTime.count() > 0 && linkedListMetrics.sortingTime.count() > 0) {
        if (arrayMetrics.sortingTime < linkedListMetrics.sortingTime) {
            double speedup = (double)linkedListMetrics.sortingTime.count() / arrayMetrics.sortingTime.count();
            cout << "  Winner: Array (" << speedup << "x faster)" << endl;
        } else {
            double speedup = (double)arrayMetrics.sortingTime.count() / linkedListMetrics.sortingTime.count();
            cout << "  Winner: LinkedList (" << speedup << "x faster)" << endl;
        }
    }
    cout << "\n========================================" << endl;
    
    cout << "\nALGORITHM MEMORY EFFICIENCY:" << endl;

    cout << "Search Algorithm Memory Usage:" << endl;

    size_t arraySearchMemory = numTransactions * sizeof(Transaction); // Array search uses direct indexing
    size_t linkedListSearchMemory = numTransactions * (sizeof(Transaction) + sizeof(void*)); // LinkedList search uses node traversal
    cout << "  Array Search: " << (arraySearchMemory / 1024.0 / 1024.0) << " MB (direct access)" << endl;
    cout << "  LinkedList Search: " << (linkedListSearchMemory / 1024.0 / 1024.0) << " MB (pointer traversal)" << endl;
    double searchMemoryRatio = (double)linkedListSearchMemory / arraySearchMemory;
    cout << "  Memory Efficiency: Array uses " << searchMemoryRatio << "x less memory for search" << endl;
    
    cout << "\nSort Algorithm Memory Usage:" << endl;

    size_t arraySortMemory = numTransactions * sizeof(Transaction) * 2; // Merge sort needs temporary arrays
    size_t linkedListSortMemory = numTransactions * (sizeof(Transaction) + sizeof(void*) * 2); // LinkedList sort needs extra pointers
    cout << "  Array Sort: " << (arraySortMemory / 1024.0 / 1024.0) << " MB (temporary arrays)" << endl;
    cout << "  LinkedList Sort: " << (linkedListSortMemory / 1024.0 / 1024.0) << " MB (pointer manipulation)" << endl;
    double sortMemoryRatio = (double)linkedListSortMemory / arraySortMemory;
    if (arraySortMemory < linkedListSortMemory) {
        cout << "  Memory Efficiency: Array uses " << sortMemoryRatio << "x less memory for sorting" << endl;
    } else {
        cout << "  Memory Efficiency: LinkedList uses " << (1.0/sortMemoryRatio) << "x less memory for sorting" << endl;
    }

    cout << "\n========================================" << endl;
    cout << "\nTOTAL PROCESSING TIME:" << endl;

    cout << "Array: " << arrayMetrics.totalTime.count() << " ms" << endl;
    cout << "LinkedList: " << linkedListMetrics.totalTime.count() << " ms" << endl;
    
    if (arrayMetrics.totalTime.count() > 0 && linkedListMetrics.totalTime.count() > 0) {
        if (arrayMetrics.totalTime < linkedListMetrics.totalTime) {
            double speedup = (double)linkedListMetrics.totalTime.count() / arrayMetrics.totalTime.count();
            cout << "Winner: Array (" << speedup << "x faster)" << endl;
        } else {
            double speedup = (double)arrayMetrics.totalTime.count() / linkedListMetrics.totalTime.count();
            cout << "Winner: LinkedList (" << speedup << "x faster)" << endl;
        }
    }
    
    cout << "\nTOTAL MEMORY USAGE:" << endl;

    cout << "Array: " << (arrayMetrics.memoryUsage / 1024.0 / 1024.0) << " MB" << endl;
    cout << "LinkedList: " << (linkedListMetrics.memoryUsage / 1024.0 / 1024.0) << " MB" << endl;
    double memoryOverhead = (100.0 * linkedListMetrics.memoryUsage / arrayMetrics.memoryUsage) - 100.0;
    cout << "Difference: LinkedList uses " << memoryOverhead << "% more memory" << endl;
    cout << "========================================" << endl;
}
