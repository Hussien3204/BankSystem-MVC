#include "Models/UserRepo.h"
#include "Utils/StringUtils.h"
#include <fstream>
#include <cstdlib>

namespace BankSystem {
namespace Models {

    static const std::string FIELD_DELIMITER = "#//#";

    static std::string SerializeUser(const stUser& user) {
        std::string line;
        line.reserve(64);

        line += user.Username + FIELD_DELIMITER;
        line += user.Password + FIELD_DELIMITER;
        line += std::to_string(user.Permissions);

        return line;
    }

    static bool DeserializeUser(const std::string& line, stUser& outUser) {
        const std::vector<std::string> fields = Utils::SplitString(line, FIELD_DELIMITER);
        if (fields.size() < 3) {
            return false;
        }

        outUser.Username = fields.at(0);
        outUser.Password = fields.at(1);
        outUser.Permissions = static_cast<int>(std::strtol(fields.at(2).c_str(), NULL, 10));
        outUser.MarkForDelete = false;
        return true;
    }

    std::vector<stUser> LoadUsersFromFile(const std::string& filePath) {
        std::vector<stUser> users;
        std::ifstream file(filePath.c_str(), std::ios::in);
        if (!file.is_open()) {
            return users;
        }

        std::string line;
        while (std::getline(file, line)) {
            line = Utils::TrimString(line);
            if (line.empty()) {
                continue;
            }

            stUser user;
            if (DeserializeUser(line, user)) {
                users.push_back(user);
            }
        }
        return users;
    }

    bool SaveUsersToFile(const std::string& filePath, const std::vector<stUser>& users) {
        std::ofstream file(filePath.c_str(), std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            return false;
        }

        for (std::size_t i = 0; i < users.size(); ++i) {
            const stUser& u = users.at(i);
            // System invariant: primary Admin account can never be purged from storage.
            if (!u.MarkForDelete || u.Username == "Admin") {
                file << SerializeUser(u) << "\n";
            }
        }
        return true;
    }

    bool AppendUserToFile(const std::string& filePath, const stUser& user) {
        std::ofstream file(filePath.c_str(), std::ios::out | std::ios::app);
        if (!file.is_open()) {
            return false;
        }

        file << SerializeUser(user) << "\n";
        return true;
    }

    bool FindUserByUsername(const std::string& username, const std::vector<stUser>& users, stUser& outUser) {
        for (std::size_t i = 0; i < users.size(); ++i) {
            if (users.at(i).Username == username) {
                outUser = users.at(i);
                return true;
            }
        }
        return false;
    }

    bool UserExistsByUsername(const std::string& username, const std::string& filePath) {
        const std::vector<stUser> users = LoadUsersFromFile(filePath);
        stUser dummy;
        return FindUserByUsername(username, users, dummy);
    }

    bool AuthenticateUser(const std::string& username, const std::string& password, const std::string& filePath, stUser& outUser) {
        const std::vector<stUser> users = LoadUsersFromFile(filePath);
        stUser candidate;
        if (FindUserByUsername(username, users, candidate) && candidate.Password == password) {
            outUser = candidate;
            return true;
        }
        return false;
    }

}
}
