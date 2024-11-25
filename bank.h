#ifndef BANK_H
#define BANK_H

#include <string>
#include "Account.h" // Account 클래스 선언 포함

class Bank {
protected:
    std::string bankname; // 은행 이름
    Account* accounts[1000]; // 최대 1000개의 계좌
    int numaccounts; // 현재 계좌 수
    bool isprimarybank;
    int dfee;
    int wfee;
    int afee;
    int cfee;

public:
    Bank(string bankname);
    ~Bank();  // Remove =default and provide implementation

    void setisprimarybank(bool isprimary);
    bool getisprimarybank() const { return isprimarybank; }
    string getbankname() const;
    int getdfee() const { return dfee; }
    int getwfee() const { return wfee; }
    int getafee() const { return afee; }
    int getcfee() const { return cfee; }
    Account* getAccount(long long accountNumber) const;
    Account* createAccount(std::string name, long long accountNumber, int password, int initialBalance = 0);

    void createaccount(std::string name, long long number, int accbalance, int password);  // Updated signature
    void printbankaccount() const;  // Prints account details
    //std::string Bank;
};


#endif // BANK_H
