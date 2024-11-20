#include <iostream>
#include <string>
#include "account.h"
#include "bank.h"

using namespace std;

class Bank {
protected:
	string bankname;
	Account* accounts[1000];//일단 1000개로 잡음
	int numaccounts;//number of accounts of specific bank
	bool isprimarybank;//primary bank specifier
	int dfee;//deposit fee
	int wfee;//withdrawal fee
	int afee;//account transfer fee
	int cfee;//cash transfer fee

public:
	Bank(string bankname);
	~Bank();
	bool getisprimarybank() { return this->isprimarybank; };
	int getdfee() { return this->dfee; }
	int getwfee() { return this->wfee; }
	int getafee() { return this->afee; }
	int getcfee() { return this->cfee; }

	void createaccount();//account에 맞춰서 짜야함
	void printbankaccount();//account에 맞춰서 짜야함
	int* depositcashtoatm(int amount);//deposit cashes to an atm to serve user(각 지폐 단위로)
	Account* createAccount(std::string name, int accountNumber, double initialBalance);
	Account* getAccount(int accountNumber) const;
};

Bank::Bank(string name) {
	bankname = name;
	numaccounts = 0;
	for (int i = 0; i < 1000; i++) {
		accounts[i] = nullptr;
	}
}

Bank::~Bank() {
	for (int i = 0; i < numaccounts; i++) {
		delete accounts[i];
	}
}

void Bank::createaccount() {
	if (numaccounts < 1000) {
		accounts[numaccounts] = new Account();
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

void Bank::printbankaccount() {
	for (int i = 0; i < numaccounts; i++) {
		cout << "Bank: " << bankname 
             << ", Username: " << accounts[i]->getAccName()
             << ", Account: " << accounts[i]->getAccNumber()
             << ", Balance: " << accounts[i]->getBalance() << endl;
	}
}

int* Bank::depositcashtoatm(int amount) {
	int result[4];
	int t = 50000;
	for (int i = 0; i < 4; i++) {
		amount = amount / t;
		result[i] = amount;
		t == 10000 ? t = t / 2 : t = t / 5;
		}
	return result;
	}




class PrimaryBank : public Bank {
public:
	PrimaryBank(string name) : Bank(name) {
		isprimarybank = true;
		dfee = 1000;
		wfee = 1000;
		afee = 2000;
		cfee = 1000;
	}
	~PrimaryBank() {};
};

class NonPrimaryBank : public Bank {
public:
	NonPrimaryBank(string name) : Bank(name) {
		isprimarybank = false;
		dfee = 2000;
		wfee = 2000;
		afee = 3000;
		cfee = 1000;
	}
	~NonPrimaryBank() {};

};
