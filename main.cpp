#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iomanip>
#include <algorithm>
#include "bank.h"
#include "atm.h"
#include "account.h"

using namespace std;

// Forward declarations
class Bank;
class ATM;
class Account;
class Card;

// Enums for system configuration
enum class ATMType { 
    SINGLE_BANK,
    MULTI_BANK 
};

enum class Language { 
    ENGLISH, 
    KOREAN,
    BILINGUAL 
};

enum class CashType {
    KRW_1000 = 1000,
    KRW_5000 = 5000,
    KRW_10000 = 10000,
    KRW_50000 = 50000
};


// Session class to handle system setup and management
class Session {
private:
    std::vector<std::shared_ptr<Bank>> banks;
    std::vector<std::shared_ptr<ATM>> atms;

    // Helper function to validate 6-digit ATM serial number
    bool isValidSerialNumber(const std::string& sn) {
        return sn.length() == 6 && std::all_of(sn.begin(), sn.end(), ::isdigit);
    }

    // Helper function to validate 12-digit account number
    bool isValidAccountNumber(const std::string& accNum) {
        return accNum.length() == 12 && std::all_of(accNum.begin(), accNum.end(), ::isdigit);
    }

public:
    Session() {}

    // Create a new bank
    std::shared_ptr<Bank> createBank(const std::string& bankName) {
        auto bank = std::make_shared<Bank>(bankName);
        banks.push_back(bank);
        return bank;
    }

    // Create a new ATM
    std::shared_ptr<ATM> createATM() {
        std::string serialNumber;
        std::string primaryBank;
        ATMType type;
        Language lang;

        // Get ATM serial number
        while (true) {
            std::cout << "Enter 6-digit ATM serial number: ";
            std::cin >> serialNumber;
            if (isValidSerialNumber(serialNumber)) break;
            std::cout << "Invalid serial number. Please enter 6 digits.\n";
        }

        // Get ATM type
        std::cout << "Select ATM type (1: Single Bank, 2: Multi-Bank): ";
        int typeChoice;
        std::cin >> typeChoice;
        type = (typeChoice == 1) ? ATMType::SINGLE_BANK : ATMType::MULTI_BANK;

        // Get language support
        std::cout << "Select language support (1: English, 2: Korean, 3: Bilingual): ";
        int langChoice;
        std::cin >> langChoice;
        switch (langChoice) {
            case 1: lang = Language::ENGLISH; break;
            case 2: lang = Language::KOREAN; break;
            default: lang = Language::BILINGUAL;
        }

        // Get primary bank
        std::cout << "Enter primary bank name: ";
        std::cin >> primaryBank;

        // Create and store ATM
        auto atm = std::make_shared<ATM>(serialNumber, type, lang, primaryBank);
        atms.push_back(atm);

        // Initialize cash in ATM
        std::cout << "Initialize cash in ATM:\n";
        int count;
        std::cout << "Enter number of 1,000 won bills: ";
        std::cin >> count;
        atm->addCash(CashType::KRW_1000, count);

        std::cout << "Enter number of 5,000 won bills: ";
        std::cin >> count;
        atm->addCash(CashType::KRW_5000, count);

        std::cout << "Enter number of 10,000 won bills: ";
        std::cin >> count;
        atm->addCash(CashType::KRW_10000, count);

        std::cout << "Enter number of 50,000 won bills: ";
        std::cin >> count;
        atm->addCash(CashType::KRW_50000, count);

        return atm;
    }

    // Display current system snapshot (REQ10.1)
    void displaySystemSnapshot() const {
        // Display ATMs information
        std::cout << "\n=== ATMs Status ===\n";
        for (const auto& atm : atms) {
            std::cout << "ATM [SN: " << atm->getSerialNumber() << "] remaining cash: {";
            auto cashStatus = atm->getCashStatus();
            std::cout << "KRW 50000: " << cashStatus[CashType::KRW_50000] << ", ";
            std::cout << "KRW 10000: " << cashStatus[CashType::KRW_10000] << ", ";
            std::cout << "KRW 5000: " << cashStatus[CashType::KRW_5000] << ", ";
            std::cout << "KRW 1000: " << cashStatus[CashType::KRW_1000] << "}\n";
        }

        // Display Accounts information
        std::cout << "\n=== Accounts Status ===\n";
        for (const auto& bank : banks) {
            for (const auto& account : bank->accounts) {
                std::cout << "Account [Bank: " << account->getBankName() 
                         << ", No: " << account->getAccountNumber() 
                         << ", Owner: " << account->getOwnerName() 
                         << "] balance: " << account->getBalance() << "\n";
            }
        }
    }

    // Handle system setup menu
    void setupSystem() {
        while (true) {
            std::cout << "\n=== System Setup Menu ===\n";
            std::cout << "1. Create Bank\n";
            std::cout << "2. Create ATM\n";
            std::cout << "3. Create Account\n";
            std::cout << "4. Display System Status\n";
            std::cout << "5. Exit Setup\n";
            std::cout << "Enter choice: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1: {
                    std::string bankName;
                    std::cout << "Enter bank name: ";
                    std::cin >> bankName;
                    createBank(bankName);
                    std::cout << "Bank created successfully!\n";
                    break;
                }
                case 2: {
                    createATM();
                    std::cout << "ATM created successfully!\n";
                    break;
                }
                case 3: {
                    if (banks.empty()) {
                        std::cout << "Please create a bank first!\n";
                        break;
                    }

                    std::string bankName, accountNumber, ownerName;
                    double initialBalance;

                    std::cout << "Enter bank name: ";
                    std::cin >> bankName;

                    auto bankIt = std::find_if(banks.begin(), banks.end(),
                        [&bankName](const auto& bank) { return bank->getName() == bankName; });

                    if (bankIt == banks.end()) {
                        std::cout << "Bank not found!\n";
                        break;
                    }

                    while (true) {
                        std::cout << "Enter 12-digit account number: ";
                        std::cin >> accountNumber;
                        if (isValidAccountNumber(accountNumber)) break;
                        std::cout << "Invalid account number. Please enter 12 digits.\n";
                    }

                    std::cout << "Enter owner name: ";
                    std::cin >> ownerName;

                    std::cout << "Enter initial balance: ";
                    std::cin >> initialBalance;

                    (*bankIt)->createAccount(accountNumber, ownerName, initialBalance);
                    std::cout << "Account created successfully!\n";
                    break;
                }
                case 4: {
                    displaySystemSnapshot();
                    break;
                }
                case 5: {
                    return;
                }
                default: {
                    std::cout << "Invalid choice. Please try again.\n";
                }
            }
        }
    }
};

int main() {
    Session session;
    session.setupSystem();
    return 0;
}