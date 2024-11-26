#include "session_manager.h"
#include <iostream>

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
        std::cout << "How many banks do you want to create? ";
        std::cin >> numBanks;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear(); // Clear the error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the remaining input in the buffer
            continue;
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
        std::cin >> numATMs;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter numbers only.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
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
        } catch (std::exception& e) {
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
        } catch (std::exception& e) {
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
        } catch (std::exception& e) {
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
        if (typeChoice != 1 && typeChoice != 2) {
            std::cout << "Invalid choice\n";
        }
    } while (typeChoice != 1 && typeChoice != 2);

    ATMType type = (typeChoice == 1) ? ATMType::SingleBank : ATMType::MultiBank;
    
    std::cout << "Support multiple languages? (1: Yes, 0: No): ";
    do {
        try {
            std::cin >> bilingualChoice;
        } catch (std::exception& e) {
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
    } catch (std::exception& e) {
        std::cout << "Invalid input. Please enter numbers only.\n";
        std::cin.clear();
        return;
    }

    while (primaryBankIndex < 0 || primaryBankIndex >= banks.size() || std::cin.fail()) {
        std::cout << "Invalid bank choice\n";
        std::cout << "Please choose a valid bank.\n";
        try {
            std::cin >> primaryBankIndex;
        } catch (std::exception& e) {
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
    }
    
    atms.push_back(new ATM(serialNumber, type, bilingualChoice == 1, 
                          banks[primaryBankIndex], banks));
    std::cout << "ATM created successfully!\n";

    ATM* lastATM = atms.back();
    depositCashToATM(lastATM);
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
    } catch (std::exception& e) {
        std::cout << "Invalid input. Please enter numbers only.\n";
        std::cin.clear();
        return;
    }

    while (bankIndex < 0 || bankIndex >= banks.size() || std::cin.fail()) {
        std::cout << "Invalid bank choice\n";
        std::cout << "Please choose a valid bank.\n";
        try {
            std::cin >> bankIndex;
        } catch (std::exception& e) {
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
    }
    
    std::cout << "Enter account holder name: ";
    try {
        std::cin >> name;
    } catch (std::exception& e) {
        std::cout << "Invalid input. Please enter a valid name.\n";
        std::cin.clear();
        return;
    }
    
    do {
        std::cout << "Enter 12-digit account number (100000000000-999999999999): ";
        try {
            std::cin >> accountNumber;
        } catch (std::exception& e) {
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
    
    std::cout << "Enter initial balance: ";
    do {
        try {
            std::cin >> initialBalance;
        } catch (std::exception& e) {
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
    } while (initialBalance < 0 || std::cin.fail());

    std::cout << "Enter password: ";
    do {
        try {
            std::cin >> password;
        } catch (std::exception& e) {
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
    } while (password < 0 || std::cin.fail());
    
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
        } catch (std::exception& e) {
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
            } catch (std::exception& e) {
                std::cout << "Invalid input. Please enter numbers only.\n";
                std::cin.clear();
            }
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter numbers only.\n";
                continue;
            }
        } while (choice < 1 || choice > 3 || std::cin.fail());
        
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