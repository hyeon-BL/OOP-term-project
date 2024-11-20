#include "atm.h"
#include <ctime>

int Transaction::nextTransactionId = 1;

Transaction::Transaction(std::string t, double a) : type(t), amount(a) {
    transactionId = nextTransactionId++;
    time_t now = time(0);
    timestamp = ctime(&now);
}

Session::Session() : activeAccount(nullptr), state(SessionState::Idle) {}

void Session::addTransaction(const Transaction& trans) {
    transactions.push_back(trans);
}

void Session::printSummary() const {
    if (transactions.empty()) return;
    
    std::cout << "=== Session Summary ===" << std::endl;
    for (const auto& trans : transactions) {
        std::cout << "Transaction ID: " << trans.transactionId << std::endl;
        std::cout << "Type: " << trans.type << std::endl;
        std::cout << "Amount: " << trans.amount << std::endl;
        std::cout << "Time: " << trans.timestamp << std::endl;
    }
}

ATM::ATM(int serial, ATMType atmType, bool bilingual, Bank* primary) 
    : serialNumber(serial), type(atmType), isBilingual(bilingual), 
      primaryBank(primary), currentSession(nullptr), 
      currentLanguage(Language::English) {
    supportedBanks.push_back(primary);
}

ATM::~ATM() {
    delete currentSession;
}

bool ATM::insertCard(Account* account) {
    if (!validateCard(account)) {
        displayMessage("Invalid card for this ATM.");
        return false;
    }
    
    if (currentSession != nullptr) {
        displayMessage("Session already in progress.");
        return false;
    }
    
    currentSession = new Session();
    currentSession->setActiveAccount(account);
    displayMessage("Card accepted. Welcome!");
    return true;
}

bool ATM::endSession() {
    if (currentSession == nullptr) return false;
    
    currentSession->printSummary();
    delete currentSession;
    currentSession = nullptr;
    displayMessage("Thank you for using our ATM.");
    return true;
}

bool ATM::validateCard(Account* account) const {
    if (type == ATMType::SingleBank) {
        return account->getBankName() == primaryBank->getbankname();
    }
    return true; // MultiBank ATM accepts all cards
}

void ATM::displayMessage(const std::string& msg) {
    // In a real implementation, this would handle bilingual support
    std::cout << msg << std::endl;
}

void ATM::setLanguage(Language lang) {
    if (!isBilingual && lang != Language::English) {
        displayMessage("This ATM only supports English.");
        return;
    }
    currentLanguage = lang;
}

// Update transaction operations
bool ATM::deposit(double amount) {
	int money[4]
	if (input = money){ ///수정 
		cout<<"50000원권 입력하시오"<<endl;
		cin>>money[0]
		cout<<"10000원권 입력하시오"<<endl;
		cin>>money[1] 
		cout<<"5000원권 입력하시오"<<endl;
		cin>>money[2] 
		cout<<"1000원권 입력하시오"<<endl;
		cin>>money[3] 
		if (money[0]+money[1]+money[2]+money[3]>50){
			displayMessage("현금이 너무 많음")
			return  
		}
		amountmoney = 50000*money[0]+10000*money[1]+5000*money[2]+1000*money[3]>50
	}
	if (input == check){// 수표 
		cout<<"입력할 수표의 장수를 입력"<<endl
		cin>>acountcheck;
		if(acountcheck>30){
			cout<<"수표가 너무 많음"<<endl
			return 0 
		}
		int checks[acountcheck]; 
		for (i=0,i<acountcheck,i++){
			cout << i <<"번째 수표의 금액을 입력하시오"<<endl
			cin >> checks[i]
		} 
		int amountcheck;
		for (i=0,i<acountcheck,i++){
			amountcheck += checks[i]
		}
		
	}
	
	if (!account){
    	cout<<" 카드가 없습니다" 
	} return false;
	
	
    if (!currentSession){
    	displayMessage("무슨 상태?.");
        return false;    	
	} 
	if (amount <= 0){
        displayMessage("잔액 없음");
        return false;
    }
    Account* account = currentSession->getActiveAccount();
	cout<<"!!!돈 넣기" 
	
	 

    if(isprimaryBank){
    	primaryBank->getdfee();
	}
	else{
		primaryBank->getdfee();
	}
	cout<<"입금수수료는"<<fee<<"입니다"<<endl 
	
	
    if (amount > fee) {
        account->deposit(amount - fee);
        Transaction trans("Deposit", amount);
        currentSession->addTransaction(trans);
        return true;
    }
    

    return false;
}

bool ATM::withdraw(double amount) {
	drawcount=0
	if (drawcount == 3){
		cout << " 3번이 초과되었으니 현재 세션을 종료하고 다시 세션을 시작하시오"
		return ; 
	}
	cout <<"출금할 금액을 입력하시오"<<endl
	cin >> withdrawmoney;
	while (withdrawmoney % 1000 != 0){
		cout<<"출금은 1000단위로 가능합니다. 다시입력해주세요"<<endl
		cout <<"출금할 금액을 입력하시오"<<endl
		cin >> withdrawmoney;
	} 
	if (withdrawmoney>50000){
		cout <<"출금은 500,000원까지 가능합니다"<<endl;
		return
	}
	if(atmmoney<fee+withdrawmoney){//수정 
		cout<<"ATM 잔액 부족"<<endl;
		return ; 
	} 
	if(계좌money<fee+withdrawmoney){//수정 
		cout<<"계좌 잔액 부족"<<endl;
		return ; 
	}
	계좌 -=fee+withdrawmoney
	cout<<"출금에 성공하였음"<<endl;
	 
	
	
	
    if (!currentSession || amount <= 0) return false;
    
    Account* account = currentSession->getActiveAccount();
    if (!account) return false;

    int fee = primaryBank->getwfee();
    if (account->withdraw(amount + fee)) {
        Transaction trans("Withdrawal", amount);
        currentSession->addTransaction(trans);
        return true;
    }
    return false;
}

bool ATM::transfer(Account* target, double amount) {
    if (!currentSession) return false;
    Transaction trans("Transfer", amount);
    currentSession->addTransaction(trans);
    return true;
}
