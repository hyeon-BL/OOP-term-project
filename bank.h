// Basic Bank class (will be expanded later)
class Bank {
private:
    std::string name;
    std::vector<std::shared_ptr<Account>> accounts;

public:
    Bank(std::string bankName) : name(bankName) {}

    std::shared_ptr<Account> createAccount(std::string accountNumber, std::string ownerName, double initialBalance = 0) {
        auto account = std::make_shared<Account>(accountNumber, ownerName, name, initialBalance);
        accounts.push_back(account);
        return account;
    }

    std::string getName() const { return name; }
};