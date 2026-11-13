#include "Views/ClientView.h"
#include "Utils/InputUtils.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

namespace BankSystem {
namespace Views {

    void ClearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
    }

    void PauseScreen() {
        std::cout << "\nPress Enter to go back to Main Menu...";
        std::cin.get();
    }
// this is 
      int ShowMainMenuAndReadChoice() {
        ClearScreen();
        std::cout << "===========================================\n";
        std::cout << "\t\tMain Menue Screen\n";
        std::cout << "===========================================\n";
        std::cout << "\t[1] Show Client List.\n";
        std::cout << "\t[2] Add New Client.\n";
        std::cout << "\t[3] Delete Client.\n";
        std::cout << "\t[4] Update Client Info.\n";
        std::cout << "\t[5] Find Client.\n";
        std::cout << "\t[6] Transactions.\n";
        std::cout << "\t[7] Manage Users.\n";
        std::cout << "\t[8] Logout.\n";
        std::cout << "===========================================\n";

        return Utils::ReadIntInRange("Choose what do you want to do? [1 to 8]? ", 1, 8);
    }


    void ShowSectionHeader(const std::string& title) {
        ClearScreen();
        std::cout << "\n-------------------------------------------\n";
        std::cout << "\t" << title << "\n";
        std::cout << "-------------------------------------------\n";
    }

    void ShowClientList(const std::vector<Models::stClient>& clients) {
        ClearScreen();
        std::cout << "\n\t\t\t\tClient List (" << clients.size() << ") Client(s).\n";
        std::cout << "________________________________________________________________________________________________\n\n";
        std::cout << "| " << std::left << std::setw(15) << "Account Number"
                  << "| " << std::left << std::setw(10) << "Pin Code"
                  << "| " << std::left << std::setw(40) << "Client Name"
                  << "| " << std::left << std::setw(12) << "Phone"
                  << "| " << std::left << std::setw(12) << "Balance" << "\n";
        std::cout << "________________________________________________________________________________________________\n\n";

        if (clients.empty()) {
            std::cout << "\t\t\t\tNo Clients Available In the System!\n";
        } else {
            for (std::size_t i = 0; i < clients.size(); ++i) {
                const Models::stClient& c = clients.at(i);
                std::cout << "| " << std::left << std::setw(15) << c.AccountNumber
                          << "| " << std::left << std::setw(10) << c.PinCode
                          << "| " << std::left << std::setw(40) << c.Name
                          << "| " << std::left << std::setw(12) << c.Phone
                          << "| " << std::left << std::setw(12) << std::fixed << std::setprecision(2) << c.AccountBalance << "\n";
            }
        }

        std::cout << "________________________________________________________________________________________________\n";
    }

    void ShowClientCard(const Models::stClient& client) {
        std::cout << "\nThe following are the client details:\n";
        std::cout << "-------------------------------------------\n";
        std::cout << "Account Number  : " << client.AccountNumber << "\n";
        std::cout << "Pin Code        : " << client.PinCode << "\n";
        std::cout << "Name            : " << client.Name << "\n";
        std::cout << "Phone           : " << client.Phone << "\n";
        std::cout << "Account Balance : " << std::fixed << std::setprecision(2) << client.AccountBalance << "\n";
        std::cout << "-------------------------------------------\n";
    }

    void ShowStatusMessage(const std::string& message, bool isError) {
        if (isError) {
            std::cout << "\n[!] " << message << "\n";
        } else {
            std::cout << "\n[+] " << message << "\n";
        }
    }

    std::string PromptAccountNumber(const std::string& prompt) {
        return Utils::ReadNonEmptyString(prompt);
    }

    void ReadClientDetails(Models::stClient& outClient) {
        outClient.PinCode = Utils::ReadNonEmptyString("Enter PinCode: ");
        outClient.Name = Utils::ReadNonEmptyString("Enter Name: ");
        outClient.Phone = Utils::ReadNonEmptyString("Enter Phone: ");
        outClient.AccountBalance = Utils::ReadPositiveDouble("Enter AccountBalance: ");
    }

    bool ConfirmAction(const std::string& prompt) {
        return Utils::ReadYesNo(prompt) == 'y';
    }

}
}
