#include "../include/CSVParser.hpp"
#include <filesystem>

CSVParser::CSVParser() : numTransactions(0), transactions(nullptr) {}

CSVParser::~CSVParser()
{
    if (transactions != nullptr)
    {
        delete[] transactions;
        transactions = nullptr;
    }
}

void CSVParser::loadCSV(const string &filePath)
{
    cout << "Looking for file: " << filePath << endl;
    cout << "Current working directory: " << filesystem::current_path() << endl; // Add #include <filesystem>
    // First pass: Count transactions
    numTransactions = 0;
    ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header

    // Count valid transactions
    while (getline(file, line))
    {
        stringstream ss(line);
        string token;

        string transaction_id;
        string sender_account;
        string receiver_account;
        double amount = 0;
        string transaction_type;
        string location;
        bool is_fraud = false;
        string payment_channel;

        int index = 0;

        while (getline(ss, token, ','))
        {
            switch (index)
            {
            case 0:
                transaction_id = token;
                break;
            case 2:
                sender_account = token;
                break;
            case 3:
                receiver_account = token;
                break;
            case 4:
                try
                {
                    amount = stod(token);
                }
                catch (...)
                {
                    amount = 0;
                }
                break;
            case 5:
                transaction_type = token;
                break;
            case 7:
                location = token;
                break;
            case 9:
                is_fraud = (token == "1" || token == "true" || token == "True");
                break;
            case 15:
                payment_channel = token;
                break;
            default:
                break;
            }
            index++;
        }

        // Only count if we captured the required fields
        if (!transaction_id.empty() && !sender_account.empty() &&
            !receiver_account.empty() && amount > 0 &&
            !transaction_type.empty() && !location.empty() &&
            !payment_channel.empty())
        {
            numTransactions++;
        }
    }

    // Allocate memory for transactions
    if (numTransactions > 0)
    {
        if (transactions != nullptr)
        {
            delete[] transactions; // Clean up any existing memory
        }
        transactions = new Transaction[numTransactions];
    }
    else
    {
        cout << "No valid transactions found in the CSV file." << endl;
        file.close();
        return;
    }

    // Second pass: Read transactions into array
    file.clear();
    file.seekg(0);
    getline(file, line); // Skip header again

    int transactionIndex = 0;
    while (getline(file, line) && transactionIndex < numTransactions)
    {
        stringstream ss(line);
        string token;

        string transaction_id;
        string sender_account;
        string receiver_account;
        double amount = 0;
        string transaction_type;
        string location;
        bool is_fraud = false;
        string payment_channel;

        int index = 0;

        while (getline(ss, token, ','))
        {
            switch (index)
            {
            case 0:
                transaction_id = token;
                break;
            case 2:
                sender_account = token;
                break;
            case 3:
                receiver_account = token;
                break;
            case 4:
                try
                {
                    amount = stod(token);
                }
                catch (...)
                {
                    amount = 0;
                }
                break;
            case 5:
                transaction_type = token;
                break;
            case 7:
                location = token;
                break;
            case 9:
                is_fraud = (token == "1" || token == "true" || token == "True");
                break;
            case 15:
                payment_channel = token;
                break;
            default:
                break;
            }
            index++;
        }

        // Only add if we captured the required fields
        if (!transaction_id.empty() && !sender_account.empty() &&
            !receiver_account.empty() && amount > 0 &&
            !transaction_type.empty() && !location.empty() &&
            !payment_channel.empty())
        {
            transactions[transactionIndex] = Transaction(transaction_id, sender_account, receiver_account, amount,
                                                         transaction_type, location, payment_channel, is_fraud);
            transactionIndex++;
        }
    }

    file.close();

    cout << "Successfully loaded " << numTransactions << " transactions from CSV file." << endl;
}
int CSVParser::getNumTransactions() const
{
    return numTransactions;
}

Transaction *CSVParser::getTransactions() const
{
    return transactions;
}