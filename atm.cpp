#include "atm.h"
#include <ctime>
#include <iostream>

using namespace std;
const double ATM::MAX_WITHDRAWAL_AMOUNT = 500000.0;
int Transaction::nextTransactionId = 1;



Transaction::Transaction(std::string t, double a, const Account* acc) 
    : type(t), amount(a) {
    transactionId = nextTransactionId++;
    time_t now = time(0);
    timestamp = ctime(&now);
    if (acc) {
        accountInfo = "Account: " + to_string(acc->getAccNumber()) + 
                     " Bank: " + acc->getBankName();
    }
}


Session::Session() 
    : activeAccount(nullptr), state(SessionState::Idle), hasTransactions(false) {}

void Session::addTransaction(const Transaction& trans) {
    transactions.push_back(trans);
}

void Session::setActiveAccount(Account* account) {
    activeAccount = account;
    state = SessionState::Active;
}

bool Session::processUserChoice(ATM* atm) {
    while (state == SessionState::Active) {
        cout << "\n=== ATM Menu ===\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Transfer\n";
        cout << "4. Check Balance\n";
        cout << "5. End Session\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                atm->deposit();
                this->sethasTransactions(true);
                break;
            case 2: {
                double amount;
                cout << "Enter amount to withdraw: ";
                do {
                    try {
                        cin >> amount;
                    } catch (std::exception& e) {
                        cout << "Invalid input. Please enter numbers only.\n";
                        cin.clear();
                        break;
                    }
                } while (amount <= 0);
                atm->withdraw(amount);
                this->sethasTransactions(true);
                break;
            }
            case 3: {
                int targetAccNum;
                double amount;
                cout << "Enter target account number: ";
                do {
                    try {
                        cin >> targetAccNum;
                    } catch (std::exception& e) {
                        cout << "Invalid input. Please enter numbers only.\n";
                        cin.clear();
                        break;
                    }
                } while (targetAccNum <= 0);

                cout << "Enter amount to transfer: ";
                do {
                    try {
                        cin >> amount;
                    } catch (std::exception& e) {
                        cout << "Invalid input. Please enter numbers only.\n";
                        cin.clear();
                        break;
                    }
                } while (amount <= 0);
                // You'll need to implement finding the target account
                Account* targetAcc = nullptr;  // This should be implemented
                if (targetAcc) {
                    atm->transfer(targetAcc, amount);
                    this->sethasTransactions(true);
                }
                break;
            }
            case 4:
                cout << "Current balance: " << activeAccount->getBalance() << endl;
                break;
            case 5:
                return false;  // End session
            default:
                cout << "Invalid option\n";
        }
    }
    return true;
}

void Session::printSummary() const {
    if (!hasTransactions || transactions.empty()) {
        cout << "No transactions were performed during this session.\n";
        return;
    }
    
    cout << "\n=== Session Summary ===\n";
    cout << "Account: " << activeAccount->getAccNumber() << "\n";
    cout << "Bank: " << activeAccount->getBankName() << "\n\n";
    
    for (const auto& trans : transactions) {
        cout << "Transaction ID: " << trans.transactionId << "\n";
        cout << "Type: " << trans.type << "\n";
        cout << "Amount: " << trans.amount << "\n";
        cout << "Time: " << trans.timestamp;
        cout << "------------------------\n";
    }
}

ATM::ATM(int serial, ATMType atmType, bool bilingual, Bank* primary, const std::vector<Bank*>& banks) // ATM(시리얼, ATM타입, 다국어지원여부, 기본은행, 은행목록)
    : serialNumber(serial), type(atmType), isBilingual(bilingual), 
      primaryBank(primary), currentSession(nullptr), 
      currentLanguage(Language::English) {
    
    primaryBank->setisprimarybank(true); // Set primary bank flag
    if (type == ATMType::MultiBank) { // MultiBank ATM supports all banks
        for (auto bank : banks) {
            supportedBanks.push_back(bank);
        }
    }
}

ATM::~ATM() {
    delete currentSession;
}

void ATM::setLanguage(Language lang) {
    if (!isBilingual && lang != Language::English) {
        cout<<"This ATM only supports English.";
        return;
    }
    currentLanguage = lang;
}

// void ATM::readCardInfo(int accNum) { //카드 정보 확인
//     bool check = false; // 카드 정보 확인 상태 
//     bool adminCheck = false; //관리자 계좌인지 확인 상태
//     bool valid = false; // Added missing variable declaration
//     Account* usingAccount = nullptr; // Added missing variable declaration
    
//     for (Bank* bank : supportedBanks) { //일반 계좌인지 확인 
//         Account* account = bank->getAccount(accNum);
//         if (account != nullptr && !account->isAdmincheck()) {
//             usingAccount = account;
//             valid = true;
//             check = true;
//             break;
//         }
        
//         if (!valid && (bank->getisprimarybank())) { // 관리자 계좌인지 확인 
//             Account* adminAcc = bank->getAccount(accNum);
//             if (adminAcc != nullptr && adminAcc->isAdmincheck()) {
//                 adminCheck = true;
//                 valid = true;
//                 check = true;
//                 break;
//             }
//         }
        
//         if (check) { //계좌번호 찾으면 종료 
//             break;
//         }
//     }
    
//     if (!valid) { // 계좌를 찾지 못한 경우
//         if (currentLanguage == Language::English) {
//             cout << "Error: No account found, returning card\n" << endl;
//         } else {
//             cout << "오류: 존재하지 않는 계좌입니다. 카드를 반환하겠습니다\n" << endl;
//         }
//         return;
//     }
    
//     if (adminCheck) { // 관리자 계정일경우 
//         if (currentLanguage == Language::English) {
//             cout << "It has been verified that it is an administrator card\n" << endl;
//         } else {
//             cout << "관리자 카드임이 확인되었습니다.\n" << endl;
//         }
//         return;
//     }
// }

int ATM::atmstart() {
    long long accNumber; // Changed to long long to handle 12 digits
    int password_in;
    int lang;
    bool isValidAccount = false;
    bool isAdminCard = false;

    if (this->IsBilingual()) {
        cout << "Choose your language.  언어를 고르세요." << endl;
        cout << "1. English, 2. Korean\n";
        cout << "1. 영어, 2. 한국어\n";
        do {
            try {
                cin >> lang;
            } catch (std::exception& e) {
                cout << "Invalid input. Please enter 1 or 2.\n";
                cin.clear();
                continue;
            }

            if (lang != 1 && lang != 2) {
                cout << "Invalid input. Please enter 1 or 2.\n";
            }
        } while (lang != 1 && lang != 2);
        if (lang == 2) {
            setLanguage(Language::Korean);
        }
        else {
            setLanguage(Language::English);
        }
    }

    // Insert card prompt
    if (currentLanguage == Language::English) {
        cout << "Please insert your card." << endl;
        cout << "Enter your 12-digit account number: ";
    }
    else {
        cout << "카드를 삽입하십시오." << endl;
        cout << "12자리 계좌번호를 입력하십시오: ";
    }
    
    do {
        try {
            cin >> accNumber;
        } catch (std::exception& e) {
            cout << "Invalid input. Please enter numbers only.\n";
            cin.clear();
            continue;
        }
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << (currentLanguage == Language::English ? 
                    "Invalid input. Please enter numbers only.\n" : 
                    "잘못된 입력입니다. 숫자만 입력하세요.\n");
            continue;
        }
        
        if (accNumber < 100000000000LL || accNumber > 999999999999LL) {
            cout << (currentLanguage == Language::English ? 
                    "Account number must be 12 digits.\n" : 
                    "계좌번호는 12자리여야 합니다.\n");
            continue;
        }
        
        break;
    } while (true);

    // Validate account
    for (Bank* bank : supportedBanks) {
        Account* account = bank->getAccount(accNumber);
        if (account != nullptr) {
            isValidAccount = true;
            if (account->isAdmincheck()) {
                isAdminCard = true;
            }
            if (validateCard(account)) {
                insertCard(account);
                break;
            }
            else {
                if (currentLanguage == Language::English) {
                    cout << "This card is not supported by this ATM." << endl;
                }
                else {
                    cout << "이 카드는 이 ATM에서 지원되지 않습니다." << endl;
                }
                return -1;
            }
        }
    }

    if (!isValidAccount) {
        if (currentLanguage == Language::English) {
            cout << "Error: No account found. Returning your card." << endl;
        }
        else {
            cout << "오류: 계좌를 찾을 수 없습니다. 카드를 반환합니다." << endl;
        }
        return -1;
    }

    if (isAdminCard) {
        if (currentLanguage == Language::English) {
            cout << "Administrator card detected." << endl;
        }
        else {
            cout << "관리자 카드가 감지되었습니다." << endl;
        }
        return 0; // Admin card does not proceed to user session
    }

    // Initialize session
    Account* account = nullptr;
    for (Bank* bank : supportedBanks) {
        account = bank->getAccount(accNumber);
        if (account != nullptr) {
            break;
        }
    }
    currentSession = new Session();
    currentSession->setActiveAccount(account);

    // Password verification
    int password_attempts = 0;
    const int max_attempts = 3;

    while (password_attempts < max_attempts) {
        if (currentLanguage == Language::English) {
            cout << "Enter your password: ";
        }
        else {
            cout << "비밀번호를 입력하십시오: ";
        }
        cin >> password_in;

        if (currentSession->getActiveAccount()->verifyPassword(password_in)) {
            if (currentLanguage == Language::English) {
                cout << "Password correct. Welcome!" << endl;
            }
            else {
                cout << "비밀번호가 확인되었습니다. 환영합니다!" << endl;
            }
            return 1; // Proceed to user session
        }
        else {
            password_attempts++;
            if (currentLanguage == Language::English) {
                cout << "Incorrect password. Attempts remaining: "
                    << (max_attempts - password_attempts) << endl;
            }
            else {
                cout << "비밀번호가 틀렸습니다. 남은 시도 횟수: "
                    << (max_attempts - password_attempts) << endl;
            }
        }
    }

    // Lock account after too many attempts
    if (password_attempts >= max_attempts) {
        if (currentLanguage == Language::English) {
            cout << "Too many incorrect password attempts. Returning your card." << endl;
        }
        else {
            cout << "비밀번호 입력 시도가 초과되었습니다. 카드를 반환합니다." << endl;
        }
        return -1;
    }

    // After successful authentication:
    while (currentSession != nullptr) {
        if (!currentSession->processUserChoice(this)) {
            if (currentSession->hasCompletedTransactions()) {
                currentSession->printSummary();
            }
            endSession();
            break;
        }
    }

    return 1;
}

bool ATM::insertCard(Account* account) {
    if (!validateCard(account)) {
        cout << "Invalid card for this ATM.";
        return false;
    }
    if (currentSession != nullptr) {
        cout << "Session already in progress.";
        return false;
    }
    currentSession = new Session();
    currentSession->setActiveAccount(account);
    cout<<"Card accepted. Welcome!";
    return true;
}


bool ATM::endSession() {
    if (currentSession == nullptr) return false;

    currentSession->printSummary();
    delete currentSession;
    currentSession = nullptr;
    cout<<"Thank you for using our ATM.";
    return true;
}


bool ATM::validateCard(Account* account) const {
    if (type == ATMType::SingleBank) {
        return account->getBankName() == primaryBank->getbankname();
    }
    return true; // MultiBank ATM accepts all cards
}




// // Update transaction operations
// void ATM::deposit() {//언어 두개
//     if (!currentSession) {
//         std::cout << "No active session." << std::endl;
//     }
//     Account* account = currentSession->getActiveAccount();
//     if (!account) {
//         std::cout << "No card inserted." << std::endl;
//     }

//     std::string depositType;
//     std::cout << "인출 타입 (money/check): ";
//     std::cin >> depositType;

//     double totalAmount = 0.0;
//     if (depositType == "money") {
//         int denominations[4];
//         const int denominationValues[4] = { 50000, 10000, 5000, 1000 };

//         for (int i = 0; i < 4; i++) {
//             std::cout << denominationValues[i] << " 원권을 입력하시오: ";
//             std::cin >> denominations[i];
//         }

//         int totalBills = 0;
//         for (int i = 0; i < 4; i++) {
//             totalBills += denominations[i];
//             totalAmount += denominations[i] * denominationValues[i];
//         }

//         if (totalBills > MAX_CASH_INPUT) {
//             std::cout << "현금이 너무 많음, 최대: " << MAX_CASH_INPUT << std::endl;
//         }
//     }
//     else if (depositType == "check") {
//         int checkCount;
//         std::cout << "입력할 수표의 장수를 입력: ";
//         std::cin >> checkCount;

//         if (checkCount > MAX_CHECK_COUNT) {
//             std::cout << "수표가 너무 많음, 최대: " << MAX_CHECK_COUNT << std::endl;
//         }

//         for (int i = 0; i < checkCount; i++) {
//             double checkAmount;
//             std::cout << (i + 1) << "번째 수표 금액을 입력: ";
//             std::cin >> checkAmount;
//             totalAmount += checkAmount;
//         }
//     }
//     else {
//         std::cout << "입금이 불가능한 타입입니다." << std::endl;
//     }

//     int fee = account->getBankName() == primaryBank->getbankname() ?
//         primaryBank->getdfee() : primaryBank->getdfee() * 2;

//     std::cout << "입금 수수료: " << fee << std::endl;
//     int additionalfee=0;
//     int totalfee = 0;
//     while (totalfee < fee) { // 수수료가 충분하지 않은 경우(수수료를 넘는 경우 고려x)
//         cout << "수수료를 넣어주세요." << totalfee << "/" << fee<<"\n";
//         cin >> additionalfee;
//         totalfee += additionalfee;
//     }
//     totalAmount += totalfee;

//     account->deposit(totalAmount - fee);
//     atmCashBalance += totalAmount;
//     Transaction trans("Deposit", totalAmount);
//     currentSession->addTransaction(trans);//?

// }

// void ATM::withdraw(double amount) {
//     if (!currentSession) {
//         std::cout << "No active session." << std::endl;
//         return;
//     }

//     if (withdrawalCount >= MAX_WITHDRAWAL_COUNT) {
//         std::cout << "Maximum withdrawal count reached. Please end session and start again." << std::endl;
//         return;
//     }

//     if (amount > MAX_WITHDRAWAL_AMOUNT) {
//         std::cout << "Amount exceeds maximum withdrawal limit of " << MAX_WITHDRAWAL_AMOUNT << std::endl;
//         return;
//     }

//     Account* account = currentSession->getActiveAccount();
//     if (!account) return;

//     int fee = account->getBankName() == primaryBank->getbankname() ?
//         primaryBank->getwfee() : primaryBank->getwfee() * 2;

//     if (atmCashBalance < (amount + fee)) {
//         std::cout << "ATM has insufficient funds." << std::endl;
//         return;
//     }

//     if (account->withdraw(amount + fee)) {
//         atmCashBalance -= amount;
//         withdrawalCount++;
//         Transaction trans("Withdrawal", amount);
//         currentSession->addTransaction(trans);
//         std::cout << "Withdrawal successful" << std::endl;
//         return;
//     }
//     std::cout << "Withdrawal failed - insufficient funds in account" << std::endl;
//     return;
// }

// bool ATM::transfer(Account* target, double amount) {
//     if (!currentSession) {
//         cout << "세션이 활성화되지 않았습니다." << endl;
//         return false;
//     }

//     cout << "계좌 송금 유형을 고르시오\n";
//     cout << "1. 계좌 송금, 2. 현금 송금\n";
//     int transfer_type = 0;
//     cin >> transfer_type;
//     int transfer_count = 0;
//     int transfer_to_account;
//     Account* transferAccount = nullptr;
//     Bank* transferBank = nullptr;
//     double fee = 0;
//     double total = 0;

//     // 은행 선택
//     cout << "송금할 계좌의 은행을 선택하세요:" << endl;
//     for (size_t i = 0; i < supportedBanks.size(); i++) {
//         cout << i + 1 << ". " << supportedBanks[i]->getbankname() << endl;
//     }
//     int bankChoice;
//     cin >> bankChoice;
    
//     if (bankChoice < 1 || bankChoice > supportedBanks.size()) {
//         cout << "잘못된 은행 선택입니다." << endl;
//         return false;
//     }
//     transferBank = supportedBanks[bankChoice - 1];

//     // 계좌 번호 입력 및 확인
//     cout << "송금할 계좌번호를 입력하시오\n";
//     cin >> transfer_to_account;
    
//     transferAccount = transferBank->getAccount(transfer_to_account);
//     if (!transferAccount) {
//         cout << "해당 계좌를 찾�� 수 없습니다." << endl;
//         return false;
//     }

//     if (transfer_type == 2) {  // 현금 송금
//         int denominations[4] = {0};
//         const int denominationValues[4] = {50000, 10000, 5000, 1000};
//         double totalAmount = 0;
//         int totalBills = 0;
        
//         for (int i = 0; i < 4; i++) {
//             cout << denominationValues[i] << " 원권을 입력하시오: ";
//             cin >> denominations[i];
//             totalBills += denominations[i];
//             totalAmount += denominations[i] * denominationValues[i];
//         }

//         if (totalBills > MAX_CASH_INPUT) {
//             cout << "현금이 너무 많음, 최대: " << MAX_CASH_INPUT << endl;
//             return false;
//         }

//         // 수수료 계산
//         if (transferBank->getisprimarybank() && primaryBank->getisprimarybank()) {
//             fee = primaryBank->getafee();
//         } else if (!transferBank->getisprimarybank() && !primaryBank->getisprimarybank()) {
//             fee = primaryBank->getafee() * 2;
//         } else {
//             fee = (int)(primaryBank->getafee() * 1.5);
//         }

//         total = totalAmount + fee;
//         cout << "수수료는 " << fee << "원입니다" << endl;
//         cout << "입금액: " << totalAmount << "원 수수료: " << fee 
//              << "원 합계: " << total << "원입니다" << endl;
        
//         if (totalAmount > 0) {
//             transferAccount->deposit(totalAmount);
//             atmCashBalance += totalAmount;
//             Transaction trans("Transfer(Cash)", totalAmount);
//             currentSession->addTransaction(trans);
//         }
//     } else {  // 계좌 이체
//         Account* sourceAccount = currentSession->getActiveAccount();
//         cout << "송금할 금액을 입력하시오\n";
//         double transfer_money;
//         cin >> transfer_money;

//         // 수수료 계산
//         if (transferBank->getisprimarybank() && primaryBank->getisprimarybank()) {
//             fee = primaryBank->getafee();
//         } else if (!transferBank->getisprimarybank() && !primaryBank->getisprimarybank()) {
//             fee = primaryBank->getafee() * 2;
//         } else {
//             fee = (int)(primaryBank->getafee() * 1.5);
//         }

//         if (transfer_money + fee > sourceAccount->getBalance()) {
//             cout << "계좌에 잔액이 부족합니다" << endl;
//             return false;
//         }

//         sourceAccount->withdraw(transfer_money + fee);
//         transferAccount->deposit(transfer_money);
//         Transaction trans("Transfer(Account)", transfer_money);
//         currentSession->addTransaction(trans);
//     }
    
//     return true;
// }
