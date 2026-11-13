#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include <string>
#include "Models/User.h"

namespace BankSystem {
namespace Controllers {

    enum enManageUsersMenuOptions {
        eListUsers   = 1,
        eAddNewUser  = 2,
        eDeleteUser  = 3,
        eUpdateUser  = 4,
        eFindUser    = 5,
        eUsersToMain = 6
    };

    // Blocks until valid credentials are provided and returns the authenticated user session.
    Models::stUser RunLoginScreen(const std::string& usersFilePath);

    // Runs the isolated Manage Users event loop and unwinds cleanly back to Main Menu.
    void RunManageUsersMenu(const std::string& usersFilePath);

}
}

#endif
