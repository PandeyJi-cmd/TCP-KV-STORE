# TCP-KEY-VALUE-Store

A thread-safe, concurrent Key-Value store built from scratch in C++20, utilizing POSIX sockets for network communication and modern C++.

## Technical Overview
This project was built to explore the challenges of building high-concurrency network services in C++. It simulates the core functionality of a Redis-like server without external dependencies.

### Key Engineering Features
*   **Networking Layer:** Implemented using low-level POSIX socket APIs (`socket`, `bind`, `listen`, `accept`).
*   **Concurrency:** Leverages C++20 `std::jthread` to handle multiple concurrent client connections, ensuring automatic thread cleanup and preventing resource leaks.
*   **Thread Safety:** Designed with a Reader-Writer lock pattern using `std::shared_mutex`. This maximizes throughput by allowing multiple concurrent `GET` operations while enforcing exclusive access for `SET` operations.

## Getting Started

### Prerequisites
*   GCC 13+ (or any C++20 compliant compiler)
*   Make/G++

### Compilation
```bash
g++ -std=c++20 Server.cpp Database.cpp -o server -pthread
./server  
