#include "Controllers/BankController.h"

int main() {
    const std::string clientsDataPath = "data/Clients.txt";
    const std::string usersDataPath   = "data/Users.txt";

    BankSystem::Controllers::RunBankSystem(clientsDataPath, usersDataPath);
    return 0;
}
