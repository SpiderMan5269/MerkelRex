#include "TransactionManager.h"
#include <fstream>
#include <iostream>

TransactionManager::TransactionManager(const std::string& csvFilename)
    : csvFilename(csvFilename)
{
    loadTransactionsFromCSV();
}

// Append to in-memory store and persist to CSV
void TransactionManager::logTransaction(const Transaction& transaction)
{
    // Keep in-memory history in sync with CSV append
    transactions.push_back(transaction);
    saveTransactionToCSV(transaction);
}

// Return all transactions for a given user
std::vector<Transaction> TransactionManager::getUserTransactions(const std::string& userId)
{
    std::vector<Transaction> userTransactions;
    for (const auto& t : transactions)
    {
        if (t.userId == userId)
        {
            userTransactions.push_back(t);
        }
    }
    return userTransactions;
}

// Return the most recent N transactions for the user
std::vector<Transaction> TransactionManager::getLastNTransactions(const std::string& userId, int n)
{
    std::vector<Transaction> userTransactions = getUserTransactions(userId);
    
    std::vector<Transaction> lastN;
    int start = (int)userTransactions.size() - n;
    if (start < 0) start = 0;
    
    for (int i = start; i < (int)userTransactions.size(); i++)
    {
        lastN.push_back(userTransactions[i]);
    }
    return lastN;
}

// Filter user transactions by product
std::vector<Transaction> TransactionManager::getTransactionsByProduct(const std::string& userId, const std::string& product)
{
    std::vector<Transaction> result;
    for (const auto& t : transactions)
    {
        if (t.userId == userId && t.product == product)
        {
            result.push_back(t);
        }
    }
    return result;
}

// Append a single transaction line to CSV
void TransactionManager::saveTransactionToCSV(const Transaction& transaction)
{
    std::ofstream file(csvFilename, std::ios::app);
    if (file.is_open())
    {
        file << transaction.toCSVString() << std::endl;
        file.close();
    }
    else
    {
        std::cerr << "Error opening " << csvFilename << " for writing!" << std::endl;
    }
}

// Eagerly load all transactions from CSV; create file if missing
void TransactionManager::loadTransactionsFromCSV()
{
    std::ifstream file(csvFilename);
    if (!file.is_open())
    {
        // File doesn't exist yet, create empty one to avoid future open errors
        std::ofstream newFile(csvFilename);
        newFile.close();
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        try
        {
            Transaction t = Transaction::fromCSVString(line);
            transactions.push_back(t);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error parsing transaction: " << line << std::endl;
        }
    }
    file.close();
}
