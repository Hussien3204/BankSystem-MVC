# Bank Client Management System (Procedural MVC)

A modular C++11 command-line banking system built with strict **Procedural MVC Architecture**, zero-OOP design, deterministic memory discipline, and automated unit testing.

---

## Architectural Overview

The codebase strictly separates data persistence, presentation, and application routing into isolated translation units using C++ namespaces and header/source decoupling:

```text
BankSystem-MVC/
├── include/                  # Public interfaces & data contracts (.h)
│   ├── Models/               # stClient memory layout & repository interface
│   ├── Views/                # Console UI rendering & input collection
│   ├── Controllers/          # Menu routing & workflow orchestration
│   └── Utils/                # Non-destructive string & stream utilities
├── src/                      # Encapsulated implementations (.cpp)
├── tests/                    # Assertion-based automated unit test suite
└── data/                     # Flat-file storage (Clients.txt)
```

---

## Core Engineering Highlights

* **Zero-OOP Modular Design:** Uses pure procedural constructs (`struct`, `enum`, `namespace`, and `static` internal linkage) to enforce encapsulation without class overhead.
* **Memory Discipline & Safety:**
  * Struct fields in `stClient` are ordered by alignment size to eliminate internal memory padding.
  * Read-only collections and strings are passed strictly by `const` reference (`const T&`).
  * Bounds-checked element access via `.at()` and pre-allocated buffers (`reserve`) minimize heap reallocations.
  * File descriptors are managed deterministically via **RAII** (`std::ifstream` / `std::ofstream`).
* **Resilient Stream Handling:** Guards `std::cin` against malformed inputs and buffer overflow loops.
* **Compiler Strictness:** Compiles cleanly with zero warnings under `-Wall -Wextra -Werror -Wconversion`.

---

## Core Features

1. **Show Client List:** Formatted tabular view of all registered bank clients.
2. **Add New Client:** Validated client creation with primary-key uniqueness checks on Account Number.
3. **Delete Client:** Safe confirmation card with soft-delete persistence filtering.
4. **Update Client Info:** In-place record modification with immediate disk synchronization.
5. **Find Client:** Fast lookup and detailed client card display.

---

## Build & Execution (PowerShell / MinGW-w64)

### 1. Compile & Launch Application (Standalone Window)
```powershell
g++ -std=c++11 -Wall -Wextra -Werror -Wconversion -Iinclude src/main.cpp src/Utils/StringUtils.cpp src/Utils/InputUtils.cpp src/Models/ClientRepo.cpp src/Views/ClientView.cpp src/Controllers/BankController.cpp -o build/BankSystem.exe

Start-Process .\build\BankSystem.exe
```

### 2. Compile & Run Automated Unit Tests
```powershell
g++ -std=c++11 -Wall -Wextra -Werror -Wconversion -Iinclude tests/test_runner.cpp src/Utils/StringUtils.cpp src/Utils/InputUtils.cpp src/Models/ClientRepo.cpp -o build/RunTests.exe

.\build\RunTests.exe
```

---

## Extensibility Roadmap

The current `BankController` and repository layers are designed as plug-and-play modules, ready for upcoming extensions without modifying existing client logic:
* **Transactions Module:** Deposit, Withdraw, and Total Balances screens.
* **User Management & Permissions:** Role-based access control via bitwise permission masks.
