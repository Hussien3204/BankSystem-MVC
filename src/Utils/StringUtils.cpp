#include "Utils/StringUtils.h"

namespace BankSystem {
namespace Utils {

    std::vector<std::string> SplitString(const std::string& source, const std::string& delimiter) {
        std::vector<std::string> tokens;
        if (source.empty() || delimiter.empty()) {
            return tokens;
        }

        // Pre-allocate capacity for standard client record fields to avoid heap reallocations.
        tokens.reserve(6);

        std::size_t startPos = 0;
        std::size_t delimPos = source.find(delimiter);

        // Index-based scanning: O(N) time complexity without destructive string::erase calls.
        while (delimPos != std::string::npos) {
            std::string token = source.substr(startPos, delimPos - startPos);
            if (!token.empty()) {
                tokens.push_back(token);
            }
            startPos = delimPos + delimiter.length();
            delimPos = source.find(delimiter, startPos);
        }

        if (startPos < source.length()) {
            std::string lastToken = source.substr(startPos);
            if (!lastToken.empty()) {
                tokens.push_back(lastToken);
            }
        }

        return tokens;
    }

    std::string TrimString(const std::string& source) {
        const std::string whitespace = " \t\r\n";
        const std::size_t first = source.find_first_not_of(whitespace);
        if (first == std::string::npos) {
            return "";
        }
        const std::size_t last = source.find_last_not_of(whitespace);
        return source.substr(first, (last - first + 1));
    }

}
}
