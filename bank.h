#ifndef BANK_H
#define BANK_H

#include <string>
#include "Account.h" // Account 클래스 선언 포함

class Bank {
protected:
    std::string bankname; // 은행 이름
    Account* accounts[1000]; // 최대 1000개의 계좌
    int numaccounts; // 현재 계좌 수

public:
    // 생성자와 소멸자
    Bank(std::string bankname);
    ~Bank();

    // 은행 정보 가져오기
    std::string getbankname() const;

    // 계좌 관련 메서드
    Account* getAccount(int accountNumber) const;
    Account* createAccount(std::string name, int accountNumber, int password, int initialBalance = 0);
    void createaccount(std::string name, int number, int balance, int password);

    // 은행의 모든 계좌 정보 출력
    void printbankaccount() const;
};

#endif // BANK_H
