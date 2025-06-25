#include <iostream>
#include <chrono>                        // For measuring performance
#include <limits>                        // For numeric_limits
#include <string>   

// Include header files
#include "LinkedListBasedCollection.hpp" // Include your linked list collection header
#include "ArrayBasedCollection.hpp"      // Include your array-based collection header
#include "Transaction.hpp"               // Transaction class header
             

using namespace std;

enum DataStructureType
{
    ARRAY,
    LINKED_LIST
}; // Enum to represent data structure types

// ========== Function Prototypes ==========
/*
void searchByTransactionTypeArray();
void searchByTransactionTypeList();
void sortByLocationArray();
void sortByLocationList();
void groupByPaymentChannelArray();
void groupByPaymentChannelList();
 */
// void showAvailableTransactionTypes(); // Function to display available transaction types

// Path to the CSV data file
string filePath = "financial_fraud_detection_dataset.csv";

// ========== Main Program ==========
int main()
{
    // Create instances of collections
    
    while (true) // Outer loop for the whole program
    {
        int dsChoice, actionChoice;
        DataStructureType dsType;

        // Loop until valid data structure selected
        while (true)
        {
            cout << "======== Data Structure Selection ========\n";
            cout << "1. Use Array-based implementation\n";
            cout << "2. Use Linked List-based implementation\n";
            cout << "3. Exit\n";
            cout << "Enter your choice (1 or 2): ";
            cin >> dsChoice;
            if (cin.fail())
            {
                cin.clear();                                         // clear error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear any extra input
            if (dsChoice == 1)
            {
                dsType = ARRAY;
                break;
            }
            else if (dsChoice == 2)
            {
                dsType = LINKED_LIST;
                break;
            }
            else if (dsChoice == 3)
            {
                cout << "Exiting program. Goodbye!\n";
                return 0; // Exit the program
            }
            else
            {
                cout << "Invalid selection. Please choose 1 or 2.\n";
            }
        }

        // Loop main menu
        while (true)
        {
            cout << "\n========== Searching by  Transaction Type ==========\n";
            // Call a function to show available transaction types
            // showAvailableTransactionTypes(); // <-- Add this function to display types
            cout << "5- Exit\n";
            cout << "Enter your choice: ";
            int transactionChoice;
            cin >> transactionChoice;
            if (transactionChoice == 5)
            {
                break; // Back to data structure selection
            }
            // TODO: Call searchByTransactionTypeArray/List based on dsType and transactionChoice
            // searchByTransactionTypeArray(transactionChoice);
            // searchByTransactionTypeList(transactionChoice);

            cout << "\n========== Sorting Options ==========\n";
            cout << "1. Sort by Location\n";
            cout << "2. Sort by Payment Channel\n";
            cout << "3. No Sorting\n";
            cout << "Enter your choice: ";
            int sortChoice;
            cin >> sortChoice;
            // TODO: Call sortByLocationArray/List or groupByPaymentChannelArray/List based on dsType and sortChoice
            // sortByLocationArray();
            // sortByLocationList();
            // groupByPaymentChannelArray();
            // groupByPaymentChannelList();

            // Show top 10 results (placeholder)
            cout << "\nShowing top 10 transactions grouped by payment channel, location or no sorting (show top 10 by location)\n";
            // TODO: Display top 10 results here

            // Show time and memory efficiency (placeholder)
            cout << "\nTime taken: ... milliseconds\n";
            cout << "Memory used: ... KB\n";

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
    }
    return 0;
}
