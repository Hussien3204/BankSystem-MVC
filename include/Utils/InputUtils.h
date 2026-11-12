#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include <string>

namespace BankSystem {
namespace Utils {

    // Stream-guarded readers to prevent infinite loops on malformed console input.
    int ReadIntInRange(const std::string& prompt, int minVal, int maxVal);
    double ReadPositiveDouble(const std::string& prompt);
    std::string ReadNonEmptyString(const std::string& prompt);
    char ReadYesNo(const std::string& prompt);

}
}

#endif
