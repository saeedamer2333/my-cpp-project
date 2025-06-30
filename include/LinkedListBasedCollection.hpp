#pragma once
#include <string>
using namespace std;
#include "Transaction.hpp"


//declaration of LinkedListBasedCollection class
class LinkedListBasedCollection { 
private:
    struct Node {
        Transaction data;
        Node* next;
    };
    Node* head;


public:
    LinkedListBasedCollection();
    ~LinkedListBasedCollection();
    void addTransaction(const Transaction& t);
    // Other member functions...
};
