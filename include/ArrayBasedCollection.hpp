#pragma once
#include <string>
using namespace std;
#include "Transaction.hpp"
// declartion of LinkedListBasedCollection class
class ArrayBasedCollection {
private:
    Transaction* transactions;
    int capacity;
    int size;
public:
    ArrayBasedCollection(int cap);
    ~ArrayBasedCollection();
    void addTransaction(const Transaction& t);
    // Other member functions...
};
