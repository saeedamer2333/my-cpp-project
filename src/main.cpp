#include <iostream>
#include <chrono> // For measuring performance
#include <limits> // For numeric_limits
#include <string>

// Include header files
// Include your linked list collection header
#include "../include/ArrayBasedCollection.hpp"
#include "../include/Transaction.hpp"
#include "../include/CSVParser.hpp" // Add this line to include the CSVParser class header
using namespace std;

int main()
{
    CSVParser csvparser;
    Transaction transaction;

    string filePath = "financial_fraud_detection_dataset.csv";

    csvparser.loadCSV(filePath); // Load CSV data

    /* Transaction transactions[] = {
            Transaction("T001", "ACC001", "ACC002", 1500.0, "TRANSFER", "New York", "ONLINE", false),
            Transaction("T002", "ACC003", "ACC004", 2500.0, "PAYMENT", "Los Angeles", "ATM", false),
            Transaction("T003", "ACC005", "ACC006", 750.0, "TRANSFER", "Chicago", "BRANCH", false),
            Transaction("T004", "ACC007", "ACC008", 3000.0, "WITHDRAWAL", "Miami", "ONLINE", true)
        };
        int numTransactions = 4; */
    Transaction *transactions;
    transactions = csvparser.getTransactions(); // Get transactions from CSV parser

    if (transactions == nullptr)
    {
        cout << "No transactions found in the CSV file." << endl;
        return 1; // Exit if no transactions are available
    }

    int numTransactions = csvparser.getNumTransactions();

    while (true)
    {
        cout << "\n========== Searching by Transaction Type ==========\n";
        Transaction::showUniqueTransactionTypes(transactions, numTransactions);
        cout << "5- Exit\n";
        cout << "Enter your choice: ";
        string searchKey;
        cin >> searchKey;

        ArrayBasedCollection arrayCollection(searchKey, numTransactions, transactions);
        arrayCollection.printGroupedByPaymentChannel(transactions, numTransactions, searchKey);
        // Ask if user wants to export to JSON
        char exportChoice;
        cout << "\nDo you want to export to JSON? (y/n): ";
        cin >> exportChoice;
        if (exportChoice == 'y' || exportChoice == 'Y')
        {
            // TODO: Call exportToJson function
            // exportToJson(...);
            cout << "Result exported to json format\n";
        }

        // Ask if user wants to exit or go back to the very beginning
        char exitChoice;
        cout << "\nDo you want to exit the program? (y/n): ";
        cin >> exitChoice;
        if (exitChoice == 'y' || exitChoice == 'Y')
        {
            cout << "Exiting program. Goodbye!\n";
            return 0;
        }
        else
        {
            // Break out of the main menu loop and return to data structure selection
            break;
        }
    }

    return 0;
}