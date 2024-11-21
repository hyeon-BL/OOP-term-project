#include "bank.h"
#include "account.h"
#include <iostream>

Bank::Bank(std::string name) : bankname(name), numaccounts(0) {
    setisprimarybank(false);
    for (int i = 0; i < 1000; i++) {
        accounts[i] = nullptr;
    }
}

Bank::~Bank() {
    for (int i = 0; i < numaccounts; i++) {
        delete accounts[i];
    }
}

void Bank::setisprimarybank(bool isprimary) {
    isprimarybank = isprimary;
    dfee = isprimary ? 1000 : 2000;
    wfee = isprimary ? 500 : 1000;
    afee = isprimary ? 500 : 1000;
    cfee = isprimary ? 1000 : 2000;
}

void Bank::createaccount(std::string name, int number, double balance) {
    if (numaccounts < 1000) {
        accounts[numaccounts] = new Account(name, number, balance, bankname);
        numaccounts++;
    }
}

Account* Bank::createAccount(std::string name, int accountNumber, double initialBalance) {
    if (numaccounts >= 1000) return nullptr;
    
    accounts[numaccounts] = new Account(name, accountNumber, initialBalance, bankname);
    numaccounts++;
    return accounts[numaccounts - 1];
}

Account* Bank::getAccount(int accountNumber) const {
    for (int i = 0; i < numaccounts; i++) {
        if (accounts[i]->getAccNumber() == accountNumber) {
            return accounts[i];
        }
    }
    return nullptr;
}

void Bank::printbankaccount() const {
    for (int i = 0; i < numaccounts; i++) {
        std::cout << "Bank: " << bankname 
             << ", Username: " << accounts[i]->getAccName()
             << ", Account: " << accounts[i]->getAccNumber()
             << ", Balance: " << accounts[i]->getBalance() << std::endl;
    }
}

int* Bank::depositcashtoatm(int amount) {
    int* result = new int[4];
    int t = 50000;
    for (int i = 0; i < 4; i++) {
        result[i] = amount / t;
        amount %= t;
        t = (t == 10000) ? t / 2 : t / 5;
    }
    return result;
}


