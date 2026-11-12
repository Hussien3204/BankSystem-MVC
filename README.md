# Bank Client Management System (Procedural MVC)

A modular C++11 command-line banking system built with strict **Procedural MVC Architecture**, zero-OOP design, deterministic memory discipline, and automated unit testing.

---

## Architectural Overview

The codebase isolates persistence, presentation, and routing into separate translation units using namespaces and header/source decoupling:

```text
BankSystem-MVC/
??? include/                  # Public interfaces & data contracts (.h)
?   ??? Models/               # Client memory layout & persistence interface
?   ??? Views/                # Console UI rendering & input collectors
?   ??? Controllers/          # Menu routing & workflow orchestration
?   ??? Utils/                # String manipulation & stream guards
??? src/                      # Concrete implementations (.cpp)
??? tests/                    # Assertion-based automated test runner
??? data/                     # Flat-file database (Clients.txt)
```

---

## Core Engineering Highlights

* **Zero-OOP Modular Architecture:** Implements clean separation via procedural modules (`struct`, `enum`, `namespace`, and `static` internal linkage) without class hierarchy overhead.
* **Memory Safety & Layout Optimization:**
  * Memory alignment in `stClient` orders members to eliminate structure padding.
  * Read-only data passes strictly by `const` reference (`const T&`).
  * Non-destructive string parsing runs in O(N) time with pre-allocated vector capacity (`reserve`).
  * File streams follow **RAII** patterns for deterministic handle lifecycle management.
* **Resilient Stream Handling:** Guards console input against type mismatch and infinite loop errors via `std::cin.clear()` and buffer flushes.
* **Compiler Strictness:** Compiles cleanly with zero warnings under `-Wall -Wextra -Werror -Wconversion`.
* **Automated Unit Testing:** Dedicated test runner validates string splitting, file operations, soft deletion, and balance calculations before deployment.

---

## System Capabilities (v2.0.0)

### 1. Client Management Subsystem
* **Show Client List:** Formatted tabular display of all registered bank clients.
* **Add New Client:** Record creation with duplicate account number validation.
* **Delete Client:** Confirmation card with soft-delete file filtering.
* **Update Client Info:** In-place record modification and disk synchronization.
* **Find Client:** Account lookup with formatted card output.

### 2. Transactions Subsystem
* **Deposit:** Direct balance increment with immediate persistence.
* **Withdraw:** Safe balance deduction with overdraft protection.
* **Total Balances:** Tabular balance summary with global liquidity aggregation.

---

## Build & Run Instructions (PowerShell / MinGW-w64)

### 1. Build and Run Main Application
```powershell
g++ -std=c++11 -Wall -Wextra -Werror -Wconversion -Iinclude src/main.cpp src/Utils/StringUtils.cpp src/Utils/InputUtils.cpp src/Models/ClientRepo.cpp src/Views/ClientView.cpp src/Views/TransactionView.cpp src/Controllers/BankController.cpp src/Controllers/TransactionController.cpp -o build/BankSystem.exe

Start-Process .\build\BankSystem.exe
```

### 2. Build and Run Test Suite
```powershell
g++ -std=c++11 -Wall -Wextra -Werror -Wconversion -Iinclude tests/test_runner.cpp src/Utils/StringUtils.cpp src/Utils/InputUtils.cpp src/Models/ClientRepo.cpp -o build/RunTests.exe

.\build\RunTests.exe
```
