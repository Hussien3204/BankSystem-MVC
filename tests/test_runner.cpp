#include <iostream>
#include <cassert>
#include "Utils/StringUtils.h"

static void TestStringUtils() {
    // Verify record tokenization accuracy and boundary handling.
    const std::string rawRecord = "A101#//#1234#//#Hussien#//#099999#//#5000.50";
    const std::vector<std::string> fields = BankSystem::Utils::SplitString(rawRecord, "#//#");

    assert(fields.size() == 5);
    assert(fields.at(0) == "A101");
    assert(fields.at(2) == "Hussien");
    assert(fields.at(4) == "5000.50");

    // Verify whitespace sanitization edge cases.
    assert(BankSystem::Utils::TrimString("   clean text \n\t ") == "clean text");
    assert(BankSystem::Utils::TrimString("     ") == "");

    std::cout << "[PASS] StringUtils unit tests verified.\n";
}

int main() {
    std::cout << "=== Executing BankSystem Unit Test Suite ===\n";
    TestStringUtils();
    std::cout << "=== All Assertions Passed ===\n";
    return 0;
}
