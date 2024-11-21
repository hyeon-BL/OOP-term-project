#include "bank.h"
#include "account.h"
#include <iostream>

using namespace std;

Bank::Bank(string name) : bankname(name), numaccounts(0) {
    for (int i = 0; i < 1000; i++) {
        accounts[i] = nullptr;
    }
}

Bank::~Bank() {
    for (int i = 0; i < numaccounts; i++) {
        delete accounts[i];
    }
}

void Bank::createaccount(string name, int number, int balance, int password) {
    if (numaccounts < 1000) {
        accounts[numaccounts] = new Account(name, number, password, balance);
        accounts[numaccounts]->setBankName(bankname);
        numaccounts++;
    }
}

Account* Bank::createAccount(string name, int accountNumber, int password, int initialBalance) {
    if (numaccounts >= 1000) return nullptr;

    accounts[numaccounts] = new Account(name, accountNumber, password, initialBalance);
    accounts[numaccounts]->setBankName(bankname);
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
        cout << "Bank: " << bankname
            << ", Username: " << accounts[i]->getAccName()
            << ", Account: " << accounts[i]->getAccNumber()
            << ", Balance: " << accounts[i]->getBalance() << endl;
    }
}
