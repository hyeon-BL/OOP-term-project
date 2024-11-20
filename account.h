#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

class Account {
private:
    std::string accName;
    int accNumber;
    double balance;
    std::string bankName;
    std::vector<std::string> transactionHistory;

public:
    Account(std::string name, int number, double initialBalance = 0.0, std::string bank = "");
    ~Account();

    std::string getAccName() const;
    int getAccNumber() const;
    double getBalance() const;
    std::string getBankName() const { return bankName; }
    std::vector<std::string> getTransactionHistory() const;

    void deposit(double amount);
    bool withdraw(double amount);
    void addTransaction(const std::string& transaction);
};

#endif