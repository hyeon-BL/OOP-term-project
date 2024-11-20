// bank.h

#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <string>
#include "account.h"

using namespace std;

class Account;  // Forward declaration of Account class

class Bank {
protected:
    string bankname;
    Account* accounts[1000]; // Array to hold pointers to Account objects
    int numaccounts;         // Number of accounts in the bank
    bool isprimarybank;      // Specifies if the bank is a primary bank
    int dfee;                // Deposit fee
    int wfee;                // Withdrawal fee
    int afee;                // Account transfer fee
    int cfee;                // Cash transfer fee

public:
    Bank(string bankname);
    virtual ~Bank();  // Remove =default and provide implementation

    bool getisprimarybank() const { return isprimarybank; }
    int getdfee() const { return dfee; }
    int getwfee() const { return wfee; }
    int getafee() const { return afee; }
    int getcfee() const { return cfee; }
    std::string getbankname() const { return bankname; }
    Account* getAccount(int accountNumber) const;
    Account* createAccount(std::string name, int accountNumber, double initialBalance = 0.0);

    void createaccount(std::string name, int number, double balance);  // Updated signature
    void printbankaccount() const;  // Prints account details
    int* depositcashtoatm(int amount);  // Deposits cash to ATM by denomination
};

// Derived classes
class PrimaryBank : public Bank {
public:
    PrimaryBank(string name);
    ~PrimaryBank() override = default;  // Use default since no special cleanup needed
};

class NonPrimaryBank : public Bank {
public:
    NonPrimaryBank(string name);
    ~NonPrimaryBank() override = default;  // Use default since no special cleanup needed
};

#endif // BANK_H
