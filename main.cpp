#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "atm.h"
#include "bank.h"
#include "account.h"
#include <functional>

void runTest(const std::string& testName, std::function<void()> testCase) {
    std::cout << "\n=== Running Test: " << testName << " ===" << std::endl;
    try {
        testCase();
        std::cout << "Test completed successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Test failed: " << e.what() << std::endl;
    }
}


//bank, account 연결 확인용
//수수료 정의해둔거 있으니까 필요하면 말하셈. atm 완성되면 초기 atm에 지폐 넣어두는것도 있긴함.
int main() {
    // 은행 생성
    Bank myBank("DaeguBank");

    // 계좌 생성 
    myBank.createAccount("Alice", 1001, 1234, 0); // 사용자 이름, 계좌번호, 비밀번호, 계좌금액
    myBank.createAccount("Bob", 1002, 5678, 0);

    // 초기 계좌 정보 출력
    cout << "Initial Account Information:" << endl;
    myBank.printbankaccount();

    return 0;
}


int main() {
    // Setup test environment
    PrimaryBank* primaryBank = new PrimaryBank("TestBank");
    NonPrimaryBank* otherBank = new NonPrimaryBank("OtherBank");
    ATM* atm = new ATM(1234, ATMType::SingleBank, true, primaryBank);
    
    // Create test accounts
    Account* primaryAccount = primaryBank->createAccount("John Doe", 12345, 100000.0);
    Account* otherAccount = otherBank->createAccount("Jane Doe", 67890, 100000.0);

    // Test cases
    runTest("Card Insertion - Primary Bank", [&]() {
        if (!atm->insertCard(primaryAccount)) {
            throw std::runtime_error("Failed to insert valid primary bank card");
        }
        atm->endSession();
    });

    runTest("Card Insertion - Other Bank", [&]() {
        if (atm->insertCard(otherAccount)) {
            throw std::runtime_error("Accepted card from non-primary bank in SingleBank ATM");
        }
    });

    runTest("Cash Deposit Test", [&]() {
        atm->insertCard(primaryAccount);
        std::cout << "Testing cash deposit - Follow the prompts:" << std::endl;
        std::cout << "1. Enter 'money' when asked for deposit type" << std::endl;
        std::cout << "2. Enter some bills (e.g., 1 50000 bill, 2 10000 bills)" << std::endl;
        atm->deposit(0); // Amount will be calculated from user input
        atm->endSession();
    });

    runTest("Check Deposit Test", [&]() {
        atm->insertCard(primaryAccount);
        std::cout << "Testing check deposit - Follow the prompts:" << std::endl;
        std::cout << "1. Enter 'check' when asked for deposit type" << std::endl;
        std::cout << "2. Enter number of checks (e.g., 2)" << std::endl;
        std::cout << "3. Enter amounts for each check" << std::endl;
        atm->deposit(0); // Amount will be calculated from user input
        atm->endSession();
    });

    runTest("Withdrawal Test Series", [&]() {
        atm->insertCard(primaryAccount);
        
        // Test 1: Valid withdrawal
        std::cout << "Testing valid withdrawal (10000 won)..." << std::endl;
        if (!atm->withdraw(10000)) {
            throw std::runtime_error("Failed to withdraw valid amount");
        }

        // Test 2: Invalid amount (not multiple of 1000)
        std::cout << "Testing invalid withdrawal amount (1500 won)..." << std::endl;
        if (atm->withdraw(1500)) {
            throw std::runtime_error("Accepted invalid withdrawal amount");
        }

        // Test 3: Withdrawal limit test
        std::cout << "Testing withdrawal limit..." << std::endl;
        atm->withdraw(10000);
        atm->withdraw(10000);
        atm->withdraw(10000); // This should be rejected (4th attempt)
        
        atm->endSession();
    });

    runTest("Session Management Test", [&]() {
        // Test double session start
        if (!atm->insertCard(primaryAccount)) {
            throw std::runtime_error("Failed to start first session");
        }
        if (atm->insertCard(primaryAccount)) {
            throw std::runtime_error("Allowed second session while first was active");
        }
        atm->endSession();
    });

    // Cleanup
    delete atm;
    delete primaryBank;
    delete otherBank;

    std::cout << "\nAll tests completed." << std::endl;
    return 0;
}
