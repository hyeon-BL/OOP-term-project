#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

using namespace std;

class Account {
private:
    string accName;                   // 사용자 이름
    int accNumber;                    // 계좌 번호
    int password;                     // 계좌 비밀번호
    int accbalance;                   // 계좌 잔액
    Bank* bankName;                  // 은행 이름
    vector<string> transactionHistory; // 거래 내역
    bool isAdmin						// 관리자 여부 

public:
    // 생성자와 소멸자
    Account(string name, int number, int password, int initialBalance = 0);
    ~Account();

    // Getter 메서드
    string getAccName() const; // 사용자 이름 확인 
    int getAccNumber() const;	// 계좌번호 확인 
    int getBalance() const;		//계좌 잔액 확인 
    string getBankName() const;	// 계좌 은행 확인 
    vector<string> getTransactionHistory() const; //계좌 기록 확인 
    bool verifyPassword;	//계좌 비밀번호 확인
	bool isAdmincheck;		//관리자 여부 확인 

    // 계좌 기능
    bool verifyPassword(int inputPassword) const; // 비밀번호 확인
    void deposit(int amount);                     // 입금
    bool withdraw(int amount);                    // 출금
    void addTransaction(const string& transaction); // 거래 내역 추가
    void setBankName(const string& bank);         // 은행 이름 설정
};

class normalAccount : public Account{ // 일반 계좌 정보 
public:
	normalAccount(Bank* bank, string name, int number, int password, int accbalance){
		void deposit(int amount);
		void withdraw(int amount);
		bool isAdmincheck() override;		
	};
normalAccount::normalAccount(Bank* bank, string name, int number, int password, int accbalance){
	this->bank = bank;
	this->accName = name;
	this->accNumber = number;
	this->password = password;
	this->accbalance = accbalance;
	this->isAdmincheck = false;
}


class admin : public Account{
public:
	admin(Bank* bank, string name, int numver, string password);
	
	bool admin(); //관리자 카드 삽입 
	bool isAdmincheck() override; // 관리자 여부 확인 
};
admin::admin(Bank* bank, string name, int numver, string password){
	this->bank = bank
	this->accname = name;
	this->accNumber = number;
	this->isAdmin = true;
}

#endif // ACCOUNT_H
