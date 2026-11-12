#include "Utils/InputUtils.h"
#include "Utils/StringUtils.h"
#include <iostream>
#include <limits>
#include <cctype>

namespace BankSystem {
namespace Utils {

    int ReadIntInRange(const std::string& prompt, int minVal, int maxVal) {
        int value = 0;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= minVal && value <= maxVal) {
                // Flush trailing newline from input buffer before next getline call.
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            // Recover stream state and discard corrupted buffer content.
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number between ("
                      << minVal << " and " << maxVal << ").\n";
        }
    }

    double ReadPositiveDouble(const std::string& prompt) {
        double value = 0.0;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= 0.0) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid amount! Please enter a valid non-negative number.\n";
        }
    }

    std::string ReadNonEmptyString(const std::string& prompt) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin >> std::ws, input);
            input = TrimString(input);
            if (!input.empty()) {
                return input;
            }
            std::cout << "Input cannot be empty! Please try again.\n";
        }
    }

    char ReadYesNo(const std::string& prompt) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin >> std::ws, input);
            if (input.length() == 1) {
                // Cast to unsigned char prior to std::tolower to avoid undefined behavior on signed char.
                unsigned char rawChar = static_cast<unsigned char>(input.at(0));
                char ch = static_cast<char>(std::tolower(rawChar));
                if (ch == 'y' || ch == 'n') {
                    return ch;
                }
            }
            std::cout << "Please enter 'y' or 'n' only.\n";
        }
    }

}
}
