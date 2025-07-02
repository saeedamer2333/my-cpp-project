#include <iostream>
#include <chrono> // For measuring performance
#include <limits> // For numeric_limits
#include <string>

// Include header files
#include "../include/DataStructureComparator.hpp"
#include "../include/Transaction.hpp"
#include "../include/CSVParser.hpp"
using namespace std;

int main()
{
    CSVParser csvparser;
    string filePath = "financial_fraud_detection_dataset.csv";
    csvparser.setFilePath(filePath);

    // Prompt for search key ONCE
    cout << "\n========== Searching by Transaction Type ==========" << endl;
    // Load first page to show available types
    if (!csvparser.loadNextPage())
    {
        cout << "No transactions found in the CSV file." << endl;
        return 1;
    }
    Transaction *transactions = csvparser.getTransactions();
    int numTransactions = 10000; // Default to 10,000 for initial display
    Transaction::showUniqueTransactionTypes(transactions, numTransactions);
    cout << "5- Exit\n";
    cout << "Enter your choice: ";
    string searchKey;
    cin >> searchKey;
    if (searchKey == "5")
    {
        cout << "Exiting program. Goodbye!\n";
        return 0;
    }
    DataStructureComparator pageComparator(transactions, numTransactions, searchKey);
    pageComparator.performComparison(); // Perform comparison for the first page    


     /*     // false = don't print summary yet
        (pageComparator); // You need to implement accumulate()
 */
/*     // Show the final summary ONCE
    cout << "\n===== FINAL PERFORMANCE COMPARISON FOR ALL DATA (" <<  << " pages) =====" << endl;
   
    cout << "All data processed. Program finished." << endl; */
    return 0;
}