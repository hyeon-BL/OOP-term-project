// Basic Account class (will be expanded later)
class Account {
private:
    std::string accountNumber;
    std::string ownerName;
    double balance;
    std::string bankName;

public:
    Account(std::string accNum, std::string owner, std::string bank, double initialBalance = 0)
        : accountNumber(accNum), ownerName(owner), bankName(bank), balance(initialBalance) {}

    std::string getAccountNumber() const { return accountNumber; }
    std::string getOwnerName() const { return ownerName; }
    std::string getBankName() const { return bankName; }
    double getBalance() const { return balance; }
};