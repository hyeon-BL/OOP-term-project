#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "atm.h"
#include "bank.h" // Ensure this header file defines the Bank class
#include "account.h"
#include <functional>

int main() {
    // 은행 생성
    Bank myBank("DaeguBank");

    // 다른 은행 생성 (MultiBank ATM용)
    Bank secondBank("SeoulBank");

    // 계좌 생성
    myBank.createAccount("Alice", 1001, 1234, 0); // 사용자 이름, 계좌번호, 비밀번호, 계좌금액
    myBank.createAccount("Bob", 1002, 5678, 0);

    // 관리자 계좌 생성
    admin* adminAccount = new admin(&myBank, "AdminUser", 9999, 4321);

    // ATM 생성
    std::vector<Bank*> supportedBanks = { &myBank, &secondBank };
    ATM atm1(101, ATMType::MultiBank, true, &myBank, supportedBanks);

    // ATM 동작 테스트
    atm1.atmstart();


    // 동적 메모리 해제
    delete adminAccount;

    return 0;
}
// int main() {
//     // Create a bank
//     Bank myBank("DaeguBank");
//     myBank.setisprimarybank(true);

//     cout << "=== Creating Regular Accounts ===" << endl;
//     myBank.createAccount("Alice", 1001, 1234, 5000);
//     myBank.createAccount("Bob", 1002, 5678, 3000);

//     cout << "\n=== Creating Admin Account ===" << endl;
//     admin* bankAdmin = new admin(&myBank, "Admin1", 9999, 0000);
    
//     cout << "\n=== Testing Admin Status ===" << endl;
//     cout << "Is Admin1 an admin? " << (bankAdmin->isAdmincheck() ? "Yes" : "No") << endl;
//     cout << "Is Alice an admin? " << (myBank.getAccount(1001)->isAdmincheck() ? "Yes" : "No") << endl;

//     cout << "\n=== Regular Account Operations ===" << endl;
//     Account* aliceAccount = myBank.getAccount(1001);
//     if (aliceAccount) {
//         aliceAccount->deposit(2000);
//         cout << "Alice's balance after deposit: " << aliceAccount->getBalance() << endl;
//     }

//     cout << "\n=== Admin Account Operations ===" << endl;
//     if (bankAdmin) {
//         // Admin specific operations could be added here
//         cout << "Admin account number: " << bankAdmin->getAccNumber() << endl;
//         cout << "Admin verification test: " << bankAdmin->verifyPassword(0000) << endl;
//     }

//     cout << "\n=== Final Account Information ===" << endl;
//     myBank.printbankaccount();

//     // Cleanup
//     delete bankAdmin;
//     return 0;
// }


//bank, account 연결 확인용
//수수료 정의해둔거 있으니까 필요하면 말하셈. atm 완성되면 초기 atm에 지폐 넣어두는것도 있긴함.
// int main() {
//     // Create a bank
//     Bank myBank("DaeguBank");
//     myBank.setisprimarybank(true); // Set as primary bank for lower fees

//     cout << "=== Creating Accounts ===" << endl;
//     // Create multiple accounts with initial balances
//     myBank.createAccount("Alice", 1001, 1234, 5000);
//     myBank.createAccount("Bob", 1002, 5678, 3000);
    
//     cout << "\n=== Initial Account Information ===" << endl;
//     myBank.printbankaccount();

//     cout << "\n=== Testing Account Operations ===" << endl;
//     // Get account references
//     Account* aliceAccount = myBank.getAccount(1001);
//     Account* bobAccount = myBank.getAccount(1002);

//     if (aliceAccount && bobAccount) {
//         // Test password verification
//         cout << "Password verification for Alice (1234): " 
//              << (aliceAccount->verifyPassword(1234) ? "Success" : "Failed") << endl;
        
//         // Test deposit
//         cout << "\nAlice deposits 2000" << endl;
//         aliceAccount->deposit(2000);
//         cout << "Alice's new balance: " << aliceAccount->getBalance() << endl;

//         // Test withdrawal
//         cout << "\nBob withdraws 1000" << endl;
//         if (bobAccount->withdraw(1000)) {
//             cout << "Withdrawal successful" << endl;
//             cout << "Bob's new balance: " << bobAccount->getBalance() << endl;
//         } else {
//             cout << "Withdrawal failed" << endl;
//         }

//         // Display final account states
//         cout << "\n=== Final Account Information ===" << endl;
//         myBank.printbankaccount();
        
//         // Display transaction history
//         cout << "\n=== Alice's Transaction History ===" << endl;
//         vector<string> aliceHistory = aliceAccount->getTransactionHistory();
//         for (const string& transaction : aliceHistory) {
//             cout << transaction << endl;
//         }
//     }

//     return 0;
// }