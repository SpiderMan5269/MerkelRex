#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Candlestick.h"
#include "InputValidator.h"
#include <map>
#include <iomanip>

// Wire up collaborators and set the initial auth state
MerkelMain::MerkelMain()
    : currentUser(nullptr)
{

}

void MerkelMain::init()
{
    std::cout << "\n=== Welcome to Merkle Trading Platform ===" << std::endl;
    authMenu();
}

// Loop until a user successfully registers or logs in
void MerkelMain::authMenu()
{
    while (currentUser == nullptr)
    {
        std::cout << "\n=== Authentication Menu ===" << std::endl;
        std::cout << "1: Register New User" << std::endl;
        std::cout << "2: Login" << std::endl;
        std::cout << "3: Quit" << std::endl;
        std::cout << "===================" << std::endl;
        
        int choice = getAuthOption();
        
        if (choice == 1)
        {
            try
            {
                registerNewUser();
                // After successful registration, currentUser is set.
            }
            catch (const std::exception& e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
        else if (choice == 2)
        {
            try
            {
                loginUser();
            }
            catch (const std::exception& e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
        else if (choice == 3)
        {
            std::cout << "Goodbye!" << std::endl;
            exit(0);
        }
        else
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    
    // User is authenticated, proceed to main menu
    std::cout << "\nWelcome, " << currentUser->getName() << "!" << std::endl;
    mainMenu();
}

// Main event loop once authenticated
void MerkelMain::mainMenu()
{
    currentTime = orderBook.getEarliestTime();
    wallet.insertCurrency("BTC", 10);

    while(true)
    {
        printMenu();
        int input = getUserOption();
        processUserOption(input);
    }
}


// Present available actions to the user
void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue   
	std::cout << "6: Continue " << std::endl;
    std::cout << "7: Show candlestick summary" << std::endl;
    std::cout << "8: Deposit money (USDT)" << std::endl;
    std::cout << "9: Withdraw money (USDT)" << std::endl;
    std::cout << "10: Transaction history" << std::endl;
    std::cout << "11: Transaction history by product" << std::endl;
    std::cout << "12: Activity statistics" << std::endl;
    std::cout << "13: Activity statistics by product" << std::endl;
    std::cout << "14: Simulate trading activity" << std::endl;
    std::cout << "15: Save wallet to CSV" << std::endl;
    std::cout << "16: Logout" << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

// Lightweight help text for orientation
void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

// Show basic stats for each product at the current timestamp
void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;



    }
}

// Capture and validate a new ask order from console input
void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::ask 
            );
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
                
                // Log the ask order as a transaction
                Transaction transaction(currentTime, currentUser->getId(), "ask",
                                      obe.product, obe.amount, obe.price,
                                      obe.amount * obe.price, wallet.getUSDTBalance());
                transactionManager.logTransaction(transaction);
                
                // Confirmation message
                std::cout << "\n=== Ask Order Placed ==="  << std::endl;
                std::cout << "Product: " << obe.product << std::endl;
                std::cout << "Amount: " << obe.amount << std::endl;
                std::cout << "Price: " << obe.price << std::endl;
                std::cout << "Total: " << (obe.amount * obe.price) << std::endl;
                std::cout << "===========================\n" << std::endl;
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }   
    }
}

// Capture and validate a new bid order from console input
void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::bid 
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
                
                // Log the bid order as a transaction
                Transaction transaction(currentTime, currentUser->getId(), "bid",
                                      obe.product, obe.amount, obe.price,
                                      obe.amount * obe.price, wallet.getUSDTBalance());
                transactionManager.logTransaction(transaction);
                
                // Confirmation message
                std::cout << "\n=== Bid Order Placed ==="  << std::endl;
                std::cout << "Product: " << obe.product << std::endl;
                std::cout << "Amount: " << obe.amount << std::endl;
                std::cout << "Price: " << obe.price << std::endl;
                std::cout << "Total: " << (obe.amount * obe.price) << std::endl;
                std::cout << "===========================\n" << std::endl;
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }   
    }
}

// Dump wallet contents
void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}
        
// Step to the next timestamp and match outstanding orders
void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl; 
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
        
    }

    currentTime = orderBook.getNextTime(currentTime);
}
 
// Guarded menu input handler with retries
int MerkelMain::getUserOption()
{
    int userOption = -1;
    std::string line;
    
    while (userOption < 0 || userOption > 16)
    {
        std::cout << "Enter option (1-16): ";
        std::getline(std::cin, line);
        
        // Handle empty input
        if (line.empty())
        {
            std::cout << "Invalid input. Please enter a number between 1 and 16." << std::endl;
            continue;
        }
        
        // Validate numeric input
        if (!InputValidator::parseInt(line, userOption))
        {
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            userOption = -1;
            continue;
        }
        
        // Validate range
        if (userOption < 0 || userOption > 16)
        {
            std::cout << "Invalid choice. Please choose a number between 1 and 16." << std::endl;
            userOption = -1;
            continue;
        }
    }
    
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

// Input handler for authentication menu (1-3 options)
int MerkelMain::getAuthOption()
{
    int userOption = -1;
    std::string line;
    
    while (userOption < 1 || userOption > 3)
    {
        std::cout << "Enter option (1-3): ";
        std::getline(std::cin, line);
        
        // Handle empty input
        if (line.empty())
        {
            std::cout << "Invalid input. Please enter a number between 1 and 3." << std::endl;
            continue;
        }
        
        // Validate numeric input
        if (!InputValidator::parseInt(line, userOption))
        {
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            userOption = -1;
            continue;
        }
        
        // Validate range
        if (userOption < 1 || userOption > 3)
        {
            std::cout << "Invalid choice. Please choose a number between 1 and 3." << std::endl;
            userOption = -1;
            continue;
        }
    }
    
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

// Route a validated menu choice to the appropriate handler
void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-13" << std::endl;
    }
    if (userOption == 1) 
    {
        printHelp();
    }
    if (userOption == 2) 
    {
        printMarketStats();
    }
    if (userOption == 3) 
    {
        enterAsk();
    }
    if (userOption == 4) 
    {
        enterBid();
    }
    if (userOption == 5) 
    {
        printWallet();
    }
    if (userOption == 6) 
    {
        gotoNextTimeframe();
    }
    if (userOption == 7)
    {
        printCandlestickSummary();
    }
    if (userOption == 8)
    {
        depositMoney();
    }
    if (userOption == 9)
    {
        withdrawMoney();
    }
    if (userOption == 10)
    {
        showTransactionHistory();
    }
    if (userOption == 11)
    {
        showTransactionHistoryByProduct();
    }
    if (userOption == 12)
    {
        showActivityStatistics();
    }
    if (userOption == 13)
    {
        showActivityStatisticsByProduct();
    }
    if (userOption == 14)
    {
        simulateTradingActivity();
    }
    if (userOption == 15)
    {
        saveWallet();
    }
    if (userOption == 16)
    {
        std::cout << "Logging out..." << std::endl;
        saveWallet(); // Auto-save on logout
        currentUser = nullptr;
        authMenu();
    }       
}

// Compute and print OHLC rows for a product and optional date range
void MerkelMain::printCandlestickSummary()
{
    std::string product, type, startDate, endDate;
    
    std::cout << "\n=== Candlestick Summary ===" << std::endl;
    std::cout << "Available products: ";
    auto products = orderBook.getKnownProducts();
    for (const auto& p : products)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    
    // Get and validate product
    while (!InputValidator::isValidProduct(product))
    {
        std::cout << "Enter product (e.g., ETH/USDT): ";
        std::getline(std::cin, product);
        if (!InputValidator::isValidProduct(product))
        {
            std::cout << "Invalid product format. Use format like ETH/USDT." << std::endl;
        }
    }
    
    // Get and validate type
    while (type != "ask" && type != "bid")
    {
        std::cout << "Enter type (ask/bid): ";
        std::getline(std::cin, type);
        if (type != "ask" && type != "bid")
        {
            std::cout << "Type must be 'ask' or 'bid'." << std::endl;
        }
    }
    
    std::cout << "Enter start date (YYYY/MM/DD HH:MM:SS) or leave blank for earliest: ";
    std::getline(std::cin, startDate);
    
    std::cout << "Enter end date (YYYY/MM/DD HH:MM:SS) or leave blank for latest: ";
    std::getline(std::cin, endDate);

    OrderBookType obType = (type == "bid") ? OrderBookType::bid : OrderBookType::ask;
    std::vector<OrderBookEntry> allOrders = orderBook.getAllOrdersForProduct(product, obType);
    
    std::vector<OrderBookEntry> filteredOrders;
    for (const OrderBookEntry& e : allOrders) {
        if ((startDate.empty() || e.timestamp >= startDate) && (endDate.empty() || e.timestamp <= endDate)) {
            filteredOrders.push_back(e);
        }
    }
    
    if (filteredOrders.empty()) {
        std::cout << "No data found for " << product << " (" << type << ")." << std::endl;
        return;
    }
    // Group by date
    std::map<std::string, std::vector<OrderBookEntry>> grouped;
    for (const OrderBookEntry& e : filteredOrders) {
        grouped[e.timestamp].push_back(e);
    }
    std::cout << "\n" << std::left << std::setw(30) << "Timestamp" 
              << std::setw(12) << "Open" 
              << std::setw(12) << "High" 
              << std::setw(12) << "Low" 
              << std::setw(12) << "Close" << std::endl;
    std::cout << std::string(78, '-') << std::endl;
    
    for (const std::pair<std::string, std::vector<OrderBookEntry>>& pair : grouped) {
        const std::vector<OrderBookEntry>& entries = pair.second;
        double open = entries.front().price;
        double close = entries.back().price;
        double high = entries.front().price;
        double low = entries.front().price;
        for (const OrderBookEntry& e : entries) {
            if (e.price > high) high = e.price;
            if (e.price < low) low = e.price;
        }
        std::cout << std::left << std::setw(30) << pair.first 
                  << std::setw(12) << open 
                  << std::setw(12) << high 
                  << std::setw(12) << low 
                  << std::setw(12) << close << std::endl;
    }
}

// Registration flow with simple validation and persistence
void MerkelMain::registerNewUser()
{
    std::string fullName, email, password;
    
    std::cout << "\n=== User Registration ===" << std::endl;
    
    // Get full name with validation
    while (!InputValidator::isValidString(fullName, 2))
    {
        std::cout << "Enter full name (at least 2 characters): ";
        std::getline(std::cin, fullName);
        if (!InputValidator::isValidString(fullName, 2))
        {
            std::cout << "Invalid name. Please enter at least 2 characters." << std::endl;
        }
    }
    
    // Get email with validation
    while (!InputValidator::isValidEmail(email))
    {
        std::cout << "Enter valid email address: ";
        std::getline(std::cin, email);
        if (!InputValidator::isValidEmail(email))
        {
            std::cout << "Invalid email format. Please use format: user@domain.com" << std::endl;
        }
    }
    
    // Get password with validation
    while (!InputValidator::isValidString(password, 3))
    {
        std::cout << "Enter password (at least 3 characters): ";
        std::getline(std::cin, password);
        if (!InputValidator::isValidString(password, 3))
        {
            std::cout << "Invalid password. Please enter at least 3 characters." << std::endl;
        }
    }
    
    try
    {
        User newUser = userManager.registerUser(fullName, email, password);
        // Set currentUser to the instance stored inside UserManager
        currentUser = userManager.findUserByEmail(email);
    }
    catch (const std::exception& e)
    {
        std::cout << "Registration error: " << e.what() << std::endl;
    }
}

// Login flow supporting either user ID or email
void MerkelMain::loginUser()
{
    std::string emailOrId, password;
    
    std::cout << "\n=== User Login ===" << std::endl;
    
    // Get credentials with validation
    while (emailOrId.empty())
    {
        std::cout << "Enter User ID or Email: ";
        std::getline(std::cin, emailOrId);
        if (emailOrId.empty())
        {
            std::cout << "User ID/Email cannot be empty." << std::endl;
        }
    }
    
    while (password.empty())
    {
        std::cout << "Enter password: ";
        std::getline(std::cin, password);
        if (password.empty())
        {
            std::cout << "Password cannot be empty." << std::endl;
        }
    }
    
    try
    {
        currentUser = userManager.loginUser(emailOrId, password);
        std::cout << "Login successful!" << std::endl;
        // Load user's wallet from CSV
        loadWallet();
    }
    catch (const std::exception& e)
    {
        std::cout << "Login failed: " << e.what() << std::endl;
        emailOrId = "";
        password = "";
    }
}

// Deposit fiat (USDT) and log the transaction
void MerkelMain::depositMoney()
{
    double amount;
    std::cout << "\n=== Deposit Money ===" << std::endl;
    std::cout << "Current USDT balance: " << wallet.getUSDTBalance() << std::endl;
    
    std::string input;
    while (!InputValidator::parseDouble(input, amount) || amount <= 0)
    {
        std::cout << "Enter amount to deposit (USDT) - must be positive: ";
        std::getline(std::cin, input);
        
        if (!InputValidator::parseDouble(input, amount))
        {
            std::cout << "Invalid amount. Please enter a valid positive number." << std::endl;
            continue;
        }
        
        if (amount <= 0)
        {
            std::cout << "Amount must be positive!" << std::endl;
        }
    }
    
    try
    {
        wallet.depositMoney(amount);
        
        // Log transaction
        Transaction transaction(currentTime, currentUser->getId(), "deposit", "USDT",
                              amount, 1.0, amount, wallet.getUSDTBalance());
        transactionManager.logTransaction(transaction);
        
        // Auto-save wallet
        saveWallet();
        
        std::cout << "Successfully deposited " << amount << " USDT" << std::endl;
        std::cout << "New USDT balance: " << wallet.getUSDTBalance() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

// Withdraw fiat (USDT) and log the transaction
void MerkelMain::withdrawMoney()
{
    double amount;
    std::cout << "\n=== Withdraw Money ===" << std::endl;
    std::cout << "Current USDT balance: " << wallet.getUSDTBalance() << std::endl;
    
    std::string input;
    while (!InputValidator::parseDouble(input, amount) || amount <= 0)
    {
        std::cout << "Enter amount to withdraw (USDT) - must be positive: ";
        std::getline(std::cin, input);
        
        if (!InputValidator::parseDouble(input, amount))
        {
            std::cout << "Invalid amount. Please enter a valid positive number." << std::endl;
            continue;
        }
        
        if (amount <= 0)
        {
            std::cout << "Amount must be positive!" << std::endl;
        }
    }
    
    try
    {
        if (!wallet.withdrawMoney(amount))
        {
            std::cout << "Insufficient USDT balance! Current balance: " << wallet.getUSDTBalance() << std::endl;
            return;
        }
        
        // Log transaction
        Transaction transaction(currentTime, currentUser->getId(), "withdraw", "USDT",
                              amount, 1.0, amount, wallet.getUSDTBalance());
        transactionManager.logTransaction(transaction);
        
        // Auto-save wallet
        saveWallet();
        
        std::cout << "Successfully withdrew " << amount << " USDT" << std::endl;
        std::cout << "New USDT balance: " << wallet.getUSDTBalance() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

// Display the latest N transactions for the active user
void MerkelMain::showTransactionHistory()
{
    std::cout << "\n=== Transaction History ===" << std::endl;
    std::vector<Transaction> lastTransactions = transactionManager.getLastNTransactions(currentUser->getId(), 5);
    
    if (lastTransactions.empty())
    {
        std::cout << "No transactions found." << std::endl;
        return;
    }
    
    std::cout << std::left << std::setw(20) << "Timestamp" 
              << std::setw(10) << "Type" 
              << std::setw(12) << "Product" 
              << std::setw(12) << "Amount" 
              << std::setw(10) << "Price" 
              << std::setw(12) << "Total" 
              << std::setw(15) << "USDT Balance" << std::endl;
    std::cout << std::string(91, '-') << std::endl;
    
    for (const auto& t : lastTransactions)
    {
        std::cout << std::left << std::setw(20) << t.timestamp
                  << std::setw(10) << t.type
                  << std::setw(12) << t.product
                  << std::setw(12) << t.amount
                  << std::setw(10) << t.price
                  << std::setw(12) << t.total
                  << std::setw(15) << t.walletBalanceAfter << std::endl;
    }
}

// Summarize buy/sell activity and balances for the active user
void MerkelMain::showActivityStatistics()
{
    std::cout << "\n=== Activity Statistics ===" << std::endl;
    std::vector<Transaction> userTransactions = transactionManager.getUserTransactions(currentUser->getId());
    
    if (userTransactions.empty())
    {
        std::cout << "No activity to display." << std::endl;
        return;
    }
    
    int askCount = 0, bidCount = 0;
    double totalMoneySpent = 0.0;
    std::map<std::string, int> asksByProduct, bidsByProduct;
    
    for (const auto& t : userTransactions)
    {
        if (t.type == "buy" || t.type == "asksale")
        {
            askCount++;
            asksByProduct[t.product]++;
        }
        else if (t.type == "sell" || t.type == "bidsale")
        {
            bidCount++;
            bidsByProduct[t.product]++;
        }
        
        if (t.type != "deposit" && t.type != "withdraw")
        {
            totalMoneySpent += t.total;
        }
    }
    
    std::cout << "Total Asks: " << askCount << std::endl;
    std::cout << "Total Bids: " << bidCount << std::endl;
    std::cout << "Total Transactions: " << userTransactions.size() << std::endl;
    std::cout << "Total Money Spent: " << totalMoneySpent << " USDT" << std::endl;
    std::cout << "Current USDT Balance: " << wallet.getUSDTBalance() << std::endl;
    
    if (!asksByProduct.empty())
    {
        std::cout << "\nAsks by Product:" << std::endl;
        for (const auto& p : asksByProduct)
        {
            std::cout << "  " << p.first << ": " << p.second << std::endl;
        }
    }
    
    if (!bidsByProduct.empty())
    {
        std::cout << "\nBids by Product:" << std::endl;
        for (const auto& p : bidsByProduct)
        {
            std::cout << "  " << p.first << ": " << p.second << std::endl;
        }
    }
}

// Generate synthetic orders (current + backdated) for demo purposes
void MerkelMain::simulateTradingActivity()
{
    std::cout << "\n=== Trading Simulation ===" << std::endl;
    std::cout << "Generating simulated trading orders..." << std::endl;
    
    std::vector<std::string> products = orderBook.getKnownProducts();
    
    // Get all historical orders to calculate price averages
    std::vector<OrderBookEntry> historicalOrders;
    for (const auto& product : products)
    {
        auto asks = orderBook.getAllOrdersForProduct(product, OrderBookType::ask);
        auto bids = orderBook.getAllOrdersForProduct(product, OrderBookType::bid);
        historicalOrders.insert(historicalOrders.end(), asks.begin(), asks.end());
        historicalOrders.insert(historicalOrders.end(), bids.begin(), bids.end());
    }
    
    // Generate current timestamp orders
    std::vector<OrderBookEntry> currentOrders = simulator.generateSimulatedOrders(products, historicalOrders);
    
    // Generate backdated orders for filtering demonstration
    std::vector<OrderBookEntry> backdatedOrders = simulator.generateBackdatedOrders(products, historicalOrders);
    
    // Combine all orders
    std::vector<OrderBookEntry> allNewOrders = currentOrders;
    allNewOrders.insert(allNewOrders.end(), backdatedOrders.begin(), backdatedOrders.end());
    
    std::cout << "Generated " << allNewOrders.size() << " new trading orders" << std::endl;
    std::cout << "  - Current timestamp: " << currentOrders.size() << " orders" << std::endl;
    std::cout << "  - Backdated (2025-2026): " << backdatedOrders.size() << " orders" << std::endl;
    
    // Insert orders into order book
    int matchedCount = 0;
    for (auto& order : allNewOrders)
    {
        orderBook.insertOrder(order);
        
        // Log transaction for each order placed
        Transaction transaction(order.timestamp, currentUser->getId(), 
                              (order.orderType == OrderBookType::ask) ? "ask" : "bid",
                              order.product, order.amount, order.price, 
                              order.amount * order.price, wallet.getUSDTBalance());
        transactionManager.logTransaction(transaction);
        matchedCount++;
    }
    
    std::cout << "\nOrders Summary:" << std::endl;
    std::cout << "  Total orders placed: " << matchedCount << std::endl;
    std::cout << "  Products covered: " << products.size() << std::endl;
    
    std::cout << "\nOrders by product:" << std::endl;
    for (const auto& product : products)
    {
        auto asks = orderBook.getOrders(OrderBookType::ask, product, currentTime);
        auto bids = orderBook.getOrders(OrderBookType::bid, product, currentTime);
        std::cout << "  " << product << ": " << (asks.size() + bids.size()) << " orders" << std::endl;
    }
    
    std::cout << "\nTrading simulation complete!" << std::endl;
    std::cout << "Use option 7 (Candlestick summary) to view simulated data by timeframe." << std::endl;
}

// Display transactions filtered by product
void MerkelMain::showTransactionHistoryByProduct()
{
    std::cout << "\n=== Transaction History by Product ===" << std::endl;
    
    std::string product;
    while (!InputValidator::isValidProduct(product))
    {
        std::cout << "Enter product (e.g., ETH/USDT): ";
        std::getline(std::cin, product);
        if (!InputValidator::isValidProduct(product))
        {
            std::cout << "Invalid product format. Use format like ETH/USDT." << std::endl;
        }
    }
    
    std::vector<Transaction> transactions = transactionManager.getTransactionsByProduct(currentUser->getId(), product);
    
    if (transactions.empty())
    {
        std::cout << "No transactions found for " << product << std::endl;
        return;
    }
    
    std::cout << "\nFound " << transactions.size() << " transactions for " << product << ":\n" << std::endl;
    
    std::cout << std::left << std::setw(20) << "Timestamp" 
              << std::setw(10) << "Type" 
              << std::setw(12) << "Amount" 
              << std::setw(10) << "Price" 
              << std::setw(12) << "Total" 
              << std::setw(15) << "USDT Balance" << std::endl;
    std::cout << std::string(79, '-') << std::endl;
    
    for (const auto& t : transactions)
    {
        std::cout << std::left << std::setw(20) << t.timestamp
                  << std::setw(10) << t.type
                  << std::setw(12) << t.amount
                  << std::setw(10) << t.price
                  << std::setw(12) << t.total
                  << std::setw(15) << t.walletBalanceAfter << std::endl;
    }
}

// Display activity statistics filtered by product
void MerkelMain::showActivityStatisticsByProduct()
{
    std::cout << "\n=== Activity Statistics by Product ===" << std::endl;
    
    std::string product;
    while (!InputValidator::isValidProduct(product))
    {
        std::cout << "Enter product (e.g., ETH/USDT): ";
        std::getline(std::cin, product);
        if (!InputValidator::isValidProduct(product))
        {
            std::cout << "Invalid product format. Use format like ETH/USDT." << std::endl;
        }
    }
    
    std::vector<Transaction> transactions = transactionManager.getTransactionsByProduct(currentUser->getId(), product);
    
    if (transactions.empty())
    {
        std::cout << "No activity found for " << product << std::endl;
        return;
    }
    
    int askCount = 0, bidCount = 0;
    double totalMoneySpent = 0.0;
    
    for (const auto& t : transactions)
    {
        if (t.type == "ask" || t.type == "asksale")
        {
            askCount++;
        }
        else if (t.type == "bid" || t.type == "bidsale")
        {
            bidCount++;
        }
        
        if (t.type != "deposit" && t.type != "withdraw")
        {
            totalMoneySpent += t.total;
        }
    }
    
    std::cout << "\n=== Statistics for " << product << " ===" << std::endl;
    std::cout << "Total Asks: " << askCount << std::endl;
    std::cout << "Total Bids: " << bidCount << std::endl;
    std::cout << "Total Transactions: " << transactions.size() << std::endl;
    std::cout << "Total Money Spent: " << totalMoneySpent << " USDT" << std::endl;
}

// Save wallet to CSV file
void MerkelMain::saveWallet()
{
    if (currentUser != nullptr)
    {
        wallet.saveToCSV(currentUser->getId());
        std::cout << "Wallet saved to wallets.csv" << std::endl;
    }
}

// Load wallet from CSV file
void MerkelMain::loadWallet()
{
    if (currentUser != nullptr)
    {
        wallet.loadFromCSV(currentUser->getId());
        std::cout << "Wallet loaded from wallets.csv" << std::endl;
    }
}
