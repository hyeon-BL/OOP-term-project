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
};

class ATM {
private:
    int serialNumber;
    ATMType type;
    bool isBilingual;
    Bank* primaryBank;
    std::vector<Bank*> supportedBanks;
    Session* currentSession;
    Language currentLanguage;

public:
    ATM(int serial, ATMType atmType, bool bilingual, Bank* primary);
    ~ATM();

    bool insertCard(Account* account);
    bool endSession();
    bool validateCard(Account* account) const;
    void displayMessage(const std::string& msg);
    void setLanguage(Language lang);
    
    // Transaction operations
    bool deposit(double amount);
    bool withdraw(double amount);
    bool transfer(Account* target, double amount);
};

#endif

