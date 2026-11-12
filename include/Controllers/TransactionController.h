#ifndef TRANSACTION_CONTROLLER_H
#define TRANSACTION_CONTROLLER_H

#include <string>

namespace BankSystem {
namespace Controllers {

    enum enTransactionMenuOptions {
        eDeposit = 1,
        eWithdraw = 2,
        eTotalBalances = 3,
        eMainMenu = 4
    };

    // Runs the isolated Transactions event loop and returns cleanly to caller on exit.
    void RunTransactionsMenu(const std::string& dataFilePath);

}
}

#endif
