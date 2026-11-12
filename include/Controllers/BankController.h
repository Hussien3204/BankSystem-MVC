#ifndef BANK_CONTROLLER_H
#define BANK_CONTROLLER_H

#include <string>

namespace BankSystem {
namespace Controllers {

    enum enMainMenuOptions {
        eListClients = 1,
        eAddNewClient = 2,
        eDeleteClient = 3,
        eUpdateClient = 4,
        eFindClient = 5,
        eExit = 6
    };

    // Starts the main application event loop using the specified flat-file data source.
    void RunBankSystem(const std::string& dataFilePath);

}
}

#endif
