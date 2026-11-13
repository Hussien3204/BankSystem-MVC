#include "Controllers/UserController.h"
#include "Models/UserRepo.h"
#include "Views/ClientView.h"
#include "Views/UserView.h"
#include <vector>

namespace BankSystem {
namespace Controllers {

    static void EnsureDefaultAdminExists(const std::string& usersFilePath) {
        const std::vector<Models::stUser> users = Models::LoadUsersFromFile(usersFilePath);
        if (users.empty()) {
            Models::stUser defaultAdmin;
            defaultAdmin.Username = "Admin";
            defaultAdmin.Password = "1234";
            defaultAdmin.Permissions = Models::ePermAll;
            Models::AppendUserToFile(usersFilePath, defaultAdmin);
        }
    }

    Models::stUser RunLoginScreen(const std::string& usersFilePath) {
        EnsureDefaultAdminExists(usersFilePath);

        bool loginFailed = false;
        Models::stUser authenticatedUser;

        while (true) {
            Views::ShowLoginHeader(loginFailed);

            std::string username;
            std::string password;
            Views::ReadLoginCredentials(username, password);

            if (Models::AuthenticateUser(username, password, usersFilePath, authenticatedUser)) {
                return authenticatedUser;
            }
            loginFailed = true;
        }
    }

    static void HandleListUsers(const std::string& usersFilePath) {
        const std::vector<Models::stUser> users = Models::LoadUsersFromFile(usersFilePath);
        Views::ShowUserList(users);
    }

    static void HandleAddNewUser(const std::string& usersFilePath) {
        do {
            Views::ShowSectionHeader("Add New User Screen");
            Views::ShowStatusMessage("Adding New User:");

            std::string username = Views::PromptUsername("Enter Username? ");
            while (Models::UserExistsByUsername(username, usersFilePath)) {
                Views::ShowStatusMessage("User with [" + username + "] already exists!", true);
                username = Views::PromptUsername("Enter another Username? ");
            }

            Models::stUser newUser;
            newUser.Username = username;
            Views::ReadUserDetails(newUser);

            if (Models::AppendUserToFile(usersFilePath, newUser)) {
                Views::ShowStatusMessage("User Added Successfully.");
            } else {
                Views::ShowStatusMessage("Failed to save user record!", true);
            }

        } while (Views::ConfirmAction("\nDo you want to add more Users? Y/N? "));
    }

    static void HandleDeleteUser(const std::string& usersFilePath) {
        Views::ShowSectionHeader("Delete Users Screen");

        const std::string username = Views::PromptUsername();

        // Business rule guard: prevent deletion of primary system administrator.
        if (username == "Admin") {
            Views::ShowStatusMessage("You cannot Delete This User.", true);
            return;
        }

        std::vector<Models::stUser> users = Models::LoadUsersFromFile(usersFilePath);
        Models::stUser targetUser;

        if (!Models::FindUserByUsername(username, users, targetUser)) {
            Views::ShowStatusMessage("User with Username (" + username + ") is Not Found!", true);
            return;
        }

        Views::ShowUserCard(targetUser);

        if (Views::ConfirmAction("\nAre you sure you want to delete this User? y/n ? ")) {
            for (std::size_t i = 0; i < users.size(); ++i) {
                if (users.at(i).Username == username) {
                    users.at(i).MarkForDelete = true;
                    break;
                }
            }

            if (Models::SaveUsersToFile(usersFilePath, users)) {
                Views::ShowStatusMessage("User Deleted Successfully.");
            } else {
                Views::ShowStatusMessage("Error saving user changes to file!", true);
            }
        }
    }

    static void HandleUpdateUser(const std::string& usersFilePath) {
        Views::ShowSectionHeader("Update Users Screen");

        const std::string username = Views::PromptUsername();
        std::vector<Models::stUser> users = Models::LoadUsersFromFile(usersFilePath);

        Models::stUser targetUser;
        if (!Models::FindUserByUsername(username, users, targetUser)) {
            Views::ShowStatusMessage("User with Username (" + username + ") is Not Found!", true);
            return;
        }

        Views::ShowUserCard(targetUser);

        if (Views::ConfirmAction("\nAre you sure you want to update this User? y/n ? ")) {
            for (std::size_t i = 0; i < users.size(); ++i) {
                if (users.at(i).Username == username) {
                    Views::ReadUserDetails(users.at(i));
                    break;
                }
            }

            if (Models::SaveUsersToFile(usersFilePath, users)) {
                Views::ShowStatusMessage("User Updated Successfully.");
            } else {
                Views::ShowStatusMessage("Error saving updated user to file!", true);
            }
        }
    }

    static void HandleFindUser(const std::string& usersFilePath) {
        Views::ShowSectionHeader("Find User Screen");

        const std::string username = Views::PromptUsername();
        const std::vector<Models::stUser> users = Models::LoadUsersFromFile(usersFilePath);

        Models::stUser foundUser;
        if (Models::FindUserByUsername(username, users, foundUser)) {
            Views::ShowUserCard(foundUser);
        } else {
            Views::ShowStatusMessage("User with Username (" + username + ") is Not Found!", true);
        }
    }

    void RunManageUsersMenu(const std::string& usersFilePath) {
        bool inManageUsers = true;

        while (inManageUsers) {
            const int rawChoice = Views::ShowManageUsersMenuAndReadChoice();
            const enManageUsersMenuOptions choice = static_cast<enManageUsersMenuOptions>(rawChoice);

            switch (choice) {
                case eListUsers:
                    HandleListUsers(usersFilePath);
                    Views::PauseUsersScreen();
                    break;

                case eAddNewUser:
                    HandleAddNewUser(usersFilePath);
                    Views::PauseUsersScreen();
                    break;

                case eDeleteUser:
                    HandleDeleteUser(usersFilePath);
                    Views::PauseUsersScreen();
                    break;

                case eUpdateUser:
                    HandleUpdateUser(usersFilePath);
                    Views::PauseUsersScreen();
                    break;

                case eFindUser:
                    HandleFindUser(usersFilePath);
                    Views::PauseUsersScreen();
                    break;

                case eUsersToMain:
                    inManageUsers = false;
                    break;
            }
        }
    }

}
}
