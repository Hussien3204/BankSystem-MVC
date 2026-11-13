#ifndef USER_REPO_H
#define USER_REPO_H

#include <string>
#include <vector>
#include "Models/User.h"

namespace BankSystem {
namespace Models {

    // Persistence operations for system users (Users.txt).
    std::vector<stUser> LoadUsersFromFile(const std::string& filePath);
    bool SaveUsersToFile(const std::string& filePath, const std::vector<stUser>& users);
    bool AppendUserToFile(const std::string& filePath, const stUser& user);

    // Lookup and authentication helpers.
    bool FindUserByUsername(const std::string& username, const std::vector<stUser>& users, stUser& outUser);
    bool UserExistsByUsername(const std::string& username, const std::string& filePath);
    bool AuthenticateUser(const std::string& username, const std::string& password, const std::string& filePath, stUser& outUser);

}
}

#endif
