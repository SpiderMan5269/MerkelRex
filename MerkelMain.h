#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "UserManager.h"
#include "User.h"
#include "TransactionManager.h"
#include "TradingSimulator.h"


// Central controller for authentication, menus, and user interactions
class MerkelMain
{
    public:
        MerkelMain();
        // Entry point for the interactive application
        void init();
    private: 
        // Core loops and UI helpers
        void mainMenu();
        void authMenu();
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        void printCandlestickSummary();

        // Auth and account flows
        void registerNewUser();
        void loginUser();
        void depositMoney();
        void withdrawMoney();
        void showTransactionHistory();
        void showTransactionHistoryByProduct();
        void showActivityStatistics();
        void showActivityStatisticsByProduct();
        void simulateTradingActivity();
        void saveWallet();
        void loadWallet();

        // Input handling
        int getUserOption();
        int getAuthOption();
        void processUserOption(int userOption);

        // State and collaborators
        std::string currentTime;
        UserManager userManager;
        TransactionManager transactionManager;
        TradingSimulator simulator;
        User* currentUser;

//        OrderBook orderBook{"20200317.csv"};
	OrderBook orderBook{"20200601.csv"};
        Wallet wallet;

};
