#pragma once

#include "Transaction.h"
#include <vector>
#include <string>

// Persists and retrieves user transactions from CSV storage
class TransactionManager
{
public:
    TransactionManager(const std::string& csvFilename = "transactions.csv");

    void logTransaction(const Transaction& transaction); // Record and save

    std::vector<Transaction> getUserTransactions(const std::string& userId);
    std::vector<Transaction> getLastNTransactions(const std::string& userId, int n);
    std::vector<Transaction> getTransactionsByProduct(const std::string& userId, const std::string& product);

    void loadTransactionsFromCSV();

private:
    std::vector<Transaction> transactions;
    std::string csvFilename;

    void saveTransactionToCSV(const Transaction& transaction);
};
