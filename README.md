# MerkelRex: Cryptocurrency Exchange Simulator (C++)

A console-based cryptocurrency exchange simulator in C++, built for the midterm of CM2005 Object-Oriented Programming (BSc Computer Science, University of London, academic direction by Goldsmiths). It replays historical order-book data and lets users register, trade and analyse market activity.

## Features

- **User accounts:** registration and login with input validation; users, wallets and transaction history persist to CSV files between sessions.
- **Order book:** loads historical asks and bids from CSV, steps through timestamps, and matches asks to bids with greedy price-time matching.
- **Trading:** place asks and bids, with wallet balance checks before any order is accepted.
- **Wallet:** multi-currency balances, USDT deposits and withdrawals, save and load.
- **Market analysis:** exchange statistics per product and OHLC candlestick summaries per timestamp.
- **Transaction history and activity statistics**, overall and filtered by product.
- **Trading simulator** that generates realistic current and backdated orders (historical average price plus variance) to exercise the matching engine and the filters.
- **Centralised input validation** for numbers, emails, products and menu choices.

## Menu

After logging in:

```
1: Print help                     9: Withdraw money (USDT)
2: Print exchange stats          10: Transaction history
3: Make an offer (ask)           11: Transaction history by product
4: Make a bid                    12: Activity statistics
5: Print wallet                  13: Activity statistics by product
6: Continue (next timestamp)     14: Simulate trading activity
7: Show candlestick summary      15: Save wallet to CSV
8: Deposit money (USDT)          16: Logout
```

## Building and running

```bash
g++ -std=c++17 *.cpp -o merklerex
./merklerex
```

The program loads `20200601.csv` from the working directory. This repository includes a **sample of the first 60,000 rows** of the original dataset (about 1 million rows), covering 127 timestamps, to keep the repo small. `users.csv`, `wallets.csv` and `transactions.csv` are created automatically on first use.

## Design

| Class | Responsibility |
|---|---|
| `MerkelMain` | Application controller: authentication, menus and user interaction |
| `OrderBook`, `OrderBookEntry` | In-memory order book, lookups and ask/bid matching |
| `CSVReader` | Parsing order-book CSV rows into entries |
| `Wallet` | Currency balances, order affordability checks, CSV persistence |
| `User`, `UserManager` | Accounts, registration, login and persistence |
| `Transaction`, `TransactionManager` | Trade records, history and statistics |
| `Candlestick`, `CandlestickCollection` | OHLC summaries |
| `TradingSimulator` | Synthetic order generation |
| `InputValidator` | Validation of all console input |

## Notes

Passwords are stored as a hash produced with `std::hash`. This keeps the coursework simple but is **not** a secure password-hashing scheme; a real system would use a salted, slow hash such as bcrypt or Argon2.

## About

Built by Akshit Sankhala as coursework for CM2005 Object-Oriented Programming. Shared here as a portfolio piece; please do not submit it as your own coursework.
