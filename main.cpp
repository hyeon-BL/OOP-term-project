#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "atm.h"
#include "bank.h"
#include "account.h"

int main() {
    try {
        std::cout << "=== ATM System Test Cases ===" << std::endl;
        
        // Create a primary bank
        PrimaryBank* bank = new PrimaryBank("TestBank");
        
        // Create ATM instance
        ATM* atm = new ATM(315785, ATMType::SingleBank, true, bank);
        
        // Create test account
        Account* testAccount = bank->createAccount("John Doe", 12345, 10000.0);
        
        // Test ATM operations
        std::cout << "=== ATM System Test ===" << std::endl;
        
        // Insert card and start session
        if (atm->insertCard(testAccount)) {
            std::cout << "Card inserted successfully" << std::endl;
            
            // Perform transactions
            if (atm->deposit(5000)) {
                std::cout << "Deposit successful" << std::endl;
            }
            
            if (atm->withdraw(2000)) {
                std::cout << "Withdrawal successful" << std::endl;
            }
            
            // End session
            atm->endSession();
        }
        
        // Cleanup
        delete atm;
        delete bank;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error occurred during testing: " << e.what() << std::endl;
        return 1;
    }
}