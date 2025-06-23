#include <iostream>
#include <chrono> // For measuring performance
#include <limits> // For numeric_limits
#include <string> // For string operations
#include "LinkedListBasedCollection.hpp" // Include your linked list collection header
#include "ArrayBasedCollection.hpp" // Include your array-based collection header
#include <algorithm> // For sorting
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
// ========== Main Program ==========
int main()
{
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
            cout << "\n========== Main Menu ==========\n";
            cout << "1. Search by Transaction Type\n";
            cout << "2. Sort by Location (Ascending)\n";
            cout << "3. Exit to Data Structure Selection\n";
            cout << "Enter your choice: ";
            cin >> actionChoice;

            if (actionChoice == 3)
            {
                // Break out of the main menu loop and return to data structure selection
                break;
            }

            auto start = chrono::high_resolution_clock::now();

            switch (actionChoice)
            {
            case 1:
                if (dsType == ARRAY)
                {
                    // TODO: Implement search by type using Array
                    // searchByTransactionTypeArray();
                    // TODO: Automatically group by payment channel (Array)
                    // groupByPaymentChannelArray();
                }
                else
                {
                    // TODO: Implement search by type using Linked List
                    // searchByTransactionTypeList();
                    // TODO: Automatically group by payment channel (List)
                    // groupByPaymentChannelList();
                }
                break;
            case 2:
                if (dsType == ARRAY)
                {
                    // TODO: Implement sort by location using Array
                    // sortByLocationArray();
                    // TODO: Automatically group by payment channel (Array)
                    // groupByPaymentChannelArray();
                }
                else
                {
                    // TODO: Implement sort by location using Linked List
                    // sortByLocationList();
                    // TODO: Automatically group by payment channel (List)
                    // groupByPaymentChannelList();
                }
                break;
            default:
                cout << " Invalid menu option. Please choose 1, 2, or 3.\n";
                continue; // Loop again without timing
            }

            // Show result here (placeholder)
            /*  cout << "\nOperation completed. (Show results here)\n";
             auto end = chrono::high_resolution_clock::now();
             auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
             cout << "Time taken: " << duration.count() << " milliseconds\n"; */

            // Ask if user wants to export to JSON
            char exportChoice;
            cout << "\nDo you want to export the result to JSON? (y/n): ";
            cin >> exportChoice;
            if (exportChoice == 'y' || exportChoice == 'Y')
            {
                if (dsType == ARRAY)
                {
                    // exportToJson(lastResultArray, "array_output.json"); 
                    cout << "Result exported to json format\n";
                }
                else
                {
                    // exportToJsonFromList(lastResultList, "list_output.json"); 
                    cout << "Result exported to json format\n";
                }
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
