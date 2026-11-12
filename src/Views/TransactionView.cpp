#include "Views/TransactionView.h"
#include "Views/ClientView.h"
#include "Utils/InputUtils.h"
#include <iostream>
#include <iomanip>

namespace BankSystem {
namespace Views {

    void PauseTransactionsScreen() {
        std::cout << "\nPress Enter to go back to Transactions Menu...";
        std::cin.get();
    }

    int ShowTransactionsMenuAndReadChoice() {
        ClearScreen();
        std::cout << "===========================================\n";
        std::cout << "\t\tTransactions Menue Screen\n";
        std::cout << "===========================================\n";
        std::cout << "\t[1] Deposit.\n";
        std::cout << "\t[2] Withdraw.\n";
        std::cout << "\t[3] Total Balances.\n";
        std::cout << "\t[4] Main Menue.\n";
        std::cout << "===========================================\n";

        return Utils::ReadIntInRange("Choose what do you want to do? [1 to 4]? ", 1, 4);
    }

    void ShowTotalBalancesTable(const std::vector<Models::stClient>& clients, double totalBalances) {
        ClearScreen();
        std::cout << "\n\t\t\t\tBalances List (" << clients.size() << ") Client(s).\n";
        std::cout << "________________________________________________________________________________________________\n\n";
        std::cout << "| " << std::left << std::setw(15) << "Account Number"
                  << "| " << std::left << std::setw(40) << "Client Name"
                  << "| " << std::left << std::setw(12) << "Balance" << "\n";
        std::cout << "________________________________________________________________________________________________\n\n";

        if (clients.empty()) {
            std::cout << "\t\t\t\tNo Clients Available In the System!\n";
        } else {
            for (std::size_t i = 0; i < clients.size(); ++i) {
                const Models::stClient& c = clients.at(i);
                std::cout << "| " << std::left << std::setw(15) << c.AccountNumber
                          << "| " << std::left << std::setw(40) << c.Name
                          << "| " << std::left << std::setw(12) << std::fixed << std::setprecision(2) << c.AccountBalance << "\n";
            }
        }

        std::cout << "________________________________________________________________________________________________\n";
        std::cout << "\n\t\t\t\t   Total Balances = " << std::fixed << std::setprecision(2) << totalBalances << "\n";
    }

    double ReadStrictlyPositiveAmount(const std::string& prompt) {
        while (true) {
            const double amount = Utils::ReadPositiveDouble(prompt);
            if (amount > 0.0) {
                return amount;
            }
            std::cout << "Amount must be strictly greater than zero!\n";
        }
    }

}
}
