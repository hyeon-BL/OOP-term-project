#include "Account.h"
#include <iostream>

using namespace std;

// 생성자
Account::Account(string name, int number, int password, int initialBalance)
    : accName(name), accNumber(number), password(password), accbalance(initialBalance), bankName("") {
}

// 소멸자
Account::~Account() {}

// Getter 메서드
string Account::getAccName() const {
    return accName;
}

int Account::getAccNumber() const {
    return accNumber;
}

int Account::getBalance() const {
    return accbalance;
}

string Account::getBankName() const {
    return bankName;
}

vector<string> Account::getTransactionHistory() const {
    return transactionHistory;
}

// 비밀번호 확인 메서드
bool Account::verifyPassword(int inputPassword) const {
    return password == inputPassword;
}

// 입금 메서드
void Account::deposit(int amount) {
    if (amount > 0) {
        accbalance += amount;
        addTransaction("Deposit: " + to_string(amount));
    } else {
        cout << "Invalid deposit amount." << endl;
    }
}

// 출금 메서드
bool Account::withdraw(int amount) {
    if (amount > 0 && amount <= accbalance) {
        accbalance -= amount;
        addTransaction("Withdrawal: " + to_string(amount));
        return true;
    } else {
        cout << "Invalid withdrawal amount or insufficient funds." << endl;
        return false;
    }
}

// 거래 내역 추가 메서드
void Account::addTransaction(const string& transaction) {
    transactionHistory.push_back(transaction);
}

// 은행 이름 설정 메서드
void Account::setBankName(const string& bank) {
    bankName = bank;
}
