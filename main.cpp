#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "atm.h"
#include "bank.h" // Ensure this header file defines the Bank class
#include "account.h"
#include "session_manager.h"
#include <functional>

int main() {
    SessionManager manager;
    manager.run();
    return 0;
}