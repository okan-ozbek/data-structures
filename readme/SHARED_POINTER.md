```aiignore
  █████████                                       █████             
 ███▒▒▒▒▒███                                     ▒▒███              
▒███    ▒▒▒  █████████████    ██████   ████████  ███████            
▒▒█████████ ▒▒███▒▒███▒▒███  ▒▒▒▒▒███ ▒▒███▒▒███▒▒▒███▒             
 ▒▒▒▒▒▒▒▒███ ▒███ ▒███ ▒███   ███████  ▒███ ▒▒▒   ▒███              
 ███    ▒███ ▒███ ▒███ ▒███  ███▒▒███  ▒███       ▒███ ███          
▒▒█████████  █████▒███ █████▒▒████████ █████      ▒▒█████           
 ▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒ ▒▒▒ ▒▒▒▒▒  ▒▒▒▒▒▒▒▒ ▒▒▒▒▒        ▒▒▒▒▒            
                                                                    
                                                                    
                                                                    
 ███████████            ███              █████                      
▒▒███▒▒▒▒▒███          ▒▒▒              ▒▒███                       
 ▒███    ▒███  ██████  ████  ████████   ███████    ██████  ████████ 
 ▒██████████  ███▒▒███▒▒███ ▒▒███▒▒███ ▒▒▒███▒    ███▒▒███▒▒███▒▒███
 ▒███▒▒▒▒▒▒  ▒███ ▒███ ▒███  ▒███ ▒███   ▒███    ▒███████  ▒███ ▒▒▒ 
 ▒███        ▒███ ▒███ ▒███  ▒███ ▒███   ▒███ ███▒███▒▒▒   ▒███     
 █████       ▒▒██████  █████ ████ █████  ▒▒█████ ▒▒██████  █████    
▒▒▒▒▒         ▒▒▒▒▒▒  ▒▒▒▒▒ ▒▒▒▒ ▒▒▒▒▒    ▒▒▒▒▒   ▒▒▒▒▒▒  ▒▒▒▒▒     
```

## Table of Contents

- [Overview](#overview)
- [Key Concepts](#key-concepts)
  - [Shared Ownership vs. Unique Ownership](#shared-ownership-vs-unique-ownership)
  - [The Control Block](#the-control-block)
  - [Reference Counting (Share Count)](#reference-counting-share-count)
  - [How Deallocation Works](#how-deallocation-works)
  - [Copy Semantics](#copy-semantics)
  - [Why Move is Deleted](#why-move-is-deleted)
  - [Deleters and `reset()`](#deleters-and-reset)
- [Implementation](#implementation)
- [Usage](#usage)

---

## Overview

This `SharedPointer<TValueType>` is a smart pointer that implements **shared ownership** of a dynamically allocated object. Multiple `SharedPointer` instances can point to the **same** object, and the object is only destroyed when the **last** `SharedPointer` owning it is destroyed or reset.

This is the same approach that `std::shared_ptr` uses. The core difference from `UniquePointer` is that shared pointers **allow copies** — every copy increments a reference count, and the resource is only freed when that count reaches zero.

| Problem                | Raw Pointer (`new`/`delete`)            | SharedPointer                                           |
|------------------------|-----------------------------------------|---------------------------------------------------------|
| **Memory leak**        | Forgot to call `delete`                 | Automatic `delete` when last owner is gone              |
| **Double-free**        | Called `delete` twice on same pointer   | Reference counting ensures exactly one `delete`         |
| **Dangling pointer**   | Used pointer after `delete`             | Object alive as long as at least one owner exists       |
| **Shared access**      | Manual coordination of who calls delete | Built-in — any owner can read, last owner cleans up     |

The class maintains two internal members:

| Member      | Type            | Description                                                |
|-------------|-----------------|------------------------------------------------------------|
| `pointer_`  | `TValueType*`   | Raw pointer to the managed object                          |
| `control_`  | `ControlBlock*` | Pointer to a heap-allocated block holding the share count  |

---

## Key Concepts

### Shared Ownership vs. Unique Ownership

With `UniquePointer`, **exactly one** pointer owns the resource at any time. Copies are forbidden — you can only **move** ownership.

With `SharedPointer`, **multiple** pointers can own the same resource simultaneously. Each copy increases the share count, and the resource stays alive until **every** owner has released it.

```
UniquePointer (exclusive):
  ptr_a ──owns──▶ Object        ← only ONE owner at a time

SharedPointer (shared):
  ptr_a ──owns──▶ Object ◀──owns── ptr_b
                    ▲
                    └───owns── ptr_c
                                    ← THREE owners, object alive until all three are gone
```

**When to use which?**

| Scenario                                              | Use                |
|-------------------------------------------------------|--------------------|
| One clear owner, resource passed around temporarily   | `UniquePointer`    |
| Multiple parts of the program need to keep it alive   | `SharedPointer`    |
| Ownership is unclear or shared across threads/caches  | `SharedPointer`    |

---

### The Control Block

This is the most important concept that separates `SharedPointer` from `UniquePointer`.

**The problem:** If multiple `SharedPointer`s point to the same object, they all need to agree on how many owners there are. You can't store the count inside each `SharedPointer` — each would have its own copy and they'd go out of sync. You also can't store it inside the managed object itself — the smart pointer shouldn't require the managed type to have special fields.

**The solution:** Allocate a **separate heap object** — the **control block** — that all `SharedPointer`s pointing to the same resource share a pointer to.

```cpp
struct ControlBlock {
    std::size_t count;   // Number of SharedPointers currently owning the resource

    ControlBlock() : count{1} {}
    ~ControlBlock() = default;
};
```

**Memory layout:**

```
SharedPointer A              SharedPointer B
┌──────────────┐            ┌──────────────┐
│ pointer_ ────┼──────┐     │ pointer_ ────┼──────┐
│ control_ ────┼───┐  │     │ control_ ────┼───┐  │
└──────────────┘   │  │     └──────────────┘   │  │
                   │  │                        │  │
                   ▼  ▼                        │  ▼
              ┌─────────┐              ┌───────────────┐
              │ count: 2│              │ Managed Object │
              └─────────┘              └───────────────┘
                   ▲                          
                   └──────────────────────────┘
                   Both A and B point to the SAME control block
```

**Why can't we just store the count inside `SharedPointer`?**

```cpp
// ✗ WRONG — each SharedPointer has its own count
class SharedPointer {
    TValueType* pointer_;
    std::size_t count_;     // A has count_=2, B has count_=2 — but they're separate variables!
};
```

If A is destroyed, it can decrement **its own** `count_` to 1, but B's `count_` is still 2. They're out of sync. With a shared `ControlBlock*`, both A and B decrement the **same** integer.

**Why not store the count inside the managed object?**

That would require the managed type to inherit from some base class or have a `count` field — this is **intrusive**. Our `SharedPointer` works with **any type** (`int`, `Vector2D`, `std::string`, etc.) without modifying it. The control block keeps the bookkeeping **external**.

---

### Reference Counting (Share Count)

The **share count** (also called **reference count**) tracks how many `SharedPointer` instances currently own the resource. It changes as follows:

| Action                          | Effect on share count                     |
|---------------------------------|-------------------------------------------|
| Construct from raw pointer      | count = **1** (new control block created) |
| Copy construct                  | count **+1**                              |
| Copy assignment                 | old target count **-1**, new target **+1**|
| Destructor                      | count **-1**                              |
| `reset()`                       | count **-1**                              |
| count reaches **0**             | **Object is deleted, control block freed**|

**Example lifecycle:**

```cpp
{
    SharedPointer<int> a{new int(42)};
    // share_count = 1  (a owns it)

    SharedPointer<int> b{a};
    // share_count = 2  (a and b own it)

    {
        SharedPointer<int> c{a};
        // share_count = 3  (a, b, and c own it)

    }  // c destroyed → share_count = 2

    b.reset();
    // share_count = 1  (only a owns it)

}  // a destroyed → share_count = 0 → delete the int, delete the control block
```

You can inspect the count at any time using `share_count()`:

```cpp
SharedPointer<int> ptr{new int(10)};
ptr.share_count();  // returns 1

SharedPointer<int> copy{ptr};
ptr.share_count();  // returns 2 — same value from either ptr or copy
```

---

### How Deallocation Works

Deallocation is handled by the private `remove_shared()` method, which is called from the destructor, `reset()`, and copy assignment:

```cpp
template<typename TDeleter = std::default_delete<TValueType>>
void remove_shared(TDeleter deleter = TDeleter{}) const {
    if (!control_) {
        return;              // Nothing to manage (default-constructed or already reset)
    }

    if (--control_->count == 0) {
        deleter(pointer_);   // Delete the managed object
        delete control_;     // Delete the control block itself
    }
}
```

**Step by step:**

1. If `control_` is `nullptr`, this `SharedPointer` doesn't own anything — do nothing.
2. Decrement the share count.
3. If the count reaches **0**, this was the **last owner**:
   - Call the deleter on the managed object (`deleter(pointer_)` — by default this calls `delete pointer_`)
   - Free the control block itself with `delete control_`
4. If the count is still > 0, other `SharedPointer`s still exist — do nothing, the resource stays alive.

**Two separate deallocations happen when count reaches 0:**

| What's freed       | How                  | Why                                            |
|---------------------|---------------------|-------------------------------------------------|
| Managed object      | `deleter(pointer_)` | The actual resource we're managing              |
| Control block       | `delete control_`   | The bookkeeping struct — no longer needed       |

The control block **cannot** be freed with the deleter because the deleter is typed to `TValueType*`, not `ControlBlock*`. It's always freed with plain `delete`.

---

### Copy Semantics

Unlike `UniquePointer`, `SharedPointer` **allows copies**. Every copy increments the share count:

**Copy constructor:**

```cpp
SharedPointer(const SharedPointer& other)
    : pointer_{ other.pointer_ }
    , control_{ other.control_ }
{
    if (control_) {
        ++control_->count;   // One more owner
    }
}
```

Both the new and old `SharedPointer` now point to the **same** `pointer_` and the **same** `control_` — they share the resource.

**Copy assignment:**

```cpp
SharedPointer& operator=(const SharedPointer& other) {
    if (this == &other) {
        return *this;          // Self-assignment guard
    }

    remove_shared();           // Release current resource (decrement old count)

    control_ = other.control_;
    pointer_ = other.pointer_;

    if (control_) {
        ++control_->count;     // Join new resource's ownership
    }

    return *this;
}
```

Copy assignment must first **release** whatever the pointer currently owns (which may trigger deallocation if it was the last owner), then **join** the new resource's ownership group.

---

### Why Move is Deleted

In our implementation, move constructor and move assignment are explicitly deleted:

```cpp
SharedPointer(SharedPointer&&) = delete;
SharedPointer& operator=(SharedPointer&&) = delete;
```

This is a simplification. `std::shared_ptr` does support moves (transferring ownership without changing the count — it's an optimization to avoid an increment followed by a decrement). Our implementation focuses on the core concept of shared ownership through **copying and reference counting**.

---

### Deleters and `reset()`

The `reset()` method allows you to release the current resource and optionally take ownership of a new one. It also accepts a custom deleter:

```cpp
template<typename TDeleter = std::default_delete<TValueType>>
void reset(TValueType* pointer = nullptr, TDeleter deleter = TDeleter{}) {
    remove_shared<TDeleter>(deleter);   // Release current resource with the given deleter

    if (pointer) {
        pointer_ = pointer;
        control_ = new ControlBlock();  // Fresh control block, count = 1
    } else {
        pointer_ = nullptr;
        control_ = nullptr;             // Empty state
    }
}
```

**Using reset with a custom deleter:**

```cpp
bool deleted = false;

auto deleter = [&deleted](const int* pointer) {
    delete pointer;
    deleted = true;
};

SharedPointer<int> ptr{new int(42)};
ptr.reset(nullptr, deleter);
// deleted is now true — the custom deleter was called
```

This is useful for testing that deallocation actually happened, or for managing resources that need special cleanup logic.

---

## Implementation

The full implementation lives in `include/SharedPointer.h` under the `dsa` namespace.

**Public API:**

| Method / Operator             | Description                                                  |
|-------------------------------|--------------------------------------------------------------|
| `SharedPointer()`             | Default constructor — empty pointer, no resource             |
| `SharedPointer(T* pointer)`   | Takes ownership of a raw pointer, creates control block      |
| `SharedPointer(const SP& o)`  | Copy constructor — shares ownership, increments count        |
| `operator=(const SP& o)`      | Copy assignment — releases old, shares new                   |
| `~SharedPointer()`            | Destructor — decrements count, deletes if last owner         |
| `reset(T* ptr, deleter)`      | Release current resource, optionally take ownership of new   |
| `swap(SP& other)`             | Swap managed pointers and control blocks                     |
| `get()`                       | Returns the raw pointer                                      |
| `is_unique()`                 | Returns `true` if share count is 1                           |
| `share_count()`               | Returns the current number of owners                         |

---

## Usage

```cpp
#include "SharedPointer.h"

// Create a shared pointer
dsa::SharedPointer<int> ptr1{new int(42)};
ptr1.share_count();   // 1

// Copy it — both now own the resource
dsa::SharedPointer<int> ptr2{ptr1};
ptr1.share_count();   // 2

// Copy assign
dsa::SharedPointer<int> ptr3{};
ptr3 = ptr1;
ptr1.share_count();   // 3

// Reset one — count decreases
ptr3.reset();
ptr1.share_count();   // 2

// Swap two shared pointers
dsa::SharedPointer<int> a{new int(10)};
dsa::SharedPointer<int> b{new int(20)};
a.swap(b);
*a.get();  // 20
*b.get();  // 10

// When all owners are gone, the resource is automatically freed
```

