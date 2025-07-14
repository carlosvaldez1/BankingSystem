🏦 Banking System
A compact C++ application that simulates essential banking functions via a smart, text-based interface. Designed to be fast, intuitive, and modular — perfect for learning or extending with your own features.

💡 What This App Can Do
Open, edit, and search customer accounts

Handle deposits, withdrawals, and transfers

Distinguish between customer and employee access

Track transactions and show recent activity

Queue customer support requests for staff handling

Think of it like a minimalist ATM system — no fluff, just focused functionality.

🧠 What Powers It Behind the Scenes
Structure	Use Case
map	Quick lookup for user logins
vector	Long-term storage of transactions
stack	Top 10 recent operations
queue	Handles service requests
BST (Binary Search Tree)	Smart storage for account data
Each structure is chosen for speed, efficiency, or simplicity. You're building with tools that scale.

🎬 Feature Breakdown
🧾 Account Control
Guided registration with safeguards

Unique account enforcement

Real-time editing and updates

🔐 Role Management
Separate login portals

Permissions handled based on role type

💸 Transactions
Add or subtract funds

Internal account-to-account transfers

Timestamped updates and transaction logging

👥 Customer Support
Submit support tickets

Managed by staff through a queue system

✨ User Experience
Uses Windows terminal colors for clarity

Instruction screens, confirmation messages, and flow prompts

🧱 Project Layout 
BankingRecordSystem/
├── BankingSystem.cpp         # Main source code
├── BankingSystem.exe         # Compiled program (Windows only)
├── BankingRecord.csv         # Stored account information
├── AccountInformation.csv    # Customer login credentials
├── EmployeeInfo.csv          # Employee access credentials
└── README.md                 # This file
🔧 Future Improvements 
Encrypting passwords (security boost)

Making it cross-platform (ditch WinAPI)

Adding graphical interfaces with Qt or web tech

Analytics page for admins
