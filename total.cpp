#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Bank;  // Forward declaration needed

#include <string>
#include <vector>

using namespace std;

class Account {
protected:
    string accName;                   // 사용자 이름
    int accNumber;                    // 계좌 번호
    int password;                     // 계좌 비밀번호
    int accbalance;                   // 계좌 잔액
    Bank* bankName;                  // 은행 이름
    vector<string> transactionHistory; // 거래 내역
    bool isAdmin;                     // 관리자 여부 

public:
    // 생성자와 소멸자
    Account(string name, int number, int password, int initialBalance = 0);
    ~Account();

    // Getter 메서드
    string getAccName() const; // 사용자 이름 확인 
    int getAccNumber() const;   // 계좌번호 확인 
    int getBalance() const;      //계좌 잔액 확인 
    string getBankName() const;   // 계좌 은행 확인 
    vector<string> getTransactionHistory() const; //계좌 기록 확인 
    virtual bool isAdmincheck() const;      //관리자 여부 확인 

    // 계좌 기능
    bool verifyPassword(int inputPassword) const; // 비밀번호 확인
    void deposit(int amount);                     // 입금
    bool withdraw(int amount);                    // 출금
    void addTransaction(const string& transaction); // 거래 내역 추가
    void setBankName(Bank& bank);         // 은행 이름 설정
};

class normalAccount : public Account { // 일반 계좌 정보 
public:
    normalAccount(Bank* bank, string name, int number, int password, int accbalance);
    bool isAdmincheck() const override;
};


class admin : public Account {
public:
    admin(Bank* bank, string name, int number, int password);  // Fix parameter type
    bool isAdmincheck() const; // Add const
};





// 생성자
Account::Account(string name, int number, int password, int initialBalance)
    : accName(name), accNumber(number), password(password), accbalance(initialBalance), bankName(nullptr), isAdmin(false) {
}

// 소멸자
Account::~Account() {}

// Getter 메서드
string Account::getAccName() const {//계좌 이름 알기 
    return accName;
}

int Account::getAccNumber() const {//계좌 번호 알기 
    return accNumber;
}

int Account::getBalance() const {//계좌 잔액 알기 
    return accbalance;
}


vector<string> Account::getTransactionHistory() const {//계좌 히스토리 저장 
    return transactionHistory;
}

bool Account::isAdmincheck() const {
    return isAdmin;
}

bool normalAccount::isAdmincheck() const {
    return false;
}

bool admin::isAdmincheck() const {
    cout << "관리자 카드가 삽입이 되었다\n";
    return true;
}

bool Account::verifyPassword(int inputPassword) const {// 비밀번호 확인 메서드
    return password == inputPassword;
}


void Account::deposit(int amount) {// 입금 메서드
    if (amount > 0) {
        accbalance += amount;
        addTransaction("Deposit: " + to_string(amount));
    }
    else {
        cout << "Invalid deposit amount." << endl;
    }
}

bool Account::withdraw(int amount) {// 출금 메서드
    if (amount > 0 && amount <= accbalance) {
        accbalance -= amount;
        addTransaction("Withdrawal: " + to_string(amount));
        return true;
    }
    else {
        cout << "Invalid withdrawal amount or insufficient funds." << endl;
        return false;
    }
}

// 거래 내역 추가 메서드
void Account::addTransaction(const string& transaction) {
    transactionHistory.push_back(transaction);
}

// 은행 이름 설정 메서드
void Account::setBankName(Bank& bank) {
    this->bankName = &bank;
}


normalAccount::normalAccount(Bank* bank, string name, int number, int password, int accbalance) : Account(name, number, password, accbalance) {
    this->bankName = bank;
    this->isAdmin = false;
}
admin::admin(Bank* bank, string name, int number, int password) : Account(name, number, password) {
    this->bankName = bank;
    this->isAdmin = true;
}




class Bank {
protected:
    std::string bankname; // 은행 이름
    Account* accounts[1000]; // 최대 1000개의 계좌
    int numaccounts; // 현재 계좌 수
    bool isprimarybank;
    int dfee;
    int wfee;
    int afee;
    int cfee;

public:
    Bank(string bankname);
    ~Bank();  // Remove =default and provide implementation

    void setisprimarybank(bool isprimary);
    bool getisprimarybank() const { return isprimarybank; }
    string getbankname() const;
    Account* getAccount(int accountNumber) const;
    Account* createAccount(std::string name, int accountNumber, int password, int initialBalance = 0);

    void createaccount(std::string name, int number, int accbalance, int password);  // Updated signature
    void printbankaccount() const;  // Prints account details
    //std::string Bank;
    int getdfee() const { return dfee; }
    int getwfee() const { return wfee; }
    int getafee() const { return afee; }
    int getcfee() const { return cfee; }

};




Bank::Bank(std::string name) : bankname(name), numaccounts(0) {
    this->bankname = bankname;
    setisprimarybank(false);
    for (int i = 0; i < 1000; i++) {
        accounts[i] = nullptr;
    }
}

Bank::~Bank() {
    for (int i = 0; i < numaccounts; i++) {
        delete accounts[i];
    }
}


void Bank::setisprimarybank(bool isprimary) {
    isprimarybank = isprimary;
    dfee = isprimary ? 1000 : 2000;
    wfee = isprimary ? 1000 : 2000;
    afee = 2000;
    cfee = 1000;
}

void Bank::createaccount(string name, int accnumber, int balance, int password) {
    if (numaccounts < 1000) {
        accounts[numaccounts] = new normalAccount(this, name, accnumber, password, balance);
        numaccounts++;
    }
}

string Bank::getbankname() const {
    return bankname;
}

Account* Bank::createAccount(string name, int accountNumber, int password, int initialBalance) {
    if (numaccounts >= 1000) return nullptr;

    accounts[numaccounts] = new normalAccount(this, name, accountNumber, password, initialBalance);
    numaccounts++;
    return accounts[numaccounts - 1];
}

Account* Bank::getAccount(int accountNumber) const {
    for (int i = 0; i < numaccounts; i++) {
        if (accounts[i]->getAccNumber() == accountNumber) {
            return accounts[i];
        }
    }
    cout << "Account not found." << endl;
    return nullptr;
}

void Bank::printbankaccount() const {
    for (int i = 0; i < numaccounts; i++) {
        cout << "Bank: " << bankname
            << ", Username: " << accounts[i]->getAccName()
            << ", Account: " << accounts[i]->getAccNumber()
            << ", Balance: " << accounts[i]->getBalance() << endl;
    }
}

string Account::getBankName() const {//계좌 은행 알기 
    return bankName ? bankName->getbankname() : "No Bank";
}





enum class ATMType { SingleBank, MultiBank };
enum class Language { English, Korean };
enum class SessionState { Idle, Active, Error };

struct Transaction {
    static int nextTransactionId;
    int transactionId;
    std::string type;
    double amount;
    std::string timestamp;

    Transaction(std::string t, double a);
};

class Session {
private:
    Account* activeAccount;
    std::vector<Transaction> transactions;
    SessionState state;

public:
    Session();
    void addTransaction(const Transaction& trans);
    void printSummary() const;
    SessionState getState() const { return state; }
    Account* getActiveAccount() const { return activeAccount; }
    void setActiveAccount(Account* account) {
        activeAccount = account;
        state = SessionState::Active;
    }
};

class ATM {
private:
    int serialNumber; //ATM 번호 
    ATMType type; //ATM타입 
    bool isBilingual;//ATM 지원 언어 
    Bank* primaryBank;//ATM의 primary bank 
    std::vector<Bank*> supportedBanks;//가능한 은행 
    Session* currentSession;
    Language currentLanguage;// 지원 언어 
    double atmCashBalance;
    int withdrawalCount;
    static const int MAX_WITHDRAWAL_COUNT = 3;
    static const double MAX_WITHDRAWAL_AMOUNT; // Remove constexpr and initialization
    static const int MAX_CASH_INPUT = 50;
    static const int MAX_CHECK_COUNT = 30;
    int cashInventory[4];

public:
    ATM(int serial, ATMType atmType, bool bilingual, Bank* primary, const std::vector<Bank*>& banks);
    ~ATM();

    bool insertCard(Account* account);
    bool endSession();
    bool validateCard(Account* account) const;
    void setLanguage(Language lang);

    // Transaction operations
    void deposit();
    void withdraw();
    void transfer();
    double getAtmCashBalance() const { return atmCashBalance; }
    void setAtmCashBalance(double amount) { atmCashBalance = amount; }
    void readCardinfo(int accNum);
    int atmstart();
    double calculateFee(Bank* recipientBank, Bank* senderBank);
    void addCash(int* bills);
};




const double ATM::MAX_WITHDRAWAL_AMOUNT = 500000.0;
int Transaction::nextTransactionId = 1;

Transaction::Transaction(std::string t, double a) : type(t), amount(a) {
    transactionId = nextTransactionId++;
    //time_t now = time(0);
    //timestamp = ctime(&now);
}

Session::Session() : activeAccount(nullptr), state(SessionState::Idle) {}

void Session::addTransaction(const Transaction& trans) {
    transactions.push_back(trans);
}

void Session::printSummary() const {
    if (transactions.empty()) return;

    std::cout << "=== Session Summary ===" << std::endl;
    for (const auto& trans : transactions) {
        std::cout << "Transaction ID: " << trans.transactionId << std::endl;
        std::cout << "Type: " << trans.type << std::endl;
        std::cout << "Amount: " << trans.amount << std::endl;
        std::cout << "Time: " << trans.timestamp << std::endl;
    }
}

ATM::ATM(int serial, ATMType atmType, bool bilingual, Bank* primary, const std::vector<Bank*>& banks)
    : serialNumber(serial), type(atmType), isBilingual(bilingual),
    primaryBank(primary), currentSession(nullptr),
    currentLanguage(Language::English) {

    primaryBank->setisprimarybank(true);
    if (type == ATMType::MultiBank) {
        for (auto bank : banks) {
            supportedBanks.push_back(bank);
        }
    }
}

ATM::~ATM() {
    delete currentSession;
}


void ATM::readCardinfo(int accNum) {
    bool valid = false;
    bool adminCheck = false;

    for (Bank* bank : supportedBanks) {
        Account* account = bank->getAccount(accNum);
        if (account != nullptr) {
            currentSession = new Session();
            currentSession->setActiveAccount(account);
            valid = true;

            // 관리자 계좌 확인
            if (account->isAdmincheck()) {
                adminCheck = true;
            }
            break;
        }
    }

    if (!valid) {
        if (currentLanguage == Language::English) {
            cout << "Error: No account found. Returning card." << endl;
        }
        else {
            cout << "오류: 존재하지 않는 계좌입니다. 카드를 반환합니다." << endl;
        }
        return;
    }

    if (adminCheck) {
        if (currentLanguage == Language::English) {
            cout << "Administrator card verified. Welcome!" << endl;
        }
        else {
            cout << "관리자 카드임이 확인되었습니다. 환영합니다!" << endl;
        }
    }
}

int ATM::atmstart() {
    long long accNumber; // Changed to long long to handle 12 digits
    int password_in;
    int lang;
    bool isValidAccount = false;
    bool isAdminCard = false;

    cout << "Choose your language.  언어를 고르세요." << endl;
    cout << "1. English, 2. Korean\n";
    cout << "1. 영어, 2. 한국어\n";

    cin >> lang;
    if (lang == 2) {
        setLanguage(Language::Korean);
    }
    else {
        setLanguage(Language::English);
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
        cin >> accNumber;

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

    return 1; // Should not reach here unless session is successful
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
    return true;
}

void ATM::setLanguage(Language lang) {
    if (!isBilingual && lang != Language::English) {
        cout<<"This ATM only supports English.";
        return;
    }
    currentLanguage = lang;
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

    int depositType;
    std::cout << "입금 타입을 고르시오" << endl;
    std::cout << "1. 현금 2. 수표 3. 취소" << endl;
    std::cin >> depositType;

    double totalAmount = 0.0;

    if (depositType == 1) {
        int denominations[4];
        const int denominationValues[4] = { 50000, 10000, 5000, 1000 };
        int totalBills;

        do {
            totalBills = 0;
            totalAmount = 0.0;

            for (int i = 0; i < 4; i++) {
                std::cout << denominationValues[i] << "원권을 입력하시오: ";
                std::cin >> denominations[i];
                totalBills += denominations[i];
                totalAmount += denominations[i] * denominationValues[i];
            }

            if (totalBills > MAX_CASH_INPUT) {
                std::cout << "입금하신 현금이 너무 많습니다. 한번에 최대: " << MAX_CASH_INPUT << "장만 입금 가능합니다. 다시 입력해주세요.\n";
            }
        } while (totalBills > MAX_CASH_INPUT);

    }
    else if (depositType == 2) {
        int checkCount;

        do {
            std::cout << "입력할 수표의 장수를 입력: ";
            std::cin >> checkCount;

            if (checkCount > MAX_CHECK_COUNT) {
                std::cout << "수표가 너무 많습니다. 최대: " << MAX_CHECK_COUNT
                    << "장만 가능합니다. 다시 입력해주세요.\n";
            }
        } while (checkCount > MAX_CHECK_COUNT);

        for (int i = 0; i < checkCount; i++) {
            double checkAmount;
            std::cout << (i + 1) << "번째 수표 금액을 입력: ";
            std::cin >> checkAmount;
            totalAmount += checkAmount;
        }

    }
    else if (depositType == 3) {
        std::cout << "계좌 입금을 취소합니다.\n";
        return;
    }
    else {
        std::cout << "입금 유형을 1.현금 2.수표 3.취소 중 하나를 골라주세요.\n";
        return;
    }

    int fee = account->getBankName() == primaryBank->getbankname() ? primaryBank->getdfee() : primaryBank->getdfee() * 2;

    std::cout << "입금 수수료: " << fee << "원\n";
    int additionalFee = 0;
    int totalFee = 0;

    while (totalFee < fee) {
        std::cout << "수수료를 넣어주세요. (" << totalFee << "/" << fee << ")\n";
        std::cin >> additionalFee;
        if (totalFee + additionalFee > fee) {
            std::cout << "입력하신 수수료가 초과되었습니다. " << "가장 최근에 입력한 " << additionalFee << "원을 반환합니다." << endl;
        }
        else {
            totalFee += additionalFee;
        }
    }

    account->deposit(totalAmount);
    atmCashBalance += (depositType == 1 ? totalAmount : 0);
    Transaction trans("Deposit", totalAmount);
    currentSession->addTransaction(trans);

    std::cout << "입금 완료. 총 입금 금액: " << totalAmount << "원이며 수수료를 제외한" << totalAmount - fee << "원이 입금됩니다" << endl;
}


void ATM::withdraw() {
    if (!currentSession) {
        cout << "No active session." << endl;
        return;
    }

    if (withdrawalCount >= MAX_WITHDRAWAL_COUNT) {
        cout << "출금 가능 횟수를 초과하였습니다. 세션을 종료한 후 다시 시도해 주세요." << endl;
        return;
    }

    cout << "출금할 총 금액을 입력하시오." << endl;
    double amount;
    cin >> amount;

    if (amount > MAX_WITHDRAWAL_AMOUNT) {
        cout << "출금하고자 하는 금액이 너무 많습니다. 최대" << MAX_WITHDRAWAL_AMOUNT << "원 출금 가능합니다" << endl;
        return;
    }

    Account* account = currentSession->getActiveAccount();
    if (!account) {
        cout << "계좌 정보를 찾을 수 없습니다" << endl;
        return;
    }

    int fee = account->getBankName() == primaryBank->getbankname() ? primaryBank->getwfee() : primaryBank->getwfee() * 2;

    if (atmCashBalance < (amount + fee)) {
        cout << "ATM에 충분한 돈이 들어있지 않아 출금이 불가합니다. 죄송합니다." << endl;
        return;
    }

    if (account->withdraw(amount + fee)) {
        atmCashBalance -= amount;
        withdrawalCount++;
        Transaction trans("Withdrawal", amount);
        currentSession->addTransaction(trans);
        cout << "출금이 완료되었습니다. 출금금액은 " << amount << "원, 수수료는 " << fee << "원입니다" << endl;

    }
    else {
        cout << "계좌에 충분한 금액이 없습니다." << endl;
    }
}


double ATM::calculateFee(Bank* recipientBank, Bank* senderBank) {
    if (recipientBank->getisprimarybank() && senderBank->getisprimarybank()) {
        return senderBank->getafee();
    }
    else if (!recipientBank->getisprimarybank() && !senderBank->getisprimarybank()) {
        return senderBank->getafee() * 2;
    }
    else {
        return (int)(senderBank->getafee() * 1.5);
    }
}


void ATM::transfer() {
    if (!currentSession) {
        cout << "No active session." << endl;
        return;
    }

    cout << "송금 유형을 고르시오." << endl;
    cout << "1.계좌 송금 2.현금 송금 3.취소" << endl;
    int transferType;
    cin >> transferType;

    Bank* transferBank = nullptr;
    Account* transferAccount = nullptr;
    double fee = 0, total = 0;

    // 은행 선택
    cout << "송금하려는 은행을 입력하시오" << endl;
    for (size_t i = 0; i < supportedBanks.size(); i++) {
        cout << i + 1 << ". " << supportedBanks[i]->getbankname() << endl;
    }
    int bankChoice;
    cin >> bankChoice;

    if (bankChoice < 1 || bankChoice > supportedBanks.size()) {
        cout << "잘못된 은행 선택입니다." << endl;
        return;
    }
    transferBank = supportedBanks[bankChoice - 1];

    // 계좌 번호 입력 및 확인
    cout << "받는 사람의 계좌 번호를 입력해주세요." << endl;
    int recipientAccountNumber;
    cin >> recipientAccountNumber;

    transferAccount = transferBank->getAccount(recipientAccountNumber);
    if (!transferAccount) {
        cout << "해당 계좌번호를 찾을 수 없습니다." << endl;
        return;
    }

    if (transferType == 2) { // 현금 송금
        int denominations[4] = { 0 };
        const int denominationValues[4] = { 50000, 10000, 5000, 1000 };
        double totalAmount = 0;
        int totalBills = 0;

        do {
            for (int i = 0; i < 4; i++) {
                std::cout << denominationValues[i] << "원권을 입력하시오: ";
                std::cin >> denominations[i];
                totalBills += denominations[i];
                totalAmount += denominations[i] * denominationValues[i];
            }

            if (totalBills > MAX_CASH_INPUT) {
                std::cout << "입금하신 현금이 너무 많습니다. 한번에 최대: " << MAX_CASH_INPUT << "장만 송금 가능합니다. 다시 입력해주세요." << endl;
            }
        } while (totalBills > MAX_CASH_INPUT);


        fee = calculateFee(transferBank, primaryBank);
        total = totalAmount + fee;

        cout << "Fee: " << fee << endl;
        cout << "Deposit amount: " << totalAmount << ", Total: " << total << endl;

        if (totalAmount > 0) {
            transferAccount->deposit(totalAmount);
            atmCashBalance += totalAmount;
            Transaction trans("Transfer(Cash)", totalAmount);
            currentSession->addTransaction(trans);
            cout << "Cash transfer successful." << endl;
        }
    }
    else if (transferType == 1) { // 계좌 송금
        Account* sourceAccount = currentSession->getActiveAccount();
        cout << "Enter transfer amount: ";
        double transferAmount;
        cin >> transferAmount;

        fee = calculateFee(transferBank, primaryBank);

        if (transferAmount + fee > sourceAccount->getBalance()) {
            cout << "Insufficient funds in the account." << endl;
            return;
        }

        sourceAccount->withdraw(transferAmount + fee);
        transferAccount->deposit(transferAmount);
        Transaction trans("Transfer(Account)", transferAmount);
        currentSession->addTransaction(trans);
        cout << "Account transfer successful." << endl;
    }
    else if (transferType == 3) {
        std::cout << "계좌 송금을 취소합니다" << endl;
        return;
    }
    else {
        std::cout << "입금 유형을 1.계좌 송금 2.현금 송금 3.취소 중 하나를 골라주세요." << endl;
        return;
    }

    return;
}


void ATM::addCash(int* bills) {
    if (!bills) return;

    // 각 지폐 단위별로 ATM에 추가
    for (int i = 0; i < 4; i++) {
        cashInventory[i] += bills[i];
        atmCashBalance += bills[i];
    }
    std::cout << "ATM cash inventory updated." << std::endl;
}



class SessionManager {
private:
    std::vector<Bank*> banks;
    std::vector<ATM*> atms;
    void cleanup();

public:
    ~SessionManager();
    void initialize();
    void createBank();
    void createATM();
    void createAccount();
    ATM* selectATM();
    void run();
    int* depositcashtoatm(ATM* atm);
    void depositCashToATM(ATM* atm);

};

SessionManager::~SessionManager() {
    cleanup();
}

void SessionManager::cleanup() {
    for (auto bank : banks) delete bank;
    for (auto atm : atms) delete atm;
    banks.clear();
    atms.clear();
}

void SessionManager::initialize() {
    int numBanks, numATMs;

    do {
        try {
            std::cout << "How many banks do you want to create? ";
            std::cin >> numBanks;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear();
            return;
        }
        if (numBanks < 1) {
            std::cout << "Please enter a valid number of banks.\n";
        }
    } while (numBanks < 1);

    for (int i = 0; i < numBanks; i++) {
        createBank();
    }

    std::cout << "How many ATMs do you want to create? ";
    do {
        try {
            std::cin >> numATMs;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear();
            return;
        }
        if (numATMs < 1) {
            std::cout << "Please enter a valid number of ATMs.\n";
        }
    } while (numATMs < 1);

    for (int i = 0; i < numATMs; i++) {
        createATM();
    }
}



void SessionManager::depositCashToATM(ATM* atm) {
    if (!atm) {
        std::cout << "Invalid ATM instance." << std::endl;
        return;
    }

    int* bills = depositcashtoatm(atm); // 사용자 입력 기반 지폐 수량 가져오기
    if (!bills) {
        std::cout << "Failed to process cash deposit." << std::endl;
        return;
    }

    atm->addCash(bills); // ATM에 현금을 추가
    //atm->displayCashInventory(); // 입금 후 현금 상태 출력

    delete[] bills; // 메모리 해제
}


int* SessionManager::depositcashtoatm(ATM* atm) {
    if (!atm) {
        std::cout << "Invalid ATM instance." << std::endl;
        return nullptr;
    }

    int* result = new int[4]; // 각 지폐 단위별 개수
    const int denominations[] = { 50000, 10000, 5000, 1000 };

    std::cout << "Deposit cash from bank to ATM. Please enter the number of bills for each denomination:" << std::endl;

    for (int i = 0; i < 4; i++) {
        int numBills;
        do {
            std::cout << denominations[i] << "won: ";
            std::cin >> numBills;

            if (std::cin.fail() || numBills < 0) {
                std::cin.clear(); // 잘못된 입력 제거
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a non-negative integer." << std::endl;
            }
            else {
                break;
            }
        } while (true);

        result[i] = numBills;
    }

    return result;
}



void SessionManager::createBank() {
    std::string bankName;
    do {
        std::cout << "Enter bank name: ";
        try {
            std::cin >> bankName;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input. Please enter a valid name.\n";
            std::cin.clear();
            return;
        }
    } while (bankName.empty());

    banks.push_back(new Bank(bankName));
    std::cout << "Bank created successfully!\n";
}

void SessionManager::createATM() {
    int serialNumber;
    int typeChoice;
    int bilingualChoice;
    int primaryBankIndex;

    std::cout << "Enter ATM serial number: ";
    try {
        std::cin >> serialNumber;
    }
    catch (std::exception& e) {
        std::cout << "Invalid input. Please enter numbers only.\n";
        std::cin.clear();
        return;
    }

    do {
        std::cout << "Enter 6-digit ATM serial number (100000-999999): ";
        try {
            std::cin >> serialNumber;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear();
            continue;
        }

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter numbers only.\n";
            continue;
        }

        if (serialNumber < 100000 || serialNumber > 999999) {
            std::cout << "Serial number must be 6 digits.\n";
            continue;
        }

        break;
    } while (true);

    if (banks.empty()) {
        std::cout << "No banks available. Please create a bank first.\n";
        this->createBank();
        return;
    }

    std::cout << "Select ATM type (1: SingleBank, 2: MultiBank): ";
    do {
        try {
            std::cin >> typeChoice;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear();
            return;
        }
        if (typeChoice != 1 && typeChoice != 2) {
            std::cout << "Invalid choice\n";
        }
    } while (typeChoice != 1 && typeChoice != 2);

    ATMType type = (typeChoice == 1) ? ATMType::SingleBank : ATMType::MultiBank;

    std::cout << "Support multiple languages? (1: Yes, 0: No): ";
    do {
        try {
            std::cin >> bilingualChoice;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear();
            return;
        }
        if (bilingualChoice != 0 && bilingualChoice != 1) {
            std::cout << "Invalid choice\n";
        }
    } while (bilingualChoice != 0 && bilingualChoice != 1);

    std::cout << "Select primary bank (";
    for (size_t i = 0; i < banks.size(); i++) {
        std::cout << i << ": " << banks[i]->getbankname() << " ";
    }
    std::cout << "): ";

    try {
        std::cin >> primaryBankIndex;
    }
    catch (std::exception& e) {
        std::cout << "Invalid input. Please enter numbers only.\n";
        std::cin.clear();
        return;
    }

    while (primaryBankIndex < 0 || primaryBankIndex >= banks.size()) {
        std::cout << "Invalid bank choice\n";
        std::cout << "Please choose a valid bank.\n";
        try {
            std::cin >> primaryBankIndex;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear();
            return;
        }
    }

    atms.push_back(new ATM(serialNumber, type, bilingualChoice == 1,
        banks[primaryBankIndex], banks));
    std::cout << "ATM created successfully!\n";


    ATM* lastATM = atms.back();
    depositCashToATM(lastATM);

}

void SessionManager::createAccount() {
    std::string name;
    long long accountNumber, initialBalance, password;
    int bankIndex;

    std::cout << "Select bank (";
    for (size_t i = 0; i < banks.size(); i++) {
        std::cout << i << ": " << banks[i]->getbankname() << " ";
    }
    std::cout << "): ";

    try {
        std::cin >> bankIndex;
    }
    catch (std::exception& e) {
        std::cout << "Invalid input. Please enter numbers only.\n";
        std::cin.clear();
        return;
    }

    while (bankIndex < 0 || bankIndex >= banks.size()) {
        std::cout << "Invalid bank choice\n";
        std::cout << "Please choose a valid bank.\n";
        try {
            std::cin >> bankIndex;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear();
            return;
        }
    }

    std::cout << "Enter account holder name: ";
    try {
        std::cin >> name;
    }
    catch (std::exception& e) {
        std::cout << "Invalid input. Please enter a valid name.\n";
        std::cin.clear();
        return;
    }

    do {
        std::cout << "Enter 12-digit account number (100000000000-999999999999): ";
        try {
            std::cin >> accountNumber;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear();
            return;
        }

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter numbers only.\n";
            continue;
        }

        // Check if number is 12 digits
        if (accountNumber < 100000000000LL || accountNumber > 999999999999LL) {
            std::cout << "Account number must be 12 digits.\n";
            continue;
        }

        break;
    } while (true);

    std::cout << "Enter initial balance: ";
    try {
        std::cin >> initialBalance;
    }
    catch (std::exception& e) {
        std::cout << "Invalid input. Please enter numbers only.\n";
        std::cin.clear();
        return;
    }

    std::cout << "Enter password: ";
    try {
        std::cin >> password;
    }
    catch (std::exception& e) {
        std::cout << "Invalid input. Please enter numbers only.\n";
        std::cin.clear();
        return;
    }

    banks[bankIndex]->createAccount(name, accountNumber, password, initialBalance);
    std::cout << "Account created successfully!\n";
}

ATM* SessionManager::selectATM() {
    if (atms.empty()) {
        std::cout << "No ATMs available.\n";
        return nullptr;
    }

    std::cout << "Select ATM:\n";
    for (size_t i = 0; i < atms.size(); i++) {
        std::cout << i << ": ATM #" << i + 1 << "\n";
    }

    int choice;
    do {
        std::cout << "Choice: ";
        try {
            std::cin >> choice;
        }
        catch (std::exception& e) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear();
        }
    } while (choice < 0 || choice >= atms.size());
    if (choice >= 0 && choice < atms.size()) {
        return atms[choice];
    }
    return nullptr;
}

void SessionManager::run() {
    initialize();

    while (true) {
        std::cout << "\nMenu:\n";
        std::cout << "1. Create new account\n";
        std::cout << "2. Use ATM\n";
        std::cout << "3. Exit\n";

        int choice;
        do {
            try {
                std::cout << "Choice: ";
                std::cin >> choice;
            }
            catch (std::exception& e) {
                std::cout << "Invalid input. Please enter numbers only.\n";
                std::cin.clear();
            }
        } while (choice < 1 || choice > 3);

        switch (choice) {
        case 1:
            createAccount();
            break;
        case 2: {
            ATM* selectedATM = selectATM();
            if (selectedATM) {
                selectedATM->atmstart();
            }
            break;
        }
        case 3:
            return;
        default:
            std::cout << "Invalid choice\n";
        }
    }
}







int main() {
    SessionManager manager;
    manager.run();
    return 0;
}

//각각 atm2개, sessionmanager2개 메서드 추가, deposit, withdraw, transfer 바뀜, atm에 member확인해야함.
