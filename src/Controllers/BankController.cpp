#include "Controllers/BankController.h"
#include "Controllers/TransactionController.h"
#include "Controllers/UserController.h"
#include "Models/ClientRepo.h"
#include "Models/UserRepo.h"
#include "Views/ClientView.h"
#include "Views/UserView.h"
#include <vector>

namespace BankSystem {
namespace Controllers {

    static void HandleListClients(const std::string& dataFilePath) {
        const std::vector<Models::stClient> clients = Models::LoadClientsFromFile(dataFilePath);
        Views::ShowClientList(clients);
    }

    static void HandleAddNewClient(const std::string& dataFilePath) {
        do {
            Views::ShowSectionHeader("Add New Client Screen");

            std::string accountNumber = Views::PromptAccountNumber("Enter Account Number: ");
            while (Models::ClientExistsByAccountNumber(accountNumber, dataFilePath)) {
                Views::ShowStatusMessage("Client with [" + accountNumber + "] already exists!", true);
                accountNumber = Views::PromptAccountNumber("Enter another Account Number: ");
            }

            Models::stClient newClient;
            newClient.AccountNumber = accountNumber;
            Views::ReadClientDetails(newClient);

            if (Models::AppendClientToFile(dataFilePath, newClient)) {
                Views::ShowStatusMessage("Client Added Successfully.");
            } else {
                Views::ShowStatusMessage("Failed to write client record to disk!", true);
            }

        } while (Views::ConfirmAction("\nDo you want to add more clients? Y/N? "));
    }

    static void HandleDeleteClient(const std::string& dataFilePath) {
        Views::ShowSectionHeader("Delete Client Screen");

        const std::string accountNumber = Views::PromptAccountNumber();
        std::vector<Models::stClient> clients = Models::LoadClientsFromFile(dataFilePath);

        Models::stClient targetClient;
        if (!Models::FindClientByAccountNumber(accountNumber, clients, targetClient)) {
            Views::ShowStatusMessage("Client with Account Number (" + accountNumber + ") is Not Found!", true);
            return;
        }

        Views::ShowClientCard(targetClient);

        if (Views::ConfirmAction("\nAre you sure you want to delete this client? y/n ? ")) {
            for (std::size_t i = 0; i < clients.size(); ++i) {
                if (clients.at(i).AccountNumber == accountNumber) {
                    clients.at(i).MarkForDelete = true;
                    break;
                }
            }

            if (Models::SaveClientsToFile(dataFilePath, clients)) {
                Views::ShowStatusMessage("Client Deleted Successfully.");
            } else {
                Views::ShowStatusMessage("Error saving changes to file!", true);
            }
        }
    }

    static void HandleUpdateClient(const std::string& dataFilePath) {
        Views::ShowSectionHeader("Update Client Info Screen");

        const std::string accountNumber = Views::PromptAccountNumber();
        std::vector<Models::stClient> clients = Models::LoadClientsFromFile(dataFilePath);

        Models::stClient targetClient;
        if (!Models::FindClientByAccountNumber(accountNumber, clients, targetClient)) {
            Views::ShowStatusMessage("Client with Account Number (" + accountNumber + ") is Not Found!", true);
            return;
        }

        Views::ShowClientCard(targetClient);

        if (Views::ConfirmAction("\nAre you sure you want to update this client? y/n ? ")) {
            for (std::size_t i = 0; i < clients.size(); ++i) {
                if (clients.at(i).AccountNumber == accountNumber) {
                    Views::ReadClientDetails(clients.at(i));
                    break;
                }
            }

            if (Models::SaveClientsToFile(dataFilePath, clients)) {
                Views::ShowStatusMessage("Client Updated Successfully.");
            } else {
                Views::ShowStatusMessage("Error saving updated client to file!", true);
            }
        }
    }

    static void HandleFindClient(const std::string& dataFilePath) {
        Views::ShowSectionHeader("Find Client Screen");

        const std::string accountNumber = Views::PromptAccountNumber();
        const std::vector<Models::stClient> clients = Models::LoadClientsFromFile(dataFilePath);

        Models::stClient foundClient;
        if (Models::FindClientByAccountNumber(accountNumber, clients, foundClient)) {
            Views::ShowClientCard(foundClient);
        } else {
            Views::ShowStatusMessage("Client with Account Number (" + accountNumber + ") is Not Found!", true);
        }
    }

    // Centralized O(1) RBAC gatekeeper before dispatching protected controller actions.
    static bool EnforcePermission(const Models::stUser& currentUser, Models::enUserPermissions requiredPerm) {
        if (Models::HasPermission(currentUser, requiredPerm)) {
            return true;
        }
        Views::ShowAccessDeniedScreen();
        Views::PauseScreen();
        return false;
    }

    void RunBankSystem(const std::string& clientsFilePath, const std::string& usersFilePath) {
        // Outer loop manages authentication sessions cleanly without stack recursion on Logout.
        while (true) {
            Models::stUser currentUser = RunLoginScreen(usersFilePath);
            bool sessionActive = true;

            while (sessionActive) {
                const int rawChoice = Views::ShowMainMenuAndReadChoice();
                const enMainMenuOptions choice = static_cast<enMainMenuOptions>(rawChoice);

                switch (choice) {
                    case eListClients:
                        if (EnforcePermission(currentUser, Models::ePermListClients)) {
                            HandleListClients(clientsFilePath);
                            Views::PauseScreen();
                        }
                        break;

                    case eAddNewClient:
                        if (EnforcePermission(currentUser, Models::ePermAddClient)) {
                            HandleAddNewClient(clientsFilePath);
                            Views::PauseScreen();
                        }
                        break;

                    case eDeleteClient:
                        if (EnforcePermission(currentUser, Models::ePermDeleteClient)) {
                            HandleDeleteClient(clientsFilePath);
                            Views::PauseScreen();
                        }
                        break;

                    case eUpdateClient:
                        if (EnforcePermission(currentUser, Models::ePermUpdateClient)) {
                            HandleUpdateClient(clientsFilePath);
                            Views::PauseScreen();
                        }
                        break;

                    case eFindClient:
                        if (EnforcePermission(currentUser, Models::ePermFindClient)) {
                            HandleFindClient(clientsFilePath);
                            Views::PauseScreen();
                        }
                        break;

                    case eTransactions:
                        if (EnforcePermission(currentUser, Models::ePermTransactions)) {
                            RunTransactionsMenu(clientsFilePath);
                        }
                        break;

                    case eManageUsers:
                        if (EnforcePermission(currentUser, Models::ePermManageUsers)) {
                            RunManageUsersMenu(usersFilePath);
                            // Refresh active user permissions in case current user updated their own role.
                            const std::vector<Models::stUser> updatedUsers = Models::LoadUsersFromFile(usersFilePath);
                            Models::FindUserByUsername(currentUser.Username, updatedUsers, currentUser);
                        }
                        break;

                    case eLogout:
                        sessionActive = false;
                        break;
                }
            }
        }
    }

}
}
