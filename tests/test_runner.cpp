#include <iostream>
#include <cassert>
#include <cstdio>
#include "Utils/StringUtils.h"
#include "Models/ClientRepo.h"

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

    // Test Append & Existence Check
    assert(BankSystem::Models::AppendClientToFile(tempFile, c1) == true);
    assert(BankSystem::Models::AppendClientToFile(tempFile, c2) == true);
    assert(BankSystem::Models::ClientExistsByAccountNumber("A100", tempFile) == true);
    assert(BankSystem::Models::ClientExistsByAccountNumber("A999", tempFile) == false);

    // Test Load & Deserialization
    std::vector<BankSystem::Models::stClient> loaded = BankSystem::Models::LoadClientsFromFile(tempFile);
    assert(loaded.size() == 2);
    assert(loaded.at(0).Name == "Ahmad Saleh");
    assert(loaded.at(1).AccountBalance == 8400.00);

    // Test Soft-Delete Filtering on Save
    loaded.at(0).MarkForDelete = true;
    assert(BankSystem::Models::SaveClientsToFile(tempFile, loaded) == true);

    const std::vector<BankSystem::Models::stClient> afterDelete = BankSystem::Models::LoadClientsFromFile(tempFile);
    assert(afterDelete.size() == 1);
    assert(afterDelete.at(0).AccountNumber == "A200");

    // Clean up temporary test artifact
    //std::remove(tempFile.c_str());
    std::cout << "[PASS] ClientRepo persistence & soft-delete tests verified.\n";
}

int main() {
    std::cout << "=== Executing BankSystem Unit Test Suite ===\n";
    TestStringUtils();
    TestClientRepo();
    std::cout << "=== All Assertions Passed ===\n";
    return 0;
}
