// bank.h

#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <string>

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
    virtual ~Bank();  // Virtual destructor for inheritance

    bool getisprimarybank() const { return isprimarybank; }
    int getdfee() const { return dfee; }
    int getwfee() const { return wfee; }
    int getafee() const { return afee; }
    int getcfee() const { return cfee; }

    void createaccount();       // Creates a new account
    void printbankaccount() const;  // Prints account details
    int* depositcashtoatm(int amount);  // Deposits cash to ATM by denomination
};

// Derived classes
class PrimaryBank : public Bank {
public:
    PrimaryBank(string name);
    ~PrimaryBank() override;
};

class NonPrimaryBank : public Bank {
public:
    NonPrimaryBank(string name);
    ~NonPrimaryBank() override;
};

#endif // BANK_H
