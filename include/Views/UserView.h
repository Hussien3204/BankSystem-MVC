#ifndef USER_VIEW_H
#define USER_VIEW_H

#include <string>
#include <vector>
#include "Models/User.h"

namespace BankSystem {
namespace Views {

    // Pauses execution specifically within the Manage Users sub-system.
    void PauseUsersScreen();

    // Renders Login header and collects raw credentials.
    void ShowLoginHeader(bool showInvalidError = false);
    void ReadLoginCredentials(std::string& outUsername, std::string& outPassword);

    // Renders standardized RBAC rejection screen when user lacks required bitmask.
    void ShowAccessDeniedScreen();

    // Renders Manage Users menu and returns validated choice [1..6].
    int ShowManageUsersMenuAndReadChoice();

    // Read-only presentation views for user records.
    void ShowUserList(const std::vector<Models::stUser>& users);
    void ShowUserCard(const Models::stUser& user);

    // Interactive form collectors including bitwise permission builder.
    std::string PromptUsername(const std::string& prompt = "Please enter Username: ");
    int ReadPermissionsMask();
    void ReadUserDetails(Models::stUser& outUser);

}
}

#endif
