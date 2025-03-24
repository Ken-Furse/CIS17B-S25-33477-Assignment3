#include <iostream>
#include <memory>
#include <stdexcept>

// Custom exception classes (All error messages)
class NegativeDepositException : public std::runtime_error {
public:
    NegativeDepositException() : std::runtime_error("Deposit amount cannot be negative.") {}
};

class OverdrawException : public std::runtime_error {
public:
    OverdrawException() : std::runtime_error("Insufficient funds for withdrawal.") {}
};

class InvalidAccountOperationException : public std::runtime_error {
public:
    InvalidAccountOperationException() : std::runtime_error("Operation not allowed on a closed account.") {}
};

// BankAccount class
class BankAccount {
private:
    std::string accountNumber;
    double balance;
    bool isActive;

public:
    // Constructor
    BankAccount(const std::string& accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance), isActive(true) {
    }

    // Deposit method
    void deposit(double amount) {
        if (!isActive) throw InvalidAccountOperationException();
        if (amount < 0) throw NegativeDepositException();
        balance += amount;
    }

    // Withdraw method
    void withdraw(double amount) {
        if (!isActive) throw InvalidAccountOperationException();
        if (amount > balance) throw OverdrawException();
        balance -= amount;
    }

    // Get balance
    double getBalance() const {
        return balance;
    }

    // Close account
    void closeAccount() {
        isActive = false;
    }
};

// Test function
void testBankAccount() {

    // Creates the account with 1000 dollars
    auto account = std::make_unique<BankAccount>("123456", 1000.0);

    //Tries to deposit, withdraw, and withdraw too much (Overdraw)
    try {
        std::cout << "Initial Balance: " << account->getBalance() << "\n";
        account->deposit(500);
        std::cout << "Balance after deposit: " << account->getBalance() << "\n";
        account->withdraw(200);
        std::cout << "Balance after withdrawal: " << account->getBalance() << "\n";

        account->withdraw(2000); // Should trigger OverdrawException
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    // Tries to deposit negative money
    try {
        account->deposit(-50); // Should trigger NegativeDepositException
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    // Closes Account
    account->closeAccount();

    //Tries to withdraw from closed account
    try {
        account->withdraw(100); // Should trigger InvalidAccountOperationException
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

int main() {
    testBankAccount();
    return 0;
}
