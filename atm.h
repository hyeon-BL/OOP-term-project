#ifndef ATM_H
#define ATM_H

#include <string>
#include <unordered_map>
#include <vector>

enum class ATMType { SINGLE_BANK, MULTI_BANK };
enum class Language { ENGLISH, KOREAN };
enum class CashType { KRW1000, KRW5000, KRW10000, KRW50000 };
enum class TransactionType { DEPOSIT, WITHDRAWAL, TRANSFER };

class Card;
class Session;
class Transaction;

class ATM {
private:
    std::string serialNumber;
    ATMType type;
    Language language;
    std::string primaryBank;
    std::map<CashType, int> availableCash;

public:
    ATM(std::string sn, ATMType t, Language lang, std::string pBank)
        : serialNumber(sn), type(t), language(lang), primaryBank(pBank) {
        // Initialize available cash to 0
        availableCash[CashType::KRW_1000] = 0;
        availableCash[CashType::KRW_5000] = 0;
        availableCash[CashType::KRW_10000] = 0;
        availableCash[CashType::KRW_50000] = 0;
    }

    void addCash(CashType type, int count) {
        availableCash[type] += count;
    }

    std::string getSerialNumber() const { return serialNumber; }
    ATMType getType() const { return type; }
    Language getLanguage() const { return language; }
    std::map<CashType, int> getCashStatus() const { return availableCash; }
    std::string getPrimaryBank() const { return primaryBank; }
};


#endif // ATM_H

