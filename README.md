Banking Record System
A mini C++ project simulating a fully functional banking system with secure login, account management, transaction tracking, and customer service features. This project is a modern, modular upgrade from legacy banking logic, implemented in BankingSystem.cpp.

A demo video and screenshots are included to showcase the interface and features.

Project Overview
This command-line interface (CLI) application supports:

Creating, modifying, and searching bank accounts

Deposits, withdrawals, and fund transfers

Customer and employee login systems

Service request queuing

Transaction history tracking

It demonstrates best practices in object-oriented programming, file handling, and data structure usage in C++.

Data Structures Used
Structure	Purpose
map<string, string>	Stores login credentials for accounts and employees
vector<string>	Maintains transaction history
stack<string>	Tracks recent transactions (LIFO)
queue<string>	Manages customer service requests (FIFO)
BST (Binary Search Tree)	Stores account records for efficient search and updates

Feature Highlights
Account Management
Guided input for account creation

Duplicate account checks

Password protection

Role-Based Access
Separate login flows for customers and employees

Role-specific menus and actions

Transactions
Deposit, withdrawal, and fund transfer

Balance updates with timestamp logging

History & Service
View last 10 transactions

Submit service requests categorized by type

UI Experience
Color-coded CLI using Windows API

Instruction screens and loading animations

Design Tradeoffs
Decision	Justification
BST over flat file search	Faster lookups, scalable structure
Console-based UI	Lightweight and easy to deploy
CSV storage instead of database	Portable and simple for small-scale systems
Windows-only dependencies	Simplifies development, limits cross-platform use
No password encryption	Left open for future enhancement

Project Structure
Banking-Record-System/
├── BankingSystem.cpp        # Main implementation
├── BankingSystem.exe        # Compiled executable
├── AccountInformation.csv   # Customer login credentials
├── EmployeeInfo.csv         # Employee login credentials
├── BankingRecord.csv        # Account data records
└── README.md                # Project documentation

Future Enhancements
Password hashing for secure authentication

Cross-platform compatibility (remove WinAPI dependencies)

GUI version using Qt or web technologies

Admin dashboard for analytics and account insights
