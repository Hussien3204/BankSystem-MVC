#ifndef TRANSACTION_VIEW_H
#define TRANSACTION_VIEW_H

#include <vector>
#include <string>
#include "Models/Client.h"

namespace BankSystem {
namespace Views {

    // Pauses execution specifically within the Transactions sub-system.
    void PauseTransactionsScreen();

    // Renders the Transactions menu and returns validated choice [1..4].
    int ShowTransactionsMenuAndReadChoice();

    // Renders the concise 3-column balances table and total liquidity footer.
    void ShowTotalBalancesTable(const std::vector<Models::stClient>& clients, double totalBalances);

    // Reads a strictly positive monetary amount (> 0.0) for deposit or withdrawal.
    double ReadStrictlyPositiveAmount(const std::string& prompt);

}
}

#endif
