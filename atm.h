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
    
    Transaction(std::string t, double a);
};

class Session {
private:
    Account* activeAccount;
    std::vector<Transaction> transactions;
    SessionState state;

public:
    Session();
    void addTransaction(const Transaction& trans);
    void printSummary() const;
    SessionState getState() const { return state; }
    Account* getActiveAccount() const { return activeAccount; }
    void setActiveAccount(Account* account) { 
        activeAccount = account;
        state = SessionState::Active;
    }
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

    bool insertCard(Account* account);
    bool endSession();
    bool validateCard(Account* account) const;
    void displayMessage(const std::string& msg);
    bool setLanguage;
    
    // Transaction operations
    void deposit(double amount);
    void withdraw(double amount);
    bool transfer(Account* target, double amount);
    double getAtmCashBalance() const { return atmCashBalance; }
    void setAtmCashBalance(double amount) { atmCashBalance = amount; }
