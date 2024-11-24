#ifndef ATM_H
#define ATM_H

#include <string>
#include <vector>
#include "bank.h"
#include "account.h"

enum class ATMType { SingleBank, MultiBank };
enum class Language { English, Korean };
enum class SessionState { Idle, Active, Error };

struct Transaction {
    static int nextTransactionId;
    int transactionId;
    std::string type;
    double amount;
    std::string timestamp;
    std::string accountInfo;  // Added field for account info
    
    Transaction(std::string t, double a, const Account* acc);
};

class ATM; // Forward declaration

class Session {
private:
    Account* activeAccount;
    std::vector<Transaction> transactions;
    SessionState state;
    bool hasTransactions;  // Added to track if any transactions occurred

public:
    Session();
    void addTransaction(const Transaction& trans);
    void printSummary() const;
    SessionState getState() const { return state; }
    Account* getActiveAccount() const { return activeAccount; }
    void setActiveAccount(Account* account);
    void sethasTransactions(bool hasTransactions) { this->hasTransactions = hasTransactions; }
    bool processUserChoice(ATM* atm);  // New method to handle user choices
    bool hasCompletedTransactions() const { return hasTransactions; }
};

class ATM {
private:
    int serialNumber; //ATM 번호 
    ATMType type; //ATM타입 
    bool isBilingual;//ATM 지원 언어 
    Bank* primaryBank;//ATM의 primary bank 
    std::vector<Bank*> supportedBanks;//가능한 은행 
    Session* currentSession;
    Language currentLanguage;// 지원 언어 
    double atmCashBalance;
    int withdrawalCount;
    static const int MAX_WITHDRAWAL_COUNT = 3;
    static const double MAX_WITHDRAWAL_AMOUNT; // Remove constexpr and initialization
    static const int MAX_CASH_INPUT = 50;
    static const int MAX_CHECK_COUNT = 30;

public:
    ATM(int serial, ATMType atmType, bool bilingual, Bank* primary, const std::vector<Bank*>& banks);
    ~ATM();

    int atmstart();
    void readCardInfo(int accNum);
    bool insertCard(Account* account);
    bool endSession();
    bool validateCard(Account* account) const;
    void setLanguage(Language lang);
    bool IsBilingual() const { return isBilingual; }
    int getSerialNumber() const { return serialNumber; }
    Bank* getPrimaryBank() const { return primaryBank; }
    ATMType getATMType() const { return type; }
    
    // Transaction operations
    void deposit();
    void withdraw(double amount);
    bool transfer(Account* target, double amount);
    double getAtmCashBalance() const { return atmCashBalance; }
    void setAtmCashBalance(double amount) { atmCashBalance = amount; }
};

#endif // ATM_H