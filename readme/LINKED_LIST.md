```
 █████        ███             █████                   █████    █████        ███           █████   
▒▒███        ▒▒▒             ▒▒███                   ▒▒███    ▒▒███        ▒▒▒           ▒▒███    
 ▒███        ████  ████████   ▒███ █████  ██████   ███████     ▒███        ████   █████  ███████  
 ▒███       ▒▒███ ▒▒███▒▒███  ▒███▒▒███  ███▒▒███ ███▒▒███     ▒███       ▒▒███  ███▒▒  ▒▒▒███▒   
 ▒███        ▒███  ▒███ ▒███  ▒██████▒  ▒███████ ▒███ ▒███     ▒███        ▒███ ▒▒█████   ▒███    
 ▒███      █ ▒███  ▒███ ▒███  ▒███▒▒███ ▒███▒▒▒  ▒███ ▒███     ▒███      █ ▒███  ▒▒▒▒███  ▒███ ███
 ███████████ █████ ████ █████ ████ █████▒▒██████ ▒▒████████    ███████████ █████ ██████   ▒▒█████ 
▒▒▒▒▒▒▒▒▒▒▒ ▒▒▒▒▒ ▒▒▒▒ ▒▒▒▒▒ ▒▒▒▒ ▒▒▒▒▒  ▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒    ▒▒▒▒▒▒▒▒▒▒▒ ▒▒▒▒▒ ▒▒▒▒▒▒     ▒▒▒▒▒                                                                                                                                                                                                                                                                                                              
```

## Table of Contents

- [Overview](#overview)
- [Key Concepts](#key-concepts)
  - [Why Use a Linked List?](#why-use-a-linked-list)
  - [Doubly Linked vs Singly Linked](#doubly-linked-vs-singly-linked)
  - [Node-Based Memory Layout](#node-based-memory-layout)
  - [O(1) Insertion and Deletion](#o1-insertion-and-deletion)
  - [O(n) Access — The Trade-off](#on-access--the-trade-off)
  - [When to Use a Linked List vs a Vector](#when-to-use-a-linked-list-vs-a-vector)
  - [Shallow vs Deep Copy](#shallow-vs-deep-copy)
  - [Memory Management — Owning Raw Pointers](#memory-management--owning-raw-pointers)
  - [The Iterator](#the-iterator)
- [Implementation](#implementation)
- [Usage](#usage)

---

## Overview

`LinkedList<TValue>` is a **doubly-linked list** — a node-based data structure where each element stores a value and two pointers: one to the next node, and one to the previous node.

Unlike a `Vector` or `Deque` that store elements in a contiguous block of memory, a linked list stores each element in a separately heap-allocated node. The nodes are connected by pointers, forming a chain.

| Operation      | Description                                | Time Complexity |
|----------------|--------------------------------------------|-----------------|
| `PushFront`    | Insert element at the front                | **O(1)**        |
| `PushBack`     | Insert element at the back                 | **O(1)**        |
| `PopFront`     | Remove element from the front              | **O(1)**        |
| `PopBack`      | Remove element from the back               | **O(1)**        |
| `PushAfter`    | Insert element after a given node          | **O(1)**        |
| `Erase`        | Remove element by iterator                 | **O(1)**        |
| `At` / `[]`    | Access element by index                    | **O(n)**        |
| `Size`         | Number of elements                         | **O(1)**        |

---

## Key Concepts

### Why Use a Linked List?

A linked list is one of the most foundational data structures in computer science. The primary reasons people choose it over alternatives like `Vector` are:

**1. O(1) insertion and deletion anywhere in the list**
Given a pointer or iterator to a node, inserting before or after it — or deleting it — requires only pointer rewiring. No elements need to be shifted, unlike a `Vector` which requires shifting all subsequent elements.

**2. No contiguous memory requirement**
A `Vector` needs a single contiguous block of memory. If you push enough elements to trigger a resize, it must allocate a larger block and move everything. A linked list allocates each node individually, so it never needs to move existing elements.

**3. Stable iterators and pointers**
When you insert or remove elements from a `Vector`, all iterators past the insertion point are invalidated (because elements shift or the buffer reallocates). Linked list iterators (pointers to nodes) remain valid as long as the node they point to exists.

**4. Efficient as a building block**
Linked lists are the backbone for implementing other data structures — LRU caches, adjacency lists for graphs, undo systems, and more.

---

### Doubly Linked vs Singly Linked

A **singly linked list** has only a `next` pointer — you can only traverse forward.  
A **doubly linked list** has both `next` and `prev` pointers — you can traverse in both directions and delete a node without needing its predecessor.

Our `LinkedList` is **doubly linked**:

```
Singly linked:
  head → [A|next] → [B|next] → [C|next] → nullptr

Doubly linked:
  head ⇄ [prev|A|next] ⇄ [prev|B|next] ⇄ [prev|C|next] → nullptr
  tail ──────────────────────────────────────────────────────^
```

The `prev` pointer is what makes `PopBack` and `Erase` O(1) — without it, you'd need to walk the entire list to find the node before the one you want to delete.

---

### Node-Based Memory Layout

Each element lives in a separately allocated `ListNode<TValue>` on the heap:

```cpp
template<typename TValue>
struct ListNode {
    TValue value;
    ListNode* next{ nullptr };
    ListNode* prev{ nullptr };
};
```

```
Heap layout (not contiguous):

  [Node A]          [Node B]          [Node C]
  value = 10        value = 20        value = 30
  next  = ──────────►next  = ──────────►next  = nullptr
  prev  = nullptr   prev  = ◄──────────prev  = ◄──────────
     ▲                                             ▲
   m_head                                       m_tail
```

The `LinkedList` class only stores two pointers and a size:

```cpp
ListNode<TValue>* m_head{ nullptr };
ListNode<TValue>* m_tail{ nullptr };
std::size_t m_size{ 0 };
```

All the actual data lives in dynamically allocated nodes scattered across heap memory.

---

### O(1) Insertion and Deletion

**PushBack** — append to the end:
```
Before: head → [A] ⇄ [B] ← tail
After:  head → [A] ⇄ [B] ⇄ [C] ← tail

Steps:
  1. Allocate new node C with prev = m_tail
  2. m_tail->next = C
  3. m_tail = C
```

**PushFront** — prepend to the front:
```
Before: head → [A] ⇄ [B] ← tail
After:  head → [Z] ⇄ [A] ⇄ [B] ← tail

Steps:
  1. Allocate new node Z with next = m_head
  2. m_head->prev = Z
  3. m_head = Z
```

**Erase (middle node B):**
```
Before: head → [A] ⇄ [B] ⇄ [C] ← tail
After:  head → [A] ⇄ [C] ← tail

Steps:
  1. B.prev->next = B.next   (A.next = C)
  2. B.next->prev = B.prev   (C.prev = A)
  3. delete B
```

No elements are shifted. Only pointer links are rewired.

---

### O(n) Access — The Trade-off

Unlike a `Vector`, a linked list provides **no random access**. To get element at index `i`, you must walk from `m_head` following `next` pointers `i` times:

```cpp
TValue& At(std::size_t index) {
    auto* current{ m_head };
    while (index > 0 && current != nullptr) {
        current = current->next;
        --index;
    }
    return current->value;
}
```

This is **O(n)** — the further into the list, the slower the access.

This is the primary trade-off of a linked list: **fast insertion/deletion, slow random access** — the opposite of a `Vector`.

---

### When to Use a Linked List vs a Vector

| Scenario | Use |
|----------|-----|
| Frequent random access by index | `Vector` |
| Frequent insertions/deletions at back only | `Vector` |
| Frequent insertions/deletions at front or middle | `LinkedList` |
| Need stable iterators after insert/delete | `LinkedList` |
| Cache-friendly iteration over all elements | `Vector` |
| Building an LRU cache, undo system, or graph adjacency list | `LinkedList` |

**Cache performance note:** Because linked list nodes are scattered across heap memory, iterating over a linked list triggers many cache misses. A `Vector`'s contiguous memory layout is much more cache-friendly for sequential iteration. This is why, in practice, `std::vector` often outperforms `std::list` even for workloads with moderate insertions — the cache penalty of pointer chasing is significant.

---

### Shallow vs Deep Copy

The copy constructor is currently `= default`, which does a **shallow copy** — it copies the `m_head`, `m_tail`, and `m_size` pointers, meaning both lists point to the same nodes. Modifying or destroying one will corrupt the other.

A proper deep copy must traverse the source list and allocate new nodes:

```cpp
LinkedList(const LinkedList& other) {
    auto* current{ other.m_head };
    while (current != nullptr) {
        PushBack(current->value);
        current = current->next;
    }
}
```

This is **O(n)** since every node must be individually copied.

---

### Memory Management — Owning Raw Pointers

Every `new ListNode<TValue>(...)` in `PushFront`, `PushBack`, and `PushAfter` creates a heap-allocated node that the `LinkedList` owns. This means:

- The destructor **must** walk the entire list and `delete` every node:

```cpp
~LinkedList() {
    ListNode<TValue>* current{ m_head };
    while (current != nullptr) {
        ListNode<TValue>* next{ current->next };
        delete current;
        current = next;
    }
}
```

- `Erase`, `PopFront`, and `PopBack` must `delete` the node they remove.
- The copy assignment operator must be careful not to leak the old nodes before replacing them.

Failing to delete nodes is a **memory leak** — the memory is allocated on the heap and never returned to the OS until the program exits.

---

### The Iterator

`LinkedListIterator<TLinkedList>` wraps a raw `ListNode<TValue>*` pointer. Unlike `VectorIterator` or `DequeIterator`, it cannot use pointer arithmetic because linked list nodes are **not contiguous in memory**.

Traversal is done by following `next` and `prev` pointers:

```cpp
LinkedListIterator& operator++() {
    if (m_node != nullptr) m_node = m_node->next;
    return *this;
}

LinkedListIterator& operator--() {
    if (m_node != nullptr) m_node = m_node->prev;
    return *this;
}
```

`begin()` returns an iterator to `m_head`, `end()` returns an iterator to `nullptr` (one past `m_tail`):

```cpp
Iterator begin() const { return Iterator{ m_head }; }
Iterator end()   const {
    if (m_tail == nullptr) return Iterator{ nullptr };
    return Iterator{ m_tail->next };
}
```

The iterator supports range-based for loops, allowing clean traversal:

```cpp
for (const auto& value : list) {
    std::cout << value << "\n";
}
```

---

## Implementation

The full implementation lives in: [`include/LinkedList.h`](../include/LinkedList.h)  
The iterator lives in: [`include/LinkedListIterator.h`](../include/LinkedListIterator.h)

### Node Lifecycle

```
PushBack(value):
    ┌──────────────────────────────────────────────────────┐
    │ 1. Allocate new ListNode<TValue>(value, nullptr, tail)│
    │ 2. m_tail->next = newNode                             │
    │ 3. m_tail = newNode                                   │
    │ 4. ++m_size                                           │
    └──────────────────────────────────────────────────────┘

PopFront():
    ┌──────────────────────────────────────────────────────┐
    │ 1. Save m_head into current                          │
    │ 2. m_head = current->next                            │
    │ 3. If m_head != nullptr: m_head->prev = nullptr      │
    │ 4. Else: m_tail = nullptr (list is now empty)        │
    │ 5. delete current                                    │
    │ 6. --m_size                                          │
    │ 7. Return saved value                                │
    └──────────────────────────────────────────────────────┘

Erase(iterator):
    ┌──────────────────────────────────────────────────────┐
    │ 1. node = iterator.pointer()                         │
    │ 2. If node->prev != nullptr: node->prev->next = node->next │
    │    Else: m_head = node->next  (erasing head)         │
    │ 3. If node->next != nullptr: node->next->prev = node->prev │
    │    Else: m_tail = node->prev  (erasing tail)         │
    │ 4. delete node                                       │
    │ 5. --m_size                                          │
    │ 6. Return Iterator{ next }                           │
    └──────────────────────────────────────────────────────┘
```

---

## Usage

```cpp
#include "LinkedList.h"

int main() {
    dsa::LinkedList<int> list{};

    // Push to both ends
    list.PushBack(10);
    list.PushBack(20);
    list.PushBack(30);
    list.PushFront(5);

    // List is now: [5, 10, 20, 30]

    // Access elements
    std::cout << list.Front() << "\n";   // 5
    std::cout << list.Back()  << "\n";   // 30
    std::cout << list.At(1)   << "\n";   // 10 (O(n))

    // Pop from both ends
    list.PopFront();  // Removes 5,  list: [10, 20, 30]
    list.PopBack();   // Removes 30, list: [10, 20]

    // Insert after a node
    list.PushAfter(0, 15);  // Insert 15 after index 0, list: [10, 15, 20]

    // Erase by iterator
    auto it = list.begin();
    ++it;                    // Points to 15
    list.Erase(it);          // list: [10, 20]

    // Range-based for loop
    for (const int& val : list) {
        std::cout << val << "\n";  // 10, 20
    }

    return 0;
}
```

