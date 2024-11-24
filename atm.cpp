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
        do {
            try {
                cin >> choice;
            } catch (std::exception& e) {
                cout << "Invalid input. Please enter numbers only.\n";
                cin.clear();
                break;
            }
        } while (choice < 1 || choice > 5);

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
    else { // SingleBank ATM only supports primary bank
        supportedBanks.push_back(primaryBank);
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

void ATM::atmstart() {
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
        return;
    }

    if (isAdminCard) {
        if (currentLanguage == Language::English) {
            cout << "Administrator card detected." << endl;
        }
        else {
            cout << "관리자 카드가 감지되었습니다." << endl;
        }
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
        try {
            cin >> password_in;
        } catch (std::exception& e) {
            if (currentLanguage == Language::English) {
                cout << "Invalid input. Please enter numbers only." << endl;
            }
            else {
                cout << "잘못된 입력입니다. 숫자만 입력하세요." << endl;
            }
            cin.clear();
            continue;
        }

        if (currentSession->getActiveAccount()->verifyPassword(password_in)) {
            if (currentLanguage == Language::English) {
                cout << "Password correct. Welcome!" << endl;
                break;
            }
            else {
                cout << "비밀번호가 확인되었습니다. 환영합니다!" << endl;
                break;
            }
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

void ATM::deposit() {
    if (!currentSession) {
        std::cout << "No active session." << std::endl;
        return;
    }
    Account* account = currentSession->getActiveAccount();
    if (!account) {
        std::cout << "No card inserted." << std::endl;
        return;
    }

    std::string depositType;
    std::cout << (currentLanguage == Language::English ? 
                  "Deposit type (money/check): " : 
                  "입금 타입 (money/check): ");
    std::cin >> depositType;

    double totalAmount = 0.0;
    if (depositType == "money") {
        int denominations[4];
        const int denominationValues[4] = { 50000, 10000, 5000, 1000 };

        for (int i = 0; i < 4; i++) {
            std::cout << denominationValues[i] << (currentLanguage == Language::English ? 
                                                 " won bills: " : 
                                                 " 원권을 입력하시오: ");
            std::cin >> denominations[i];
        }

        int totalBills = 0;
        for (int i = 0; i < 4; i++) {
            totalBills += denominations[i];
            totalAmount += denominations[i] * denominationValues[i];
        }

        if (totalBills > MAX_CASH_INPUT) {
            std::cout << (currentLanguage == Language::English ? 
                         "Too many bills, maximum: " : 
                         "현금이 너무 많음, 최대: ") << MAX_CASH_INPUT << std::endl;
            return;
        }
    }
    else if (depositType == "check") {
        int checkCount;
        std::cout << (currentLanguage == Language::English ? 
                     "Enter number of checks: " : 
                     "입력할 수표의 장수를 입력: ");
        std::cin >> checkCount;

        if (checkCount > MAX_CHECK_COUNT) {
            std::cout << (currentLanguage == Language::English ? 
                         "Too many checks, maximum: " : 
                         "수표가 너무 많음, 최대: ") << MAX_CHECK_COUNT << std::endl;
            return;
        }

        for (int i = 0; i < checkCount; i++) {
            double checkAmount;
            std::cout << (currentLanguage == Language::English ? 
                         "Enter amount for check " : 
                         "수표 금액을 입력: ") << (i + 1) << ": ";
            std::cin >> checkAmount;
            totalAmount += checkAmount;
        }
    }
    else {
        std::cout << (currentLanguage == Language::English ? 
                     "Invalid deposit type." : 
                     "입금이 불가능한 타입입니다.") << std::endl;
        return;
    }

    int fee = account->getBankName() == primaryBank->getbankname() ?
        primaryBank->getdfee() : primaryBank->getdfee() * 2;

    account->deposit(totalAmount - fee);
    atmCashBalance += totalAmount;
    Transaction trans("Deposit", totalAmount, account);
    currentSession->addTransaction(trans);
}

void ATM::withdraw(double amount) {
    if (!currentSession) {
        std::cout << (currentLanguage == Language::English ? 
                     "No active session." : 
                     "활성화된 세션이 없습니다.") << std::endl;
        return;
    }

    if (withdrawalCount >= MAX_WITHDRAWAL_COUNT) {
        std::cout << (currentLanguage == Language::English ? 
                     "Maximum withdrawal count reached." : 
                     "출금 한도에 도달했습니다.") << std::endl;
        return;
    }

    if (amount > MAX_WITHDRAWAL_AMOUNT) {
        std::cout << (currentLanguage == Language::English ? 
                     "Amount exceeds maximum withdrawal limit of " : 
                     "출금 한도 초과: ") << MAX_WITHDRAWAL_AMOUNT << std::endl;
        return;
    }

    Account* account = currentSession->getActiveAccount();
    if (!account) return;

    int fee = account->getBankName() == primaryBank->getbankname() ?
        primaryBank->getwfee() : primaryBank->getwfee() * 2;

    if (atmCashBalance < amount) {
        std::cout << (currentLanguage == Language::English ? 
                     "ATM has insufficient funds." : 
                     "ATM 잔액이 부족합니다.") << std::endl;
        return;
    }

    if (account->withdraw(amount + fee)) {
        atmCashBalance -= amount;
        withdrawalCount++;
        Transaction trans("Withdrawal", amount, account);
        currentSession->addTransaction(trans);
        std::cout << (currentLanguage == Language::English ? 
                     "Withdrawal successful" : 
                     "출금이 성공했습니다") << std::endl;
        return;
    }
    
    std::cout << (currentLanguage == Language::English ? 
                 "Withdrawal failed - insufficient funds" : 
                 "출금 실패 - 잔액 부족\n");
}

bool ATM::transfer(Account* target, double amount) {
    if (!currentSession || !target) return false;
    
    Account* source = currentSession->getActiveAccount();
    if (!source) return false;

    int fee = primaryBank->getafee();
    // Calculate fee based on bank relationships
    if (source->getBankName() != primaryBank->getbankname() && 
        target->getBankName() != primaryBank->getbankname()) {
        fee *= 2;
    } else if (source->getBankName() != primaryBank->getbankname() || 
               target->getBankName() != primaryBank->getbankname()) {
        fee = static_cast<int>(fee * 1.5);
    }

    if (source->withdraw(amount + fee)) {
        target->deposit(amount);
        Transaction trans("Transfer", amount, source);
        currentSession->addTransaction(trans);
        return true;
    }

    return false;
}
