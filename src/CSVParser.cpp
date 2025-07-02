#include "../include/CSVParser.hpp"
#include <filesystem>

const int PAGE_SIZE = 1000; // Much smaller page size for low memory usage

CSVParser::CSVParser() : numTransactions(0), transactions(nullptr), filePath(""), pageCounter(0), isStreamMode(false), totalProcessed(0) {}

CSVParser::~CSVParser()
{
    if (transactions != nullptr)
    {
        delete[] transactions;
        transactions = nullptr;
    }
    closeStream();
}

void CSVParser::setFilePath(const string &path)
{
    filePath = path;
    pageCounter = 0;
}

// Always loads the next page of data from the CSV file
bool CSVParser::loadNextPage()
{
    if (filePath.empty())
    {
        cerr << "ERROR: No file path set for CSVParser." << endl;
        return false;
    }

    if (transactions != nullptr)
    {
        delete[] transactions;
        transactions = nullptr;
    }
    numTransactions = 0;

    ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "   ERROR: Cannot open file " << filePath << endl;
        return false;
    }

    int startLine = pageCounter * PAGE_SIZE;
    int endLine = startLine + PAGE_SIZE;
    int currentLine = 0;
    string line;

    // Skip header
    if (!getline(file, line))
    {
        cerr << "ERROR: Cannot read header line from file" << endl;
        file.close();
        return false;
    }

    // Skip lines until startLine
    while (currentLine < startLine && getline(file, line))
    {
        currentLine++;
    }

    // Allocate memory for this page
    transactions = new Transaction[PAGE_SIZE];
    int validTransactions = 0;
    string transaction_id, sender_account, receiver_account;
    string transaction_type, location, payment_channel;
    double amount;
    bool is_fraud;

    // Load up to PAGE_SIZE transactions
    while (currentLine < endLine && getline(file, line))
    {
        if (line.empty() || line.length() < 10)
            continue;
        CSVParser::ParseResult result = parseLineWithValidation(line, transaction_id, sender_account, receiver_account, amount, transaction_type, location, payment_channel, is_fraud);
        if (result == ParseResult::SUCCESS)
        {
            transactions[validTransactions] = Transaction(transaction_id, sender_account, receiver_account, amount, transaction_type, location, payment_channel, is_fraud);
            validTransactions++;
        }
        currentLine++;
    }
    numTransactions = validTransactions;
    file.close();
    pageCounter++;
    if (numTransactions == 0)
    {
        cout << "No more data to load." << endl;
        return false;
    }
  
    return true;
}

int CSVParser::getNumTransactions() { return numTransactions; }
Transaction *CSVParser::getTransactions() { return transactions; }

// Enhanced helper method to parse a single CSV line with detailed validation
CSVParser::ParseResult CSVParser::parseLineWithValidation(const string &line, string &transaction_id,
                                                          string &sender_account, string &receiver_account,
                                                          double &amount, string &transaction_type,
                                                          string &location, string &payment_channel,
                                                          bool &is_fraud)
{
    stringstream ss(line);
    string token;
    int index = 0;
    int tokenCount = 0;

    // Reset values
    amount = 0;
    is_fraud = false;
    transaction_id.clear();
    sender_account.clear();
    receiver_account.clear();
    transaction_type.clear();
    location.clear();
    payment_channel.clear();

    while (getline(ss, token, ','))
    {
        tokenCount++;

        // Remove leading/trailing whitespace
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);

        switch (index)
        {
        case 0:
            transaction_id = token;
            if (transaction_id.length() > 50)
            { // Reasonable limit
                return ParseResult::VALIDATION_ERROR;
            }
            break;
        case 2:
            sender_account = token;
            if (sender_account.length() > 30)
            {
                return ParseResult::VALIDATION_ERROR;
            }
            break;
        case 3:
            receiver_account = token;
            if (receiver_account.length() > 30)
            {
                return ParseResult::VALIDATION_ERROR;
            }
            break;
        case 4:
            try
            {
                amount = stod(token);
                if (amount < 0 || amount > 1000000)
                { // Reasonable range check
                    return ParseResult::VALIDATION_ERROR;
                }
            }
            catch (const invalid_argument &)
            {
                return ParseResult::PARSE_ERROR;
            }
            catch (const out_of_range &)
            {
                return ParseResult::PARSE_ERROR;
            }
            break;
        case 5:
            transaction_type = token;
            if (transaction_type.length() > 20)
            {
                return ParseResult::VALIDATION_ERROR;
            }
            break;
        case 7:
            location = token;
            if (location.length() > 50)
            {
                return ParseResult::VALIDATION_ERROR;
            }
            break;
        case 9:
            is_fraud = (token == "1" || token == "true" || token == "True" || token == "TRUE");
            break;
        case 15:
            payment_channel = token;
            if (payment_channel.length() > 30)
            {
                return ParseResult::VALIDATION_ERROR;
            }
            break;
        }
        index++;

        if (index > 16)
            break; // Prevent excessive parsing
    }

    // Check if we have minimum required tokens
    if (tokenCount < 16)
    {
        return ParseResult::MALFORMED;
    }

    // Validate required fields are not empty and have valid content
    if (transaction_id.empty() || sender_account.empty() ||
        receiver_account.empty() || amount <= 0 ||
        transaction_type.empty() || location.empty() ||
        payment_channel.empty())
    {
        return ParseResult::VALIDATION_ERROR;
    }

    // Additional business logic validation
    if (sender_account == receiver_account)
    {
        return ParseResult::VALIDATION_ERROR; // Same sender and receiver
    }

    return ParseResult::SUCCESS;
}

// Legacy method for backward compatibility
bool CSVParser::parseLine(const string &line, string &transaction_id, string &sender_account,
                          string &receiver_account, double &amount, string &transaction_type,
                          string &location, string &payment_channel, bool &is_fraud)
{
    return parseLineWithValidation(line, transaction_id, sender_account, receiver_account,
                                   amount, transaction_type, location, payment_channel, is_fraud) == ParseResult::SUCCESS;
}

// Streaming methods for low memory usage
bool CSVParser::initializeStreaming()
{
    if (filePath.empty())
    {
        cerr << "ERROR: No file path set for streaming." << endl;
        return false;
    }

    fileStream.open(filePath);
    if (!fileStream.is_open())
    {
        cerr << "ERROR: Cannot open file for streaming: " << filePath << endl;
        return false;
    }

    // Skip header line
    string headerLine;
    if (!getline(fileStream, headerLine))
    {
        cerr << "ERROR: Cannot read header line for streaming" << endl;
        return false;
    }

    isStreamMode = true;
    totalProcessed = 0;
    cout << "Might Take Seconds To Process Please Wait" << endl;
    return true;
}

bool CSVParser::getNextTransaction(Transaction &transaction)
{
    if (!isStreamMode || !fileStream.is_open())
    {
        return false;
    }

    string line;
    while (getline(fileStream, line))
    {
        if (line.empty() || line.length() < 10)
            continue;

        string transaction_id, sender_account, receiver_account;
        string transaction_type, location, payment_channel;
        double amount;
        bool is_fraud;

        ParseResult result = parseLineWithValidation(line, transaction_id, sender_account,
                                                     receiver_account, amount, transaction_type,
                                                     location, payment_channel, is_fraud);

        if (result == ParseResult::SUCCESS)
        {
            transaction = Transaction(transaction_id, sender_account, receiver_account,
                                      amount, transaction_type, location, payment_channel, is_fraud);
            totalProcessed++;
            return true;
        }
    }

    return false; // End of file or no more valid transactions
}

void CSVParser::closeStream()
{
    if (fileStream.is_open())
    {
        fileStream.close();
    }
    isStreamMode = false;
    cout << "Streaming mode closed. Total processed: " << totalProcessed << endl;
}

long long CSVParser::getTotalProcessed() const
{
    return totalProcessed;
}