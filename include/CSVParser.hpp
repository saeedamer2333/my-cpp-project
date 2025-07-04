#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Transaction.hpp"

using namespace std;

class CSVParser
{
public:
    // Enum for parse result types - better error categorization
    enum class ParseResult
    {
        SUCCESS,
        PARSE_ERROR,      // Numeric conversion errors
        VALIDATION_ERROR, // Field validation failures
        MALFORMED         // Insufficient columns or structure issues
    };

private:
    int numTransactions = 0;
    Transaction *transactions = nullptr;
    std::string filePath;
    int pageCounter = 0;
    ifstream fileStream; // For streaming
    bool isStreamMode = false;
    long long totalProcessed = 0;

    // Helper methods for cleaner code and robust error handling
    bool expandCapacity(int &capacity);
    ParseResult parseLineWithValidation(const string &line, string &transaction_id,
                                        string &sender_account, string &receiver_account,
                                        double &amount, string &transaction_type,
                                        string &location, string &payment_channel, bool &is_fraud);
    bool parseLine(const string &line, string &transaction_id, string &sender_account,
                   string &receiver_account, double &amount, string &transaction_type,
                   string &location, string &payment_channel, bool &is_fraud);

public:
    CSVParser();
    ~CSVParser();
    void setFilePath(const std::string &path);
    bool loadNextPage();
    int getNumTransactions();
    Transaction *getTransactions();

    // New streaming methods for low memory usage
    bool initializeStreaming();
    bool getNextTransaction(Transaction &transaction);
    void closeStream();
    long long getTotalProcessed() const;
};
