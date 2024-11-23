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