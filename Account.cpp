#include "Account.h"
#include "Bank.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 생성자
Account::Account(string name, long long number, int password, int initialBalance)
    : accName(name), accNumber(number), password(password), accbalance(initialBalance), bankName(nullptr), isAdmin(false) {
}

// 소멸자
Account::~Account() {}

// Getter 메서드
string Account::getAccName() const {//계좌 이름 알기 
    return accName;
}

long long Account::getAccNumber() const {//계좌 번호 알기 
    return accNumber;
}

int Account::getBalance() const {//계좌 잔액 알기 
    return accbalance;
}

string Account::getBankName() const {//계좌 은행 알기 
    return bankName ? bankName->getbankname() : "No Bank";
}

vector<string> Account::getTransactionHistory() const {//계좌 히스토리 저장 
    return transactionHistory;
}

bool Account::isAdmincheck() const {
    return isAdmin;
}

bool normalAccount::isAdmincheck() const {
    return false;
}

bool admin::isAdmincheck() const {
    cout << "관리자 카드가 삽입이 되었다\n";
    return true;
}

bool Account::verifyPassword(int inputPassword) const {// 비밀번호 확인 메서드
    return password == inputPassword;
}

void Account::deposit(int amount) {// 입금 메서드
    if (amount > 0) {
        accbalance += amount;
        addTransaction("Deposit: " + to_string(amount));
    } else {
        cout << "Invalid deposit amount." << endl;
    }
}

bool Account::withdraw(int amount) {// 출금 메서드
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
void Account::setBankName(Bank& bank) {
    this->bankName = &bank;
}


normalAccount::normalAccount(Bank* bank, string name, long long number, int password, int accbalance) : Account(name, number, password, accbalance) {
    this->bankName = bank;
    this->isAdmin = false;
}
admin::admin(Bank* bank, string name, int number, int password) : Account(name, number, password) {
    this->bankName = bank;
    this->isAdmin = true;
}