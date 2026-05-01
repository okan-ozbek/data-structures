```
  █████████   █████                       █████     
 ███▒▒▒▒▒███ ▒▒███                       ▒▒███      
▒███    ▒▒▒  ███████    ██████    ██████  ▒███ █████
▒▒█████████ ▒▒▒███▒    ▒▒▒▒▒███  ███▒▒███ ▒███▒▒███ 
 ▒▒▒▒▒▒▒▒███  ▒███      ███████ ▒███ ▒▒▒  ▒██████▒  
 ███    ▒███  ▒███ ███ ███▒▒███ ▒███  ███ ▒███▒▒███ 
▒▒█████████   ▒▒█████ ▒▒████████▒▒██████  ████ █████
 ▒▒▒▒▒▒▒▒▒     ▒▒▒▒▒   ▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒  ▒▒▒▒ ▒▒▒▒▒  
```

## Table of Contents

- [Overview](#overview)
- [Key Concepts](#key-concepts)
  - [LIFO — Last In, First Out](#lifo--last-in-first-out)
  - [Stack as a Deque Wrapper](#stack-as-a-deque-wrapper)
  - [How std::stack Does It](#how-stdstack-does-it)
  - [Why Wrapping a Deque is Correct](#why-wrapping-a-deque-is-correct)
  - [LIFO vs FIFO — Stack vs Queue](#lifo-vs-fifo--stack-vs-queue)
  - [All Operations are O(1)](#all-operations-are-o1)
  - [Emplace with Perfect Forwarding](#emplace-with-perfect-forwarding)
- [Implementation](#implementation)
- [Usage](#usage)

---

## Overview

`Stack<TValue>` is a **Last-In, First-Out (LIFO)** data structure — elements are added at the top and removed from the top. Think of a stack of plates: you put a plate on top, and you take a plate from the top.

| Operation  | Description                              | Time Complexity |
|------------|------------------------------------------|-----------------|
| `Push`     | Insert element at the top (back)         | **O(1)**        |
| `Pop`      | Remove and return element from the top   | **O(1)**        |
| `Front`    | Peek at the bottom element               | **O(1)**        |
| `Back`     | Peek at the top element                  | **O(1)**        |
| `IsEmpty`  | Check if the stack has no elements       | **O(1)**        |
| `Size`     | Get the number of elements               | **O(1)**        |
| `Swap`     | Swap two stacks                          | **O(1)**        |

All operations are O(1) because the `Stack` delegates directly to `Deque<TValue>`, which uses a **circular buffer** internally.

---

## Key Concepts

### LIFO — Last In, First Out

The defining property of a stack is that the element inserted **last** is the element removed **first**:

```
Push(A) → Push(B) → Push(C)

Stack state: [A, B, C]
              ▲           ▲
           bottom        top
           (oldest)    (newest)

Pop() → returns C  ← most recently pushed
Pop() → returns B
Pop() → returns A
```

This contrasts with a **Queue** (FIFO — First In, First Out) where the oldest element is removed first.

**Real-world analogies:**
- A stack of plates — you take from the top
- A browser back button — last visited page is first to go back to
- Function call stack — last function called is first to return
- Undo history — most recent action is undone first

---

### Stack as a Deque Wrapper

The `Stack` class is a **thin wrapper** around `Deque<TValue>`. It owns a private `Deque<TValue> m_deque` field and restricts the interface to only Stack-relevant operations:

```cpp
template<typename TValue>
class Stack {
public:
    void Push(const TValue& item) { m_deque.PushBack(item);   }  // Add to top (back)
    TValue Pop()                  { return m_deque.PopBack(); }  // Remove from top (back)
    TValue& Front() const         { return m_deque.Front();   }  // Peek at bottom
    TValue& Back()  const         { return m_deque.Back();    }  // Peek at top

private:
    Deque<TValue> m_deque;
};
```

Unlike the `Queue`, which uses **opposite ends** of the deque (push back, pop front), the `Stack` uses **only the back end**:

| Stack Operation | Deque Operation | End Used |
|-----------------|-----------------|----------|
| `Push`          | `PushBack`      | Back     |
| `Pop`           | `PopBack`       | Back     |
| `Front`         | `Front`         | Front    |
| `Back`          | `Back`          | Back     |

---

### How std::stack Does It

This design mirrors exactly how the C++ Standard Library implements `std::stack`:

```cpp
// From the C++ Standard Library (simplified):
template<typename T, typename Container = std::deque<T>>
class stack {
public:
    void push(const T& value) { c.push_back(value);   }
    void pop()                { c.pop_back();         }
    T& top()                  { return c.back();      }

protected:
    Container c;  // The underlying container (default: std::deque<T>)
};
```

Just like `std::stack` wraps `std::deque` by default, our `Stack` wraps `dsa::Deque`. The `std::stack` is customizable — you can pass `std::vector` as the underlying container, which is often faster due to simpler memory layout. Our implementation uses `dsa::Deque` directly.

> **Note:** `std::stack<T, std::vector<T>>` is a common real-world choice. `std::vector` has better cache performance for pure stack operations since it never needs `PopFront`. The Deque-backed approach is used here to mirror the standard library default and demonstrate the wrapper pattern.

---

### Why Wrapping a Deque is Correct

A `Deque` supports O(1) operations at **both** ends. A `Stack` only needs O(1) at **one** end:

- **`PushBack`** and **`PopBack`** are both O(1) on the Deque — perfect for a stack
- No wasted interface — the `Stack` exposes only `Push`, `Pop`, `Front`, `Back`
- Memory management, copy, move, and destructor are all handled by the underlying `Deque`

---

### LIFO vs FIFO — Stack vs Queue

The key difference between `Stack` and `Queue` is **which end you pop from**:

| Property    | Queue (FIFO)                            | Stack (LIFO)                          |
|-------------|-----------------------------------------|---------------------------------------|
| Insert      | `PushBack` — adds to back               | `PushBack` — adds to back             |
| Remove      | `PopFront` — removes from front         | `PopBack` — removes from back         |
| Order       | First inserted is first removed         | Last inserted is first removed        |
| Use case    | Job queues, message buffers, BFS        | Undo history, call stacks, DFS        |

Both are backed by the same `Deque<TValue>` — they differ only in **which `Pop` method** is called:

```cpp
// Queue delegates to:
TValue Pop() { return m_deque.PopFront(); }  // FIFO — oldest element

// Stack delegates to:
TValue Pop() { return m_deque.PopBack(); }   // LIFO — newest element
```

---

### All Operations are O(1)

Since every `Stack` operation delegates to `Deque`, and the `Deque` uses a circular buffer:

- **`Push`** → `Deque::PushBack` → places element at `m_back`, increments `m_back` with wrap-around → **O(1)**
- **`Pop`** → `Deque::PopBack` → decrements `m_back`, reads and destroys element → **O(1)**
- **`Front`** / **`Back`** → direct array access → **O(1)**
- **`Swap`** → `Deque::Swap` → swaps two pointers and a few integers → **O(1)**

The only time anything is O(n) is when the underlying `Deque` needs to resize, which is amortized O(1) across many pushes.

---

### Emplace with Perfect Forwarding

`Emplace` constructs an element **directly inside the deque's buffer** using perfect forwarding — no temporary is created:

```cpp
template<typename... TArgs>
void Emplace(TArgs&&... args) {
    m_deque.EmplaceBack(std::forward<TArgs>(args)...);
}
```

vs `Push`, which copies or moves from an already-constructed value:

```cpp
// Push: value is already constructed, then moved in
stack.Push(MyType{5, 10});  // Temporary created, then moved in

// Emplace: constructed directly inside the deque buffer
stack.Emplace(5, 10);       // No temporary — MyType(5, 10) built in-place
```

---

## Implementation

The full implementation lives in: [`include/Stack.h`](../include/Stack.h)
The underlying container is: [`include/Deque.h`](../include/Deque.h)

### Data Flow

```
Stack::Push(value)
    └─► Deque::PushBack(value)
            └─► new (&m_data[m_back]) TValue(value)
            └─► m_back = (m_back + 1) % m_capacity
            └─► ++m_size

Stack::Pop()
    └─► Deque::PopBack()
            └─► m_back = (m_back + m_capacity - 1) % m_capacity
            └─► value = std::move(m_data[m_back])
            └─► m_data[m_back].~TValue()
            └─► --m_size
            └─► return value
```

### Stack vs Queue Data Flow Comparison

```
Queue::Pop()  →  Deque::PopFront()   ←  removes from the FRONT (oldest)
Stack::Pop()  →  Deque::PopBack()    ←  removes from the BACK  (newest)
                      ▲
            Same deque, different operation
```

---

## Usage

```cpp
#include "Stack.h"

int main() {
    dsa::Stack<int> s{};

    // Add elements (LIFO — last in, first out)
    s.Push(10);
    s.Push(20);
    s.Push(30);

    // Peek without removing
    std::cout << s.Back()  << "\n";  // 30 (top / most recent)
    std::cout << s.Front() << "\n";  // 10 (bottom / oldest)

    // Remove elements in reverse insertion order
    std::cout << s.Pop() << "\n";  // 30
    std::cout << s.Pop() << "\n";  // 20
    std::cout << s.Pop() << "\n";  // 10

    // Emplace — construct directly inside the stack
    struct Frame { std::string name; int returnAddress; };
    dsa::Stack<Frame> callStack{};
    callStack.Emplace("main",       0x0000);
    callStack.Emplace("update",     0x1234);
    callStack.Emplace("renderPass", 0x5678);

    // Unwind the call stack (LIFO order)
    while (!callStack.IsEmpty()) {
        callStack.Pop();
    }

    // Check state
    std::cout << s.IsEmpty() << "\n";  // true
    std::cout << s.Size()    << "\n";  // 0

    // Swap two stacks in O(1)
    dsa::Stack<int> s2{};
    s2.Push(99);
    s.Swap(s2);

    std::cout << s.Back() << "\n";  // 99

    return 0;
}
```

