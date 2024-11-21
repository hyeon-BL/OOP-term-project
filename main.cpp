#include "atm.h"
#include "bank.h"
#include "account.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Create banks
    Bank wooriBank("Woori Bank");
    Bank shinhanBank("Shinhan Bank");
    vector<Bank*> banks = {&wooriBank, &shinhanBank};
    // Create ATMs
    ATM singleBankATM(1001, ATMType::SingleBank, false, &wooriBank, banks);  // Only accepts Woori Bank cards
    ATM multiBankATM(1002, ATMType::MultiBank, true, &wooriBank, banks);     // Accepts all bank cards
    
    // Create accounts in different banks
    Account* wooriAcc1 = wooriBank.createAccount("John Doe", 1001, 1000000);
    Account* wooriAcc2 = wooriBank.createAccount("Jane Smith", 1002, 500000);
    Account* shinhanAcc = shinhanBank.createAccount("Bob Johnson", 2001, 750000);

    std::cout << "=== Testing Single Bank ATM ===" << std::endl;
    
    // Try Woori Bank card in single bank ATM (should succeed)
    if (singleBankATM.insertCard(wooriAcc1)) {
        std::cout << "Successfully inserted Woori Bank card into single bank ATM" << std::endl;
        singleBankATM.endSession();
    }
    
    // Try Shinhan Bank card in single bank ATM (should fail)
    if (!singleBankATM.insertCard(shinhanAcc)) {
        std::cout << "Correctly rejected Shinhan Bank card in single bank ATM" << std::endl;
    }

    std::cout << "\n=== Testing Multi Bank ATM ===" << std::endl;
    
    // Try Woori Bank card in multi bank ATM (should succeed)
    if (multiBankATM.insertCard(wooriAcc1)) {
        std::cout << "Successfully inserted Woori Bank card into multi bank ATM" << std::endl;
        multiBankATM.endSession();
    }
    
    // Try Shinhan Bank card in multi bank ATM (should succeed)
    if (multiBankATM.insertCard(shinhanAcc)) {
        std::cout << "Successfully inserted Shinhan Bank card into multi bank ATM" << std::endl;
        multiBankATM.endSession();
    }

    return 0;
}
