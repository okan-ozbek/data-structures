```aiignore
 █████                                                    █████████                         
▒▒███                                                    ███▒▒▒▒▒███     ███         ███    
 ▒███         ██████   ██████   ████████  ████████      ███     ▒▒▒     ▒███        ▒███    
 ▒███        ███▒▒███ ▒▒▒▒▒███ ▒▒███▒▒███▒▒███▒▒███    ▒███          ███████████ ███████████
 ▒███       ▒███████   ███████  ▒███ ▒▒▒  ▒███ ▒███    ▒███         ▒▒▒▒▒███▒▒▒ ▒▒▒▒▒███▒▒▒ 
 ▒███      █▒███▒▒▒   ███▒▒███  ▒███      ▒███ ▒███    ▒▒███     ███    ▒███        ▒███    
▒███████████▒▒██████ ▒▒████████ █████     ████ █████    ▒▒█████████     ▒▒▒         ▒▒▒     
▒▒▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒ ▒▒▒▒▒     ▒▒▒▒ ▒▒▒▒▒      ▒▒▒▒▒▒▒▒▒                                                                                               
```
**Fundamental data structures, built from scratch in C++**

![C++](https://img.shields.io/badge/C%2B%2B-23-blue?logo=cplusplus&logoColor=white)
![License](https://img.shields.io/badge/License-Copyright-red)
![Status](https://img.shields.io/badge/Status-In%20Progress-yellow)
![Purpose](https://img.shields.io/badge/Purpose-Self%20Learning-brightgreen)

---

## About

This project is a **self-learning initiative** to gain a deeper understanding of the fundamental data structures that power not only C++, but programming as a whole. Every structure in the standard library: vectors, strings, maps, smart pointers, etc. are built on top of core concepts like dynamic memory management, pointer arithmetic, and object lifetimes.

The goal is: **understand the fundementals of data structures in C++**

By implementing each data structure from scratch using raw memory management (`::operator new`, placement new, explicit destructors), this project forces a level of understanding that you can't get from just reading documentation or calling `std::vector::push_back()`.

---

## Use of AI

This project uses **AI as an assistant** not as the author. The code itself is **~90% hand-written**. AI is used when stuck on a concept, to debug tricky issues, or to get unstuck on C++ nuances (template linker errors, move semantics edge cases, etc.).

The **documentation** (README files) is mainly AI-generated, because AI can explain concepts in a more concise and structured way than writing it all out manually and the focus of this project is on the *implementation*, not the write-ups.

> **TL;DR:** I write the code, AI helps me understand and document it.

---

## Data Structures

### Implemented

| # | Structure | std:: Equivalent | Header | Docs |
|---|-----------|-----------------|--------|------|
| 1 | **Vector** | `std::vector` | [`Vector.h`](include/Vector.h) | [📄 VECTOR.md](readme/VECTOR.md) |
| 2 | **String** | `std::string` | [`String.h`](include/String.h) | [📄 STRING.md](readme/STRING.md) |
| 3 | **UniquePointer** | `std::unique_ptr` | [`UniquePointer.h`](include/UniquePointer.h) | [📄 UNIQUE_POINTER.md](readme/UNIQUE_POINTER.md) |
| 4 | **SharedPointer** | `std::shared_ptr` | [`SharedPointer.h`](include/SharedPointer.h) | [📄 SHARED_POINTER.md](readme/SHARED_POINTER.md) |
| 5 | **Deque** | `std::deque` | [`Deque.h`](include/Deque.h) | [📄 DEQUE.md](readme/DEQUE.md) |
| 6 | **Queue** | `std::queue` | [`Queue.h`](include/Queue.h) | [📄 QUEUE.md](readme/QUEUE.md) |
| 7 | **Stack** | `std::stack` | [`Stack.h`](include/Stack.h) | [📄 STACK.md](readme/STACK.md) |

### Upcoming

| # | Structure | Description |
|---|-----------|-------------|
| 8 | **Set** | Unique-element container with fast lookup |
| 9 | **Map** | Key-value associative container |
| 10 | **Linked List** | Node-based sequential container |
| 11 | **Binary Tree** | Hierarchical node-based structure |
| 12 | **Graph** | Vertex and edge-based structure |
| 13 | **Heap** | Priority-based tree structure |

---

## Built With

- **Language:** C++23
- **Build System:** CMake
- **IDE:** CLion
- **Compiler:** MinGW (GCC)

---

## Project Structure

```
data-structures/
├── include/                # All header files
│   ├── Vector.h            # Generic dynamic array
│   ├── VectorIterator.h    # Iterator for Vector
│   ├── String.h            # Custom string class
│   ├── StringIterator.h    # Iterator for String
│   ├── UniquePointer.h     # Exclusive-ownership smart pointer
│   ├── SharedPointer.h     # Reference-counted smart pointer
│   ├── Deque.h             # Double-ended circular buffer queue
│   ├── DequeIterator.h     # Ring-buffer-aware iterator for Deque
│   ├── Queue.h             # FIFO queue (Deque wrapper)
│   └── Stack.h             # LIFO stack (Deque wrapper)
├── readme/                 # Documentation per data structure
│   ├── VECTOR.md
│   ├── STRING.md
│   ├── UNIQUE_POINTER.md
│   ├── SHARED_POINTER.md
│   ├── DEQUE.md
│   ├── QUEUE.md
│   └── STACK.md
├── tests/                  # GTest test files
│   ├── TestVector.cpp
│   ├── TestString.cpp
│   ├── TestUniquePointer.cpp
│   ├── TestSharedPointer.cpp
│   ├── TestDeque.cpp
│   ├── TestQueue.cpp
│   └── TestStack.cpp
├── src/
│   └── main.cpp            # GTest runner
└── README.md               # This file
```

---

## License

Copyright © 2026 Okan Ozbek. All rights reserved.
