#include "bank.h"
#include "account.h"
#include <iostream>

using namespace std;
Bank::Bank(std::string name) : bankname(name), numaccounts(0) {
    this->bankname = bankname;
    setisprimarybank(false);
    for (int i = 0; i < 1000; i++) {
        accounts[i] = nullptr;
    }
}

Bank::~Bank() {
    for (int i = 0; i < numaccounts; i++) {
        delete accounts[i];
    }
}


void Bank::setisprimarybank(bool isprimary) {
    isprimarybank = isprimary;
    dfee = isprimary ? 1000 : 2000;
    wfee = isprimary ? 500 : 1000;
    afee = isprimary ? 500 : 1000;
    cfee = isprimary ? 1000 : 2000;
}

void Bank::createaccount(string name, int accnumber, int balance, int password) {
    if (numaccounts < 1000) {
        accounts[numaccounts] = new Account(name, accnumber, password, balance);
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
    cout << "Account not found." << endl;
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

// normalAccount* Bank :: makeaccount{// 계좌 생성 
// 	string accName;
// 	int accNumber;
// 	int password;
// 	if (language = "한국어"){
// 		cout<<"사용자 이름을 입력하시오"<<endl;
// 		cin >> accName;
// 		cout<<"계좌 번호를 입력하시오"<<endl;
// 		cin >> accNumber;
// 		cout<<"비밀번호를 입력하시오"<<endl;
// 		cin >> password;
// 	}else{
// 		cout<<"input user name"<<endl;
// 		cin >> accName;
// 		cout<<"input account number"<<endl;
// 		cin >> accNumber;
// 		cout<<"input password"<<endl;
// 		cin >> password;
// 	}
// 	normalAccount* newaccount;
// 	newaccount = new normalAccount(this, accName, accNumver, password);
// 	return newaccount
// }

// Admin* Bank::makeadminaccount(string accName, string accNumber){ // 관리자 계좌 생성 
// 	Admins* newAdmin
// 	newAdmin = new Admin(this, accName, accNumver);
// 	return newAdmin;
// }
