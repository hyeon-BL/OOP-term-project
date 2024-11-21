#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

using namespace std;

class Account {
private:
    string accName;                   // 계좌 이름
    int accNumber;                    // 계좌 번호
    int password;                     // 계좌 비밀번호
    int accbalance;                   // 계좌 잔액
    string bankName;                  // 은행 이름
    vector<string> transactionHistory; // 거래 내역

public:
    // 생성자와 소멸자
    Account(string name, int number, int password, int initialBalance = 0);
    ~Account();

    // Getter 메서드
    string getAccName() const;
    int getAccNumber() const;
    int getBalance() const;
    string getBankName() const;
    vector<string> getTransactionHistory() const;

    // 계좌 기능
    bool verifyPassword(int inputPassword) const; // 비밀번호 확인
    void deposit(int amount);                     // 입금
    bool withdraw(int amount);                    // 출금
    void addTransaction(const string& transaction); // 거래 내역 추가
    void setBankName(const string& bank);         // 은행 이름 설정
};

#endif // ACCOUNT_H
