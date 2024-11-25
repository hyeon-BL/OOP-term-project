#ifndef ACCOUNT_H
#define ACCOUNT_H

class Bank;  // Forward declaration needed

#include <string>
#include <vector>

using namespace std;

class Account {
protected:
    string accName;                   // 사용자 이름
    int accNumber;                    // 계좌 번호
    int password;                     // 계좌 비밀번호
    int accbalance;                   // 계좌 잔액
    Bank* bankName;                  // 은행 이름
    vector<string> transactionHistory; // 거래 내역
    bool isAdmin;                     // 관리자 여부 

public:
    // 생성자와 소멸자
    Account(string name, long long number, int password, int initialBalance = 0);
    ~Account();

    // Getter 메서드
    string getAccName() const; // 사용자 이름 확인 
    long long getAccNumber() const;	// 계좌번호 확인 
    int getBalance() const;		//계좌 잔액 확인 
    string getBankName() const;	// 계좌 은행 확인 
    vector<string> getTransactionHistory() const; //계좌 기록 확인 
    virtual bool isAdmincheck() const;		//관리자 여부 확인 

    // 계좌 기능
    bool verifyPassword(int inputPassword) const; // 비밀번호 확인
    void deposit(int amount);                     // 입금
    bool withdraw(int amount);                    // 출금
    void addTransaction(const string& transaction); // 거래 내역 추가
    void setBankName(Bank& bank);         // 은행 이름 설정
};

class normalAccount : public Account{ // 일반 계좌 정보 
public:
    normalAccount(Bank* bank, string name, long long number, int password, int accbalance);
    bool isAdmincheck() const override;		
};


class admin : public Account{
public:
	admin(Bank* bank, string name, int number, int password);  // Fix parameter type
    bool isAdmincheck() const; // Add const
};

#endif
