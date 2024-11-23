#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
// #include "atm.h"
#include "bank.h" // Ensure this header file defines the Bank class
#include "account.h"
#include <functional>



//bank, account 연결 확인용
//수수료 정의해둔거 있으니까 필요하면 말하셈. atm 완성되면 초기 atm에 지폐 넣어두는것도 있긴함.
int main() {
    // Create a bank
    Bank myBank("DaeguBank");
    myBank.setisprimarybank(true); // Set as primary bank for lower fees

    cout << "=== Creating Accounts ===" << endl;
    // Create multiple accounts with initial balances
    myBank.createAccount("Alice", 1001, 1234, 5000);
    myBank.createAccount("Bob", 1002, 5678, 3000);
    
    cout << "\n=== Initial Account Information ===" << endl;
    myBank.printbankaccount();

    cout << "\n=== Testing Account Operations ===" << endl;
    // Get account references
    Account* aliceAccount = myBank.getAccount(1001);
    Account* bobAccount = myBank.getAccount(1002);

    if (aliceAccount && bobAccount) {
        // Test password verification
        cout << "Password verification for Alice (1234): " 
             << (aliceAccount->verifyPassword(1234) ? "Success" : "Failed") << endl;
        
        // Test deposit
        cout << "\nAlice deposits 2000" << endl;
        aliceAccount->deposit(2000);
        cout << "Alice's new balance: " << aliceAccount->getBalance() << endl;

        // Test withdrawal
        cout << "\nBob withdraws 1000" << endl;
        if (bobAccount->withdraw(1000)) {
            cout << "Withdrawal successful" << endl;
            cout << "Bob's new balance: " << bobAccount->getBalance() << endl;
        } else {
            cout << "Withdrawal failed" << endl;
        }

        // Display final account states
        cout << "\n=== Final Account Information ===" << endl;
        myBank.printbankaccount();
        
        // Display transaction history
        cout << "\n=== Alice's Transaction History ===" << endl;
        vector<string> aliceHistory = aliceAccount->getTransactionHistory();
        for (const string& transaction : aliceHistory) {
            cout << transaction << endl;
        }
    }

    return 0;
}