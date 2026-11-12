#include <iostream>
#include <cassert>
#include <cstdio>
#include <cmath>
#include "Utils/StringUtils.h"
#include "Models/ClientRepo.h"

static bool AreDoublesEqual(double a, double b, double epsilon = 0.001) {
    return std::fabs(a - b) < epsilon;
}

static void TestStringUtils() {
    const std::string rawRecord = "A101#//#1234#//#Hussien#//#099999#//#5000.50";
    const std::vector<std::string> fields = BankSystem::Utils::SplitString(rawRecord, "#//#");

    assert(fields.size() == 5);
    assert(fields.at(0) == "A101");
    assert(fields.at(2) == "Hussien");
    assert(fields.at(4) == "5000.50");

    assert(BankSystem::Utils::TrimString("   clean text \n\t ") == "clean text");
    assert(BankSystem::Utils::TrimString("     ") == "");

    std::cout << "[PASS] StringUtils unit tests verified.\n";
}

static void TestClientRepo() {
    const std::string tempFile = "build/test_clients.txt";
    std::remove(tempFile.c_str());

    BankSystem::Models::stClient c1;
    c1.AccountNumber = "A100";
    c1.PinCode = "1111";
    c1.Name = "Ahmad Saleh";
    c1.Phone = "0911111111";
    c1.AccountBalance = 2500.75;

    BankSystem::Models::stClient c2;
    c2.AccountNumber = "A200";
    c2.PinCode = "2222";
    c2.Name = "Sara Ali";
    c2.Phone = "0922222222";
    c2.AccountBalance = 8400.00;

    assert(BankSystem::Models::AppendClientToFile(tempFile, c1) == true);
    assert(BankSystem::Models::AppendClientToFile(tempFile, c2) == true);
    assert(BankSystem::Models::ClientExistsByAccountNumber("A100", tempFile) == true);
    assert(BankSystem::Models::ClientExistsByAccountNumber("A999", tempFile) == false);

    std::vector<BankSystem::Models::stClient> loaded = BankSystem::Models::LoadClientsFromFile(tempFile);
    assert(loaded.size() == 2);
    assert(loaded.at(0).Name == "Ahmad Saleh");
    assert(AreDoublesEqual(loaded.at(1).AccountBalance, 8400.00));

    loaded.at(0).MarkForDelete = true;
    assert(BankSystem::Models::SaveClientsToFile(tempFile, loaded) == true);

    const std::vector<BankSystem::Models::stClient> afterDelete = BankSystem::Models::LoadClientsFromFile(tempFile);
    assert(afterDelete.size() == 1);
    assert(afterDelete.at(0).AccountNumber == "A200");

    std::remove(tempFile.c_str());
    std::cout << "[PASS] ClientRepo persistence & soft-delete tests verified.\n";
}

static void TestTransactions() {
    const std::string tempFile = "build/test_transactions.txt";
    std::remove(tempFile.c_str());

    BankSystem::Models::stClient c1;
    c1.AccountNumber = "T100";
    c1.PinCode = "1234";
    c1.Name = "Rami";
    c1.Phone = "0933333333";
    c1.AccountBalance = 1000.00;

    BankSystem::Models::stClient c2;
    c2.AccountNumber = "T200";
    c2.PinCode = "5678";
    c2.Name = "Nour";
    c2.Phone = "0944444444";
    c2.AccountBalance = 2500.50;

    BankSystem::Models::AppendClientToFile(tempFile, c1);
    BankSystem::Models::AppendClientToFile(tempFile, c2);

    // 1. Verify Deposit (+500 to T100 -> 1500.00)
    double updatedBalance = 0.0;
    assert(BankSystem::Models::DepositOrWithdrawByAccountNumber("T100", 500.00, tempFile, updatedBalance) == true);
    assert(AreDoublesEqual(updatedBalance, 1500.00));

    // 2. Verify Overdraft Guard (-2000 from 1500.00 must be rejected)
    assert(BankSystem::Models::DepositOrWithdrawByAccountNumber("T100", -2000.00, tempFile, updatedBalance) == false);

    // 3. Verify Valid Withdraw (-300 from 1500.00 -> 1200.00)
    assert(BankSystem::Models::DepositOrWithdrawByAccountNumber("T100", -300.00, tempFile, updatedBalance) == true);
    assert(AreDoublesEqual(updatedBalance, 1200.00));

    // 4. Verify Total Balances Aggregation (1200.00 + 2500.50 = 3700.50)
    const std::vector<BankSystem::Models::stClient> clients = BankSystem::Models::LoadClientsFromFile(tempFile);
    const double total = BankSystem::Models::CalculateTotalBalances(clients);
    assert(AreDoublesEqual(total, 3700.50));

    std::remove(tempFile.c_str());
    std::cout << "[PASS] Financial Transactions unit tests verified.\n";
}

int main() {
    std::cout << "=== Executing BankSystem Unit Test Suite (v2.0.0) ===\n";
    TestStringUtils();
    TestClientRepo();
    TestTransactions();
    std::cout << "=== All Assertions Passed ===\n";
    return 0;
}
