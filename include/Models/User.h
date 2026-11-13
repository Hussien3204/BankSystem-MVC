#ifndef USER_H
#define USER_H

#include <string>

namespace BankSystem {
namespace Models {

    // Bitwise permission flags: each capability occupies a distinct binary bit.
    enum enUserPermissions {
        ePermAll          = -1,
        ePermListClients  = 1 << 0, // 1
        ePermAddClient    = 1 << 1, // 2
        ePermDeleteClient = 1 << 2, // 4
        ePermUpdateClient = 1 << 3, // 8
        ePermFindClient   = 1 << 4, // 16
        ePermTransactions = 1 << 5, // 32
        ePermManageUsers  = 1 << 6  // 64
    };

    // Memory-aligned structure: strings (32B) -> int (4B) -> bool (1B) to minimize struct padding.
    struct stUser {
        std::string Username;
        std::string Password;
        int Permissions;
        bool MarkForDelete;

        stUser() : Permissions(0), MarkForDelete(false) {}
    };

    // Pure inline bitwise check: O(1) CPU instruction with zero heap overhead.
    inline bool HasPermission(const stUser& user, enUserPermissions requiredPerm) {
        if (user.Permissions == ePermAll) {
            return true;
        }
        const int mask = static_cast<int>(requiredPerm);
        return (user.Permissions & mask) == mask;
    }

}
}

#endif
