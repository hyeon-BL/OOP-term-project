#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>


using namespace std;


class Bank;  // Forward declaration needed

#include <string>
#include <vector>

using namespace std;

class Account {
protected:
    string accName;                   // 사용자 이름
    long long accNumber;                    // 계좌 번호
    int password;                     // 계좌 비밀번호
    int accbalance;                   // 계좌 잔액
    Bank* bankName;                  // 은행 이름
    vector<string> transactionHistory; // 거래 내역
    bool isAdmin;                     // 관리자 여부 
    int tID;                          // 거래 ID

public:
    // 생성자와 소멸자
    Account(string name, long long number, int password, int initialBalance = 0);
    ~Account();

    // Getter 메서드
    string getAccName() const; // 사용자 이름 확인 
    long long getAccNumber() const;   // 계좌번호 확인 
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
    void plustransactionID() { tID++; }
    int gettransactionID() { return tID; }
    Bank* getBank() const {
        return bankName;
    }
};

class normalAccount : public Account { // 일반 계좌 정보 
public:
    normalAccount(Bank* bank, string name, long long number, int password, int accbalance);
    bool isAdmincheck() const override;
};


class admin : public Account {
public:
    admin(Bank* bank, string name, int number, int password);  // Fix parameter type
    bool isAdmincheck() const; // Add const
};





// 생성자
Account::Account(string name, long long number, int password, int initialBalance)
    : accName(name), accNumber(number), password(password), accbalance(initialBalance), bankName(nullptr), isAdmin(false), tID(0) {
}

// 소멸자
Account::~Account() {}

// Getter 메서드
string Account::getAccName() const {//계좌 이름 알기 
    return accName;
}

long long Account::getAccNumber() const {//계좌 번호 알기 
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


normalAccount::normalAccount(Bank* bank, string name, long long number, int password, int accbalance) : Account(name, number, password, accbalance) {
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
    Account* getAccount(long long accountNumber) const;
    Account* createAccount(std::string name, long long accountNumber, int password, int initialBalance = 0);

    void createaccount(std::string name, long long number, int accbalance, int password);  // Updated signature
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

void Bank::createaccount(string name, long long accnumber, int balance, int password) {
    if (numaccounts < 1000) {
        accounts[numaccounts] = new normalAccount(this, name, accnumber, password, balance);
        numaccounts++;
    }
}

string Bank::getbankname() const {
    return bankname;
}

Account* Bank::createAccount(string name, long long accountNumber, int password, int initialBalance) {
    if (numaccounts >= 1000) return nullptr;

    accounts[numaccounts] = new normalAccount(this, name, accountNumber, password, initialBalance);
    numaccounts++;
    return accounts[numaccounts - 1];
}

Account* Bank::getAccount(long long accountNumber) const {
    for (int i = 0; i < numaccounts; i++) {
        if (accounts[i]->getAccNumber() == accountNumber) {
            return accounts[i];
        }
    }
    return nullptr;
}

void Bank::printbankaccount() const {
    for (int i = 0; i < numaccounts; i++) {
        if (accounts[i]->getAccNumber() == 1111) {
            continue;
        }
        cout << "Bank: " << bankname
            << ", Username: " << accounts[i]->getAccName()
            << ", Account Number: " << accounts[i]->getAccNumber()
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
    std::string accountInfo;  // Added field for account info

    Transaction(std::string t, double a, const Account* acc);
};

class ATM; // Forward declaration

class Session {
private:
    Account* activeAccount;
    std::vector<Transaction> transactions;
    SessionState state;
    bool hasTransactions;  // Added to track if any transactions occurred

public:
    Session();
    void addTransaction(const Transaction& trans);
    void printSummary() const;
    SessionState getState() const { return state; }
    Account* getActiveAccount() const { return activeAccount; }
    void setActiveAccount(Account* account);
    void sethasTransactions(bool hasTransactions) { this->hasTransactions = hasTransactions; }
    bool processUserChoice(ATM* atm);
    bool processAdminChoice(ATM* atm);// New method to handle user choices
    bool hasCompletedTransactions() const { return hasTransactions; }
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
    string filePath = to_string(serialNumber) + "history.txt";
    int TransactionID;
    string history;
    vector<std::string> transactionLog;

public:
    ATM(int serial, ATMType atmType, bool bilingual, Bank* primary, const std::vector<Bank*>& banks);
    ~ATM();

    void atmstart();
    //void readCardInfo(int accNum);
    bool insertCard(Account* account);
    bool endSession();
    bool validateCard(Account* account) const;
    void setLanguage(Language lang);
    Language getLanguage() const { return currentLanguage; }
    bool IsBilingual() const { return isBilingual; }
    int getSerialNumber() const { return serialNumber; }
    Bank* getPrimaryBank() const { return primaryBank; }
    ATMType getATMType() const { return type; }
    void showhistory();

    // Transaction operations
    void deposit();
    void withdraw();
    void transfer();
    double getAtmCashBalance() const { return atmCashBalance; }
    void setAtmCashBalance(double amount) { atmCashBalance = amount; }
    int calculateFee(Bank* recipientBank, Bank* senderBank);
    void addCash(int* bills);
    void showCashInventory() const;
    void recordTransaction(const std::string& transaction);
    void displayTransactionHistory() const;
    void saveTransactionHistoryToFile(const std::string& filename) const;

    void depositCash(int amount);
    void withdrawCash(int amount);
};

std::vector<Bank*> banks;
std::vector<ATM*> atms;

void printATMsinfo() {
    for (size_t i = 0; i < atms.size(); i++) {
        std::cout << "ATM serial number: " << atms[i]->getSerialNumber() << endl;
        atms[i]->showCashInventory();
    }
}

void printAccountsinfo() {
    for (size_t i = 0; i < banks.size(); i++) {
        banks[i]->printbankaccount();
    }
}


void ATM::recordTransaction(const std::string& transaction) {
    transactionLog.push_back(transaction);
}

// 거래 기록을 출력하는 함수
void ATM::displayTransactionHistory() const {
    if (transactionLog.empty()) {
        if (currentLanguage == Language::English) {
            std::cout << "No transactions recorded." << std::endl;
        }
        else {
            std::cout << "거래 내역이 없습니다." << std::endl;
        }
        return;
    }
    if (currentLanguage == Language::English) {
        std::cout << "Transaction History:\n";
    }
    else {
        std::cout << "거래 기록:\n";
    }
    for (const auto& transaction : transactionLog) {
        std::cout << transaction << std::endl;
    }
}

// 거래 기록을 파일로 저장하는 함수
void ATM::saveTransactionHistoryToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    for (const auto& transaction : transactionLog) {
        outFile << transaction << std::endl;
    }

    outFile.close();
    std::cout << "Transaction history saved to " << filename << std::endl;
}

// 현금 입금 기능
void ATM::depositCash(int amount) {
    if (amount <= 0) {
        std::cout << "Invalid deposit amount." << std::endl;
        return;
    }

    std::cout << "Deposited: " << amount << " successfully." << std::endl;
    recordTransaction("Deposit: " + std::to_string(amount) + " won");
}

// 현금 출금 기능
void ATM::withdrawCash(int amount) {
    if (amount <= 0) {
        std::cout << "Invalid withdrawal amount." << std::endl;
        return;
    }

    std::cout << "Withdrawn: " << amount << " successfully." << std::endl;
    recordTransaction("Withdrawal: " + std::to_string(amount) + " won");
}



const double ATM::MAX_WITHDRAWAL_AMOUNT = 500000.0;
int Transaction::nextTransactionId = 1;



Transaction::Transaction(std::string t, double a, const Account* acc)
    : type(t), amount(a) {
    transactionId = nextTransactionId++;

    // 현재 시간 가져오기
    time_t now = time(0);
    char timestampBuffer[26]; // ctime_s에 필요한 크기

    // 안전하게 timestamp를 생성
    if (ctime_s(timestampBuffer, sizeof(timestampBuffer), &now) == 0) {
        timestamp = timestampBuffer;
    }
    else {
        timestamp = "Error retrieving timestamp";
    }

    // Account 정보 포함
    if (acc) {
        std::ostringstream oss;
        oss << "Account: " << acc->getAccNumber()
            << " Bank: " << acc->getBankName();
        accountInfo = oss.str();
    }
}



Session::Session()
    : activeAccount(nullptr), state(SessionState::Idle), hasTransactions(false) {}

void Session::addTransaction(const Transaction& trans) {
    transactions.push_back(trans);
}

void Session::setActiveAccount(Account* account) {
    this->activeAccount = account;
    state = SessionState::Active;
}

bool Session::processUserChoice(ATM* atm) {
    while (state == SessionState::Active) {
        if (atm->getLanguage() == Language::English) {
            std::cout << "\n=== ATM Menu ===\n";
            std::cout << "1. Deposit\n";
            std::cout << "2. Withdraw\n";
            std::cout << "3. Transfer\n";
            std::cout << "4. Check Balance\n";
            std::cout << "5. End Session\n";
            std::cout << "/ : Show ATM and Account Info\n";
            std::cout << "Choose an option: ";
        }
        else {
            std::cout << "\n=== ATM 메뉴 ===\n";
            std::cout << "1. 입금\n";
            std::cout << "2. 출금\n";
            std::cout << "3. 이체\n";
            std::cout << "4. 잔액 확인\n";
            std::cout << "5. 세션 종료\n";
            std::cout << "/ : ATM 및 계좌 정보 표시\n";
            std::cout << "옵션을 선택하세요: ";
        }


        string userInput;
        int choice = -1;  // 유효하지 않은 값으로 초기화

        do {
            try {
                cin >> userInput;

                // 숫자인 경우 숫자로 변환
                if (isdigit(userInput[0]) && userInput.size() == 1) {
                    choice = stoi(userInput);
                }
                else if (userInput == "/") {
                    // '/' 입력 처리
                    printATMsinfo();
                    printAccountsinfo();
                    break;  // 다시 메뉴를 출력하도록 루프 종료
                }
                else {
                    throw std::invalid_argument("Invalid input.");
                }
            }
            catch (std::exception& e) {
                cout << "Invalid input. Please enter a valid option (1-5 or /).\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 입력 버퍼 정리
                choice = -1;  // choice를 초기화하여 유효한 입력이 들어올 때까지 반복
            }
        } while (choice < 1 || choice > 5);

        if (choice == -1) {
            continue;  // 루프를 반복하여 다시 메뉴 선택
        }

        // 유효한 선택에 따라 처리
        switch (choice) {
        case 1:
            atm->deposit();
            this->sethasTransactions(true);
            break;
        case 2:
            atm->withdraw();
            this->sethasTransactions(true);
            break;
        case 3:
            atm->transfer();
            this->sethasTransactions(true);
            break;
        case 4:
            if (atm->getLanguage() == Language::English) {
                std::cout << "Current balance: " << activeAccount->getBalance() << std::endl;
            }
            else {
                std::cout << "현재 잔액: " << activeAccount->getBalance() << std::endl;
            }
            break;
        case 5:
            return false;  // 세션 종료
        default:
            cout << "Invalid option\n";  // 사실상 필요 없는 기본 처리
        }
    }
    return true;
}


bool Session::processAdminChoice(ATM* atm) {
    while (state == SessionState::Active) {
        if (atm->getLanguage() == Language::English) {
            std::cout << "\n=== ATM Menu ===\n";
            std::cout << "1. Show Transaction History\n";
            std::cout << "2. Cancel\n";
        }
        else {
            std::cout << "\n=== ATM 메뉴 ===\n";
            std::cout << "1. 거래 내역 표시\n";
            std::cout << "2. 취소\n";
        }

        int choice;
        do {
            try {
                cin >> choice;
            }
            catch (std::exception& e) {
                cout << "Invalid input. Please enter numbers only.\n";
                cin.clear();
                break;
            }
        } while (choice < 1 || choice > 2);

        switch (choice) {
        case 1:
            atm->displayTransactionHistory();
            atm->saveTransactionHistoryToFile("transaction_history.txt");
            return false;
        case 2: {
            return false;
        }
        default:
            cout << "Invalid option\n";
        }
    }
    return true;
}


void Session::printSummary() const {

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

    ofstream writeFile(filePath.data());
    if (writeFile.is_open()) {
        writeFile << "";
        writeFile.close();
    }
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
        cout << "This ATM only supports English.";
        return;
    }
    currentLanguage = lang;
}

void ATM::showhistory() {
    if (history == "") {
        cout << "No record" << endl;
        return;
    }
    cout << history << endl;
}

void ATM::atmstart() {
    long long accNumber; // Changed to long long to handle 12 digits
    int password_in;
    int lang;
    bool isValidAccount = false;
    bool isAdminCard = false;
    withdrawalCount = 0;

    if (this->IsBilingual() == false) {
        this->setLanguage(Language::English);
    }
    if (this->IsBilingual()) {
        cout << "Choose your language.  언어를 고르세요." << endl;
        cout << "1. English, 2. Korean\n";
        cout << "1. 영어, 2. 한국어\n";
        do {
            try {
                cin >> lang;
            }
            catch (std::exception& e) {
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
        cout << "Enter your 12-digit account number or 4- digit admin number: ";
    }
    else {
        cout << "카드를 삽입하십시오." << endl;
        cout << "12자리 계좌번호를 입력하십시오: ";
    }

    do {
        try {
            cin >> accNumber;
        }
        catch (std::exception& e) {
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
        if (accNumber == 1111) {
            isAdminCard = true;
            break;
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
            return;
        }
        else {
            cout << "오류: 계좌를 찾을 수 없습니다. 카드를 반환합니다." << endl;
            return;
        }
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
        }
        catch (std::exception& e) {
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
    if (isAdminCard) {
        while (currentSession != nullptr) {
            if (!currentSession->processAdminChoice(this)) {
                if (currentSession->hasCompletedTransactions()) {
                    //currentSession->printSummary();
                }
                endSession();
                break;
            }
        }
    }
    else {
        while (currentSession != nullptr) {
            if (!currentSession->processUserChoice(this)) {
                if (currentSession->hasCompletedTransactions()) {
                    //currentSession->printSummary();
                }
                endSession();
                break;
            }
        }
    }
    // After successful authentication:


}

bool ATM::insertCard(Account* account) {
    if (!validateCard(account)) {
        if (currentLanguage == Language::English) {
            cout << "Invalid card for this ATM.";
        }
        else {
            cout << "이 ATM에는 유효하지 않은 카드입니다.";
        }
        return false;
    }
    if (currentSession != nullptr) {
        if (currentLanguage == Language::English) {
            cout << "Session already in progress.";
        }
        else {
            cout << "세션이 이미 진행 중입니다.";
        }
        return false;
    }
    currentSession = new Session();
    currentSession->setActiveAccount(account);
    if (currentLanguage == Language::English) {
        cout << "Card accepted. Welcome!\n";
    }
    else {
        cout << "카드가 인식되었습니다. 환영합니다!\n";
        return true;
    }
}


bool ATM::endSession() {
    if (currentSession == nullptr) return false;

    currentSession->printSummary();
    delete currentSession;
    currentSession = nullptr;
    cout << "Thank you for using our ATM.";
    return true;
}


bool ATM::validateCard(Account* account) const {
    if (type == ATMType::SingleBank) {
        return account->getBankName() == primaryBank->getbankname();
    }
    return true; // MultiBank ATM accepts all cards
}


int ATM::calculateFee(Bank* recipientBank, Bank* senderBank) {
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


void ATM::addCash(int* bills) {
    if (!bills) return;

    const int denominationValues[4] = { 50000, 10000, 5000, 1000 };
    for (int i = 0; i < 4; i++) {
        cashInventory[i] += bills[i];
        atmCashBalance += bills[i] * denominationValues[i];
    }

    std::cout << "ATM cash inventory updated." << std::endl;
    showCashInventory();
}


void ATM::showCashInventory() const {
    const int denominationValues[4] = { 50000, 10000, 5000, 1000 };
    if (currentLanguage == Language::English) {
        std::cout << "ATM Cash Inventory:" << std::endl;
    }
    else {
        std::cout << "ATM 현금 재고:" << std::endl;
    }
    for (int i = 0; i < 4; i++) {
        if (currentLanguage == Language::English) {
            std::cout << denominationValues[i] << " won bills: " << cashInventory[i] << std::endl;
        }
        else {
            std::cout << denominationValues[i] << "원권: " << cashInventory[i] << "장" << std::endl;
        }
    }
    if (currentLanguage == Language::English) {
        std::cout << "Total Cash Balance: " << atmCashBalance << " won" << std::endl;
    }
    else {
        std::cout << "전체 금액: " << atmCashBalance << "원" << std::endl;
    }
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
    if (currentLanguage == Language::English) {
        std::cout << "Choose deposit type" << endl;
        std::cout << "1. Cash 2. Check 3. Cancel" << endl;
    }
    else {
        std::cout << "입금 타입을 고르시오" << endl;
        std::cout << "1. 현금 2. 수표 3. 취소" << endl;
    }
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
                if (currentLanguage == Language::English) {
                    std::cout << "Enter the number of " << denominationValues[i] << " won bills: ";
                }
                else {
                    std::cout << denominationValues[i] << "원권을 입금해주세요: ";
                }
                std::cin >> denominations[i];
                totalBills += denominations[i];
                totalAmount += denominations[i] * denominationValues[i];
            }

            if (totalBills > MAX_CASH_INPUT) {
                if (currentLanguage == Language::English) {
                    std::cout << "Too many bills deposited. Maximum: " << MAX_CASH_INPUT << " bills allowed. Please try again.\n";
                }
                else {
                    std::cout << "입금하신 현금이 너무 많습니다. 한번에 최대: " << MAX_CASH_INPUT << "장만 입금 가능합니다. 다시 입력해주세요.\n";
                }
            }
            else if (totalBills <= 0) {
                if (currentLanguage == Language::English) {
                    std::cout << "Invalid input. Please enter numbers greater than 0.\n";
                }
                else {
                    std::cout << "잘못된 입력입니다. 0보다 큰 숫자를 입력해주세요.\n";
                }
            }
        } while (totalBills > MAX_CASH_INPUT);

        for (int i = 0; i < 4; i++) {
            cashInventory[i] += denominations[i];
        }

    }
    else if (depositType == 2) {
        int checkCount;

        do {
            if (currentLanguage == Language::English) {
                std::cout << "Enter the number of checks to deposit: ";
            }
            else {
                std::cout << "입금할 수표의 장수를 입력: ";
            }
            std::cin >> checkCount;

            if (checkCount > MAX_CHECK_COUNT) {
                if (currentLanguage == Language::English) {
                    std::cout << "Exceeded maximum number of checks. Maximum: " << MAX_CHECK_COUNT << " checks allowed. Please try again.\n";
                }
                else {
                    std::cout << "수표가 너무 많습니다. 최대: " << MAX_CHECK_COUNT
                        << "장만 가능합니다. 다시 입력해주세요.\n";
                }
            }
        } while (checkCount > MAX_CHECK_COUNT);

        for (int i = 0; i < checkCount; i++) {
            double checkAmount;
            if (currentLanguage == Language::English) {
                std::cout << "Enter the amount for check " << (i + 1) << ": ";
            }
            else {
                std::cout << (i + 1) << "번째 수표 금액을 입력: ";
            }
            std::cin >> checkAmount;
            totalAmount += checkAmount;
        }

    }
    else if (depositType == 3) {
        if (currentLanguage == Language::English) {
            std::cout << "Cancelling account deposit.\n";
        }
        else {
            std::cout << "계좌 입금을 취소합니다.\n";
        }
        return;
    }
    else {
        if (currentLanguage == Language::English) {
            std::cout << "Please choose 1. Cash 2. Check 3. Cancel.\n";
        }
        else {
            std::cout << "입금 유형을 1.현금 2.수표 3.취소 중 하나를 골라주세요.\n";
        }
        return;
    }

    int fee = account->getBankName() == primaryBank->getbankname() ? primaryBank->getdfee() : primaryBank->getdfee() * 2;

    if (currentLanguage == Language::English) {
        std::cout << "Deposit fee: " << fee << " won\n";
    }
    else {
        std::cout << "입금 수수료: " << fee << "원\n";
    }
    int additionalFee = 0;
    int totalFee = 0;

    while (totalFee < fee) {
        if (currentLanguage == Language::English) {
            std::cout << "Insert the fee. (" << totalFee << "/" << fee << ")\n";
        }
        else {
            std::cout << "수수료를 넣어주세요. (" << totalFee << "/" << fee << ")\n";
        }
        std::cin >> additionalFee;
        if (totalFee + additionalFee > fee) {
            if (currentLanguage == Language::English) {
                std::cout << "The fee you entered exceeds the required amount. Returning the most recent " << additionalFee << " won.\n";
            }
            else {
                std::cout << "입력하신 수수료가 초과되었습니다. " << "가장 최근에 입력한 " << additionalFee << "원을 반환합니다." << endl;
            }
        }
        else {
            totalFee += additionalFee;
        }
    }
    totalAmount += totalFee;
    this->cashInventory[3] += fee / 1000;
    string message;
    message = "Account number: " + to_string(account->getAccNumber()) + " User Name: " + account->getAccName() + " -> ID:" + to_string(account->gettransactionID()) + " -> Deposit " + to_string(totalAmount) + "\n";

    account->deposit(totalAmount - fee);
    atmCashBalance += (depositType == 1 ? totalAmount : 0);
    atmCashBalance += (depositType == 2 ? fee : 0);
    Transaction trans("Deposit", totalAmount - fee, account);
    currentSession->addTransaction(trans);

    if (currentLanguage == Language::English) {
        std::cout << "Deposit successful. Total amount deposited: " << long long(totalAmount - fee) << " won. Fee: " << totalFee << " won.\n";
    }
    else {
        std::cout << "입금 완료. 총 입금 금액: " << totalAmount << "원이며 수수료를 제외한" << long long(totalAmount - fee) << "원이 입금됩니다" << endl;
    }
    account->plustransactionID();
    ofstream outfile;
    outfile.open(filePath, ios_base::app);
    outfile << message;
    outfile.close();
    recordTransaction(message);
    showCashInventory();//
}


void ATM::withdraw() {
    if (!currentSession) {
        if (currentLanguage == Language::English) {
            cout << "No active session." << endl;
        }
        else {
            cout << "활성 세션이 없습니다." << endl;
        }
        return;
    }

    if (withdrawalCount >= MAX_WITHDRAWAL_COUNT) {
        if (currentLanguage == Language::English) {
            cout << "Exceeded maximum withdrawal count. Please try again after ending the session." << endl;
        }
        else {
            cout << "출금 가능 횟수를 초과하였습니다. 세션을 종료한 후 다시 시도해 주세요." << endl;
        }
        return;
    }

    double amount;
    if (currentLanguage == Language::English) {
        cout << "Enter the amount to withdraw: ";
    }
    else {
        cout << "출금할 금액을 입력하세요: ";
    }
    do {
        try {
            cin >> amount;
        }
        catch (std::exception& e) {
            cout << "Invalid input. Please enter numbers only.\n";
            cin.clear();
            break;
        }
    } while (amount <= 0);

    if (amount > MAX_WITHDRAWAL_AMOUNT) {
        if (currentLanguage == Language::English) {
            cout << "The amount you want to withdraw is too high. Maximum withdrawal amount is " << MAX_WITHDRAWAL_AMOUNT << " won." << endl;
        }
        else {
            cout << "출금하고자 하는 금액이 너무 많습니다. 최대" << MAX_WITHDRAWAL_AMOUNT << "원 출금 가능합니다" << endl;
        }
        return;
    }

    Account* account = currentSession->getActiveAccount();
    if (!account) {
        if (currentLanguage == Language::English) {
            cout << "No card inserted." << endl;
        }
        else {
            cout << "카드가 삽입되지 않았습니다." << endl;
        }
        return;
    }

    // 출금 금액을 ATM 잔액에서 차감 가능한지 먼저 확인
    int fee = account->getBankName() == primaryBank->getbankname() ? primaryBank->getwfee() : primaryBank->getwfee() * 2;
    double totalAmount = amount + fee;

    if (atmCashBalance < totalAmount) {
        if (currentLanguage == Language::English) {
            cout << "ATM does not have enough cash to withdraw the requested amount. Sorry for the inconvenience." << endl;
        }
        else {
            cout << "ATM에 충분한 돈이 들어있지 않아 출금이 불가합니다. 죄송합니다." << endl;
        }
        return;
    }

    // 지폐별 출금 계산
    const int denominationValues[4] = { 50000, 10000, 5000, 1000 };
    int withdrawalBills[4] = { 0, 0, 0, 0 };
    double remainingAmount = totalAmount - fee;

    for (int i = 0; i < 4; i++) {
        int neededBills = static_cast<int>(remainingAmount / denominationValues[i]);
        if (neededBills > cashInventory[i]) {
            neededBills = cashInventory[i];
        }
        withdrawalBills[i] = neededBills;
        remainingAmount -= neededBills * denominationValues[i];
    }

    if (remainingAmount > 0) {
        if (currentLanguage == Language::English) {
            cout << "ATM does not have enough bills to satisfy the requested amount. Withdrawal is cancelled." << endl;
        }
        else {
            cout << "ATM에 요청하신 금액을 충족할 충분한 지폐가 없습니다. 출금이 취소됩니다." << endl;
        }
        return;
    }

    // 출금 지폐 차감 및 ATM 현금 잔액 업데이트
    for (int i = 0; i < 4; i++) {
        cashInventory[i] -= withdrawalBills[i];
        atmCashBalance -= withdrawalBills[i] * denominationValues[i];
    }

    // 계좌에서 금액 차감
    if (account->withdraw(totalAmount)) {
        withdrawalCount++;
        Transaction trans("Withdrawal", amount, account);
        currentSession->addTransaction(trans);

        if (currentLanguage == Language::English) {
            cout << "Withdrawal successful. Withdrawn amount: " << long long(amount) << " won, Fee: " << fee << " won." << endl;
            cout << "Withdrawn bills: ";
            for (int i = 0; i < 4; i++) {
                if (withdrawalBills[i] > 0) {
                    cout << denominationValues[i] << " won bills " << withdrawalBills[i] << " ";
                }
            }
            account->plustransactionID();
            cout << endl;
        }
        else {
            cout << "출금이 완료되었습니다. 출금 금액: " << long long(amount) << "원, 수수료: " << fee << "원입니다." << endl;
            cout << "인출된 지폐: ";
            for (int i = 0; i < 4; i++) {
                if (withdrawalBills[i] > 0) {
                    cout << denominationValues[i] << "원권 " << withdrawalBills[i] << "장 ";
                }
            }
            account->plustransactionID();
            cout << endl;
        }
    }
    else {
        if (currentLanguage == Language::English) {
            cout << "Insufficient funds in the account." << endl;
        }
        else {
            cout << "계좌에 충분한 금액이 없습니다." << endl;
        }
        return;
    }
    string message;
    message = "Account number:" + to_string(account->getAccNumber()) + " User Name: " + account->getAccName() + " -> ID: " + to_string(account->gettransactionID()) + " -> withdrawal " + to_string(amount) + "\n";
    history += message;
    ofstream outfile;
    outfile.open(filePath, ios_base::app);
    outfile << message;
    recordTransaction(message);
    showCashInventory();
}



void ATM::transfer() {
    if (!currentSession) {
        if (currentLanguage == Language::English) {
            cout << "No active session." << endl;
        }
        else {
            cout << "활성 세션이 없습니다." << endl;
        }
        return;
    }
    if (currentLanguage == Language::English) {
        cout << "Choose the transfer type." << endl;
        cout << "1. Account Transfer 2. Cash Transfer 3. Cancel" << endl;
    }
    else {
        cout << "송금 유형을 고르시오." << endl;
        cout << "1.계좌 송금 2.현금 송금 3.취소" << endl;
    }
    int transferType;
    cin >> transferType;

    if (transferType != 1 && transferType != 2) {
        if (currentLanguage == Language::English) {
            cout << "Transfer canceled." << endl;
        }
        else {
            cout << "송금이 취소되었습니다." << endl;
        }
        return;
    }

    Bank* transferBank = nullptr;
    Account* transferAccount = nullptr;
    double fee = 0, total = 0;

    // 은행 선택
    if (currentLanguage == Language::English) {
        cout << "Choose the bank to transfer to." << endl;
    }
    else {
        cout << "송금하려는 은행을 입력하시오" << endl;
    }
    for (size_t i = 0; i < banks.size(); i++) {
        cout << i + 1 << ". " << banks[i]->getbankname() << endl;
    }
    int bankChoice;
    cin >> bankChoice;

    if (bankChoice < 1 || bankChoice > banks.size()) {
        if (currentLanguage == Language::English) {
            cout << "Invalid bank choice." << endl;
        }
        else {
            cout << "잘못된 은행 선택입니다." << endl;
        }
        return;
    }
    transferBank = banks[bankChoice - 1];

    // 계좌 번호 입력 및 확인
    if (currentLanguage == Language::English) {
        cout << "Enter the recipient's account number." << endl;
    }
    else {
        cout << "받는 사람의 계좌 번호를 입력해주세요." << endl;
    }
    long long recipientAccountNumber;
    cin >> recipientAccountNumber;


    transferAccount = transferBank->getAccount(recipientAccountNumber);
    if (!transferAccount) {
        if (currentLanguage == Language::English) {
            cout << "Invalid account number." << endl;
        }
        else {
            cout << "잘못된 계좌번호 입니다." << endl;
        }
        return;
    }

    if (transferType == 1) {
        if (recipientAccountNumber == currentSession->getActiveAccount()->getAccNumber()) {
            if (currentLanguage == Language::English) {
                cout << "Cannot transfer to the same account." << endl;
            }
            else {
                cout << "같은 계좌로 송금할 수 없습니다" << endl;
            }
            return;
        }
        Account* sourceAccount = currentSession->getActiveAccount();
        if (currentLanguage == Language::English) {
            cout << "Enter the amount to transfer: ";
        }
        else {
            cout << "송금할 금액을 입력하시오: ";
        }
        int transferAmount;
        cin >> transferAmount;

        int fee = calculateFee(sourceAccount->getBank(), transferAccount->getBank());
        if (transferAmount + fee > sourceAccount->getBalance()) {
            if (currentLanguage == Language::English) {
                cout << "Insufficient funds in the account." << endl;
            }
            else {
                cout << "계좌 잔액이 부족합니다." << endl;
            }
            return;
        }
        if (currentLanguage == Language::English) {
            cout << "Transfer fee: " << fee << " won\n";
        }
        else {
            cout << "송금 수수료: " << fee << "원\n";
        }

        //송금
        sourceAccount->withdraw(transferAmount + fee);
        transferAccount->deposit(transferAmount);
        Transaction trans("Transfer(Account)", transferAmount, sourceAccount);
        currentSession->addTransaction(trans);
        if (currentLanguage == Language::English) {
            cout << "Account transfer successful." << endl;
            cout << "Transfer amount: " << long long(transferAmount) << " won, " << "Balance: " << sourceAccount->getBalance() << " won" << endl;
            sourceAccount->plustransactionID();
        }
        else {
            cout << "계좌 송금이 완료되었습니다." << endl;
            cout << "송금 금액: " << long long(transferAmount) << "원, " << "잔액: " << sourceAccount->getBalance() << "원" << endl;
            sourceAccount->plustransactionID();
        }
        string message;
        message = "Account number: " + to_string(currentSession->getActiveAccount()->getAccNumber()) + " User Name: " + currentSession->getActiveAccount()->getAccName() + " -> ID: " + to_string(sourceAccount->gettransactionID()) + " -> transfer: " + to_string(transferAmount) + " To " + to_string(recipientAccountNumber) + "\n";
        history += message;
        ofstream outfile;
        outfile.open(filePath, ios_base::app);
        outfile << message;
        outfile.close();
        recordTransaction(message);

    }

    else if (transferType == 2) { // 현금 송금
        if (recipientAccountNumber == currentSession->getActiveAccount()->getAccNumber()) {
            if (currentLanguage == Language::Korean) {
                cout << "입금 메뉴로 수행해 주세요." << endl; //active account와 현금송금 계좌가 같음.
                return;
            }
            else {
                cout << "Cannot transfer to the same account." << endl;
                return;
            }
        }
        int denominations[4] = { 0 };
        const int denominationValues[4] = { 50000, 10000, 5000, 1000 };
        double totalAmount = 0;
        int totalBills = 0;

        do {
            for (int i = 0; i < 4; i++) {
                if (currentLanguage == Language::English) {
                    cout << "Enter the number of " << denominationValues[i] << " won bills: ";
                }
                else {
                    cout << denominationValues[i] << "원권을 입력하시오: ";
                }
                std::cin >> denominations[i];
                totalBills += denominations[i];
                totalAmount += denominations[i] * denominationValues[i];
            }

            if (totalBills > MAX_CASH_INPUT) {
                if (currentLanguage == Language::English) {
                    cout << "The cash you entered is too much. Maximum: " << MAX_CASH_INPUT << " bills can be transferred at once. Please try again." << endl;
                }
                else {
                    cout << "입금하신 현금이 너무 많습니다. 한번에 최대: " << MAX_CASH_INPUT << "장만 송금 가능합니다. 다시 입력해주세요." << endl;
                }
            }
        } while (totalBills > MAX_CASH_INPUT);

        int fee = 1000;
        if (currentLanguage == Language::English) {
            cout << "Transfer fee: " << fee << " won\n";
        }
        else {
            cout << "송금 수수료: " << fee << "원\n";
        }
        int totalFee = 0;
        int additionalFee = 0;

        // 수수료를 사용자로부터 받기
        while (totalFee < fee) {
            if (currentLanguage == Language::English) {
                cout << "Please insert the fee. (" << totalFee << "/" << fee << ")\n";
            }
            else {
                cout << "수수료를 넣어주세요. (" << totalFee << "/" << fee << ")\n";
            }
            std::cin >> additionalFee;

            if (totalFee + additionalFee > fee) {
                if (currentLanguage == Language::English) {
                    cout << "The fee you entered exceeds the required fee. Returning the most recently entered " << additionalFee << " won." << endl;
                }
                else {
                    cout << "입력하신 수수료가 초과되었습니다. " << "가장 최근에 입력한 " << additionalFee << "원을 반환합니다." << endl;
                }
            }
            else {
                totalFee += additionalFee;
            }
        }
        this->cashInventory[3] += fee/1000;
        atmCashBalance += fee;

        if (currentLanguage == Language::English) {
            cout << "Fee: " << totalFee << " won\n";
            cout << "Deposit amount: " << long long(totalAmount) << endl;
        }
        else {
            cout << "수수료: " << totalFee << "원\n";
            cout << "입금 금액: " << long long(totalAmount) << endl;
        }

        if (totalAmount > 0) {
            transferAccount->deposit(totalAmount); // 현금 송금
            atmCashBalance += totalAmount;

            // 현금 재고 업데이트
            const int denominationValues[4] = { 50000, 10000, 5000, 1000 };
            for (int i = 0; i < 4; i++) {
                cashInventory[i] += denominations[i];
            }

            Transaction trans("Transfer(Cash)", totalAmount, transferAccount);
            currentSession->addTransaction(trans);
            transferAccount->plustransactionID();
            if (currentLanguage == Language::English) {
                cout << "Cash transfer successful." << endl;
            }
            else {
                cout << "현금 송금이 완료되었습니다." << endl;
            }
        }
        string message;
        message = "Account number: " + to_string(currentSession->getActiveAccount()->getAccNumber()) + " User Name: " + currentSession->getActiveAccount()->getAccName() + " -> ID: " + to_string(transferAccount->gettransactionID()) + " -> transfer " + to_string(totalAmount) + " To " + to_string(recipientAccountNumber) + "\n";
        history += message;
        ofstream outfile;
        outfile.open(filePath, ios_base::app);
        outfile << message;
        outfile.close();
        recordTransaction(message);
    }

    else {
        if (currentLanguage == Language::English) {
            cout << "Please choose 1. Account Transfer 2. Cash Transfer 3. Cancel" << endl;
        }
        else {
            std::cout << "입금 유형을 1.계좌 송금 2.현금 송금 3.취소 중 하나를 골라주세요." << endl;
        }
        return;
    }

    return;
}


class SessionManager {
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
    void runAdminMenu(ATM* atm);

private:
    void cleanup();
};

void SessionManager::runAdminMenu(ATM* atm) {
    if (!atm) {
        if (atm->getLanguage() == Language::English) {
            std::cout << "No ATM available for admin menu." << std::endl;
        }
        else {
            std::cout << "관리자 메뉴를 위한 ATM이 없습니다." << std::endl;
        }
        return;
    }

    while (true) {
        if (atm->getLanguage() == Language::English) {
            std::cout << "\nAdmin Menu:\n";
            std::cout << "1. View Transaction History\n";
            std::cout << "2. Save Transaction History to File\n";
            std::cout << "3. Exit Admin Menu\n";
            std::cout << "Choice: ";
        }
        else {
            std::cout << "\n관리자 메뉴:\n";
            std::cout << "1. 거래 내역 보기\n";
            std::cout << "2. 거래 내역 파일로 저장\n";
            std::cout << "3. 관리자 메뉴 종료\n";
            std::cout << "선택: ";
        }

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            atm->displayTransactionHistory();
            break;
        case 2:
            atm->saveTransactionHistoryToFile("transaction_history.txt");
            break;
        case 3:
            return;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}
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
    int initialBalance, password;
    long long accountNumber;
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

        if (accountNumber < 100000000000 || accountNumber > 999999999999) {
            std::cout << "Account number must be 12 digits.\n";
            continue;
        }
        break;
    } while (true);

    try {
        auto t = banks[bankIndex]->getAccount(accountNumber);
        if (t != nullptr) {
            throw std::runtime_error("Account already exists.");
        }
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << "\n";
        return;
    }
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
        std::cout << i << ": ATM #" << atms[i]->getSerialNumber()
            << " (Bank: " << atms[i]->getPrimaryBank()->getbankname() << ")"
            << (atms[i]->getATMType() == ATMType::MultiBank ? " [Multi-Bank]" : " [Single-Bank]")
            << "\n";
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
    banks[0]->createAccount("Admin", 1111, 1111, 0);
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



int main() {
    SessionManager manager;
    manager.run();
    return 0;
}
