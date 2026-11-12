#ifndef CLIENT_VIEW_H
#define CLIENT_VIEW_H

#include <vector>
#include <string>
#include "Models/Client.h"

namespace BankSystem {
namespace Views {

    // Clears terminal buffer portably and pauses execution until user acknowledgment.
    void ClearScreen();
    void PauseScreen();

    // Renders primary navigation menu and returns validated user selection [1..6].
    int ShowMainMenuAndReadChoice();

    // Presentation functions: strictly read-only access to model data via const references.
    void ShowSectionHeader(const std::string& title);
    void ShowClientList(const std::vector<Models::stClient>& clients);
    void ShowClientCard(const Models::stClient& client);
    void ShowStatusMessage(const std::string& message, bool isError = false);

    // Form input collectors: gather raw data without touching persistence layer.
    std::string PromptAccountNumber(const std::string& prompt = "Please enter Account Number: ");
    void ReadClientDetails(Models::stClient& outClient);
    bool ConfirmAction(const std::string& prompt);

}
}

#endif
