#include "atm.h"
#include <ctime>

const double ATM::MAX_WITHDRAWAL_AMOUNT = 500000.0;
// Define static constants
// const double ATM::MAX_WITHDRAWAL_AMOUNT = 500000.0;

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
    if (!currentSession) {
        std::cout << "No active session." << std::endl;
        return false;
    }

    if (withdrawalCount >= MAX_WITHDRAWAL_COUNT) {
        std::cout << "Maximum withdrawal count reached. Please end session and start again." << std::endl;
        return false;
    }

    if (amount > MAX_WITHDRAWAL_AMOUNT) {
        std::cout << "Amount exceeds maximum withdrawal limit of " << MAX_WITHDRAWAL_AMOUNT << std::endl;
        return false;
    }

    if (static_cast<int>(amount) % 1000 != 0) {
        std::cout << "Amount must be in multiples of 1000" << std::endl;
        return false;
    }

    Account* account = currentSession->getActiveAccount();
    if (!account) return false;

    int fee = account->getBankName() == primaryBank->getbankname() ? 
              primaryBank->getwfee() : primaryBank->getwfee() * 2;

    if (atmCashBalance < (amount + fee)) {
        std::cout << "ATM has insufficient funds." << std::endl;
        return false;
    }

    if (account->withdraw(amount + fee)) {
        atmCashBalance -= amount;
        withdrawalCount++;
        Transaction trans("Withdrawal", amount);
        currentSession->addTransaction(trans);
        std::cout << "Withdrawal successful" << std::endl;
        return true;
    }

    std::cout << "Withdrawal failed - insufficient funds in account" << std::endl;
    return false;
}

bool ATM::transfer(Account* target, double amount) {
    if (!currentSession) return false;
    Transaction trans("Transfer", amount);
    currentSession->addTransaction(trans);
    return true;
}
