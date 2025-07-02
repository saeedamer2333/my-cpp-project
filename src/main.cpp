#include <iostream>
#include <chrono> // For measuring performance
#include <limits> // For numeric_limits
#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"

// Include header files
#include "../include/DataStructureComparator.hpp"
#include "../include/Transaction.hpp"
#include "../include/CSVParser.hpp"
#include "../include/ArrayBasedCollection.hpp"
#include "../include/LinkedListBasedCollection.hpp"

using namespace std;

int main()
{
    CSVParser csvparser;
    string filePath = "financial_fraud_detection_dataset.csv";
    csvparser.setFilePath(filePath);

    cout << "\n========== Searching by Transaction Type ==========" << endl;

    while (true)
    {

    // Load first page to show available types (same as original)
    if (!csvparser.loadNextPage())
    {
        cout << "No transactions found in the CSV file." << endl;
        return 1;
    }

    Transaction *firstPageTransactions = csvparser.getTransactions();
    int firstPageSize = csvparser.getNumTransactions();

    // Show available transaction types
    Transaction::showUniqueTransactionTypes(firstPageTransactions, firstPageSize);

    cout << "5- Exit\n";
    
    string searchKey;
    

   bool validInput = false;
    
    while (!validInput) {
        cout << "Enter your choice: ";
        cin >> searchKey;
        
        // Validate input
        if (searchKey == "5") {
            cout << "Exiting program. Goodbye!\n";
            return 0;
        }
        else if (searchKey == "1" || searchKey == "2" || searchKey == "3" || searchKey == "4") {
            // Convert number to transaction type
            if (searchKey == "1") searchKey = "CASH_OUT";
            else if (searchKey == "2") searchKey = "PAYMENT";
            else if (searchKey == "3") searchKey = "CASH_IN";
            else if (searchKey == "4") searchKey = "TRANSFER";
            
            validInput = true;
            cout << "Selected transaction type: " << searchKey << endl;
        }
        else {
            cout << "Invalid choice! Please enter 1, 2, 3, 4, or 5." << endl;
            // Clear input buffer in case of invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Initialize data structures

    // Initialize streaming for processing all data
    if (!csvparser.initializeStreaming())
    {
        cout << "Failed to initialize streaming mode." << endl;
        return 1;
    }



    // Performance tracking
    
    long long totalProcessed = 0;
    long long matchingTransactions = 0;

    // Process data in batches efficiently for both Array and LinkedList
    const int BATCH_SIZE = 1000;                 // Small batches to avoid lag with LinkedList
    vector<Transaction> allMatchingTransactions; // Store all matching transactions
    allMatchingTransactions.reserve(100000);     // Reserve space for efficiency

    Transaction currentTransaction;

   

    // Stream ALL transactions and collect matching ones
    while (csvparser.getNextTransaction(currentTransaction))
    {
        totalProcessed++;

        // Collect matching transactions for processing
        if (currentTransaction.getTransactionType() == searchKey)
        {
            allMatchingTransactions.push_back(currentTransaction);
            matchingTransactions++;
        }

     /*    // Progress update every 500,000 transactions
        if (totalProcessed % 500000 == 0)
        {
            auto currentTime = chrono::high_resolution_clock::now();
            auto elapsed = chrono::duration_cast<chrono::seconds>(currentTime - startTime);
            double rate = totalProcessed / (elapsed.count() > 0 ? elapsed.count() : 1);
            cout << "Progress: " << totalProcessed << " rows processed in " << elapsed.count()
                 << "s (" << (int)rate << " rows/sec). Found " << matchingTransactions
                 << " matching transactions." << endl;
        } */
    }

    csvparser.closeStream();

    

    // Now process ALL matching transactions with both data structures
    if (matchingTransactions > 0)
    {

        // Convert vector to array for DataStructureComparator
        Transaction *allMatchingArray = new Transaction[allMatchingTransactions.size()];
        for (long long i = 0; i < matchingTransactions; i++)
        {
            allMatchingArray[i] = allMatchingTransactions[i];
        }

         ArrayBasedCollection arrayCollection(searchKey, matchingTransactions, allMatchingArray);
        arrayCollection.printGroupedByPaymentChannel(allMatchingArray, matchingTransactions, searchKey);

        // Process LinkedList in small batches (like before) to avoid issues
        cout << "\n========== FEW SECONDS OF PROCESSING THE COMPLETED LINKED LIST AND ARRAY WITH " << matchingTransactions << " MATCHING TRANSACTIONS " << searchKey << " ==========" << endl;
        cout << " For TRANSACTION TYPE " << searchKey << " WITH " << BATCH_SIZE << " BATCHES ==========" << endl;

        const int LL_BATCH_SIZE = 1000; // Small batches for LinkedList
        long long linkedListTotalTime = 0;
        int llBatchCount = 0;
        long long totalLinkedListTime = 0;
        
        for (long long i = 0; i < matchingTransactions; i += LL_BATCH_SIZE)
        {
            long long batchSize = min((long long)LL_BATCH_SIZE, matchingTransactions - i);
            Transaction *llBatch = &allMatchingArray[i];
            auto llBatchStart = chrono::high_resolution_clock::now();
            DataStructureComparator fullComparatorForLinkedList(llBatch, batchSize, searchKey);
            fullComparatorForLinkedList.processArrayStructureSilent(); // Array processing silently
            // Process LinkedList silently
            fullComparatorForLinkedList.processLinkedListStructureSilent(); // LinkedList processing silently

             auto llBatchEnd = chrono::high_resolution_clock::now();
            
            auto batchTime = chrono::duration_cast<chrono::microseconds>(llBatchEnd - llBatchStart);
            totalLinkedListTime += batchTime.count();
            llBatchCount++;
         
        }
        DataStructureComparator fullComparator(allMatchingArray, matchingTransactions, searchKey);
        fullComparator.setLinkedListTime(totalLinkedListTime);
        fullComparator.setArrayTime(llBatchCount);

        fullComparator.displayFinalSummary(); // Display final summary

     

        delete[] allMatchingArray;
    }
    else
    {
        cout << "No matching transactions found for search key: " << searchKey << endl;
    }

   

    // Export option (using matching transactions if any, otherwise first page)
    char exportChoice;
    // Validate export choice input
        bool validExportChoice = false;
    
    while (!validExportChoice) {
        cout << "\nWould you like to export the top 10 results as JSON? (y/n): ";
        cin >> exportChoice;
        
        if (exportChoice == 'y' || exportChoice == 'Y' || exportChoice == 'n' || exportChoice == 'N') {
            validExportChoice = true;
        }
        else {
            cout << "Invalid input! Please enter 'y' for Yes or 'n' for No." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    if (exportChoice == 'y' || exportChoice == 'Y')
    {
        cout << "\nExporting top 10 results to exports/top10_results.json..." << endl;
        system("mkdir exports >nul 2>nul");

        nlohmann::json jarray = nlohmann::json::array();

        if (matchingTransactions > 0)
        {
            // Export from matching transactions
            int exportCount = (matchingTransactions < 10) ? matchingTransactions : 10;
            for (int i = 0; i < exportCount; ++i)
            {
                jarray.push_back(allMatchingTransactions[i].to_json());
            }
            cout << "Top " << exportCount << " matching results exported!" << endl;
        }
        else
        {
            // Fallback to first page
            int exportCount = (firstPageSize < 10) ? firstPageSize : 10;
            for (int i = 0; i < exportCount; ++i)
            {
                jarray.push_back(firstPageTransactions[i].to_json());
            }
            cout << "Top " << exportCount << " sample results exported!" << endl;
        }

        ofstream outFile("exports/top10_results.json");
        outFile << jarray.dump(4);
        outFile.close();
        cout << "Results exported to exports/top10_results.json" << endl;
    }

  
    string exitChoice;

     bool validExitChoice = false;
    
    while (!validExitChoice) {
        cout << "\nDo you want to exit? (y/n): ";
        cin >> exitChoice;
        
        if (exitChoice == "y" || exitChoice == "Y" || exitChoice == "n" || exitChoice == "N") {
            validExitChoice = true;
        }
        else {
            cout << " Invalid input! Please enter 'y' for Yes or 'n' for No." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    
    if (exitChoice == "y" || exitChoice == "Y")
    {
        cout << "Exiting program. Goodbye!\n";
        return 0;
    }
    }
}