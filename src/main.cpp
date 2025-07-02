#include <iostream>
#include <chrono> // For measuring performance
#include <limits> // For numeric_limits
#include <string>
#include "json.hpp"

// Include header files
#include "../include/DataStructureComparator.hpp"
#include "../include/Transaction.hpp"
#include "../include/CSVParser.hpp"
using namespace std;

int main()
{
    CSVParser csvparser;
    string filePath = "C:/Users/user/Downloads/projects/my-cpp-project/financial_fraud_detection_dataset.csv";
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
    int numTransactions = csvparser.getNumTransactions(); // Use actual loaded page size
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

<<<<<<< Updated upstream
    // Ask user if they want to export the top 10 results as JSON
    char exportChoice;
    cout << "\nWould you like to export the top 10 results as JSON? (y/n): ";
    cin >> exportChoice;
    if (exportChoice == 'y' || exportChoice == 'Y') {
        cout << "\nExporting top 10 results to exports/top10_results.json..." << endl;
        system("mkdir exports >nul 2>nul"); // Create folder if it doesn't exist (Windows)
        nlohmann::json jarray = nlohmann::json::array();
        int exportCount = (numTransactions < 10) ? numTransactions : 10;
        for (int i = 0; i < exportCount; ++i) {
            jarray.push_back(transactions[i].to_json());
        }
        std::ofstream outFile("exports/top10_results.json");
        outFile << jarray.dump(4);
        outFile.close();
        cout << "Top 10 results exported to exports/top10_results.json" << endl;
    }
    return 0;
=======
    cout << "Do u wanna to export as json file? (y/n)" << endl;
    string exportChoice;
    cin >> exportChoice;
    if (exportChoice == "y")
    {
        // Call export function (not implemented in this snippet)
        cout << "Exporting results to JSON file..." << endl;
    }

    cout << " do you wanna to exist? (y/n)" << endl;
    string exitChoice;
    cin >> exitChoice;
    if (exitChoice == "y")
    {
        cout << "Exiting program. Goodbye!\n";
        return 0;
    }
>>>>>>> Stashed changes
}