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
    if (!currentSession || amount <= 0) return false;
    
    Account* account = currentSession->getActiveAccount();
    if (!account) return false;

    int fee = primaryBank->getdfee();
    if (amount > fee) {
        account->deposit(amount - fee);
        Transaction trans("Deposit", amount);
        currentSession->addTransaction(trans);
        return true;
    }
    return false;
}

bool ATM::withdraw(double amount) {
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