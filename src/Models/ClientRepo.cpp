#include "Models/ClientRepo.h"
#include "Utils/StringUtils.h"
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <iomanip>

namespace BankSystem {
namespace Models {

    // Internal linkage: storage delimiter is encapsulated strictly within this translation unit.
    static const std::string FIELD_DELIMITER = "#//#";

    static std::string FormatBalance(double balance) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << balance;
        return oss.str();
    }

    static std::string SerializeClient(const stClient& client) {
        std::string line;
        line.reserve(128);

        line += client.AccountNumber + FIELD_DELIMITER;
        line += client.PinCode + FIELD_DELIMITER;
        line += client.Name + FIELD_DELIMITER;
        line += client.Phone + FIELD_DELIMITER;
        line += FormatBalance(client.AccountBalance);

        return line;
    }

    static bool DeserializeClient(const std::string& line, stClient& outClient) {
        const std::vector<std::string> fields = Utils::SplitString(line, FIELD_DELIMITER);
        if (fields.size() < 5) {
            return false;
        }

        outClient.AccountNumber = fields.at(0);
        outClient.PinCode = fields.at(1);
        outClient.Name = fields.at(2);
        outClient.Phone = fields.at(3);
        outClient.AccountBalance = std::strtod(fields.at(4).c_str(), NULL);
        outClient.MarkForDelete = false;
        return true;
    }

    std::vector<stClient> LoadClientsFromFile(const std::string& filePath) {
        std::vector<stClient> clients;
        std::ifstream file(filePath.c_str(), std::ios::in);
        if (!file.is_open()) {
            return clients;
        }

        std::string line;
        while (std::getline(file, line)) {
            line = Utils::TrimString(line);
            if (line.empty()) {
                continue;
            }

            stClient client;
            if (DeserializeClient(line, client)) {
                clients.push_back(client);
            }
        }
        // RAII closes file descriptor automatically on scope exit.
        return clients;
    }

    bool SaveClientsToFile(const std::string& filePath, const std::vector<stClient>& clients) {
        std::ofstream file(filePath.c_str(), std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            return false;
        }

        for (std::size_t i = 0; i < clients.size(); ++i) {
            if (!clients.at(i).MarkForDelete) {
                file << SerializeClient(clients.at(i)) << "\n";
            }
        }
        return true;
    }

    bool AppendClientToFile(const std::string& filePath, const stClient& client) {
        std::ofstream file(filePath.c_str(), std::ios::out | std::ios::app);
        if (!file.is_open()) {
            return false;
        }

        file << SerializeClient(client) << "\n";
        return true;
    }

    bool FindClientByAccountNumber(const std::string& accountNumber, const std::vector<stClient>& clients, stClient& outClient) {
        for (std::size_t i = 0; i < clients.size(); ++i) {
            if (clients.at(i).AccountNumber == accountNumber) {
                outClient = clients.at(i);
                return true;
            }
        }
        return false;
    }
// Internal linkage: storage delimiter is encapsulated strictly within this translation unit.
    bool ClientExistsByAccountNumber(const std::string& accountNumber, const std::string& filePath) {
        const std::vector<stClient> clients = LoadClientsFromFile(filePath);
        stClient dummy;
        return FindClientByAccountNumber(accountNumber, clients, dummy);
    }

// Financial transaction operations (v2.0.0): atomic balance mutation and aggregation.
        bool DepositOrWithdrawByAccountNumber(const std::string& accountNumber, double deltaAmount, const std::string& filePath, double& outNewBalance) {
        std::vector<stClient> clients = LoadClientsFromFile(filePath);

        for (std::size_t i = 0; i < clients.size(); ++i) {
            if (clients.at(i).AccountNumber == accountNumber) {
                const double projectedBalance = clients.at(i).AccountBalance + deltaAmount;

                // Prevent negative balance mutations at the persistence boundary.
                if (projectedBalance < 0.0) {
                    return false;
                }

                clients.at(i).AccountBalance = projectedBalance;
                if (SaveClientsToFile(filePath, clients)) {
                    outNewBalance = projectedBalance;
                    return true;
                }
                return false;
            }
        }
        return false;
    }
// Aggregation operation: computes the total balance across all clients in memory.
    double CalculateTotalBalances(const std::vector<stClient>& clients) {
        double total = 0.0;
        for (std::size_t i = 0; i < clients.size(); ++i) {
            total += clients.at(i).AccountBalance;
        }
        return total;
    }


}
}
