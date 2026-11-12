#include "Controllers/BankController.h"

int main() {
    const std::string clientsDataPath = "data/Clients.txt";
    BankSystem::Controllers::RunBankSystem(clientsDataPath);
    return 0;
}
