#include <iostream>
#include <chrono>
#include <limits>
#include <string>
#include <fstream>
#include <algorithm> // For std::min
#include "json.hpp"

// Include header files
#include "../include/DataStructureComparator.hpp"
#include "../include/Transaction.hpp"
#include "../include/CSVParser.hpp"
#include "../include/ArrayBasedCollection.hpp"
#include "../include/LinkedListBasedCollection.hpp"

using namespace std;

// --- Forward Declarations for Helper Functions ---

// Handles the entire search process using the two-pass method
void handleSearch(CSVParser &csvparser, string &searchKey);

// Prompts user for exporting data to JSON (now takes a pointer and size)
void askToExport(const Transaction *transactions, long long count, int exportLimit);

// Gets a valid search key from the user (no changes needed)
string getSearchKeyFromUser(Transaction *firstPageTransactions, int firstPageSize);

// --- Main Function ---

int main()
{
    CSVParser csvparser;
    string filePath = "financial_fraud_detection_dataset.csv";
    csvparser.setFilePath(filePath);

    // Load the first page ONCE to show available transaction types.
    if (!csvparser.loadNextPage())
    {
        cout << "Error: No transactions found in the CSV file." << endl;
        return 1;
    }
    Transaction *firstPageTransactions = csvparser.getTransactions();
    int firstPageSize = csvparser.getNumTransactions();

    // Main program loop to allow multiple searches
    while (true)
    {
        cout << "\n========== New Search: Select Transaction Type ==========" << endl;

        string searchKey = getSearchKeyFromUser(firstPageTransactions, firstPageSize);
        if (searchKey.empty())
        { // User chose to exit
            break;
        }

        cout << "Searching for transaction type: " << searchKey << endl;

        // The core logic is now in this function
        handleSearch(csvparser, searchKey);

        // Ask if the user wants to continue
        string continueChoice;
        while (true)
        {
            cout << "\nDo you want to perform another search? (y/n): ";
            cin >> continueChoice;
            if (continueChoice == "y" || continueChoice == "Y" || continueChoice == "n" || continueChoice == "N")
            {
                break;
            }
            cout << "Invalid input! Please enter 'y' for Yes or 'n' for No." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (continueChoice == "n" || continueChoice == "N")
        {
            cout << "Exiting program. Goodbye!\n";
            break;
        }
    }

    return 0;
}

// --- Helper Function Implementations ---

/**
 * @brief Handles the full search process using a two-pass approach to avoid std::vector.
 */
void handleSearch(CSVParser &csvparser, string &searchKey)
{
    // --- 1. FIRST PASS: Count matching transactions ---
    cout << "Pass 1: Counting matching transactions... Please wait." << endl;
    if (!csvparser.initializeStreaming())
    {
        cout << "Failed to initialize streaming for counting." << endl;
        return;
    }

    long long matchingCount = 0;
    long long totalProcessed = 0;
    Transaction tempTransaction;
    while (csvparser.getNextTransaction(tempTransaction))
    {
        totalProcessed++;
        if (tempTransaction.getTransactionType() == searchKey)
        {
            matchingCount++;
        }
    }
    csvparser.closeStream();

    cout << "Finished scanning " << totalProcessed << " rows." << endl;
    cout << "Found " << matchingCount << " matching transactions for type '" << searchKey << "'." << endl;

    if (matchingCount == 0)
    {
        cout << "No matching transactions found to process." << endl;
        // Offer to export from the first page as a fallback
        askToExport(csvparser.getTransactions(), csvparser.getNumTransactions(), 10);
        return;
    }

    // --- 2. Allocate memory and perform the SECOND PASS to populate the array ---
    cout << "\nPass 2: Loading matching transactions into memory..." << endl;

    // Allocate a single, perfectly sized dynamic array
    Transaction *allMatchingArray = new Transaction[matchingCount];

    if (!csvparser.initializeStreaming())
    {
        cout << "Failed to initialize streaming for populating." << endl;
        delete[] allMatchingArray; // Clean up memory
        return;
    }

    long long currentIndex = 0;
    while (csvparser.getNextTransaction(tempTransaction) && currentIndex < matchingCount)
    {
        if (tempTransaction.getTransactionType() == searchKey)
        {
            allMatchingArray[currentIndex] = tempTransaction;
            currentIndex++;
        }
    }
    csvparser.closeStream();
    cout << "Loading complete." << endl;

    // --- 3. Process collected data and measure performance ---
    DataStructureComparator finalComparator(allMatchingArray, matchingCount, searchKey);

    // --- Array Performance ---
    cout << "\n--- Measuring Array Performance ---" << endl;
    auto arrayStart = chrono::high_resolution_clock::now();

    // Create ArrayBasedCollection to get detailed timing
    ArrayBasedCollection arrayCollection(searchKey, matchingCount, allMatchingArray);
    arrayCollection.processSilently(allMatchingArray, matchingCount, searchKey);

    auto arrayEnd = chrono::high_resolution_clock::now();
    long long arrayTime = chrono::duration_cast<chrono::microseconds>(arrayEnd - arrayStart).count();

    // Set the detailed timing metrics
    finalComparator.setArrayTime(arrayTime);
    finalComparator.setArraySearchTime(arrayCollection.getSearchTime().count() * 1000); // Convert ms to μs
    finalComparator.setArraySortTime(arrayCollection.getSortTime().count() * 1000);     // Convert ms to μs
    cout << "Array processing completed." << endl;

    const int LL_BATCH_SIZE = 1000;
    long long totalLinkedListTime = 0;
    long long totalSearchTime = 0;
    long long totalSortTime = 0;

    for (long long i = 0; i < matchingCount; i += LL_BATCH_SIZE)
    {
        long long currentBatchSize = min((long long)LL_BATCH_SIZE, matchingCount - i);
        Transaction *batch_ptr = &allMatchingArray[i];

        auto llBatchStart = chrono::high_resolution_clock::now();

        // Create LinkedListBasedCollection directly to get detailed timing
        LinkedListBasedCollection batchLinkedList(searchKey, currentBatchSize, batch_ptr);
        batchLinkedList.processSilently(searchKey);

        auto llBatchEnd = chrono::high_resolution_clock::now(); // Accumulate all the timing metrics
        totalLinkedListTime += chrono::duration_cast<chrono::microseconds>(llBatchEnd - llBatchStart).count();

        // Get timing directly in microseconds (no conversion needed)
        long long batchSearchTime = batchLinkedList.getSearchTime().count();
        long long batchSortTime = batchLinkedList.getSortTime().count();

        totalSearchTime += batchSearchTime;
        totalSortTime += batchSortTime;
    }

    // Set the accumulated times in the final comparator
    finalComparator.setLinkedListTime(totalLinkedListTime);
    finalComparator.setLinkedListSearchTime(totalSearchTime);
    finalComparator.setLinkedListSortTime(totalSortTime);
    cout << "Linked List simulation completed." << endl;

    arrayCollection.printGroupedByPaymentChannel(allMatchingArray, matchingCount, searchKey);

    // --- 4. Display Results ---
    cout << "\n========== Comparison Summary ==========" << endl;
    finalComparator.displayFinalSummary();

    // --- 5. Export Option & Cleanup ---
    askToExport(allMatchingArray, matchingCount, 10);

    // CRITICAL: Clean up the dynamically allocated memory
    delete[] allMatchingArray;
}

/**
 * @brief Prompts the user to select a transaction type.
 */
string getSearchKeyFromUser(Transaction *firstPageTransactions, int firstPageSize)
{
    Transaction::showUniqueTransactionTypes(firstPageTransactions, firstPageSize);
    cout << "5- Exit\n";

    string choice;
    while (true)
    {
        cout << "Enter your choice (1-5): ";
        cin >> choice;

        if (choice == "1")
            return "withdrawal";
        if (choice == "2")
            return "deposit";
        if (choice == "3")
            return "transfer";
        if (choice == "4")
            return "payment";
        if (choice == "5")
        {
            cout << "Exit option selected." << endl;
            return ""; // Use empty string to signal exit
        }
        cout << "choice: " << choice << endl;
        cout << "Invalid choice! Please enter a number between 1 and 5." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

/**
 * @brief Asks the user if they want to export results (using a raw pointer and count).
 */
void askToExport(const Transaction *transactions, long long count, int exportLimit)
{
    char exportChoice;
    while (true)
    {
        cout << "\nWould you like to export the top results as JSON? (y/n): ";
        cin >> exportChoice;
        if (exportChoice == 'y' || exportChoice == 'Y' || exportChoice == 'n' || exportChoice == 'N')
        {
            break;
        }
        cout << "Invalid input! Please enter 'y' for Yes or 'n' for No." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (exportChoice == 'y' || exportChoice == 'Y')
    {
        cout << "\nExporting top results to exports/top_results.json..." << endl;
        system("mkdir -p exports"); // More portable command for creating directory

        nlohmann::json j_array = nlohmann::json::array();
        long long countToExport = min((long long)exportLimit, count);

        if (countToExport > 0)
        {
            for (int i = 0; i < countToExport; ++i)
            {
                j_array.push_back(transactions[i].to_json());
            }
            cout << "Top " << countToExport << " results exported." << endl;
        }
        else
        {
            cout << "No results to export." << endl;
            return;
        }

        ofstream outFile("exports/top_results.json");
        outFile << j_array.dump(4);
        outFile.close();
        cout << "File saved successfully." << endl;
    }
}