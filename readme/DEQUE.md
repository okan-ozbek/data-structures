```
 ██████████                                         
▒▒███▒▒▒▒███                                        
 ▒███   ▒▒███  ██████   ████████ █████ ████  ██████ 
 ▒███    ▒███ ███▒▒███ ███▒▒███ ▒▒███ ▒███  ███▒▒███
 ▒███    ▒███▒███████ ▒███ ▒███  ▒███ ▒███ ▒███████ 
 ▒███    ███ ▒███▒▒▒  ▒███ ▒███  ▒███ ▒███ ▒███▒▒▒  
 ██████████  ▒▒██████ ▒▒███████  ▒▒████████▒▒██████ 
▒▒▒▒▒▒▒▒▒▒    ▒▒▒▒▒▒   ▒▒▒▒▒███   ▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒  
                           ▒███                     
                           █████                    
                          ▒▒▒▒▒                       
```

## Table of Contents

- [Overview](#overview)
- [Key Concepts](#key-concepts)
  - [What is a Circular Buffer?](#what-is-a-circular-buffer)
  - [Why a Circular Buffer for a Deque?](#why-a-circular-buffer-for-a-deque)
  - [Front and Back Indices](#front-and-back-indices)
  - [Wrap-Around with Modulo](#wrap-around-with-modulo)
  - [Why All Operations are O(1)](#why-all-operations-are-o1)
  - [Resizing and Linearization](#resizing-and-linearization)
  - [The Ring-Buffer-Aware Iterator](#the-ring-buffer-aware-iterator)
  - [Raw Memory: `::operator new` and `::operator delete`](#raw-memory-operator-new-and-operator-delete)
  - [Placement New and Explicit Destructors](#placement-new-and-explicit-destructors)
  - [Emplace with Variadic Templates](#emplace-with-variadic-templates)
- [Implementation](#implementation)
- [Usage](#usage)

---

## Overview

`Deque<TValue>` is a **double-ended queue** — a data structure that supports efficient insertion and removal from **both the front and the back**. Unlike a `Vector`, which can only efficiently push and pop at the back, a `Deque` supports:

| Operation    | Description                                 | Time Complexity |
|--------------|---------------------------------------------|-----------------|
| `PushBack`   | Insert element at the back                  | **O(1)**        |
| `PushFront`  | Insert element at the front                 | **O(1)**        |
| `PopBack`    | Remove element from the back                | **O(1)**        |
| `PopFront`   | Remove element from the front               | **O(1)**        |
| `operator[]` | Random access by logical index              | **O(1)**        |
| `Front`      | Access the first element                    | **O(1)**        |
| `Back`       | Access the last element                     | **O(1)**        |
| `Resize`     | Grow or shrink the buffer                   | **O(n)**        |

All of the above O(1) operations are made possible by the **circular buffer** implementation.

---

## Key Concepts

### What is a Circular Buffer?

A circular buffer (also called a ring buffer) is a **fixed-size array where the end wraps around to the beginning**. Instead of treating memory as a straight line, it treats the underlying array as a continuous loop.

```
Physical memory layout (capacity = 8):

   Index:  0     1     2     3     4     5     6     7
         ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
         │  C  │  D  │  E  │     │     │  A  │  B  │  C  │
         └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
                                          ▲           ▲
                                        front        back

Logical order (as seen by the user): [A, B, C, D, E]
```

The data is spread across the end and the beginning of the physical array, but the user always sees a clean, ordered sequence through logical indexing.

---

### Why a Circular Buffer for a Deque?

Without a circular buffer, `PushFront` on a plain array would require **shifting all existing elements** one position to the right — an O(n) operation. Similarly, `PopFront` would require shifting everything left.

With a circular buffer:
- **`PushFront`** simply decrements the `front` index (with wrap-around) and places the element there — **O(1)**.
- **`PushBack`** places the element at `back` and increments the `back` index — **O(1)**.
- **`PopFront`** reads the element at `front`, then increments the `front` index — **O(1)**.
- **`PopBack`** decrements the `back` index, then reads and destroys the element — **O(1)**.

No element ever needs to be shifted. The buffer "rotates" in place.

---

### Front and Back Indices

The deque maintains two indices into the underlying array:

| Index    | Points to                                        |
|----------|--------------------------------------------------|
| `m_front`| The physical index of the **first valid element** |
| `m_back` | The physical index of the **next insertion slot** (one past last) |

```
Initial state (capacity = 10, empty):
   m_front = 0
   m_back  = 0
   m_size  = 0

After PushBack(10), PushBack(20), PushBack(30):
   m_front = 0
   m_back  = 3
   m_size  = 3

   Index:  0     1     2     3     ...
         ┌─────┬─────┬─────┬─────┬─────┐
         │ 10  │ 20  │ 30  │     │ ... │
         └─────┴─────┴─────┴─────┴─────┘
           ▲                 ▲
         front              back

After PopFront():
   m_front = 1
   m_back  = 3
   m_size  = 2

   Index:  0     1     2     3     ...
         ┌─────┬─────┬─────┬─────┬─────┐
         │     │ 20  │ 30  │     │ ... │
         └─────┴─────┴─────┴─────┴─────┘
                 ▲           ▲
               front        back
```

After a `PopFront`, the slot at index 0 is "abandoned" — the front pointer simply moves forward, no shifting needed.

---

### Wrap-Around with Modulo

The key to the circular buffer is **modular arithmetic** to keep indices within bounds:

```cpp
// Move back forward (with wrap-around)
m_back = (m_back + 1) % m_capacity;

// Move front forward (with wrap-around)
m_front = (m_front + 1) % m_capacity;

// Move front backward (with wrap-around for PushFront)
m_front = (m_front + m_capacity - 1) % m_capacity;
```

The `+ m_capacity` before the subtraction prevents unsigned underflow — since `m_front` is `std::size_t` (unsigned), subtracting 1 when it's 0 would wrap to a huge number. Adding `m_capacity` first ensures the result is always positive.

**Example — wrapping around the end:**

```
Capacity = 5, m_back = 4 (pointing to last slot)

After PushBack:
   m_back = (4 + 1) % 5 = 0  ← wraps back to the beginning!

   Index:  0     1     2     3     4
         ┌─────┬─────┬─────┬─────┬─────┐
         │ new │     │     │     │ old │
         └─────┴─────┴─────┴─────┴─────┘
           ▲                             
          back (new insertion point)     
```

**Logical index to physical index:**

When you access element `i` logically (e.g., `dq[2]`), the physical address is:

```cpp
std::size_t physical = (m_front + i) % m_capacity;
return m_data[physical];
```

This means `dq[0]` always refers to the front-most element regardless of where `m_front` physically is.

---

### Why All Operations are O(1)

| Operation    | What happens                                         | Why O(1)                                  |
|--------------|------------------------------------------------------|-------------------------------------------|
| `PushBack`   | Placement new at `m_back`, then `m_back = (m_back + 1) % m_capacity` | No shifting, just index arithmetic |
| `PushFront`  | `m_front = (m_front + m_capacity - 1) % m_capacity`, then placement new | No shifting, just index arithmetic |
| `PopBack`    | Decrement `m_back`, destroy element at new `m_back`  | No shifting, just index arithmetic        |
| `PopFront`   | Read + destroy element at `m_front`, then increment `m_front` | No shifting, just index arithmetic |
| `operator[]` | `(m_front + index) % m_capacity`                    | Single arithmetic operation               |
| `Front`      | `m_data[m_front]`                                    | Direct array access                       |
| `Back`       | `m_data[(m_back + m_capacity - 1) % m_capacity]`    | Single arithmetic operation               |

Every operation is a **constant-time index calculation** into a fixed array — no allocation, no iteration, no shifting.

---

### Resizing and Linearization

When the buffer is full (`m_size == m_capacity`), the deque must grow. Resizing:

1. **Allocates a new, larger buffer** with `::operator new`.
2. **Moves all elements** from the old buffer to the new one in **logical order** — element `i` is moved from physical index `(m_front + i) % m_capacity` to `newData[i]`.
3. **Destroys the old elements** with explicit destructor calls.
4. **Frees the old buffer** with `::operator delete`.
5. **Resets `m_front = 0`** and **`m_back = m_size % m_capacity`**.

This step is called **linearization** — the circular layout is "unwound" into a straight sequence starting at index 0 in the new buffer. This makes the next round of operations clean and predictable.

```
Before resize (capacity = 5, wrapped buffer):
   m_front = 3, m_back = 2, m_size = 4

   Index:  0     1     2     3     4
         ┌─────┬─────┬─────┬─────┬─────┐
         │  C  │  D  │     │  A  │  B  │
         └─────┴─────┴─────┴─────┴─────┘
                             ▲     
                           front   

After resize (capacity = 10):
   m_front = 0, m_back = 4

   Index:  0     1     2     3     4    5..9
         ┌─────┬─────┬─────┬─────┬─────┬────┐
         │  A  │  B  │  C  │  D  │     │ .. │
         └─────┴─────┴─────┴─────┴─────┴────┘
           ▲                 ▲
         front              back
```

Resize is **O(n)** since every element must be moved.

---

### The Ring-Buffer-Aware Iterator

A standard raw-pointer iterator does not work correctly for a circular buffer, because the elements are **not contiguous in logical order** — they may wrap around the end of the underlying array.

The `DequeIterator<TDeque>` stores:
- A **pointer to the parent `Deque`** (`const TDeque* m_deque`)
- A **logical index** (`std::size_t m_index`) representing the distance from the front

Every time you dereference or access the iterator, it computes the **physical index** on the fly:

```cpp
ValueType& operator*() const {
    std::size_t physical = (m_deque->m_front + m_index) % m_deque->m_capacity;
    return m_deque->m_data[physical];
}
```

Increment and decrement only change the **logical index**:

```cpp
DequeIterator& operator++() { ++m_index; return *this; }
DequeIterator& operator--() { --m_index; return *this; }
```

`begin()` and `end()` return logical indices `0` and `m_size` respectively:

```cpp
Iterator begin() const { return Iterator{ this, 0 }; }
Iterator end()   const { return Iterator{ this, m_size }; }
```

Because `DequeIterator` needs access to `m_front`, `m_data`, and `m_capacity` (all private), `Deque` declares it as a **friend class**:

```cpp
template<typename>
friend class DequeIterator;
```

This is the idiomatic C++ pattern for container/iterator pairs that require private access.

---

### Raw Memory: `::operator new` and `::operator delete`

The deque uses the same low-level memory model as `Vector` — separating allocation from construction:

```cpp
// Allocate raw memory for N TValue objects — no constructors called
TValue* data = static_cast<TValue*>(::operator new(N * sizeof(TValue)));

// Free raw memory — no destructors called
::operator delete(data);
```

This allows the deque to hold capacity slots without initializing any objects, constructing them later with placement new only when elements are actually pushed.

---

### Placement New and Explicit Destructors

Elements are constructed in-place using **placement new**:

```cpp
// Construct TValue at the back slot by copying
new (&m_data[m_back]) TValue(item);

// Construct TValue at the front slot using forwarded constructor args
new (&m_data[m_front]) TValue(std::forward<Args>(args)...);
```

When elements are removed, their destructors are called explicitly before the slot is reused:

```cpp
m_data[index].~TValue();
```

This cleanly separates **object lifetime** from **memory lifetime** — memory stays allocated while objects are constructed and destroyed within it as needed.

---

### Emplace with Variadic Templates

`EmplaceBack` and `EmplaceFront` construct elements **directly in the buffer** using perfect forwarding, avoiding any temporary objects:

```cpp
template<typename... Args>
void EmplaceBack(Args&&... args) {
    if (IsFull()) { Resize(m_capacity * 2); }
    new (&m_data[m_back]) TValue(std::forward<Args>(args)...);
    m_back = (m_back + 1) % m_capacity;
    ++m_size;
}
```

This is more efficient than `PushBack` when constructing complex objects — no copy or move is needed as the object is built directly in its final location.

---

## Implementation

The full implementation lives in: [`include/Deque.h`](../include/Deque.h)  
The iterator lives in: [`include/DequeIterator.h`](../include/DequeIterator.h)

### Circular Buffer Lifecycle

```
PushBack(value):
    ┌──────────────────────────────────────────┐
    │ 1. If full → Resize(m_capacity * 2)      │
    │ 2. Placement new at m_data[m_back]       │
    │ 3. m_back = (m_back + 1) % m_capacity    │
    │ 4. ++m_size                              │
    └──────────────────────────────────────────┘

PushFront(value):
    ┌──────────────────────────────────────────────────────────┐
    │ 1. If full → Resize(m_capacity * 2)                      │
    │ 2. m_front = (m_front + m_capacity - 1) % m_capacity     │
    │ 3. Placement new at m_data[m_front]                      │
    │ 4. ++m_size                                              │
    └──────────────────────────────────────────────────────────┘

PopFront():
    ┌───────────────────────────────────────────────────────┐
    │ 1. Read & move element at m_data[m_front]             │
    │ 2. Call m_data[m_front].~TValue()                     │
    │ 3. m_front = (m_front + 1) % m_capacity               │
    │ 4. --m_size                                           │
    │ 5. Return moved value                                 │
    └───────────────────────────────────────────────────────┘

PopBack():
    ┌──────────────────────────────────────────────────────────┐
    │ 1. m_back = (m_back + m_capacity - 1) % m_capacity       │
    │ 2. Read & move element at m_data[m_back]                 │
    │ 3. Call m_data[m_back].~TValue()                         │
    │ 4. --m_size                                              │
    │ 5. Return moved value                                    │
    └──────────────────────────────────────────────────────────┘

Resize(newCapacity):
    ┌──────────────────────────────────────────────────────────┐
    │ 1. Allocate new raw buffer                               │
    │ 2. For each logical index i in [0, m_size):              │
    │    a. physical = (m_front + i) % m_capacity              │
    │    b. Move-construct newData[i] from m_data[physical]    │
    │    c. Destroy m_data[physical]                           │
    │ 3. Free old buffer                                       │
    │ 4. m_data = newData, m_front = 0, m_back = m_size        │
    └──────────────────────────────────────────────────────────┘
```

---

## Usage

```cpp
#include "Deque.h"

int main() {
    dsa::Deque<int> dq{};

    // Push to both ends
    dq.PushBack(10);
    dq.PushBack(20);
    dq.PushFront(5);
    dq.PushFront(1);

    // Deque is now: [1, 5, 10, 20]

    // Access elements
    std::cout << dq.Front() << "\n";  // 1
    std::cout << dq.Back()  << "\n";  // 20
    std::cout << dq[1]      << "\n";  // 5

    // Pop from both ends
    dq.PopFront();  // Removes 1
    dq.PopBack();   // Removes 20

    // Deque is now: [5, 10]

    // Emplace directly in buffer (no temporary created)
    struct Point { int x, y; Point(int x, int y) : x{x}, y{y} {} };
    dsa::Deque<Point> points{};
    points.EmplaceBack(3, 4);   // Constructs Point(3, 4) at back
    points.EmplaceFront(1, 2);  // Constructs Point(1, 2) at front

    // Range-based for loop (uses DequeIterator)
    for (const auto& p : points) {
        std::cout << p.x << ", " << p.y << "\n";
    }

    // Swap two deques in O(1)
    dsa::Deque<int> other{};
    other.PushBack(99);
    dq.Swap(other);  // Just swaps pointers and indices

    // Clear all elements (memory is kept)
    dq.Clear();

    return 0;
}
```

