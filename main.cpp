#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>



using namespace std;

// Forward declarations
class Card;
class Bank;
class ATM;
class Transaction;
class BankCustomer;

class Transaction {
public:
    using Date = std::string;

    Transaction(const std::string& details, Date date, double amount)
        : customerDetails(details), date(date), amount(amount) {}

    double getAmount() const {
        return amount;
    }

private:
    std::string customerDetails;
    Date date;
    double amount;
};

class Card {
public:
    using Pin = int;
    using CardId = std::string;

    Card(const CardId& id, Pin pin) : cardId(id), pin(pin) {}

    const CardId& getCardID() const {
        return cardId;
    }

    Pin getPIN() const {
        return pin;
    }

private:
    CardId cardId;
    Pin pin;
};

class Bank {
public:
    using BankName = std::string;
    using Card = std::string;
    using CardSet = std::set<Card>;

    Bank(BankName name, CardSet cards, std::string branch) : bankName(name), validCards(cards), branch(branch) {}

    bool acceptCard(const Card& card) const {
        return validCards.find(card) != validCards.end();
    }

private:
    BankName bankName;
    CardSet validCards;
    std::string branch;
};

class BankCustomer {
public:
    using AccountId = int;
    using Amount = double;



 BankCustomer(AccountId id, const Bank::CardSet& cards, Amount balance, string name, string type)
        : accountId(id), cards(cards), balance(balance), pinEntered(false), name(name), type(type) {}

    void enterPin() {
        pinEntered = true;
    }

    void insertCard(const Card& card) {
        cards.insert(card.getCardID());
    }

    Amount getBalance() const {
        return balance;
    }

    void showTotalBalance() const {
        cout << "Total Balance: $" << balance << endl;
    }

    void withdraw(AccountId currentAccountId, Amount amount) {
        if (pinEntered && balance >= amount) {
            balance -= amount;
            cout << "Withdrawal of $" << amount << " successful. Remaining balance: $" << balance << endl;
            transactions.push_back(Transaction("Withdrawal", getCurrentDate(), amount));
        } else {
            cout << "Withdrawal failed. Insufficient balance." << endl;
        }
    }

    void deposit(AccountId currentAccountId, Amount amount) {
        if (pinEntered) {
            balance += amount;
            cout << "Deposit of $" << amount << " successful. New balance: $" << balance << endl;
            transactions.push_back(Transaction("Deposit", getCurrentDate(), amount));
        } else {
            cout << "Deposit failed. PIN not entered." << endl;
        }
    }

    void showdata() {
        cout << "Name: " << name << endl;
        cout << "Account No: " << accountId  << endl;
        cout << "Account type: " << type << endl;
    }

private:
    AccountId accountId;
    Bank::CardSet cards;
    Amount balance;
    bool pinEntered;
    string name; // Assuming 'name' and 'type' were missing in the original code
    string type;
    std::string getCurrentDate() const {
        // Implement a function to get the current date
        return "2024-01-08";
    }
    std::vector<Transaction> transactions;
};

class ATM {
public:
    ATM(Bank& bank) : currentCard(nullptr), pinCorrect(false), accountID(0), bank(bank) {}

    bool getStatus() const {
        return currentCard != nullptr && pinCorrect;
    }

    void returnCard() {
        currentCard = nullptr;
    }

    bool acceptCard(Card* card) {
        currentCard = card;
        return bank.acceptCard(card->getCardID());
    }

    void insertCard(BankCustomer& customer) {
        if (currentCard != nullptr) {
            customer.insertCard(*currentCard);
        }
    }

private:
    Card* currentCard;
    bool pinCorrect;
    int accountID;
    Bank& bank;
};





int main() {
    // Create Bank, ATM, and other objects
    Bank bank("MyBank", {"Card1", "Card2"}, "MainBranch");
    ATM atm(bank);
    BankCustomer customer(46, {"Card1"}, 10000.0, "Sahil Krishna", "Savings Account");

    cout << "=============================================================================================" << endl;
    cout << "                                WELCOME to SA Bank Limited                                      " << endl;
    cout << "=============================================================================================" << endl;

    bool exitProgram = false;
    const int maxAttempts = 3;

    while (!exitProgram) {
        int attempts = 0;

        // Prompt user to insert a card
        char insertCardChoice;
        cout << "Please Insert card? (Y/N): ";
        cin >> insertCardChoice;

        if (toupper(insertCardChoice) == 'Y') {
            // Handle card insertion
            Card card("Card1", 1234);

            if (atm.acceptCard(&card)) {
                // Prompt for PIN entry
                int enteredPIN;

                do {
                    cout << "Enter PIN: ";
                    cin >> enteredPIN;

                    if (enteredPIN == card.getPIN()) {
                        customer.enterPin();
                        cout << "You are successfully verified. " << endl;

                        // Display welcome message
                        cout << "************************ SA Bank Dashboard ************************ " << endl;

                        while (true) {
                            cout << endl
                                 << endl;
                            cout << "Enter Your Choice\n";
                            cout << "\t1. Display Details \n";
                            cout << "\t2. Balance Enquiry\n";
                            cout << "\t3. Deposit Money\n";
                            cout << "\t4. Withdraw Money\n";
                            cout << "\t5. Cancel\n";

                            int choice;
                            cin >> choice;

                            switch (choice) {
                                case 1:
                                    // Details for the customer
                                    customer.showdata();
                                    break;
                                case 2:
                                    cout << "Balance: $" << customer.getBalance() << endl;
                                    break;
                                case 3:
                                    // Deposit functionality
                                    double depositAmount;
                                    cout << "Enter amount to deposit: $";
                                    cin >> depositAmount;
                                    customer.deposit(1, depositAmount);
                                    break;

                                case 4:
                                    // Withdrawal functionality
                                    double withdrawalAmount;
                                    cout << "Enter amount to withdraw: $";
                                    cin >> withdrawalAmount;
                                    customer.withdraw(1, withdrawalAmount);
                                    break;
                                case 5:
                                    cout << "Transaction canceled." << endl;
                                    atm.returnCard();
                                    exitProgram = true;
                                    break;
                                default:
                                    cout << "Invalid choice." << endl;
                            }

                            if (exitProgram) {
                                break;
                            }
                        }
                        break; // Exit the PIN entry loop
                    } else {
                        cout << "Incorrect PIN." << endl;
                        attempts++;
                    }
                } while (attempts < maxAttempts);

                if (attempts == maxAttempts) {
                    cout << "You have made 3 invalid attempts. So your pin is blocked." << endl;
                }

                atm.returnCard();
            } else {
                cout << "Card not accepted by the ATM." << endl;
            }
        } else {
            cout << "No card inserted. Exiting..." << endl;
            break; // Exit the main loop
        }
    }

    return 0;
}

