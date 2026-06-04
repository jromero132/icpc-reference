# ICPC Team Reference Notebook (C++11)

[![Buy Me a Coffee](https://img.shields.io/badge/Buy%20Me%20a%20Coffee-ffdd00?logo=buy-me-a-coffee&logoColor=black)](<https://buymeacoffee.com/jromero132> "Buy Me a Coffee - jromero132")
[![C++11 Full Badge](https://img.shields.io/badge/C%2B%2B11-00599C?style=flat&labelColor=00599C&logo=c%2B%2B&logoColor=white)](https://en.cppreference.com/w/cpp/11)
[![Tests](https://github.com/jromero132/icpc-reference/actions/workflows/ci.yml/badge.svg?label=Tests)](https://github.com/jromero132/icpc-reference/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg?style=flat-square)](https://opensource.org/licenses/MIT)
![Badge](https://hitscounter.dev/api/hit?url=https%3A%2F%2Fgithub.com%2Fjromero132%2Ficpc-team-reference&amp;label=Visitors&amp;icon=heart-fill&amp;color=%23198754&amp;message=&amp;style=flat&amp;tz=UTC)

A highly optimized collection of core data structures and classic algorithms optimized for
competitive programming contests (ICPC Competitions, Codeforces, TopCoder, etc.). This repository
serves as a permanent digital codebook, refined for fast copy-pasting, minimal constant-factor
overhead, and strict **C++11** standard environments.

---

## ⚡ Design Philosophy & Contest Constraints

- **C++11 Compliance**: Code structures strictly avoid post-C++11 language extensions to ensure
  seamless compilation on older contest servers (e.g., standard judge environments).
- **Stack-Overflow Countermeasures**: Massive structures allocation footprints use heap-backed
  storage models to entirely eliminate stack-exhaustion errors under deep recursion or massive
  limits.
- **Amortized Execution Profiles**: Algorithmic implementations focus heavily on optimizing tight
  loops to maximize execution speed against strict time limits.

## 📁 Repository Layout

```text
├── code/
│   └── data_structures/      # Highly optimized, encapsulated reference headers
├── tests/
│   └── data_structures/      # Comprehensive unit verification boundaries
│   └── doctest.h             # High-speed test framework engine
└── .github/
    └── workflows/
        └── ci.yml            # Automated compilation and validation scripts
