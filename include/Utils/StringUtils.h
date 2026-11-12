#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

namespace BankSystem {
namespace Utils {

    // Splits a string by delimiter without mutating or copying the original input buffer.
    std::vector<std::string> SplitString(const std::string& source, const std::string& delimiter);

    // Strips leading and trailing whitespace characters safely.
    std::string TrimString(const std::string& source);

}
}

#endif
