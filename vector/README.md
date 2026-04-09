# DynamicArray\<T\>

A generic, templated dynamic array implementation in C++ that manages raw memory manually — similar to how `std::vector` works under the hood.

---

## Table of Contents

- [Overview](#overview)
- [Key Concepts](#key-concepts)
  - [Raw Memory: `::operator new` and `::operator delete`](#raw-memory-operator-new-and-operator-delete)
  - [Placement New: `new (&data[i]) T(value)`](#placement-new-new-datai-tvalue)
  - [Explicit Destructor Calls: `data[i].~T()`](#explicit-destructor-calls-datai-t)
  - [Rvalue References: `T&&`](#rvalue-references-t)
  - [`std::move`](#stdmove)
  - [`std::forward`](#stdforward)
  - [Variadic Templates: `template<typename... Args>`](#variadic-templates-templatetypename-args)
  - [Iterators](#iterators)
- [Implementation](#implementation)
- [Usage](#usage)

---

## Overview

This `DynamicArray<T>` separates **memory allocation** from **object construction**. This is the same approach that `std::vector` uses internally, and it gives full control over when and where objects are created, moved, and destroyed.

The two memory models in C++:

| High-Level (`new T[]` / `delete[]`)           | Low-Level (`::operator new` / `::operator delete`) |
|------------------------------------------------|-----------------------------------------------------|
| Allocates memory **and** constructs all objects | Allocates **raw memory** only — no constructors      |
| Frees memory **and** destroys all objects       | Frees **raw memory** only — no destructors           |
| Simple but inflexible                          | Full control over object lifetimes                   |

This implementation uses the **low-level** model.

---

## Key Concepts

### Raw Memory: `::operator new` and `::operator delete`

These are the lowest-level memory functions in C++. They allocate and free raw bytes — **no constructors or destructors are called**.

```cpp
// Allocate raw memory for 10 T objects (no objects are created)
T* data = static_cast<T*>(::operator new(10 * sizeof(T)));

// Free the raw memory (no destructors are called)
::operator delete(data, 10 * sizeof(T));
```

**Why not `new T[10]`?**

`new T[10]` allocates memory **and** default-constructs all 10 elements immediately. For a dynamic array with `size = 0` and `capacity = 10`, you don't want 10 constructed objects sitting in memory — you want empty slots ready for future use.

**Think of it like this:**
- `::operator new` → reserving an empty parking lot (just space)
- `new T[10]` → reserving a parking lot and filling every spot with a car

---

### Placement New: `new (&data[i]) T(value)`

Placement new **constructs an object at a specific memory address** that has already been allocated. It does **not** allocate memory — it only calls the constructor.

```cpp
// Construct a T at the address &data[size_] by copying 'value'
new (&data_[size_]) T(value);

// Construct a T at the address &data[size_] by moving 'value'
new (&data_[size_]) T(std::move(value));

// Construct a T at the address &data[size_] using default constructor
new (&data_[i]) T{};
```

**Breaking it down:**

| Part                | Meaning                                          |
|---------------------|--------------------------------------------------|
| `new`               | Placement new operator (does NOT allocate memory) |
| `(&data_[size_])`   | The memory address to construct the object at     |
| `T(value)`          | Call T's constructor with `value` as argument     |

**Why use it?**

Because `::operator new` gives you raw, uninitialized memory. You can't just assign to it (`data[i] = value` is **undefined behavior** on raw memory — there's no object there yet). Placement new is how you **bring an object to life** at that address.

---

### Explicit Destructor Calls: `data[i].~T()`

When using raw memory, you must **manually destroy objects** before freeing the memory. Calling `~T()` directly runs the destructor without freeing memory.

```cpp
// Destroy the object at index i (memory is NOT freed)
data_[i].~T();
```

This is the counterpart to placement new:

| Operation              | Effect                          |
|------------------------|---------------------------------|
| `new (&data[i]) T(v)` | Construct an object at address  |
| `data[i].~T()`        | Destroy the object at address   |

**Important:** You should never call `~T()` on an object that was allocated with `new T[]`, because `delete[]` will call the destructor again — resulting in a **double-free** (undefined behavior).

---

### Rvalue References: `T&&`

An **rvalue reference** (`T&&`) binds to **temporary objects** — values that are about to be destroyed and whose resources can be safely stolen.

```cpp
// Lvalue reference (binds to existing, named objects)
void push_back(const T& value);

// Rvalue reference (binds to temporaries / moved-from objects)
void push_back(T&& value);
```

**When does each get called?**

```cpp
DynamicArray<std::string> arr;

std::string name = "hello";
arr.push_back(name);              // Calls push_back(const T&) — 'name' is an lvalue
arr.push_back("world");           // Calls push_back(T&&) — temporary string is an rvalue
arr.push_back(std::move(name));   // Calls push_back(T&&) — std::move casts 'name' to rvalue
```

**Why does this matter?**

- **Copying** (`const T&`): Duplicates all data — allocations, deep copies, etc. Slow for large objects.
- **Moving** (`T&&`): Steals the internal resources (pointers, buffers) from the source. The source is left in a valid but empty state. Fast.

For a `push_back` that takes a temporary `Vector2(5, 10)`, there's no reason to copy it — the temporary is about to die anyway. Moving lets you transfer its data directly into the array.

---

### `std::move`

`std::move` does **not** move anything. It's a **cast** that converts an lvalue into an rvalue reference, signaling that you're okay with the object being moved from.

```cpp
T item = std::move(data_[size_]);
```

**What happens here:**

1. `std::move(data_[size_])` casts `data_[size_]` to `T&&` (rvalue reference)
2. `T item = ...` calls T's **move constructor** (`T(T&&)`)
3. The move constructor steals the resources from `data_[size_]`
4. `data_[size_]` is left in a valid but unspecified state (empty/zeroed)

**Without `std::move`:**

```cpp
T item = data_[size_];  // Calls COPY constructor — slower, duplicates data
```

**With `std::move`:**

```cpp
T item = std::move(data_[size_]);  // Calls MOVE constructor — faster, steals resources
```

Used in `resize()` to efficiently relocate elements from old memory to new memory:

```cpp
new (&data[i]) T(std::move(data_[i]));  // Move-construct into new buffer
data_[i].~T();                          // Destroy the moved-from husk in old buffer
```

---

### `std::forward`

`std::forward` is used for **perfect forwarding** — it preserves the value category (lvalue vs rvalue) of a forwarded argument. It's used in `emplace_back`:

```cpp
template<typename... Args>
void emplace_back(Args&&... args) {
    new (&data_[size_]) T(std::forward<Args>(args)...);
}
```

**Why not just use `std::move`?**

`std::move` **always** casts to rvalue. But `emplace_back` might receive both lvalues and rvalues:

```cpp
int x = 5;
int y = 10;

arr.emplace_back(x, y);     // x and y are lvalues — should be COPIED
arr.emplace_back(3, 7);     // 3 and 7 are rvalues — should be MOVED
```

| Function          | Behavior                                             |
|-------------------|------------------------------------------------------|
| `std::move(arg)`    | Always casts to rvalue (always tries to move)        |
| `std::forward<T>(arg)` | Preserves original category (moves rvalues, copies lvalues) |

`std::forward` ensures that whatever the caller passed in gets forwarded to `T`'s constructor **exactly as it was** — no unnecessary copies, no accidental moves.

---

### Variadic Templates: `template<typename... Args>`

A **parameter pack** that accepts **zero or more** template arguments of any type. The `...` is the pack expansion syntax.

```cpp
template<typename... Args>
void emplace_back(Args&&... args) {
    new (&data_[size_]) T(std::forward<Args>(args)...);
}
```

**Breaking it down:**

| Part               | Meaning                                                     |
|--------------------|-------------------------------------------------------------|
| `typename... Args` | Declare a parameter pack — zero or more types               |
| `Args&&... args`   | Function takes rvalue references to each type in the pack   |
| `args...`          | Expand the pack (pass all arguments to the constructor)     |

**Example — what the compiler generates:**

```cpp
// You write:
arr.emplace_back(5, 10);

// Compiler generates (conceptually):
// Args = {int, int}
// args = {5, 10}
new (&data_[size_]) Vector2(std::forward<int>(5), std::forward<int>(10));
```

**Why use it?**

`emplace_back` constructs the object **directly in the array's memory** using the arguments you provide. No temporary object is created, no copy or move needed:

```cpp
arr.push_back(Vector2(5, 10));  // Creates temporary, then moves it into the array
arr.emplace_back(5, 10);        // Constructs Vector2 directly in the array — faster
```

---

### Iterators

An **iterator** is an object that points to an element inside a container and lets you traverse the container without knowing its internal structure. It's the bridge between your container and C++ algorithms / range-based `for` loops.

#### What is an iterator?

Think of an iterator as a **generalized pointer**. A raw pointer `T*` already lets you:

- Dereference it (`*ptr`) to access the element
- Increment it (`++ptr`) to move to the next element
- Compare it (`ptr1 != ptr2`) to check if you've reached the end

An iterator wraps a raw pointer and exposes the same operations through operator overloads, so it behaves like a pointer but can be customized.

#### How `DynamicArrayIterator` works

The `DynamicArrayIterator<TDynamicArray>` class is templated on the container type. It stores a single `ValueType*` pointer internally:

```cpp
template<typename TDynamicArray>
class DynamicArrayIterator {
public:
    using ValueType = TDynamicArray::ValueType;

    explicit DynamicArrayIterator(ValueType* ptr) : ptr_{ptr} {}

private:
    ValueType* ptr_;  // Raw pointer to an element in the array
};
```

All iterator behavior comes from **operator overloads** on this pointer:

| Operator          | What it does                                | Example           |
|-------------------|---------------------------------------------|-------------------|
| `operator++`      | Move forward to the next element (`++ptr_`) | `++it`            |
| `operator--`      | Move backward to the previous element       | `--it`            |
| `operator+`       | Return a new iterator offset forward        | `it + 3`          |
| `operator-`       | Return a new iterator offset backward       | `it - 2`          |
| `operator*`       | Dereference — access the element            | `*it`             |
| `operator->`      | Access a member of the element              | `it->x`           |
| `operator[]`      | Access element at offset from iterator      | `it[2]`           |
| `operator==`/`!=` | Compare two iterators for equality          | `it != arr.end()` |
| `operator<`/`>`   | Compare iterator positions                  | `it < other`      |
| `operator<=`/`>=` | Compare iterator positions (inclusive)       | `it <= other`     |

#### `const`-qualified operators

Most operators are marked `const` because they **don't modify the iterator itself** (its `ptr_` doesn't change). This is critical — without `const`, you can't use iterators that are passed by `const` reference (e.g., in `erase(const Iterator& first, const Iterator& last)`).

```cpp
// Without const — only works on non-const iterators
bool operator==(const DynamicArrayIterator& other) { return ptr_ == other.ptr_; }

// With const — works on both const and non-const iterators ✓
bool operator==(const DynamicArrayIterator& other) const { return ptr_ == other.ptr_; }
```

#### `begin()` and `end()`

The container exposes two methods that return iterators:

```cpp
Iterator begin() const { return Iterator{data_}; }          // Points to first element
Iterator end()   const { return Iterator{data_ + size_}; }  // Points one-past-last element
```

**Why one-past-the-end?**

The `end()` iterator does **not** point to a valid element. It serves as a sentinel — a marker that says "you've gone past the last element." This convention enables:

- Empty ranges: when `begin() == end()`, the container is empty
- Simple loop conditions: `it != end()` naturally stops after the last element
- Consistent half-open ranges: `[begin, end)` — includes the first, excludes the last

```
Memory layout (array with 3 elements):

  data_[0]    data_[1]    data_[2]    (one past last)
  ┌─────┐    ┌─────┐    ┌─────┐    ┌ ─ ─ ─ ┐
  │  10 │    │  20 │    │  30 │    │  ???  │
  └─────┘    └─────┘    └─────┘    └ ─ ─ ─ ┘
    ▲                                  ▲
  begin()                            end()
```

#### Range-based `for` loop

When you write a range-based `for` loop, the compiler translates it into iterator calls:

```cpp
// What you write:
for (auto& element : arr) {
    std::cout << element << "\n";
}

// What the compiler generates (conceptually):
for (auto it = arr.begin(); it != arr.end(); ++it) {
    auto& element = *it;
    std::cout << element << "\n";
}
```

For this to work, your container **must** provide `begin()` and `end()` methods that return an iterator type with `operator++`, `operator!=`, and `operator*`.

#### Using iterators with `erase()`

Iterators identify **which element(s) to remove** without ambiguity:

```cpp
DynamicArray<int> arr;
arr.push_back(10);
arr.push_back(20);
arr.push_back(30);
arr.push_back(40);
arr.push_back(50);

// Erase single element (the second element, value 20)
arr.erase(arr.begin() + 1);
// arr is now: [10, 30, 40, 50]

// Erase a range [begin+1, begin+3) — removes 30 and 40
arr.erase(arr.begin() + 1, arr.begin() + 3);
// arr is now: [10, 50]
```

**How `erase` works internally:**

```
Before erase(begin() + 1):    [10, 20, 30, 40, 50]   size = 5
                                     ▲
                                  destroy

Step 1: Destroy element at index 1          → data_[1].~T()
Step 2: Move data_[2] into data_[1]         → new (&data_[1]) T(std::move(data_[2]))
Step 3: Move data_[3] into data_[2]         → new (&data_[2]) T(std::move(data_[3]))
Step 4: Move data_[4] into data_[3]         → new (&data_[3]) T(std::move(data_[4]))
Step 5: --size_

After:                         [10, 30, 40, 50]       size = 4
```

#### Manual iteration

Besides range-based `for`, you can iterate manually with full control:

```cpp
// Forward iteration
for (auto it = arr.begin(); it != arr.end(); ++it) {
    std::cout << *it << " ";
}

// Backward iteration (manual, using operator--)
for (auto it = arr.end() - 1; it >= arr.begin(); --it) {
    std::cout << *it << " ";
}

// Random access via operator[]
auto it = arr.begin();
std::cout << it[2] << "\n";  // Access 3rd element from iterator position
```

---

## Implementation

The full implementation lives in a single header file: [`include/DynamicArray.h`](include/Vector.h)

### Memory Lifecycle

```
push_back(value):
    ┌─────────────────────────────────────┐
    │ 1. Check if size_ == capacity_      │
    │    → Yes: resize(capacity_ * 2)     │
    │ 2. Placement new at data_[size_]    │
    │ 3. ++size_                          │
    └─────────────────────────────────────┘

resize(new_capacity):
    ┌──────────────────────────────────────────────────┐
    │ 1. ::operator new → allocate new raw buffer      │
    │ 2. Move-construct each element into new buffer   │
    │ 3. Destroy each element in old buffer (~T())     │
    │ 4. ::operator delete → free old buffer           │
    │ 5. Point data_ to new buffer                     │
    └──────────────────────────────────────────────────┘

clear():
    ┌──────────────────────────────────────────────────┐
    │ 1. Call ~T() on each element (indices 0..size_)  │
    │ 2. Set size_ = 0                                 │
    │ 3. Memory is kept — capacity_ unchanged          │
    └──────────────────────────────────────────────────┘
```

---

## Usage

```cpp
#include "DynamicArray.h"

int main() {
    // Default constructor (capacity = 10, size = 0)
    DynamicArray<int> arr;

    // Add elements
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);

    // Construct elements directly in the array
    arr.emplace_back(4);

    // Access elements
    std::cout << arr[0] << std::endl;  // 1
    arr[0] = 99;                       // Assignment via T& operator[]

    // Remove last element
    arr.pop_back();

    // Query state
    std::cout << arr.size() << std::endl;      // 3
    std::cout << arr.capacity() << std::endl;  // 10
    std::cout << arr.is_empty() << std::endl;  // false

    // Clear all elements (memory is kept)
    arr.clear();

    return 0;
}
```

