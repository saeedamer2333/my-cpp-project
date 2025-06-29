#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Transaction.hpp"
using namespace std;

class CSVParser {
public:
    CSVParser() = default;

    void loadCSV(const string& filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filePath << endl;
            return;
        }

        string line;
        getline(file, line); // Skip header

        while (getline(file, line)) {
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

            while (getline(ss, token, ',')) {
                switch (index) {
                    case 0: transaction_id = token; break;
                    case 2: sender_account = token; break;
                    case 3: receiver_account = token; break;
                    case 4: 
                        try { amount = stod(token); }
                        catch (...) { amount = 0; }
                        break;
                    case 5: transaction_type = token; break;
                    case 7: location = token; break;
                    case 9:
                        is_fraud = (token == "1" || token == "true" || token == "True");
                        break;
                    case 15: payment_channel = token; break;
                    default: break;
                }
                index++;
            }

            // Only add if we captured the required fields
            if ( ! transaction_id.empty() && ! sender_account.empty() && ! receiver_account.empty() && amount > 0 && is_fraud != 'FALSE' && ! transaction_type.empty() && ! location.empty() && ! payment_channel.empty()) {
                // Update the arguments below to match the Transaction constructor in Transaction.hpp
                Transaction t(transaction_id, sender_account, receiver_account, amount,
                              transaction_type, location, payment_channel, is_fraud);

             
            }
        }

        file.close();
    }
};
