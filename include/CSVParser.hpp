#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Transaction.hpp"

using namespace std;

class CSVParser
{
private:
    int numTransactions = 0;
    Transaction *transactions = nullptr;
    

public:
    CSVParser();
    ~CSVParser();
    void loadCSV(const string &filePath);
    int getNumTransactions() const;
    Transaction* getTransactions() const;
};
