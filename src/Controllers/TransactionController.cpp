#include "Controllers/TransactionController.h"
#include "Models/ClientRepo.h"
#include "Views/ClientView.h"
#include "Views/TransactionView.h"
#include <vector>
#include <sstream>
#include <iomanip>

namespace BankSystem {
namespace Controllers {

    static std::string MoneyToString(double amount) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << amount;
        return oss.str();
    }

    static Models::stClient PromptUntilExistingClient(const std::string& dataFilePath) {
        while (true) {
            const std::string accountNumber = Views::PromptAccountNumber("Please enter AccountNumber? ");
            const std::vector<Models::stClient> clients = Models::LoadClientsFromFile(dataFilePath);

            Models::stClient client;
            if (Models::FindClientByAccountNumber(accountNumber, clients, client)) {
                return client;
            }
            Views::ShowStatusMessage("Client with [" + accountNumber + "] does not exist.", true);
        }
    }

    static void HandleDeposit(const std::string& dataFilePath) {
        Views::ShowSectionHeader("Deposit Screen");

        const Models::stClient client = PromptUntilExistingClient(dataFilePath);
        Views::ShowClientCard(client);

        const double depositAmount = Views::ReadStrictlyPositiveAmount("\nPlease enter deposit amount? ");

        if (Views::ConfirmAction("\nAre you sure you want to perform this transaction? y/n ? ")) {
            double newBalance = 0.0;
            if (Models::DepositOrWithdrawByAccountNumber(client.AccountNumber, depositAmount, dataFilePath, newBalance)) {
                Views::ShowStatusMessage("Amount Deposited Successfully. New Balance is: " + MoneyToString(newBalance));
            } else {
                Views::ShowStatusMessage("Transaction failed due to a storage error!", true);
            }
        }
    }

    static void HandleWithdraw(const std::string& dataFilePath) {
        Views::ShowSectionHeader("Withdraw Screen");

        const Models::stClient client = PromptUntilExistingClient(dataFilePath);
        Views::ShowClientCard(client);

        double withdrawAmount = Views::ReadStrictlyPositiveAmount("\nPlease enter withdraw amount? ");

        // Validate sufficient funds in controller workflow before confirmation.
        while (withdrawAmount > client.AccountBalance) {
            Views::ShowStatusMessage("Amount Exceeds the balance, you can withdraw up to : " + MoneyToString(client.AccountBalance), true);
            withdrawAmount = Views::ReadStrictlyPositiveAmount("Please enter another amount? ");
        }

        if (Views::ConfirmAction("\nAre you sure you want to perform this transaction? y/n ? ")) {
            double newBalance = 0.0;
            if (Models::DepositOrWithdrawByAccountNumber(client.AccountNumber, -withdrawAmount, dataFilePath, newBalance)) {
                Views::ShowStatusMessage("Amount Withdrawn Successfully. New Balance is: " + MoneyToString(newBalance));
            } else {
                Views::ShowStatusMessage("Transaction rejected!", true);
            }
        }
    }

    static void HandleTotalBalances(const std::string& dataFilePath) {
        const std::vector<Models::stClient> clients = Models::LoadClientsFromFile(dataFilePath);
        const double totalBalances = Models::CalculateTotalBalances(clients);
        Views::ShowTotalBalancesTable(clients, totalBalances);
    }

    void RunTransactionsMenu(const std::string& dataFilePath) {
        bool inTransactions = true;

        while (inTransactions) {
            const int rawChoice = Views::ShowTransactionsMenuAndReadChoice();
            const enTransactionMenuOptions choice = static_cast<enTransactionMenuOptions>(rawChoice);

            switch (choice) {
                case eDeposit:
                    HandleDeposit(dataFilePath);
                    Views::PauseTransactionsScreen();
                    break;

                case eWithdraw:
                    HandleWithdraw(dataFilePath);
                    Views::PauseTransactionsScreen();
                    break;

                case eTotalBalances:
                    HandleTotalBalances(dataFilePath);
                    Views::PauseTransactionsScreen();
                    break;

                case eMainMenu:
                    // Exit loop cleanly to unwind stack frame back to BankController.
                    inTransactions = false;
                    break;
            }
        }
    }

}
}
