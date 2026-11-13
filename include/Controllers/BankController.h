#ifndef BANK_CONTROLLER_H
#define BANK_CONTROLLER_H

#include <string>

namespace BankSystem {
namespace Controllers {

    enum enMainMenuOptions {
        eListClients  = 1,
        eAddNewClient = 2,
        eDeleteClient = 3,
        eUpdateClient = 4,
        eFindClient   = 5,
        eTransactions = 6,
        eManageUsers  = 7,
        eLogout       = 8
    };

    // Launches the authenticated multi-session banking loop using client and user repositories.
    void RunBankSystem(const std::string& clientsFilePath, const std::string& usersFilePath);

}
}

#endif
