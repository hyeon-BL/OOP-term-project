#include "atm.h"
#include <ctime>

const double ATM::MAX_WITHDRAWAL_AMOUNT = 500000.0;
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

ATM::ATM(int serial, ATMType atmType, bool bilingual, Bank* primary, const std::vector<Bank*>& banks) // ATM(시리얼, ATM타입, 다국어지원여부, 기본은행, 은행목록)
    : serialNumber(serial), type(atmType), isBilingual(bilingual), 
      primaryBank(primary), currentSession(nullptr), 
      currentLanguage(Language::English) {
    
    primaryBank->setisprimarybank(true); // Set primary bank flag
    if (type == ATMType::MultiBank) { // MultiBank ATM supports all banks
        for (auto bank : banks) {
            supportedBanks.push_back(bank);
        }
    }
}

ATM::~ATM() {
    delete currentSession;
}

void ATM::readCardinfo(){//카드 정보 확인
	bool check = false // 카드 정보 확인 상태 
	bool admincheck = false;//관리자 계좌인지 확인 상태  
	for (Bank* bank : bankList) { //일반 계좌인지 확인 
        for (normalAccount* account : bank->getAccountList()) {
            if (accNum == account->getNum()) { // 계좌 번호 일치
                usingAccount = account;
                valid = true;
                break;
            }
        } 
        if (!valid && (bank->getBankName() == this->getBankname())) {// 관리자 계좌인지 확인 
            for (Admin* admin : bank->getAdminList()) {
                if (admin->getNum() == accNum) {
                    isAdmin = true;
                    adminCheck = true;
                    valid = true;
                    break;
                }
            }
        }
        
        if check://계좌번호 찾으면 종료 
			break 
			
	if (!valid) { // 계좌를 찾지 못한 경우
        if (isEnglish)
            cout << "Error: No account found, returning card\n" << endl;
        else
            cout << "오류: 존재하지 않는 계좌입니다. 카드를 반환하겠습니다\n" << endl;
        return;
    }
    
    if (adminCheck) { // 관리자 계정일경우 
    	if (isEnglish){
            cout << "It has been verified that it is an administrator card\n" << endl;
        }else{
            cout << "관리자 카드임이 확인되었습니다.\n" << endl;
    	}
        return; 
    }	
} 
}

int ATM::atmstart(){ // atm 시작 
	if(isEnglish){
		cout<<"카드를 기계에 넣으시오"<<endl;
		cout<<"계좌번호를 입력하시오"<<endl;
	}
	else{
		cout<<"please inser your card"<<endl;
		cout<<"please wirte your account nummer"<<endl;
	}
	cin >> accNumber;
	readCardinfo(accNumber);
	if(단일 은행, 비주력 은행이 아닐때){// ATM기가 단일이지만 카드가 비주력 은행이 아닐때 
		if (isEnglish){
            cout << "this can use\n" << endl;
        }else{
            cout << "이 카드는 해당 ATM에서 사용 불가하다.\n" << endl;
    	}
        return; 
	} 
	int password_check = 0;
	while(password_check <3){//비밀번호 일치 여부 확인 
		cin>>password_in
		if (password != password_in ){
			if (isEnglish){
            cout << "password incorrect\n" << endl;
	        }else{
	            cout << "비밀번호 틀림.\n" << endl;
    		}
    		password_ceck+=1;
		}
		}
	if (password_check == 4){//비밀번호가 3번 초과하여 틀릴시 
		if (isEnglish){
            cout << "password incorrect over 3 times\n" << endl;
	        }else{
	            cout << "비밀번호가 3번 초과하여 틀렸습니다. 정지\n" << endl;
    		}
	}	
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

void ATM::setLanguage(Language lang) {// 언어 선택 
    if (!isBilingual && lang != Language::English) {
        displayMessage("This ATM only supports English.");
        return;
    }
    currentLanguage = lang;
}

// Update transaction operations
void ATM::deposit(double amount) {
    if (!currentSession) {
        std::cout << "No active session." << std::endl;
        return false;
    }

    Account* account = currentSession->getActiveAccount();
    if (!account) {
        std::cout << "No card inserted." << std::endl;
        return false;
    }

    std::string depositType;
    std::cout << "인출 타입 (money/check): ";
    std::cin >> depositType;

    double totalAmount = 0.0;
    if (depositType == "money") {
        int denominations[4];
        const int denominationValues[4] = {50000, 10000, 5000, 1000};
        
        for (int i = 0; i < 4; i++) {
            std::cout << denominationValues[i] << " 원권을 입력하시오: ";
            std::cin >> denominations[i];
        }

        int totalBills = 0;
        for (int i = 0; i < 4; i++) {
            totalBills += denominations[i];
            totalAmount += denominations[i] * denominationValues[i];
        }

        if (totalBills > MAX_CASH_INPUT) {
            std::cout << "현금이 너무 많음, 최대: " << MAX_CASH_INPUT << std::endl;
            return false;
        }
    } else if (depositType == "check") {
        int checkCount;
        std::cout << "입력할 수표의 장수를 입력: ";
        std::cin >> checkCount;

        if (checkCount > MAX_CHECK_COUNT) {
            std::cout << "수표가 너무 많음, 최대: " << MAX_CHECK_COUNT << std::endl;
            return false;
        }

        for (int i = 0; i < checkCount; i++) {
            double checkAmount;
            std::cout << (i + 1) << "번째 수표 금액을 입력: ";
            std::cin >> checkAmount;
            totalAmount += checkAmount;
        }
    } else {
        std::cout << "입금이 불가능한 타입입니다." << std::endl;
        return false;
    }

    int fee = account->getBankName() == primaryBank->getbankname() ? 
              primaryBank->getdfee() : primaryBank->getdfee() * 2;
    
    std::cout << "입금 수수료: " << fee << std::endl;
    
    if (totalAmount > fee) {
        account->deposit(totalAmount - fee);
        atmCashBalance += totalAmount;
        Transaction trans("Deposit", totalAmount);
        currentSession->addTransaction(trans);
        return true;
    }

    return false;
}

bool ATM::withdraw(double amount) {
    if (!currentSession) { // 활성화 안됨 
        std::cout << "No active session." << std::endl;
        return false;
    }

    if (withdrawalCount >= MAX_WITHDRAWAL_COUNT) { // 최대 인출 횟수 초과 
        std::cout << "Maximum withdrawal count reached. Please end session and start again." << std::endl;
        return false;
    }

    if (amount > MAX_WITHDRAWAL_AMOUNT) {// 최대 인출 한도 초과 
        std::cout << "Amount exceeds maximum withdrawal limit of " << MAX_WITHDRAWAL_AMOUNT << std::endl;
        return false;
    }

    if (static_cast<int>(amount) % 1000 != 0) {// 무조건 1000 이상 입금 가능 
        std::cout << "Amount must be in multiples of 1000" << std::endl;
        return false;
    }

    Account* account = currentSession->getActiveAccount();
    if (!account) return false;

    int fee = account->getBankName() == primaryBank->getbankname() ? 
              primaryBank->getwfee() : primaryBank->getwfee() * 2; // 수수료 

    if (atmCashBalance < (amount + fee)) {//ATM 잔액 부족 
        std::cout << "ATM has insufficient funds." << std::endl;
        return false;
    }

    if (account->withdraw(amount + fee)) {//출금 성공 
        atmCashBalance -= amount;
        withdrawalCount++;
        Transaction trans("Withdrawal", amount);
        currentSession->addTransaction(trans);
        std::cout << "Withdrawal successful" << std::endl;
        return true;
    }

    std::cout << "Withdrawal failed - insufficient funds in account" << std::endl; // 계좌 잔액 부족
    return false;
}

bool ATM::transfer(Account* target, double amount) {
	cout<<"계좌 송금 유형을 고르시오"<<endl;
	cout<<"1. 계좌 송금, 2. 현금 송금"<<endl;
	int transfer_type = 0;
	cin<<transfer_type;
	int transfer_count=0 
	
	cout<<"송금할 계좌 은행을 입력하시오"<<endl;
	cin<<transfer_to_bank;
	while (true) {
        if (bankmap[transfer_to_bank] != nullptr) { // bankmap은 뱅크 목록 
            transferBank = bankmap[transfer_to_bank];
            break;
        } else {
            cout << "해당 은행을 찾을 수 없습니다. 다시 제대로 입력해주세요." << endl;
            transfer_count += 1;
        }
        if (transfer_count == 3) {
            cout << "시도 횟수가 초과되었습니다. 카드를 다시 가져가세요." << endl;
            return false;
        }
    }
	while (true) {
        if (transferBank->getAccountMap()[transfer_to_account] != nullptr) {//getaccountmap은 계좌 목록 
            transferAccount = transferBank->getAccountMap()[transfer_to_account];
            break;
        } else {
            cout << "해당 계좌를 찾을 수 없습니다. 다시 제대로 입력해주세요." << endl;
            transfer_count += 1;
        }
        if (transfer_count == 3) {
            cout << "시도 횟수가 초과되었습니다. 카드를 다시 가져가세요." << endl;
            return false;
        }
    }
	
	if (transfer_type == 2) {//현금일때 돈 받기 
        int denominations[4];
        const int denominationValues[4] = {50000, 10000, 5000, 1000};
        
        for (int i = 0; i < 4; i++) {
            std::cout << denominationValues[i] << " 원권을 입력하시오: ";
            std::cin >> denominations[i];
        }

        int totalBills = 0;
        for (int i = 0; i < 4; i++) {
            totalBills += denominations[i];
            totalAmount += denominations[i] * denominationValues[i];
        }

        if (totalBills > MAX_CASH_INPUT) {
            std::cout << "현금이 너무 많음, 최대: " << MAX_CASH_INPUT << std::endl;
            return false;
        }
        // 수수료 
        
        if (둘다 주거래 은행이라면){
        	fee=2000; 
		}
		else if(둘다 비주거래 은행){
			fee=4000;
		}
		else{
			fee=3000; 
		}
		cout<<"수수료는"<<fee<<"입니다"<<endl;
		total=totlalBills+fee // 수수료 포함 입금 가격
		cout<<"입금액: "<<totalBills<<"수수료:"<<fee<"합계:"<<total<<"입니다"<<endl; 
	}
	else{//계좌간 이체
		cout<<"출금할 계좌를 입력하시오"<<endl;
		string account_out;
		cin>>account_out;
		if(account_out != 뱅크의 계좌 목록){
			cout<<"존재하지 않는 계좌입니다"<<endl;
			return; 
		} 
		cout<<"송금할 금액을 입력하시오"<<endl;
		int transfer_money;
		cin>>transfer_money;
		if (둘다 주거래 은행이라면){
        	fee=2000; 
		}
		else if(둘다 비주거래 은행){
			fee=4000;
		}
		else{
			fee=3000; 
		}
		if(transfer_money+fee>현재 계좌){
			cout<<"계좌에 잔액이 부족합니다"<<endl; 
		} 
	}
	*usingAccount -= (transfer_money + fee);// 보내는 사람 계좌 차감 
    *transferAccount += transfer_money;// 받는 사람 계좌 잔액 증가 
}
	if(transfer_count)
    if (!currentSession) return false;
    Transaction trans("Transfer", amount);
    currentSession->addTransaction(trans);
    return true;
