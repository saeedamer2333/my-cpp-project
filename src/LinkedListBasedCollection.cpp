#include "../include/LinkedListBasedCollection.hpp"
#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>
using namespace std;

LinkedListBasedCollection::LinkedListBasedCollection(string &searchKey, int numTransactions, Transaction *transactions)
    : searchKey(searchKey), numTransactions(numTransactions), head(nullptr),
      searchTime(chrono::milliseconds::zero()), sortTime(chrono::milliseconds::zero())
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
    for (int i = 0; i < numTransactions; i++) {
        insertTransaction(transactions[i]);
    }
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

void LinkedListBasedCollection::processSilently(string &searchKey)
{
    if (head == nullptr) {
        return;
    }

    // Sort by payment channel first with timing
    auto channelSortStart = chrono::high_resolution_clock::now();
    head = mergeSortByPaymentChannel(head);
    auto channelSortEnd = chrono::high_resolution_clock::now();
    auto channelSortTime = chrono::duration_cast<chrono::milliseconds>(channelSortEnd - channelSortStart);

    chrono::milliseconds totalSearchTime = chrono::milliseconds::zero();
    chrono::milliseconds totalAmountSortTime = chrono::milliseconds::zero();
    
    TransactionNode* current = head;
    
    while (current != nullptr) {
        string currentChannel = current->transaction.getPaymentChannel();
        
        // Count transactions in this channel
        TransactionNode* temp = current;
        int channelSize = 0;
        while (temp != nullptr && temp->transaction.getPaymentChannel() == currentChannel) {
            channelSize++;
            temp = temp->next;
        }

        // Search for matching transactions with timing (SILENTLY)
        auto searchStart = chrono::high_resolution_clock::now();
        TransactionNode* groupHead = nullptr;
        int groupSize = searchByTransactionTypeInChannel(current, currentChannel, searchKey, groupHead);
        auto searchEnd = chrono::high_resolution_clock::now();
        totalSearchTime += chrono::duration_cast<chrono::milliseconds>(searchEnd - searchStart);

        if (groupSize > 0) {
            // Sort the group with timing (SILENTLY)
            auto amountSortStart = chrono::high_resolution_clock::now();
            groupHead = mergeSortByAmountThenLocation(groupHead);
            auto amountSortEnd = chrono::high_resolution_clock::now();
            totalAmountSortTime += chrono::duration_cast<chrono::milliseconds>(amountSortEnd - amountSortStart);

            // NO PRINTING - just process silently
        }

        // Clean up group linked list
        clearGroupList(groupHead);
        
        // Move to next channel
        while (current != nullptr && current->transaction.getPaymentChannel() == currentChannel) {
            current = current->next;
        }
    }

    // Store total timing metrics
    searchTime = totalSearchTime;
    sortTime = channelSortTime + totalAmountSortTime;
}

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
