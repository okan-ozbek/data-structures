```aiignore
 █████  █████             ███                                       
▒▒███  ▒▒███             ▒▒▒                                        
 ▒███   ▒███  ████████   ████   ████████ █████ ████  ██████         
 ▒███   ▒███ ▒▒███▒▒███ ▒▒███  ███▒▒███ ▒▒███ ▒███  ███▒▒███        
 ▒███   ▒███  ▒███ ▒███  ▒███ ▒███ ▒███  ▒███ ▒███ ▒███████         
 ▒███   ▒███  ▒███ ▒███  ▒███ ▒███ ▒███  ▒███ ▒███ ▒███▒▒▒          
 ▒▒████████   ████ █████ █████▒▒███████  ▒▒████████▒▒██████         
  ▒▒▒▒▒▒▒▒   ▒▒▒▒ ▒▒▒▒▒ ▒▒▒▒▒  ▒▒▒▒▒███   ▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒          
                                   ▒███                             
                                   █████                            
                                  ▒▒▒▒▒                             
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
  - [What is a Smart Pointer?](#what-is-a-smart-pointer)
  - [Ownership and RAII](#ownership-and-raii)
  - [Why Unique (Exclusive) Ownership?](#why-unique-exclusive-ownership)
  - [Deleters](#deleters)
  - [Custom Deleters with Lambdas](#custom-deleters-with-lambdas)
  - [Move Semantics: Transferring Ownership](#move-semantics-transferring-ownership)
  - [The Rule of Five (Restricted)](#the-rule-of-five-restricted)
  - [`make_unique` and Perfect Forwarding](#make_unique-and-perfect-forwarding)
- [Implementation](#implementation)
- [Usage](#usage)

---

## Overview

This `UniquePointer<TValueType, TDeleter>` is a smart pointer that **owns and manages** a dynamically allocated object through a pointer and **automatically disposes of it** when the `UniquePointer` goes out of scope. It enforces **exclusive ownership** — at any point in time, exactly **one** `UniquePointer` owns the resource.

This is the same approach that `std::unique_ptr` uses. The goal is to eliminate manual `new`/`delete` calls and prevent the most common memory bugs in C++:

| Problem                | Raw Pointer (`new`/`delete`)            | UniquePointer (Smart Pointer)                |
|------------------------|-----------------------------------------|----------------------------------------------|
| **Memory leak**        | Forgot to call `delete`                 | Destructor automatically calls `delete`       |
| **Double-free**        | Called `delete` twice on same pointer   | Only one owner — destroyed exactly once       |
| **Dangling pointer**   | Used pointer after `delete`             | Ownership transfer nullifies the source       |
| **Exception safety**   | `delete` skipped if exception thrown    | Destructor runs during stack unwinding        |

The class maintains two internal members:

| Member      | Type          | Description                                          |
|-------------|---------------|------------------------------------------------------|
| `pointer_`  | `TValueType*` | Raw pointer to the managed object                    |
| `deleter_`  | `TDeleter`    | Callable that knows how to destroy the managed object |

By default, `TDeleter` is `std::default_delete<TValueType>`, which simply calls `delete pointer_`.

---

## Key Concepts

### What is a Smart Pointer?

A **smart pointer** is a class that wraps a raw pointer and manages its lifetime automatically. When the smart pointer is destroyed (goes out of scope, is reset, etc.), it automatically frees the managed resource. This is what makes it "smart" — the memory management is built into the object's lifecycle.

**The core idea:**

```cpp
// Raw pointer — YOU are responsible for calling delete
Vector2D* raw = new Vector2D(10, 10);
// ... use raw ...
delete raw;  // Easy to forget, especially with early returns or exceptions

// Smart pointer — delete happens automatically
UniquePointer<Vector2D> smart = make_unique<Vector2D>(10, 10);
// ... use smart (same syntax: smart->x, *smart, etc.) ...
// When 'smart' goes out of scope → destructor calls delete automatically
```

The smart pointer **overloads `->` and `*`** so it behaves identically to a raw pointer from the caller's perspective, while silently managing the memory behind the scenes.

---

### Ownership and RAII

**RAII** stands for **Resource Acquisition Is Initialization**. It's the C++ idiom where:

1. A resource (memory, file handle, lock, etc.) is **acquired in the constructor**
2. The resource is **released in the destructor**
3. The object's lifetime is tied to scope — when scope ends, the destructor runs

`UniquePointer` is a textbook example of RAII:

```cpp
void process() {
    UniquePointer<Vector2D> ptr = make_unique<Vector2D>(5, 10);
    // ptr owns the Vector2D

    if (some_condition) {
        return;  // ptr is destroyed here → Vector2D is deleted ✓
    }

    throw std::runtime_error("oops");
    // ptr is destroyed during stack unwinding → Vector2D is deleted ✓

}  // ptr is destroyed here → Vector2D is deleted ✓
```

**No matter how the function exits** — normal return, early return, or exception — the destructor always runs and the memory is always freed. This is impossible to guarantee with raw `new`/`delete`.

---

### Why Unique (Exclusive) Ownership?

A `UniquePointer` enforces that **exactly one pointer owns the resource at a time**. This is achieved by:

1. **Deleting the copy constructor and copy assignment** — you cannot copy a `UniquePointer`
2. **Providing move constructor and move assignment** — you can **transfer** ownership

```cpp
UniquePointer<Vector2D> a = make_unique<Vector2D>(1, 2);

UniquePointer<Vector2D> b = a;              // ✗ COMPILE ERROR — copy deleted
UniquePointer<Vector2D> c = std::move(a);   // ✓ Ownership transferred: a → c
// 'a' is now nullptr, 'c' owns the Vector2D
```

**Why not allow copies?**

If two `UniquePointer`s pointed to the same object, both destructors would call `delete` on the same pointer — **double-free** (undefined behavior). Exclusive ownership eliminates this entirely.

**Ownership diagram:**

```
Before move:
  a ──owns──▶ Vector2D(1, 2)
  c ──owns──▶ nullptr

After std::move(a) → c:
  a ──owns──▶ nullptr
  c ──owns──▶ Vector2D(1, 2)
```

---

### Deleters

A **deleter** is the callable object that `UniquePointer` invokes to destroy the managed resource. It's the second template parameter:

```cpp
template<typename TValueType, typename TDeleter = std::default_delete<TValueType>>
class UniquePointer;
```

**The default deleter** (`std::default_delete<TValueType>`) simply calls `delete`:

```cpp
// What std::default_delete does internally:
template<typename T>
struct default_delete {
    void operator()(T* ptr) const {
        delete ptr;
    }
};
```

**Why make the deleter configurable?**

Not all resources are freed with `delete`. You might manage:

| Resource              | How to free it            |
|-----------------------|---------------------------|
| `new` object          | `delete ptr`              |
| `new[]` array         | `delete[] ptr`            |
| `malloc` memory       | `free(ptr)`               |
| File handle (`FILE*`) | `fclose(ptr)`             |
| Socket descriptor     | `close(fd)`               |
| Custom allocator      | `allocator.deallocate(p)` |

By parameterizing the deleter, `UniquePointer` can manage **any** resource — not just `new`-allocated memory.

**How the deleter is used internally:**

```cpp
// In the destructor:
~UniquePointer() {
    if (pointer_ != nullptr) {
        deleter_(pointer_);  // Calls the deleter on the managed pointer
    }
}

// In reset():
void reset(TValueType* other = nullptr) {
    auto* stale = pointer_;
    pointer_ = other;
    if (stale != nullptr) {
        deleter_(stale);  // Deleter frees the old resource
    }
}
```

The deleter is **moved** along with the pointer during move operations, ensuring the correct cleanup function always stays with the resource it knows how to free.

---

### Custom Deleters with Lambdas

You can pass a **lambda** as a custom deleter. This is useful for testing (verifying destruction happened) or for managing non-standard resources.

**The challenge:** A lambda's type is unique and unnamed — you can't spell it out. Use `decltype()` to capture it:

```cpp
bool deleted = false;

auto deleter = [&deleted](const Vector2D* pointer) {
    delete pointer;
    deleted = true;
};

// decltype(deleter) gets the lambda's unique type
UniquePointer<Vector2D, decltype(deleter)> pointer(new Vector2D(10, 10), deleter);
```

**Breaking it down:**

| Part                        | Meaning                                                        |
|-----------------------------|----------------------------------------------------------------|
| `[&deleted]`                | Capture `deleted` **by reference** — changes are visible outside |
| `(const Vector2D* pointer)` | The deleter receives the raw pointer to free                    |
| `decltype(deleter)`         | The compiler deduces the lambda's unnamed type                  |

**Why capture by reference (`&`)?**

If you capture by value (`[deleted]`), the lambda gets its own **copy** of `deleted`. Setting it to `true` inside the lambda modifies the copy — the original variable outside remains `false`. By capturing by reference, the lambda and the outer scope share the same `bool`.

**Common gotcha — `mutable`:**

A lambda captures by value as `const` by default. Without `mutable`, you can't even modify the captured copy:

```cpp
auto bad  = [deleted](auto* p) { deleted = true; delete p; };          // ✗ Compile error
auto good = [deleted](auto* p) mutable { deleted = true; delete p; };  // ✓ But modifies a copy
auto best = [&deleted](auto* p) { deleted = true; delete p; };         // ✓ Modifies the original
```

**Use case — verifying destruction in tests:**

```cpp
void test_destructor() {
    bool deleted{false};

    {
        auto deleter = [&deleted](const Vector2D* pointer) {
            delete pointer;
            deleted = true;
        };

        UniquePointer<Vector2D, decltype(deleter)> pointer(new Vector2D(10, 10), deleter);
        // deleted is still false here
    }
    // Scope ended → destructor ran → deleter called → deleted is now true

    assert(deleted);  // ✓
}
```

---

### Move Semantics: Transferring Ownership

Since copying is disabled, the only way to transfer a `UniquePointer` is through **move semantics**. Moving transfers ownership of the resource from one `UniquePointer` to another.

#### Move constructor

```cpp
UniquePointer(UniquePointer&& other) noexcept 
    : pointer_{other.pointer_}
    , deleter_{std::move(other.deleter_)}
{ 
    other.pointer_ = nullptr;  // Source gives up ownership
}
```

**What happens step by step:**

1. `pointer_` is initialized with the source's pointer (the address is copied)
2. `deleter_` is move-constructed from the source's deleter
3. The source's `pointer_` is set to `nullptr` — it no longer owns anything
4. When the source is eventually destroyed, its destructor sees `nullptr` and does nothing

#### Move assignment operator

```cpp
UniquePointer& operator=(UniquePointer&& other) noexcept {
    if (this == &other) return *this;  // Self-assignment guard

    reset(other.release());            // Free our resource, take theirs
    deleter_ = std::move(other.deleter_);

    return *this;
}
```

This uses `release()` and `reset()` together:
- `other.release()` removes ownership from `other` and returns the raw pointer
- `reset(...)` frees our current resource and takes ownership of the new one

---

### The Rule of Five (Restricted)

The Rule of Five states: if you define any one of the five special members, define all five. `UniquePointer` defines all five — but **restricts** two of them:

| Special Member             | Status      | Reason                                        |
|----------------------------|-------------|-----------------------------------------------|
| **Destructor**             | ✓ Defined   | Calls `deleter_(pointer_)` to free the resource |
| **Move constructor**       | ✓ Defined   | Transfers ownership from source to `this`      |
| **Move assignment**        | ✓ Defined   | Frees current, takes ownership from source     |
| **Copy constructor**       | ✗ Deleted   | Copying would create two owners → double-free  |
| **Copy assignment**        | ✗ Deleted   | Same reason — exclusive ownership required     |

```cpp
UniquePointer(const UniquePointer&) = delete;
UniquePointer& operator=(const UniquePointer&) = delete;
```

By explicitly `= delete`-ing the copy operations, any attempt to copy a `UniquePointer` results in a clear **compile-time error** rather than subtle runtime bugs.

---

### `make_unique` and Perfect Forwarding

`make_unique` is a **factory function** that constructs an object and wraps it in a `UniquePointer` in a single step. It avoids writing `new` manually:

```cpp
template<typename TValueType, typename... TArgs>
UniquePointer<TValueType> make_unique(TArgs&&... args) {
    return UniquePointer<TValueType>(new TValueType(std::forward<TArgs>(args)...));
}
```

**Breaking it down:**

| Part                     | Meaning                                                    |
|--------------------------|------------------------------------------------------------|
| `typename... TArgs`      | Variadic template — accepts zero or more argument types    |
| `TArgs&&... args`        | Universal references to each argument                      |
| `std::forward<TArgs>(args)...` | Perfect forwarding — preserves lvalue/rvalue-ness    |

**Why use `make_unique` instead of raw `new`?**

```cpp
// Raw new — verbose, error-prone, type written twice
UniquePointer<Vector2D> ptr(new Vector2D(10, 10));

// make_unique — cleaner, type written once, exception-safe
auto ptr = make_unique<Vector2D>(10, 10);
```

**Exception safety:** In expressions like `foo(UniquePointer<A>(new A), UniquePointer<B>(new B))`, the compiler may evaluate `new A` and `new B` before constructing either `UniquePointer`. If `new B` throws after `new A` succeeds, `A` is leaked. `make_unique` eliminates this because allocation and wrapping happen atomically within the function call.

---

## Implementation

The full implementation lives in a single header file: [`include/UniquePointer.h`](include/UniquePointer.h)

### Ownership Lifecycle

```
Construction:
    ┌──────────────────────────────────────────────────┐
    │ UniquePointer(new T(args...))                    │
    │ → pointer_ = raw pointer                         │
    │ → deleter_ = default (std::default_delete<T>)    │
    └──────────────────────────────────────────────────┘

Move:
    ┌──────────────────────────────────────────────────┐
    │ UniquePointer b = std::move(a)                   │
    │ → b.pointer_ = a.pointer_                        │
    │ → b.deleter_ = std::move(a.deleter_)             │
    │ → a.pointer_ = nullptr                           │
    └──────────────────────────────────────────────────┘

Reset:
    ┌──────────────────────────────────────────────────┐
    │ ptr.reset(new T(args...))                        │
    │ → deleter_(old pointer_)   // free old resource  │
    │ → pointer_ = new pointer                         │
    └──────────────────────────────────────────────────┘

Release:
    ┌──────────────────────────────────────────────────┐
    │ T* raw = ptr.release()                           │
    │ → returns pointer_                               │
    │ → pointer_ = nullptr                             │
    │ → caller now owns the raw pointer                │
    └──────────────────────────────────────────────────┘

Destruction (out of scope):
    ┌──────────────────────────────────────────────────┐
    │ ~UniquePointer()                                 │
    │ → if pointer_ != nullptr: deleter_(pointer_)     │
    │ → resource is freed                              │
    └──────────────────────────────────────────────────┘
```

### Methods

| Method                          | Description                                                     |
|---------------------------------|-----------------------------------------------------------------|
| `operator->`                    | Access member of the managed object (`ptr->x`)                  |
| `operator*`                     | Dereference the managed object (`*ptr`)                         |
| `operator bool`                 | Check if the pointer is non-null (`if (ptr)`)                   |
| `release()`                     | Give up ownership, return the raw pointer                       |
| `reset(TValueType*)`            | Free current resource, take ownership of a new one              |
| `get()`                         | Return the raw pointer without releasing ownership              |
| `get_deleter()`                 | Return a reference to the deleter                               |
| `swap(UniquePointer&)`          | Swap ownership between two `UniquePointer`s                     |

---

## Usage

```cpp
#include "UniquePointer.h"

struct Vector2D {
    int x, y;
    Vector2D(int x, int y) : x{x}, y{y} {}
};

int main() {
    // Factory construction (preferred)
    auto ptr = make_unique<Vector2D>(10, 20);

    // Access members
    std::cout << ptr->x << std::endl;  // 10
    std::cout << (*ptr).y << std::endl; // 20

    // Check validity
    if (ptr) {
        std::cout << "ptr is valid" << std::endl;
    }

    // Transfer ownership
    auto ptr2 = std::move(ptr);
    // ptr is now nullptr, ptr2 owns the Vector2D

    // Reset with a new object
    ptr2.reset(new Vector2D(30, 40));
    std::cout << ptr2->x << std::endl;  // 30

    // Release ownership (caller must manually delete)
    Vector2D* raw = ptr2.release();
    // ptr2 is now nullptr, raw must be deleted manually
    delete raw;

    // Swap two pointers
    auto a = make_unique<Vector2D>(1, 2);
    auto b = make_unique<Vector2D>(3, 4);
    a.swap(b);
    // a->x == 3, b->x == 1

    // Custom deleter with lambda
    bool deleted = false;
    {
        auto deleter = [&deleted](const Vector2D* p) {
            delete p;
            deleted = true;
        };
        UniquePointer<Vector2D, decltype(deleter)> ptr3(new Vector2D(5, 6), deleter);
    }
    // deleted == true — destructor called our custom deleter

    return 0;
}
```

