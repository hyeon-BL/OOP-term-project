
#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <vector>
#include "bank.h"
#include "atm.h"

class SessionManager {
private:
    std::vector<Bank*> banks;
    std::vector<ATM*> atms;

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
    
private:
    void cleanup();
};

#endif