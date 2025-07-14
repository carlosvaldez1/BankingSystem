/* ===========================================
   Banking System - Mini Project
   ===========================================*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <iomanip>
#include <cstdio>
#include <conio.h> // For _getch()
#include <ctime>
#include <cstdlib>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <windows.h> // For SetConsoleTextAttribute and Sleep
#include <functional> // For std::function
#include <limits>     // For numeric_limits

using namespace std;

// Global variables (reduced reliance where possible)
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Data structures
map<string, string> accountCredentials;  // Hash table for account credentials
map<string, string> employeeCredentials; // Hash table for employee credentials
vector<string> transactionHistory;       // Vector to store transaction history (all transactions)
queue<string> serviceQueue;              // Queue for customer service requests
stack<string> recentTransactions;        // Stack for recent transactions (last 10 shown to user)

// Forward declarations
void fordelay(int);
void close_application(void);
int main();
void showEmployeeMenu();
void employeeLogin();
void customerLogin();
void showCustomerMenu();
void displayAppTitle();
void showLoginPage();
void showSignupPage();
void showInstructions();
void showLoadingScreen();
void setConsoleColor(int color);
void loadAllCredentials();
void saveAllCredentials();
string getSecurePasswordInput();
string getCurrentDateTime(); // Helper to get current date/time

// Bank Account Class
class Bank
{
private:
    struct AccountNode
    {
        string account_number;
        string name;
        string dob;
        string age;
        string address;
        string phone;
        string balance;
        string acc_type;
        string creation_date;
        string last_transaction;
        AccountNode *left;
        AccountNode *right;

        // Constructor for AccountNode
        AccountNode(string acc_no, string n, string d, string a,
                    string addr, string ph, string bal, string type,
                    string date, string last_trans_date)
            : account_number(acc_no), name(n), dob(d), age(a),
              address(addr), phone(ph), balance(bal), acc_type(type),
              creation_date(date), last_transaction(last_trans_date), left(nullptr), right(nullptr) {}
    };

    AccountNode *root; // Root of the BST

    // Private helper for inserting into BST
    AccountNode *insert(AccountNode *node, const string &acc_no, const string &n, const string &d, const string &a,
                        const string &addr, const string &ph, const string &bal, const string &type, const string &date, const string &last_trans_date)
    {
        if (node == nullptr)
        {
            return new AccountNode(acc_no, n, d, a, addr, ph, bal, type, date, last_trans_date);
        }
        if (acc_no < node->account_number)
        {
            node->left = insert(node->left, acc_no, n, d, a, addr, ph, bal, type, date, last_trans_date);
        }
        else if (acc_no > node->account_number) // Added check for greater to avoid duplicates or issues
        {
            node->right = insert(node->right, acc_no, n, d, a, addr, ph, bal, type, date, last_trans_date);
        }
        return node;
    }

    // Private helper for searching in BST
    AccountNode *search(AccountNode *node, const string &acc_no)
    {
        if (node == nullptr || node->account_number == acc_no)
        {
            return node;
        }
        if (acc_no < node->account_number)
        {
            return search(node->left, acc_no);
        }
        return search(node->right, acc_no);
    }

    // Private helper for in-order traversal (for display_all)
    void inorder(AccountNode *node)
    {
        if (node != nullptr)
        {
            inorder(node->left);
            cout << "\t" << left
                 << setw(20) << node->account_number
                 << setw(30) << node->name
                 << setw(20) << node->acc_type
                 << "Rs " << node->balance << "\n";
            inorder(node->right);
        }
    }

    // Private helper to deallocate BST memory
    void clearTree(AccountNode *node)
    {
        if (node != nullptr)
        {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

    // Private helper to save accounts to file (recursive inorder traversal)
    void saveAccountsToFileHelper(AccountNode *node, ofstream &file)
    {
        if (node != nullptr)
        {
            saveAccountsToFileHelper(node->left, file);
            file << node->account_number << "," << node->name << "," << node->dob << ","
                 << node->age << "," << node->address << "," << node->phone << ","
                 << node->balance << "," << node->acc_type << "," << node->creation_date << "," // Added creation date
                 << node->last_transaction << "\n"; // Added last transaction date
            saveAccountsToFileHelper(node->right, file);
        }
    }

public:
    // Constructor
    Bank() : root(nullptr)
    {
        loadAccountsFromFile();
    }

    // Destructor
    ~Bank()
    {
        clearTree(root);
    }

    // Public method to load accounts from CSV
    void loadAccountsFromFile()
    {
        ifstream file("Bank_Record.csv");
        if (!file.is_open())
        {
            // If file doesn't exist, it's fine for first run, just return
            return;
        }

        string line;
        while (getline(file, line))
        {
            vector<string> tokens;
            string token;
            size_t start = 0;
            size_t end = line.find(',');

            while (end != string::npos)
            {
                tokens.push_back(line.substr(start, end - start));
                start = end + 1;
                end = line.find(',', start);
            }
            tokens.push_back(line.substr(start, end)); // Add the last token

            if (tokens.size() >= 10) // Changed to 10 to include last_transaction
            {
                // Ensure all fields are present
                root = insert(root, tokens[0], tokens[1], tokens[2], tokens[3],
                              tokens[4], tokens[5], tokens[6], tokens[7], tokens[8], tokens[9]);
            }
        }
        file.close();
    }

    // Public method to save accounts to CSV
    void saveAccountsToFile()
    {
        ofstream file("Bank_Record.csv");
        if (!file.is_open())
        {
            setConsoleColor(12);
            cout << "\n\tError: Could not open Bank_Record.csv for saving accounts.";
            setConsoleColor(7);
            return;
        }

        saveAccountsToFileHelper(root, file);
        file.close();
    }

    // Public method to create a new account
    void createNewAccount();
    // Public method to modify an existing account
    void modifyAccountDetails();
    // Public method to search for an account
    void searchAccountRecord();
    // Public method for deposit/withdrawal transactions
    void handleDepositWithdrawal();
    // Public method to display all accounts (for employees)
    void displayAllAccounts();
    // Public method for fund transfers
    void performFundTransfer();
    // Public method to view transaction history
    void viewTransactionHistory();
    // Public method for customer to request service
    void submitServiceRequest();
    // Public method for employees to process service requests
    void manageServiceQueue();
};

// --- Utility Functions Implementation ---

// A simple delay function (using Sleep for better precision than a busy-wait loop)
void fordelay(int milliseconds)
{
    Sleep(milliseconds);
}

// Function to safely close the application
void close_application(void)
{
    displayAppTitle();
    setConsoleColor(12);
    cout << "\n\n\n\n\t\tThank you for using our banking system!";
    setConsoleColor(7);
    cout << "\n\n\t\tProgram is now closing...\n\n";
    fordelay(2000); // Small delay before exiting
    exit(0);
}

// Function to set console text color
void setConsoleColor(int color)
{
    SetConsoleTextAttribute(hConsole, color);
}

// Function to display the main application title
void displayAppTitle()
{
    system("cls"); // Clear console screen
    setConsoleColor(11); // Cyan color
    cout << "\n\n\t\t*********************************************";
    cout << "\n\t\t* *";
    cout << "\n\t\t* WELCOME TO THE BANKING SYSTEM       *"; // Generic title
    cout << "\n\t\t* *";
    cout << "\n\t\t*********************************************\n\n";
    setConsoleColor(7); // White color
}

// Function to display loading animation
void showLoadingScreen()
{
    system("cls");
    setConsoleColor(10); // Green color
    cout << "\n\n\n\n\n\t\t\tLoading";
    for (int i = 0; i < 6; i++)
    {
        fordelay(200); // Shorter delay for quicker load
        cout << ".";
    }
    system("cls");
}

// Function to display important instructions to the user
void showInstructions()
{
    displayAppTitle();
    setConsoleColor(14); // Yellow color
    cout << "\n\t\tIMPORTANT INSTRUCTIONS:\n";
    setConsoleColor(7); // White color
    cout << "\n\t1. Keep your account number and password confidential.";
    cout << "\n\t2. Do not share your OTP or password with anyone.";
    cout << "\n\t3. Always log out after completing your transactions.";
    cout << "\n\t4. Report any suspicious activity to the bank immediately.";
    cout << "\n\t5. Regularly update your password for security.";
    cout << "\n\n\tPress any key to continue...";
    _getch(); // Wait for user input
}

// Function to display the login page options
void showLoginPage()
{
    displayAppTitle();
    setConsoleColor(14); // Yellow color
    cout << "\n\t\tLOGIN PAGE\n";
    setConsoleColor(7); // White color
    cout << "\n\t1. Employee Login";
    cout << "\n\t2. Customer Login";
    cout << "\n\t3. New Customer Registration";
    cout << "\n\t4. Return to Main Menu";
    cout << "\n\t5. Exit";
    cout << "\n\n\tEnter your choice: ";
}

// Function to display the signup page title
void showSignupPage()
{
    displayAppTitle();
    setConsoleColor(14); // Yellow color
    cout << "\n\t\tNEW CUSTOMER REGISTRATION\n";
    setConsoleColor(7); // White color
}

// Function to get password input securely (masked with asterisks)
string getSecurePasswordInput()
{
    string password;
    char ch;
    while (true)
    {
        ch = _getch(); // Get character without echoing to console
        if (ch == 13) // Enter key
            break;
        else if (ch == 8) // Backspace key
        {
            if (!password.empty())
            {
                cout << "\b \b"; // Erase character from console
                password.pop_back();
            }
        }
        else if (isprint(ch)) // Check if printable character
        {
            password.push_back(ch);
            cout << '*'; // Display asterisk
        }
    }
    cout << endl; // New line after password input
    return password;
}

// Helper function to get current date and time as a string
string getCurrentDateTime()
{
    time_t now = time(0);
    string dt = ctime(&now);
    // ctime adds a newline, remove it
    dt.erase(remove(dt.begin(), dt.end(), '\n'), dt.end());
    return dt;
}

// Load all credentials from CSV files into maps
void loadAllCredentials()
{
    // Load account credentials from Account_info.csv
    ifstream accFile("Account_info.csv");
    if (accFile.is_open())
    {
        string line;
        while (getline(accFile, line))
        {
            size_t pos = line.find(',');
            if (pos != string::npos)
            {
                string acc_no = line.substr(0, pos);
                string pass = line.substr(pos + 1);
                accountCredentials[acc_no] = pass;
            }
        }
        accFile.close();
    }
    // If file doesn't exist, accountCredentials map remains empty, which is fine

    // Load employee credentials from Employee_info.csv
    ifstream empFile("Employee_info.csv");
    if (empFile.is_open())
    {
        string line;
        while (getline(empFile, line))
        {
            size_t pos = line.find(',');
            if (pos != string::npos)
            {
                string emp_id = line.substr(0, pos);
                string pass = line.substr(pos + 1);
                employeeCredentials[emp_id] = pass;
            }
        }
        empFile.close();
    }
    else
    {
        // Create a default admin if Employee_info.csv does not exist
        employeeCredentials["admin"] = "admin123";
        saveAllCredentials(); // Save this default admin to the file
    }
}

// Save all credentials from maps to CSV files
void saveAllCredentials()
{
    // Save employee credentials to Employee_info.csv
    ofstream empFile("Employee_info.csv");
    if (empFile.is_open())
    {
        for (const auto &pair : employeeCredentials)
        {
            empFile << pair.first << "," << pair.second << "\n";
        }
        empFile.close();
    }
    else
    {
        setConsoleColor(12);
        cout << "\n\tError: Could not open Employee_info.csv for saving credentials.";
        setConsoleColor(7);
    }

    // Save account credentials to Account_info.csv
    ofstream accFile("Account_info.csv");
    if (accFile.is_open())
    {
        for (const auto &pair : accountCredentials)
        {
            accFile << pair.first << "," << pair.second << "\n";
        }
        accFile.close();
    }
    else
    {
        setConsoleColor(12);
        cout << "\n\tError: Could not open Account_info.csv for saving credentials.";
        setConsoleColor(7);
    }
}


// --- Bank Class Member Functions Implementation ---

// Function to create a new customer account
void Bank::createNewAccount()
{
    string account_number, name, dob, age, address, phone, deposit_amount_str, acc_type, password;
    int choice_int;
    bool accountExists;

    do {
        displayAppTitle();
        cout << "\n\t\tACCOUNT CREATION\n";
        cout << "\n\tEnter Account Number: ";
        cin >> account_number;

        accountExists = (accountCredentials.find(account_number) != accountCredentials.end());

        if (accountExists)
        {
            setConsoleColor(12);
            cout << "\n\tAccount No. " << account_number << " already exists!";
            setConsoleColor(7);
            cout << "\n\t1. Try a different Account Number\n\t2. Login To Existing Account\n\tChoice: ";
            cin >> choice_int;
            if (choice_int == 2) {
                customerLogin(); // Go to customer login
                return; // Exit this function
            }
        }
    } while (accountExists && choice_int == 1); // Loop if account exists and user wants to try again

    // Clear input buffer after reading account number
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n\tEnter Name: ";
    getline(cin, name);

    cout << "\n\tEnter Date of Birth (DD/MM/YYYY): "; // Changed format for clarity
    getline(cin, dob); // Use getline for full date string

    cout << "\n\tEnter Age: ";
    cin >> age;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

    cout << "\n\tEnter Address (single line): ";
    getline(cin, address);

    cout << "\n\tEnter Phone Number: ";
    cin >> phone;

    cout << "\n\tWould you like to make an initial deposit?\n\t1. Yes\n\t2. No\n\tChoice: ";
    cin >> choice_int;
    if (choice_int == 1)
    {
        float depo_val;
        cout << "\n\tEnter the amount to deposit: Rs ";
        while (!(cin >> depo_val) || depo_val < 0) {
            setConsoleColor(12);
            cout << "\n\tInvalid amount. Please enter a non-negative number: Rs ";
            setConsoleColor(7);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        deposit_amount_str = to_string(depo_val);
    }
    else
    {
        deposit_amount_str = "0.0"; // Initialize with 0.0 for consistency
    }

    cout << "\n\tSelect Type of Account:\n\t1. Saving\n\t2. Current\n\tChoice: ";
    cin >> choice_int;
    acc_type = (choice_int == 1) ? "Saving" : "Current";

    string creation_date_time = getCurrentDateTime();

    cout << "\n\tEnter a Password for your Account: ";
    password = getSecurePasswordInput();

    // Add to in-memory credentials map and BST
    accountCredentials[account_number] = password;
    root = insert(root, account_number, name, dob, age, address, phone, deposit_amount_str, acc_type, creation_date_time, creation_date_time);

    // Save all changes to files
    saveAllCredentials(); // Save updated account credentials
    saveAccountsToFile(); // Save updated bank records

    setConsoleColor(10); // Green color
    cout << "\n\tAccount created successfully!";
    cout << "\n\tAccount Number: " << account_number;
    cout << "\n\tInitial Balance: Rs " << deposit_amount_str;
    setConsoleColor(7);

    cout << "\n\n\tPress any key to return to Main Menu...";
    _getch();
    main(); // Return to main menu
}


void Bank::modifyAccountDetails()
{
    displayAppTitle();
    cout << "\n\t\tACCOUNT MODIFICATION\n";

    string acc_no;
    cout << "\n\tEnter the Account Number to modify: ";
    cin >> acc_no;

    AccountNode *account = search(root, acc_no);
    if (account == nullptr)
    {
        setConsoleColor(12);
        cout << "\n\tAccount Doesn't Exist!";
        setConsoleColor(7);
    }
    else
    {
        setConsoleColor(11); // Cyan
        cout << "\n\tAccount Found! Current Details:";
        setConsoleColor(7); // White
        cout << "\n\tAccount NO.: " << account->account_number;
        cout << "\n\tName: " << account->name;
        cout << "\n\tDOB: " << account->dob;
        cout << "\n\tAge: " << account->age;
        cout << "\n\tAddress: " << account->address;
        cout << "\n\tPhone number: " << account->phone;
        cout << "\n\tType Of Account: " << account->acc_type;
        cout << "\n\tDate of Account creation: " << account->creation_date;
        cout << "\n\tAmount deposited: Rs " << account->balance;
        cout << "\n\tLast Transaction: " << account->last_transaction;

        int choice;
        string newValue;
        do
        {
            cout << "\n\n\tWhat would you like to update?";
            cout << "\n\t1. Name";
            cout << "\n\t2. Date of Birth";
            cout << "\n\t3. Age";
            cout << "\n\t4. Address";
            cout << "\n\t5. Phone Number";
            cout << "\n\t6. Change Account Password"; // New option
            cout << "\n\t7. Done Updating"; // Changed from 6 to 7
            cout << "\n\tChoice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer for getline

            switch (choice)
            {
            case 1:
                cout << "\n\tEnter New Name: ";
                getline(cin, newValue);
                account->name = newValue;
                setConsoleColor(10); cout << "\n\tName updated successfully."; setConsoleColor(7);
                break;
            case 2:
                cout << "\n\tEnter New Date of Birth (DD/MM/YYYY): ";
                getline(cin, newValue);
                account->dob = newValue;
                setConsoleColor(10); cout << "\n\tDate of Birth updated successfully."; setConsoleColor(7);
                break;
            case 3:
                cout << "\n\tEnter New Age: ";
                cin >> newValue;
                account->age = newValue;
                setConsoleColor(10); cout << "\n\tAge updated successfully."; setConsoleColor(7);
                break;
            case 4:
                cout << "\n\tEnter New Address: ";
                getline(cin, newValue);
                account->address = newValue;
                setConsoleColor(10); cout << "\n\tAddress updated successfully."; setConsoleColor(7);
                break;
            case 5:
                cout << "\n\tEnter New Phone Number: ";
                cin >> newValue;
                account->phone = newValue;
                setConsoleColor(10); cout << "\n\tPhone Number updated successfully."; setConsoleColor(7);
                break;
            case 6:
                cout << "\n\tEnter New Password: ";
                newValue = getSecurePasswordInput();
                accountCredentials[acc_no] = newValue; // Update in credentials map
                saveAllCredentials(); // Save updated credentials
                setConsoleColor(10); cout << "\n\tPassword updated successfully."; setConsoleColor(7);
                break;
            case 7:
                setConsoleColor(10);
                cout << "\n\tFinishing updates...";
                setConsoleColor(7);
                break;
            default:
                setConsoleColor(12);
                cout << "\n\tInvalid Choice! Please try again.";
                setConsoleColor(7);
                break;
            }
            if (choice != 7) {
                cout << "\n\n\tPress any key to continue modifications...";
                _getch();
                displayAppTitle();
                cout << "\n\t\tACCOUNT MODIFICATION\n";
            }
        } while (choice != 7);

        saveAccountsToFile(); // Save updated account details to Bank_Record.csv
        setConsoleColor(10);
        cout << "\n\tAccount details saved successfully!";
        setConsoleColor(7);
    }

    cout << "\n\n\tPress any key to return to menu...";
    _getch();
    showCustomerMenu(); // Assume customer is logged in, or route to main menu if called by employee
}


void Bank::searchAccountRecord()
{
    displayAppTitle();
    cout << "\n\t\tACCOUNT SEARCH\n";

    int search_type_choice;
    cout << "\n\tSearch by:\n\t1. Account Number\n\t2. Name\n\tChoice: ";
    while (!(cin >> search_type_choice) || (search_type_choice < 1 || search_type_choice > 2)) {
        setConsoleColor(12);
        cout << "\n\tInvalid choice. Please enter 1 or 2: ";
        setConsoleColor(7);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

    if (search_type_choice == 1)
    {
        string acc_no;
        cout << "\n\tEnter Account Number to search: ";
        cin >> acc_no;

        AccountNode *account = search(root, acc_no);
        if (account == nullptr)
        {
            setConsoleColor(12);
            cout << "\n\tAccount Doesn't Exist!";
            setConsoleColor(7);
        }
        else
        {
            setConsoleColor(11);
            cout << "\n\tAccount Details Found:";
            setConsoleColor(7);
            cout << "\n\tAccount NO.: " << account->account_number;
            cout << "\n\tName: " << account->name;
            cout << "\n\tDOB: " << account->dob;
            cout << "\n\tAge: " << account->age;
            cout << "\n\tAddress: " << account->address;
            cout << "\n\tPhone number: " << account->phone;
            cout << "\n\tType Of Account: " << account->acc_type;
            cout << "\n\tDate of Account creation: " << account->creation_date;
            cout << "\n\tBalance: Rs " << account->balance;
            cout << "\n\tLast Transaction: " << account->last_transaction;
        }
    }
    else if (search_type_choice == 2)
    {
        string name_search;
        cout << "\n\tEnter Name (or part of name) to search: ";
        getline(cin, name_search);

        bool found = false;
        // Lambda function for in-order traversal to search by name
        function<void(AccountNode *)> searchByNameTraversal = [&](AccountNode *node)
        {
            if (node == nullptr)
                return;

            searchByNameTraversal(node->left);

            // Case-insensitive search for name
            string node_name_lower = node->name;
            string search_name_lower = name_search;
            transform(node_name_lower.begin(), node_name_lower.end(), node_name_lower.begin(), ::tolower);
            transform(search_name_lower.begin(), search_name_lower.end(), search_name_lower.begin(), ::tolower);

            if (node_name_lower.find(search_name_lower) != string::npos)
            {
                if (!found)
                {
                    setConsoleColor(11);
                    cout << "\n\tAccounts Found (matching '" << name_search << "'):";
                    setConsoleColor(7);
                    found = true;
                }

                cout << "\n\n\tAccount NO.: " << node->account_number;
                cout << "\n\tName: " << node->name;
                cout << "\n\tType Of Account: " << node->acc_type;
                cout << "\n\tBalance: Rs " << node->balance;
                cout << "\n\tLast Transaction: " << node->last_transaction;
                cout << "\n\t---------------------------";
            }

            searchByNameTraversal(node->right);
        };

        searchByNameTraversal(root);

        if (!found)
        {
            setConsoleColor(12);
            cout << "\n\tNo accounts with that name (or matching part) were found!";
            setConsoleColor(7);
        }
    }

    cout << "\n\n\tPress any key to return to menu...";
    _getch();
    // Decide which menu to return to based on context (e.g., employee or customer)
    // For simplicity, let's assume this is called from customer menu, or main menu for employee
    // You might want to pass a flag to this function to decide where to return
    if (cin.peek() == EOF) { // Simple check if EOF implies called from employee context
         showEmployeeMenu();
    } else {
        showCustomerMenu();
    }
}


void Bank::handleDepositWithdrawal()
{
    displayAppTitle();
    cout << "\n\t\tTRANSACTIONS\n";

    string acc_no;
    cout << "\n\tEnter Account Number: ";
    cin >> acc_no;

    AccountNode *account = search(root, acc_no);
    if (account == nullptr)
    {
        setConsoleColor(12);
        cout << "\n\tAccount Doesn't Exist!";
        setConsoleColor(7);
    }
    else
    {
        setConsoleColor(11);
        cout << "\n\tAccount Details:";
        setConsoleColor(7);
        cout << "\n\tAccount NO.: " << account->account_number;
        cout << "\n\tName: " << account->name;
        cout << "\n\tCurrent Balance: Rs " << account->balance;

        int transaction_type_choice;
        cout << "\n\n\t1. Deposit\n\t2. Withdraw\n\tChoice: ";
        while (!(cin >> transaction_type_choice) || (transaction_type_choice < 1 || transaction_type_choice > 2)) {
            setConsoleColor(12);
            cout << "\n\tInvalid choice. Please enter 1 or 2: ";
            setConsoleColor(7);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        float amount;
        cout << "\n\tEnter Amount: Rs ";
        while (!(cin >> amount) || amount <= 0) { // Validate positive amount
            setConsoleColor(12);
            cout << "\n\tInvalid amount. Please enter a positive number: Rs ";
            setConsoleColor(7);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        float current_balance = stof(account->balance);
        string transaction_description;

        if (transaction_type_choice == 1) // Deposit
        {
            current_balance += amount;
            transaction_description = "Deposit: +Rs " + to_string(amount) + " to " + acc_no;
            setConsoleColor(10);
            cout << "\n\tDeposit Successful!";
            setConsoleColor(7);
        }
        else // Withdraw
        {
            if (amount > current_balance)
            {
                setConsoleColor(12);
                cout << "\n\tInsufficient Balance!";
                setConsoleColor(7);
                amount = 0; // Prevent balance change
            }
            else
            {
                current_balance -= amount;
                transaction_description = "Withdrawal: -Rs " + to_string(amount) + " from " + acc_no;
                setConsoleColor(10);
                cout << "\n\tWithdrawal Successful!";
                setConsoleColor(7);
            }
        }

        if (amount > 0) { // Only update if a valid transaction occurred
            account->balance = to_string(current_balance);
            transactionHistory.push_back(transaction_description);
            recentTransactions.push(transaction_description);

            // Update last transaction date
            account->last_transaction = getCurrentDateTime();
            saveAccountsToFile(); // Save updated balance and last transaction date
            cout << "\n\tNew Balance: Rs " << account->balance;
        }
    }

    cout << "\n\n\tPress any key to return to menu...";
    _getch();
    showCustomerMenu();
}


void Bank::displayAllAccounts()
{
    displayAppTitle();
    cout << "\n\t\tALL ACCOUNT HOLDERS\n";

    if (root == nullptr) {
        setConsoleColor(12);
        cout << "\n\tNo accounts to display.";
        setConsoleColor(7);
    } else {
        setConsoleColor(14); // Yellow
        cout << left
             << "\n\t" << setw(20) << "Account No."
             << setw(30) << "Name"
             << setw(20) << "Type"
             << setw(15) << "Balance" << "\n";
        cout << "\t" << string(85, '-') << "\n";
        setConsoleColor(7); // White

        inorder(root); // Perform in-order traversal to display accounts
    }


    cout << "\n\n\tPress any key to return to menu...";
    _getch();
    showEmployeeMenu(); // Return to employee menu
}


void Bank::performFundTransfer()
{
    displayAppTitle();
    cout << "\n\t\tFUND TRANSFER\n";

    string from_acc_no, to_acc_no;
    float amount;

    cout << "\n\tEnter Your Account Number (Sender): ";
    cin >> from_acc_no;

    AccountNode *from_account = search(root, from_acc_no);
    if (from_account == nullptr)
    {
        setConsoleColor(12);
        cout << "\n\tSender Account Doesn't Exist!";
        setConsoleColor(7);
        cout << "\n\n\tPress any key to return to menu...";
        _getch();
        showCustomerMenu();
        return;
    }

    cout << "\n\tEnter Recipient Account Number: ";
    cin >> to_acc_no;

    AccountNode *to_account = search(root, to_acc_no);
    if (to_account == nullptr)
    {
        setConsoleColor(12);
        cout << "\n\tRecipient Account Doesn't Exist!";
        setConsoleColor(7);
        cout << "\n\n\tPress any key to return to menu...";
        _getch();
        showCustomerMenu();
        return;
    }

    // Prevent transferring to the same account
    if (from_acc_no == to_acc_no) {
        setConsoleColor(12);
        cout << "\n\tCannot transfer to the same account!";
        setConsoleColor(7);
        cout << "\n\n\tPress any key to return to menu...";
        _getch();
        showCustomerMenu();
        return;
    }


    cout << "\n\tEnter Amount to Transfer: Rs ";
    while (!(cin >> amount) || amount <= 0) {
        setConsoleColor(12);
        cout << "\n\tInvalid amount. Please enter a positive number: Rs ";
        setConsoleColor(7);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    float from_balance = stof(from_account->balance);
    if (amount > from_balance)
    {
        setConsoleColor(12);
        cout << "\n\tInsufficient Balance in Sender Account!";
        setConsoleColor(7);
    }
    else
    {
        from_balance -= amount;
        from_account->balance = to_string(from_balance);

        float to_balance = stof(to_account->balance);
        to_balance += amount;
        to_account->balance = to_string(to_balance);

        string transaction_time = getCurrentDateTime();
        from_account->last_transaction = transaction_time;
        to_account->last_transaction = transaction_time;

        string trans_sender = "Transfer Out: -Rs " + to_string(amount) + " to " + to_acc_no + " (From " + from_acc_no + ")";
        string trans_receiver = "Transfer In: +Rs " + to_string(amount) + " from " + from_acc_no + " (To " + to_acc_no + ")";

        transactionHistory.push_back(trans_sender);
        transactionHistory.push_back(trans_receiver);
        recentTransactions.push(trans_sender);
        recentTransactions.push(trans_receiver);

        saveAccountsToFile(); // Save updated balances and last transaction dates

        setConsoleColor(10);
        cout << "\n\tTransfer Successful!";
        cout << "\n\tYour New Balance (Sender): Rs " << from_account->balance;
        setConsoleColor(7);
    }

    cout << "\n\n\tPress any key to return to menu...";
    _getch();
    showCustomerMenu();
}


void Bank::viewTransactionHistory()
{
    displayAppTitle();
    cout << "\n\t\tTRANSACTION HISTORY\n";

    if (recentTransactions.empty()) // Use recentTransactions for user display
    {
        setConsoleColor(12);
        cout << "\n\tNo recent transactions to display.";
        setConsoleColor(7);
    }
    else
    {
        setConsoleColor(14);
        cout << "\n\tLast 10 Recent Transactions (newest first):";
        setConsoleColor(7);
        cout << "\n\t--------------------------------------------------\n";

        stack<string> tempStack = recentTransactions; // Use a temporary stack
        int count = 0;

        while (!tempStack.empty() && count < 10) // Display up to 10
        {
            cout << "\t" << tempStack.top() << "\n";
            tempStack.pop();
            count++;
        }
        if (recentTransactions.size() > 10) {
            cout << "\n\t(More transactions available, showing last 10)";
        }
    }

    cout << "\n\n\tPress any key to return to menu...";
    _getch();
    showCustomerMenu();
}


void Bank::submitServiceRequest()
{
    displayAppTitle();
    cout << "\n\t\tREQUEST CUSTOMER SERVICE\n";

    string acc_no, request_description;
    cout << "\n\tEnter Your Account Number: ";
    cin >> acc_no;

    AccountNode *account = search(root, acc_no);
    if (account == nullptr)
    {
        setConsoleColor(12);
        cout << "\n\tAccount Doesn't Exist!";
        setConsoleColor(7);
    }
    else
    {
        int service_choice;
        cout << "\n\tSelect service type:";
        cout << "\n\t1. Technical Issue";
        cout << "\n\t2. Account Query";
        cout << "\n\t3. Loan Information";
        cout << "\n\t4. Other";
        cout << "\n\tChoice: ";
        while (!(cin >> service_choice) || (service_choice < 1 || service_choice > 4)) {
            setConsoleColor(12);
            cout << "\n\tInvalid choice. Please enter a number between 1 and 4: ";
            setConsoleColor(7);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer before getline

        string service_type_str;
        switch (service_choice)
        {
        case 1: service_type_str = "Technical Issue"; break;
        case 2: service_type_str = "Account Query";   break;
        case 3: service_type_str = "Loan Information"; break;
        case 4:
        default: service_type_str = "Other"; break;
        }

        cout << "\n\tPlease describe your request (single line): ";
        getline(cin, request_description);

        string service_request_string = getCurrentDateTime() + " | Account: " + acc_no + " | Name: " + account->name + " | Type: " + service_type_str + " | Desc: " + request_description;
        serviceQueue.push(service_request_string);

        setConsoleColor(10);
        cout << "\n\tYour service request has been queued!";
        cout << "\n\tCurrent queue position: " << serviceQueue.size();
        setConsoleColor(7);
    }

    cout << "\n\n\tPress any key to return to menu...";
    _getch();
    showCustomerMenu();
}


void Bank::manageServiceQueue()
{
    displayAppTitle();
    cout << "\n\t\tSERVICE QUEUE MANAGEMENT\n";

    if (serviceQueue.empty())
    {
        setConsoleColor(12);
        cout << "\n\tNo pending service requests.";
        setConsoleColor(7);
    }
    else
    {
        setConsoleColor(14);
        cout << "\n\tPending Service Requests: " << serviceQueue.size();
        setConsoleColor(7);
        cout << "\n\t--------------------------------------------------\n";
        cout << "\n\tNext in queue: " << serviceQueue.front();

        int queue_action_choice;
        cout << "\n\n\tWhat would you like to do?";
        cout << "\n\t1. Process Next Request";
        cout << "\n\t2. View All Requests in Queue";
        cout << "\n\t3. Return to Employee Menu";
        cout << "\n\tChoice: ";
        while (!(cin >> queue_action_choice) || (queue_action_choice < 1 || queue_action_choice > 3)) {
            setConsoleColor(12);
            cout << "\n\tInvalid choice. Please enter 1, 2, or 3: ";
            setConsoleColor(7);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (queue_action_choice == 1)
        {
            cout << "\n\tProcessing request: " << serviceQueue.front();
            serviceQueue.pop();
            setConsoleColor(10);
            cout << "\n\tRequest processed successfully!";
            setConsoleColor(7);
            cout << "\n\tRemaining requests: " << serviceQueue.size();
            cout << "\n\n\tPress any key to continue...";
            _getch();
            manageServiceQueue(); // Recurse to process more or return
        }
        else if (queue_action_choice == 2)
        {
            queue<string> tempQueue = serviceQueue;
            int count = 1;

            cout << "\n\n\tAll pending requests:";
            while (!tempQueue.empty())
            {
                cout << "\n\t" << count << ". " << tempQueue.front();
                tempQueue.pop();
                count++;
            }
            cout << "\n\n\tPress any key to return...";
            _getch();
            manageServiceQueue(); // Return to queue options
        }
    }

    cout << "\n\n\tPress any key to return to menu...";
    _getch();
    showEmployeeMenu(); // Always return to employee menu from here
}


// --- Menu Functions Implementation ---

// Customer Menu
void showCustomerMenu()
{
    Bank bank_operations; // Create a Bank object to access its methods

    displayAppTitle();
    setConsoleColor(14); // Yellow
    cout << "\n\t\tCUSTOMER MENU\n";
    setConsoleColor(7); // White
    cout << "\n\t1. Create New Account";
    cout << "\n\t2. Search My Account";
    cout << "\n\t3. Deposit/Withdraw Funds";
    cout << "\n\t4. Modify My Account Details";
    cout << "\n\t5. Transfer Funds";
    cout << "\n\t6. View Transaction History";
    cout << "\n\t7. Request Customer Service";
    cout << "\n\t8. Log Out";
    cout << "\n\t0. Exit Application";
    cout << "\n\n\tEnter your choice: ";

    int choice;
    cin >> choice;

    // Clear the input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice)
    {
    case 1: bank_operations.createNewAccount(); break;
    case 2: bank_operations.searchAccountRecord(); break;
    case 3: bank_operations.handleDepositWithdrawal(); break;
    case 4: bank_operations.modifyAccountDetails(); break;
    case 5: bank_operations.performFundTransfer(); break;
    case 6: bank_operations.viewTransactionHistory(); break;
    case 7: bank_operations.submitServiceRequest(); break;
    case 8: showLoadingScreen(); main(); break; // Log out returns to main menu
    case 0: close_application(); break;
    default:
        setConsoleColor(12);
        cout << "\n\tInvalid choice! Please try again.";
        setConsoleColor(7);
        fordelay(1500); // Wait 1.5 seconds
        showCustomerMenu(); // Recursive call to show menu again
    }
}

// Employee Menu
void showEmployeeMenu()
{
    Bank bank_operations; // Create a Bank object to access its methods

    displayAppTitle();
    setConsoleColor(14); // Yellow
    cout << "\n\t\tEMPLOYEE MENU\n";
    setConsoleColor(7); // White
    cout << "\n\t1. Create New Customer Account";
    cout << "\n\t2. Search Customer Account";
    cout << "\n\t3. Modify Customer Account Details";
    cout << "\n\t4. View All Bank Accounts";
    cout << "\n\t5. Process Customer Service Requests";
    cout << "\n\t6. Add New Employee Account";
    cout << "\n\t7. Log Out";
    cout << "\n\t0. Exit Application";
    cout << "\n\n\tEnter your choice: ";

    int choice;
    cin >> choice;

    // Clear the input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice)
    {
    case 1: bank_operations.createNewAccount(); break; // Employee can also create accounts
    case 2: bank_operations.searchAccountRecord(); break;
    case 3: bank_operations.modifyAccountDetails(); break;
    case 4: bank_operations.displayAllAccounts(); break;
    case 5: bank_operations.manageServiceQueue(); break;
    case 6:
        {
            displayAppTitle();
            cout << "\n\t\tADD NEW EMPLOYEE ACCOUNT\n";

            string emp_id, password;
            cout << "\n\tEnter New Employee ID: ";
            cin >> emp_id;

            if (employeeCredentials.count(emp_id)) // Use .count() for map
            {
                setConsoleColor(12);
                cout << "\n\tError: Employee ID '" << emp_id << "' already exists!";
                setConsoleColor(7);
            }
            else
            {
                cout << "\n\tEnter Password for New Employee: ";
                password = getSecurePasswordInput();

                employeeCredentials[emp_id] = password; // Add to map
                saveAllCredentials(); // Save updated employee credentials

                setConsoleColor(10);
                cout << "\n\tEmployee account '" << emp_id << "' created successfully!";
                setConsoleColor(7);
            }
            cout << "\n\n\tPress any key to return to menu...";
            _getch();
            showEmployeeMenu(); // Return to employee menu
        }
        break;
    case 7: showLoadingScreen(); main(); break; // Log out
    case 0: close_application(); break;
    default:
        setConsoleColor(12);
        cout << "\n\tInvalid choice! Please try again.";
        setConsoleColor(7);
        fordelay(1500); // Wait 1.5 seconds
        showEmployeeMenu(); // Recursive call
    }
}

// Employee Login function
void employeeLogin()
{
    system("cls");
    displayAppTitle();

    string emp_id, password;
    cout << "\n\t\tEMPLOYEE LOGIN\n";
    cout << "\n\tEnter Employee ID: ";
    cin >> emp_id;
    cout << "\n\tEnter Password: ";
    password = getSecurePasswordInput();

    if (employeeCredentials.count(emp_id) && employeeCredentials[emp_id] == password)
    {
        setConsoleColor(10);
        cout << "\n\tLogin Successful! Welcome, " << emp_id << "!";
        setConsoleColor(7);
        fordelay(1000); // Shorter delay
        showEmployeeMenu(); // Go to employee menu
    }
    else
    {
        setConsoleColor(12);
        cout << "\n\tInvalid Employee ID or Password!";
        setConsoleColor(7);

        int choice;
        cout << "\n\n\t1. Try Again\n\t2. Main Menu\n\t0. Exit\n\tChoice: ";
        cin >> choice;

        if (choice == 1) {
            employeeLogin();
        } else if (choice == 0) {
            close_application();
        } else {
            main(); // Return to main menu
        }
    }
}

// Customer Login function
void customerLogin()
{
    system("cls");
    displayAppTitle();

    string acc_no, password;
    cout << "\n\t\tCUSTOMER LOGIN\n";
    cout << "\n\tEnter Account Number: ";
    cin >> acc_no;
    cout << "\n\tEnter Password: ";
    password = getSecurePasswordInput();

    if (accountCredentials.count(acc_no) && accountCredentials[acc_no] == password)
    {
        setConsoleColor(10);
        cout << "\n\tLogin Successful!";
        setConsoleColor(7);
        fordelay(1000); // Shorter delay
        showCustomerMenu(); // Go to customer menu
    }
    else
    {
        setConsoleColor(12);
        cout << "\n\tInvalid Account Number or Password!";
        setConsoleColor(7);

        int choice;
        cout << "\n\n\t1. Try Again\n\t2. Main Menu\n\t0. Exit\n\tChoice: ";
        cin >> choice;

        if (choice == 1) {
            customerLogin();
        } else if (choice == 0) {
            close_application();
        } else {
            main(); // Return to main menu
        }
    }
}

// Main function - entry point of the application
int main()
{
    // Load credentials and account data at startup
    loadAllCredentials();
    srand(time(0)); // Seed random number generator

    showLoadingScreen();
    displayAppTitle(); // Display main title after loading

    setConsoleColor(14); // Yellow color for welcome message
    cout << "\n\t\tWELCOME TO THE BANKING SYSTEM\n"; // Generic welcome message
    setConsoleColor(7); // White color

    cout << "\n\t1. Login";
    cout << "\n\t2. Instructions";
    cout << "\n\t0. Exit";
    cout << "\n\n\tEnter your choice: ";

    int main_choice;
    while (!(cin >> main_choice) || (main_choice < 0 || main_choice > 2)) {
        setConsoleColor(12);
        cout << "\n\tInvalid choice. Please enter 1, 2, or 0: ";
        setConsoleColor(7);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer for subsequent inputs

    switch (main_choice)
    {
    case 1:
        showLoginPage();
        int login_type_choice;
        while (!(cin >> login_type_choice) || (login_type_choice < 1 || login_type_choice > 5)) {
            setConsoleColor(12);
            cout << "\n\tInvalid choice. Please enter a number between 1 and 5: ";
            setConsoleColor(7);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

        switch (login_type_choice)
        {
        case 1: employeeLogin(); break;
        case 2: customerLogin(); break;
        case 3:
            {
                Bank bank_obj_for_signup; // Temporary object for new account creation
                bank_obj_for_signup.createNewAccount();
            }
            break;
        case 4: main(); break; // Return to main menu
        case 5: close_application(); break;
        }
        break;
    case 2:
        showInstructions();
        main(); // Return to main menu after showing instructions
        break;
    case 0:
        close_application();
        break;
    }

    return 0;
}
