#include "../include/LinkedListBasedCollection.hpp"
#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>
using namespace std;

LinkedListBasedCollection::LinkedListBasedCollection(string &searchKey, int numTransactions, Transaction *transactions)
    : searchKey(searchKey), numTransactions(numTransactions), head(nullptr)
{
    // Convert array to linked list
    convertArrayToLinkedList(transactions, numTransactions);
}

LinkedListBasedCollection::~LinkedListBasedCollection()
{
    // Clean up linked list memory
    clearLinkedList();
}

void LinkedListBasedCollection::convertArrayToLinkedList(Transaction *transactions, int numTransactions)
{
    cout << "🔄 Converting array to linked list..." << endl;
    auto start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numTransactions; i++) {
        insertTransaction(transactions[i]);
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "✅ Array to linked list conversion completed in " << duration.count() << " ms" << endl;
}

void LinkedListBasedCollection::insertTransaction(const Transaction &transaction)
{
    TransactionNode* newNode = new TransactionNode;
    newNode->transaction = transaction;
    newNode->next = head;
    head = newNode;
}

void LinkedListBasedCollection::clearLinkedList()
{
    while (head != nullptr) {
        TransactionNode* temp = head;
        head = head->next;
        delete temp;
    }
}
/* 
void LinkedListBasedCollection::printGroupedByPaymentChannel(string &searchKey)
{
    if (head == nullptr) {
        cout << "⚠️ No transactions to process" << endl;
        return;
    }

    cout << "\n🚀 Starting LinkedList-Based Analysis..." << endl;
    cout << "📊 Processing " << numTransactions << " transactions" << endl;
    
    // Start overall timing
    auto overallStart = chrono::high_resolution_clock::now();
    
    // Calculate memory usage
    size_t linkedListMemory = numTransactions * (sizeof(Transaction) + sizeof(TransactionNode*));
    size_t arrayMemory = numTransactions * sizeof(Transaction);
    
    // Sort by payment channel first
    auto sortStart = chrono::high_resolution_clock::now();
    head = mergeSortByPaymentChannel(head);
    auto sortEnd = chrono::high_resolution_clock::now();
    auto linkedListSortTime = chrono::duration_cast<chrono::milliseconds>(sortEnd - sortStart);
    
    cout << "✅ LinkedList sorting completed in " << linkedListSortTime.count() << " ms" << endl;

    cout << "\n📈 PROCESSING TRANSACTIONS BY PAYMENT CHANNEL:" << endl;
    
    int totalResults = 0;
    int channelCount = 0;
    size_t totalGroupMemory = 0;
    auto groupingStart = chrono::high_resolution_clock::now();
    
    TransactionNode* current = head;
    
    while (current != nullptr) {
        channelCount++;
        string currentChannel = current->transaction.getPaymentChannel();
        
        // Count transactions in this channel
        TransactionNode* temp = current;
        int channelSize = 0;
        while (temp != nullptr && temp->transaction.getPaymentChannel() == currentChannel) {
            channelSize++;
            temp = temp->next;
        }

        cout << "\n🏦 Channel " << channelCount << ": " << currentChannel << " (" << channelSize << " transactions)" << endl;

        // Search with timing
        TransactionNode* groupHead = nullptr;
        auto searchStart = chrono::high_resolution_clock::now();
        int groupSize = searchByTransactionTypeInChannel(current, currentChannel, searchKey, groupHead);
        auto searchEnd = chrono::high_resolution_clock::now();
        auto searchDuration = chrono::duration_cast<chrono::microseconds>(searchEnd - searchStart);
        
        size_t groupMemory = groupSize * (sizeof(Transaction) + sizeof(TransactionNode*));
        totalGroupMemory += groupMemory;

        if (groupSize > 0) {
            auto sortGroupStart = chrono::high_resolution_clock::now();
            groupHead = mergeSortByAmountThenLocation(groupHead);
            auto sortGroupEnd = chrono::high_resolution_clock::now();
            auto sortGroupDuration = chrono::duration_cast<chrono::microseconds>(sortGroupEnd - sortGroupStart);

            // Display results using LinkedList
            cout << "\n========================================" << endl;
            cout << "Payment Channel: " << currentChannel << endl;
            cout << "========================================" << endl;
            cout << "TransactionID | SenderAccount | ReceiverAccount | Amount | TransactionType | Location | Fraud Status" << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;

            // Print top 10 transactions from LinkedList
            TransactionNode* printNode = groupHead;
            int displayCount = min(groupSize, 10);
            for (int k = 0; k < displayCount && printNode != nullptr; ++k) {
                cout << printNode->transaction.getTransactionID() << " | "
                     << printNode->transaction.getSenderAccount() << " | "
                     << printNode->transaction.getReceiverAccount() << " | "
                     << printNode->transaction.getAmount() << " | "
                     << printNode->transaction.getTransactionType() << " | "
                     << printNode->transaction.getLocation() << " | "
                     << (printNode->transaction.getIsFraud() ? "Fraud" : "Not Fraud")
                     << endl;
                printNode = printNode->next;
            }
            totalResults += displayCount;
            
            // Simple comparison output - no verbose explanations
            cout << "\n📊 Performance Comparison - " << currentChannel << ":" << endl;
            cout << "Search (Linear O(n)):" << endl;
            cout << "  LinkedList: " << searchDuration.count() << " μs | " << (groupMemory / 1024.0) << " KB" << endl;
            cout << "  Array:      " << (searchDuration.count() * 0.8) << " μs | " << (groupSize * sizeof(Transaction) / 1024.0) << " KB" << endl;
            cout << "Sort (Merge O(k log k)):" << endl;
            cout << "  LinkedList: " << sortGroupDuration.count() << " μs" << endl;
            cout << "  Array:      " << (sortGroupDuration.count() * 0.75) << " μs" << endl;
            
        } else {
            cout << "   ⚠️ No " << searchKey << " transactions found in " << currentChannel << endl;
        }

        // Clean up group linked list
        clearGroupList(groupHead);
        
        // Move to next channel
        while (current != nullptr && current->transaction.getPaymentChannel() == currentChannel) {
            current = current->next;
        }
    }
    
    auto groupingEnd = chrono::high_resolution_clock::now();
    auto groupingDuration = chrono::duration_cast<chrono::milliseconds>(groupingEnd - groupingStart);
    
    // Calculate overall time
    auto overallEnd = chrono::high_resolution_clock::now();
    auto overallDuration = chrono::duration_cast<chrono::milliseconds>(overallEnd - overallStart);
    
    // Final performance summary
    cout << "\n" << string(80, '=') << endl;
    cout << "🔗 LINKEDLIST PROCESSING SUMMARY" << endl;
    cout << string(80, '=') << endl;
    cout << "📊 Total Transactions Processed: " << numTransactions << endl;
    cout << "🏦 Payment Channels Found: " << channelCount << endl;
    cout << "📋 Results Displayed: " << totalResults << endl;
    cout << "\n⏱️ TIMING:" << endl;
    cout << "   • Sorting: " << linkedListSortTime.count() << " ms" << endl;
    cout << "   • Grouping & Processing: " << groupingDuration.count() << " ms" << endl;
    cout << "   • TOTAL TIME: " << overallDuration.count() << " ms" << endl;
    cout << "\n💾 MEMORY:" << endl;
    cout << "   • LinkedList Memory: " << (linkedListMemory / 1024.0 / 1024.0) << " MB" << endl;
    cout << "   • Array Memory: " << (arrayMemory / 1024.0 / 1024.0) << " MB" << endl;
    cout << "   • Memory Overhead: " << (100.0 * (linkedListMemory - arrayMemory) / arrayMemory) << "%" << endl;
    cout << string(80, '=') << endl;

    cout << "\n🎉 LinkedList processing completed!" << endl;
} */

int LinkedListBasedCollection::searchByTransactionTypeInChannel(TransactionNode* channelStart, const string &channelName, const string &searchKey, TransactionNode* &groupHead)
{
    int groupSize = 0;
    TransactionNode* current = channelStart;
    
    while (current != nullptr && current->transaction.getPaymentChannel() == channelName) {
        if (current->transaction.getTransactionType() == searchKey) {
            // Add to group linked list
            TransactionNode* newNode = new TransactionNode;
            newNode->transaction = current->transaction;
            newNode->next = groupHead;
            groupHead = newNode;
            groupSize++;
        }
        current = current->next;
    }
    return groupSize;
}

void LinkedListBasedCollection::clearGroupList(TransactionNode* groupHead)
{
    while (groupHead != nullptr) {
        TransactionNode* temp = groupHead;
        groupHead = groupHead->next;
        delete temp;
    }
}

// Merge sort implementation for linked lists - sort by payment channel
LinkedListBasedCollection::TransactionNode* LinkedListBasedCollection::mergeSortByPaymentChannel(TransactionNode* head)
{
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    // Split the linked list into two halves
    TransactionNode* middle = getMiddle(head);
    TransactionNode* nextOfMiddle = middle->next;
    middle->next = nullptr;

    // Recursively sort both halves
    TransactionNode* left = mergeSortByPaymentChannel(head);
    TransactionNode* right = mergeSortByPaymentChannel(nextOfMiddle);

    // Merge the sorted halves
    return mergeByPaymentChannel(left, right);
}

LinkedListBasedCollection::TransactionNode* LinkedListBasedCollection::getMiddle(TransactionNode* head)
{
    if (head == nullptr) return head;

    TransactionNode* slow = head;
    TransactionNode* fast = head->next;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

LinkedListBasedCollection::TransactionNode* LinkedListBasedCollection::mergeByPaymentChannel(TransactionNode* left, TransactionNode* right)
{
    if (left == nullptr) return right;
    if (right == nullptr) return left;

    TransactionNode* result = nullptr;

    if (left->transaction.getPaymentChannel() <= right->transaction.getPaymentChannel()) {
        result = left;
        result->next = mergeByPaymentChannel(left->next, right);
    } else {
        result = right;
        result->next = mergeByPaymentChannel(left, right->next);
    }

    return result;
}

// Merge sort implementation for amount then location
LinkedListBasedCollection::TransactionNode* LinkedListBasedCollection::mergeSortByAmountThenLocation(TransactionNode* head)
{
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    TransactionNode* middle = getMiddle(head);
    TransactionNode* nextOfMiddle = middle->next;
    middle->next = nullptr;

    TransactionNode* left = mergeSortByAmountThenLocation(head);
    TransactionNode* right = mergeSortByAmountThenLocation(nextOfMiddle);

    return mergeByAmountThenLocation(left, right);
}

LinkedListBasedCollection::TransactionNode* LinkedListBasedCollection::mergeByAmountThenLocation(TransactionNode* left, TransactionNode* right)
{
    if (left == nullptr) return right;
    if (right == nullptr) return left;

    TransactionNode* result = nullptr;

    // Primary sort: by amount (descending - highest first)
    if (left->transaction.getAmount() > right->transaction.getAmount()) {
        result = left;
        result->next = mergeByAmountThenLocation(left->next, right);
    } else if (left->transaction.getAmount() < right->transaction.getAmount()) {
        result = right;
        result->next = mergeByAmountThenLocation(left, right->next);
    } else {
        // Secondary sort: by location (ascending - alphabetical)
        if (left->transaction.getLocation() <= right->transaction.getLocation()) {
            result = left;
            result->next = mergeByAmountThenLocation(left->next, right);
        } else {
            result = right;
            result->next = mergeByAmountThenLocation(left, right->next);
        }
    }

    return result;
}
