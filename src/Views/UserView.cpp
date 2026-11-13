#include "Views/UserView.h"
#include "Views/ClientView.h"
#include "Utils/InputUtils.h"
#include <iostream>
#include <iomanip>

namespace BankSystem {
namespace Views {

    void PauseUsersScreen() {
        std::cout << "\nPress Enter to go back to Manage Users Menu...";
        std::cin.get();
    }

    void ShowLoginHeader(bool showInvalidError) {
        ClearScreen();
        std::cout << "\n-------------------------------------------\n";
        std::cout << "\t\tLogin Screen\n";
        std::cout << "-------------------------------------------\n";
        if (showInvalidError) {
            std::cout << "Invalid Username/Password!\n";
        }
    }

    void ReadLoginCredentials(std::string& outUsername, std::string& outPassword) {
        outUsername = Utils::ReadNonEmptyString("Enter Username? ");
        outPassword = Utils::ReadNonEmptyString("Enter Password? ");
    }

    void ShowAccessDeniedScreen() {
        ClearScreen();
        std::cout << "\n-------------------------------------------\n";
        std::cout << "Access Denied,\n";
        std::cout << "You dont Have Permission To Do this,\n";
        std::cout << "Please Conact Your Admin.\n";
        std::cout << "-------------------------------------------\n";
    }

    int ShowManageUsersMenuAndReadChoice() {
        ClearScreen();
        std::cout << "===========================================\n";
        std::cout << "\t\tManage Users Menue Screen\n";
        std::cout << "===========================================\n";
        std::cout << "\t[1] List Users.\n";
        std::cout << "\t[2] Add New User.\n";
        std::cout << "\t[3] Delete User.\n";
        std::cout << "\t[4] Update User.\n";
        std::cout << "\t[5] Find User.\n";
        std::cout << "\t[6] Main Menue.\n";
        std::cout << "===========================================\n";

        return Utils::ReadIntInRange("Choose what do you want to do? [1 to 6]? ", 1, 6);
    }

    void ShowUserList(const std::vector<Models::stUser>& users) {
        ClearScreen();
        std::cout << "\n\t\t\t\tUsers List (" << users.size() << ") User(s).\n";
        std::cout << "________________________________________________________________________________________________\n\n";
        std::cout << "| " << std::left << std::setw(20) << "User Name"
                  << "| " << std::left << std::setw(15) << "Password"
                  << "| " << std::left << std::setw(15) << "Permissions" << "\n";
        std::cout << "________________________________________________________________________________________________\n\n";

        if (users.empty()) {
            std::cout << "\t\t\t\tNo Users Available In the System!\n";
        } else {
            for (std::size_t i = 0; i < users.size(); ++i) {
                const Models::stUser& u = users.at(i);
                std::cout << "| " << std::left << std::setw(20) << u.Username
                          << "| " << std::left << std::setw(15) << u.Password
                          << "| " << std::left << std::setw(15) << u.Permissions << "\n";
            }
        }

        std::cout << "________________________________________________________________________________________________\n";
    }

    void ShowUserCard(const Models::stUser& user) {
        std::cout << "\nThe following are the user details:\n";
        std::cout << "-------------------------------------------\n";
        std::cout << "Username    : " << user.Username << "\n";
        std::cout << "Password    : " << user.Password << "\n";
        std::cout << "Permissions : " << user.Permissions << "\n";
        std::cout << "-------------------------------------------\n";
    }

    std::string PromptUsername(const std::string& prompt) {
        return Utils::ReadNonEmptyString(prompt);
    }

    int ReadPermissionsMask() {
        if (ConfirmAction("\nDo you want to give full access? y/n? ")) {
            return Models::ePermAll;
        }

        int permissions = 0;
        std::cout << "\nDo you want to give access to :\n";

        if (ConfirmAction("\nShow Client List? y/n? ")) {
            permissions |= Models::ePermListClients;
        }
        if (ConfirmAction("\nAdd New Client? y/n? ")) {
            permissions |= Models::ePermAddClient;
        }
        if (ConfirmAction("\nDelete Client? y/n? ")) {
            permissions |= Models::ePermDeleteClient;
        }
        if (ConfirmAction("\nUpdate Client? y/n? ")) {
            permissions |= Models::ePermUpdateClient;
        }
        if (ConfirmAction("\nFind Client? y/n? ")) {
            permissions |= Models::ePermFindClient;
        }
        if (ConfirmAction("\nTransactions? y/n? ")) {
            permissions |= Models::ePermTransactions;
        }
        if (ConfirmAction("\nManage Users? y/n? ")) {
            permissions |= Models::ePermManageUsers;
        }

        return permissions;
    }

    void ReadUserDetails(Models::stUser& outUser) {
        outUser.Password = Utils::ReadNonEmptyString("Enter Password? ");
        outUser.Permissions = ReadPermissionsMask();
    }

}
}
