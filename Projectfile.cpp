// Project: High-Function ATM Management System. 

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <unordered_map>
#include <vector>
#include <limits>

using namespace std;

// Function declarations
void optionScreen1();
void optionScreen2();
bool welcomeScreen(string &userID);
void checkBalance(double balance);
double depositMoney(double &balance);
double withdrawMoney(double &balance);
void showTransactionHistory(const string &userID);
void saveTransaction(const string &userID, const string &transaction);
void handleSessionTimeout();
void adminLogin();

struct User {
    string id;
    string password;
    double balance;
};

unordered_map<string, User> loadUserData();
void saveUserData(const unordered_map<string, User> &users);

const int MAX_WITHDRAWAL_LIMIT = 50000; // Example ATM limit

// Main ATM system function
int main() {
    auto users = loadUserData();
    bool isAuthenticated = false;
    string userID;

    // Welcome and authentication
    isAuthenticated = welcomeScreen(userID);

    if (!isAuthenticated) {
        cout << "Invalid ID or Password. Exiting." << endl;
        return 0;
    }

    double bankBalance = users[userID].balance;

    int option1, option2;
    bool check = true;

    while (check) {
        optionScreen1();
        cout << "Enter option: ";
        cin >> option1;

        if (option1 == 1) {
            checkBalance(bankBalance);
        } else if (option1 == 2) {
            bankBalance = depositMoney(bankBalance);
            saveTransaction(userID, "Deposit: +" + to_string(bankBalance));
        } else if (option1 == 3) {
            bankBalance = withdrawMoney(bankBalance);
            saveTransaction(userID, "Withdrawal: -" + to_string(bankBalance));
        }

        optionScreen2();
        cout << "Enter option: ";
        cin >> option2;

        if (option2 == 4) {
            continue;
        } else if (option2 == 5) {
            check = false;
        }
    }

    // Save updated balance before exiting
    users[userID].balance = bankBalance;
    saveUserData(users);

    cout << "Thank you for using the ATM. Have a great day!" << endl;
    return 0;
}

// Function to display the options for the user
void optionScreen1() {
    cout << "\nPlease select one of the options below:\n";
    cout << "1. CHECK ACCOUNT BALANCE\n";
    cout << "2. DEPOSIT MONEY\n";
    cout << "3. WITHDRAW MONEY\n";
}

// Function to display exit or back options
void optionScreen2() {
    cout << "\nPlease select one of the options below:\n";
    cout << "4. Back to Options\n";
    cout << "5. EXIT\n";
}

// Function to authenticate the user
bool welcomeScreen(string &userID) {
    string password;
    auto users = loadUserData(); // Use updated loadUserData function

    cout << "\n********** HELLO AND WELCOME TO ATM SYSTEM **********\n";
    cout << "To move forward please enter your ID and password\n";
    cout << "Enter your bank ID: ";
    cin >> userID;
    cout << "Enter your bank password: ";
    cin >> password;

    if (users.find(userID) != users.end() && users[userID].password == password) {
        cout << "Authentication successful! You can proceed.\n";
        return true;
    } else {
        return false;
    }
}

// Function to check balance
void checkBalance(double balance) {
    cout << "Account Balance is: $" << balance << "\n";
}

// Function to deposit money
double depositMoney(double &balance) {
    double amount;
    cout << "Enter the amount to deposit: ";
    cin >> amount;
    balance += amount;
    cout << "Deposited: $" << amount << "\n";
    return balance;
}

// Function to withdraw money
double withdrawMoney(double &balance) {
    double amount;
    cout << "Enter the amount to withdraw: ";
    cin >> amount;
    if (amount > balance) {
        cout << "Insufficient funds!\n";
    } else if (amount > MAX_WITHDRAWAL_LIMIT) {
        cout << "Withdrawal limit exceeded! Max limit is $" << MAX_WITHDRAWAL_LIMIT << "\n";
    } else {
        balance -= amount;
        cout << "Withdrawn: $" << amount << "\n";
    }
    return balance;
}

// Function to load user data from a file
unordered_map<string, User> loadUserData() {
    unordered_map<string, User> users;
    ifstream inFile("users.txt");
    string id, password;
    double balance;

    while (inFile >> id >> password >> balance) {
        users[id] = User{id, password, balance};
    }

    inFile.close();
    return users;
}

// Function to save user data to a file
void saveUserData(const unordered_map<string, User> &users) {
    ofstream outFile("users.txt");
    for (const auto &user : users) {
        outFile << user.second.id << " " << user.second.password << " " << user.second.balance << "\n";
    }
    outFile.close();
}

// Function to save transactions
void saveTransaction(const string &userID, const string &transaction) {
    ofstream outFile(userID + "_transactions.txt", ios::app);
    time_t now = time(0);
    char *dt = ctime(&now);
    outFile << dt << " " << transaction << "\n";
    outFile.close();
}

// Function to display transaction history
void showTransactionHistory(const string &userID) {
    ifstream inFile(userID + "_transactions.txt");
    string line;
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            cout << line << endl;
        }
    }
    inFile.close();
}