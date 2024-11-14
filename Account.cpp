#include "account.h"
#include <iostream>
#include <sstream>

using namespace std;

Account::Account(string name, int number, double initialBalance) {
    accName = name;
    accNumber = number;
    balance = initialBalance;
}

Account::~Account() {
}
string Account::getAccName() const {
    return accName;
}

int Account::getAccNumber() const {
    return accNumber;
}

double Account::getBalance() const {
    return balance;
}

// 거래 내역 반환
vector<string> Account::getTransactionHistory() const {
    return transactionHistory;
}

// 입금 함수
void Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        addTransaction("Deposit: " + to_string(amount));
    } else {
        cout << "Invalid deposit amount." << endl;
    }
}

// 출금 함수
bool Account::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        addTransaction("Withdrawal: " + to_string(amount));
        return true;
    } else {
        cout << "Invalid withdrawal amount or insufficient funds." << endl;
        return false;
    }
}

// 거래 내역 추가 함수
void Account::addTransaction(const string& transaction) {
    transactionHistory.push_back(transaction);
}
