#ifndef CLIENT_H
#define CLIENT_H

#include <string>

namespace BankSystem {
namespace Models {

    // Memory-aligned structure: 8-byte aligned members placed before 1-byte bool to minimize padding.
    struct stClient {
        std::string AccountNumber;
        std::string PinCode;
        std::string Name;
        std::string Phone;
        double AccountBalance;
        bool MarkForDelete;

        stClient() : AccountBalance(0.0), MarkForDelete(false) {}
    };

}
}

#endif
