```
    ██████                                           
  ███▒▒▒▒███                                         
 ███    ▒▒███ █████ ████  ██████  █████ ████  ██████ 
▒███     ▒███▒▒███ ▒███  ███▒▒███▒▒███ ▒███  ███▒▒███
▒███   ██▒███ ▒███ ▒███ ▒███████  ▒███ ▒███ ▒███████ 
▒▒███ ▒▒████  ▒███ ▒███ ▒███▒▒▒   ▒███ ▒███ ▒███▒▒▒  
 ▒▒▒██████▒██ ▒▒████████▒▒██████  ▒▒████████▒▒██████ 
   ▒▒▒▒▒▒ ▒▒   ▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒    ▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒    
```

## Table of Contents

- [Overview](#overview)
- [Key Concepts](#key-concepts)
  - [FIFO — First In, First Out](#fifo--first-in-first-out)
  - [Queue as a Deque Wrapper](#queue-as-a-deque-wrapper)
  - [How std::queue Does It](#how-stdqueue-does-it)
  - [Why Wrapping a Deque is Correct](#why-wrapping-a-deque-is-correct)
  - [All Operations are O(1)](#all-operations-are-o1)
  - [Emplace with Perfect Forwarding](#emplace-with-perfect-forwarding)
- [Implementation](#implementation)
- [Usage](#usage)

---

## Overview

`Queue<TValue>` is a **First-In, First-Out (FIFO)** data structure — elements are added at the back and removed from the front. Think of a real-world queue (a line of people): the first person to join is the first person to leave.

| Operation  | Description                              | Time Complexity |
|------------|------------------------------------------|-----------------|
| `Push`     | Insert element at the back               | **O(1)**        |
| `Pop`      | Remove and return element from the front | **O(1)**        |
| `Front`    | Peek at the front element                | **O(1)**        |
| `Back`     | Peek at the back element                 | **O(1)**        |
| `IsEmpty`  | Check if the queue has no elements       | **O(1)**        |
| `Size`     | Get the number of elements               | **O(1)**        |
| `Swap`     | Swap two queues                          | **O(1)**        |

All operations are O(1) because the `Queue` delegates directly to `Deque<TValue>`, which uses a **circular buffer** internally.

---

## Key Concepts

### FIFO — First In, First Out

The defining property of a queue is that the element that was inserted **first** is the element that is removed **first**:

```
Push(A) → Push(B) → Push(C)

Queue state: [A, B, C]
              ▲           ▲
            front        back
            (oldest)   (newest)

Pop() → returns A
Pop() → returns B
Pop() → returns C
```

This contrasts with a **Stack** (which is LIFO — Last In, First Out) where the newest element is removed first.

**Real-world analogies:**
- A printer job queue — first job sent is first job printed
- A ticket line — first person in line is first person served
- A message buffer — first message received is first message processed

---

### Queue as a Deque Wrapper

The `Queue` class is a **thin wrapper** around `Deque<TValue>`. It owns a private `Deque<TValue> m_deque` field and restricts the interface to only Queue-relevant operations:

```cpp
template<typename TValue>
class Queue {
public:
    void Push(const TValue& item) { m_deque.PushBack(item);    }  // Add to back
    TValue Pop()                  { return m_deque.PopFront(); }  // Remove from front
    TValue& Front() const         { return m_deque.Front();    }  // Peek at front
    TValue& Back()  const         { return m_deque.Back();     }  // Peek at back

private:
    Deque<TValue> m_deque;
};
```

The `Deque` handles all the memory management, resizing, and circular buffer logic. The `Queue` simply defines **which ends are used**:

| Queue Operation | Deque Operation    | End Used    |
|-----------------|--------------------|-------------|
| `Push`          | `PushBack`         | Back        |
| `Pop`           | `PopFront`         | Front       |
| `Front`         | `Front`            | Front       |
| `Back`          | `Back`             | Back        |

---

### How std::queue Does It

This design mirrors exactly how the C++ Standard Library implements `std::queue`:

```cpp
// From the C++ Standard Library (simplified):
template<typename T, typename Container = std::deque<T>>
class queue {
public:
    void push(const T& value) { c.push_back(value);   }
    void pop()                { c.pop_front();        }
    T& front()                { return c.front();     }
    T& back()                 { return c.back();      }

protected:
    Container c;  // The underlying container (default: std::deque<T>)
};
```

Just like `std::queue` wraps `std::deque` by default, our `Queue` wraps `dsa::Deque`. The `std::queue` is even customizable — you can swap `std::deque` for `std::list` as the underlying container. Our implementation uses `dsa::Deque` directly.

---

### Why Wrapping a Deque is Correct

A `Deque` supports O(1) operations at **both** ends — but a `Queue` only needs O(1) at one end for insertion and O(1) at the other for removal. Using a `Deque` as the backing store means:

- No wasted operations — both `PushBack` and `PopFront` are already O(1) on the Deque
- No custom memory management needed in `Queue` — the Deque handles it all
- Copy, move, and destructor are all implicitly correct — the Deque handles them

An alternative would be a singly-linked list, but that incurs heap allocations per element and poor cache performance. A circular-buffer Deque gives **amortized O(1) push/pop with contiguous memory** — much better cache behavior.

---

### All Operations are O(1)

Since every `Queue` operation delegates to `Deque`, and the `Deque` uses a circular buffer:

- **`Push`** → `Deque::PushBack` → places element at `m_back`, increments `m_back` with wrap-around → **O(1)**
- **`Pop`** → `Deque::PopFront` → reads element at `m_front`, increments `m_front` with wrap-around → **O(1)**
- **`Front`** / **`Back`** → direct array access → **O(1)**
- **`Swap`** → `Deque::Swap` → swaps two pointers and a few integers → **O(1)**

The only time anything is O(n) is when the underlying `Deque` needs to resize (allocate more memory), which is amortized O(1) across many pushes.

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
// Push: value is already constructed, then copied into the deque
queue.Push(MyType{5, 10});     // Temporary created, then moved in

// Emplace: constructed directly inside the deque buffer
queue.Emplace(5, 10);          // No temporary — MyType(5, 10) built in-place
```

---

## Implementation

The full implementation lives in: [`include/Queue.h`](../include/Queue.h)  
The underlying container is: [`include/Deque.h`](../include/Deque.h)

### Data Flow

```
Queue::Push(value)
    └─► Deque::PushBack(value)
            └─► new (&m_data[m_back]) TValue(value)
            └─► m_back = (m_back + 1) % m_capacity
            └─► ++m_size

Queue::Pop()
    └─► Deque::PopFront()
            └─► value = std::move(m_data[m_front])
            └─► m_data[m_front].~TValue()
            └─► m_front = (m_front + 1) % m_capacity
            └─► --m_size
            └─► return value
```

---

## Usage

```cpp
#include "Queue.h"

int main() {
    dsa::Queue<int> q{};

    // Add elements (FIFO — first in, first out)
    q.Push(10);
    q.Push(20);
    q.Push(30);

    // Peek without removing
    std::cout << q.Front() << "\n";  // 10 (oldest)
    std::cout << q.Back()  << "\n";  // 30 (newest)

    // Remove elements in insertion order
    std::cout << q.Pop() << "\n";  // 10
    std::cout << q.Pop() << "\n";  // 20
    std::cout << q.Pop() << "\n";  // 30

    // Emplace — construct directly inside the queue
    struct Task { int priority; std::string name; };
    dsa::Queue<Task> tasks{};
    tasks.Emplace(1, "Render frame");
    tasks.Emplace(2, "Update physics");

    // Check state
    std::cout << q.IsEmpty() << "\n";  // true
    std::cout << q.Size()    << "\n";  // 0

    // Swap two queues in O(1)
    dsa::Queue<int> q2{};
    q2.Push(99);
    q.Swap(q2);

    std::cout << q.Front() << "\n";  // 99

    return 0;
}
```

