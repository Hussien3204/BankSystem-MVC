#ifndef CLIENT_REPO_H
#define CLIENT_REPO_H

#include <string>
#include <vector>
#include "Models/Client.h"

namespace BankSystem {
namespace Models {

    // Persistence layer interface: isolates flat-file storage details from Views and Controllers.
    std::vector<stClient> LoadClientsFromFile(const std::string& filePath);
    bool SaveClientsToFile(const std::string& filePath, const std::vector<stClient>& clients);
    bool AppendClientToFile(const std::string& filePath, const stClient& client);

    // Query helpers operating on in-memory collections or persistent storage.
    bool FindClientByAccountNumber(const std::string& accountNumber, const std::vector<stClient>& clients, stClient& outClient);
    bool ClientExistsByAccountNumber(const std::string& accountNumber, const std::string& filePath);

}
}

#endif
