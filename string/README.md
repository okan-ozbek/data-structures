# String

A custom string implementation in C++ that manages raw memory manually — similar to how `std::string` works under the hood.

---

## Table of Contents

- [Overview](#overview)
- [Key Concepts](#key-concepts)
  - [Raw Memory: `::operator new` and `::operator delete`](#raw-memory-operator-new-and-operator-delete)
  - [Placement New: `new (&data[i]) char(c)`](#placement-new-new-datai-charc)
  - [Null Termination](#null-termination)
  - [The Rule of Five](#the-rule-of-five)
  - [Move Semantics: `String&&`](#move-semantics-string)
  - [Operator Overloading](#operator-overloading)
  - [Friend Functions: `operator<<`](#friend-functions-operator)
  - [Iterators](#iterators)
- [Implementation](#implementation)
- [Usage](#usage)

---

## Overview

This `String` class manages a dynamically allocated `char*` buffer using **raw memory management** — the same approach used by `std::string` internally. It separates memory allocation from character construction, giving full control over the buffer lifecycle.

| High-Level (`new char[]` / `delete[]`)         | Low-Level (`::operator new` / `::operator delete`) |
|------------------------------------------------|-----------------------------------------------------|
| Allocates memory **and** initializes all chars  | Allocates **raw memory** only — no initialization    |
| Frees memory **and** cleans up all chars        | Frees **raw memory** only — no cleanup               |
| Simple but inflexible                          | Full control over the buffer                         |

The string maintains three internal members:

| Member      | Type          | Description                                      |
|-------------|---------------|--------------------------------------------------|
| `data_`     | `char*`       | Pointer to the raw character buffer               |
| `size_`     | `std::size_t` | Number of characters currently in the string      |
| `capacity_` | `std::size_t` | Total allocated space (in chars) for the buffer   |

A default capacity of `10` is used when no initial content is provided.

---

## Key Concepts

### Raw Memory: `::operator new` and `::operator delete`

These are the lowest-level memory functions in C++. They allocate and free raw bytes — **no constructors or initializers are called**.

```cpp
// Allocate raw memory for 10 chars (no values are written)
char* data = static_cast<char*>(::operator new(10 * sizeof(char)));

// Free the raw memory (no cleanup is performed)
::operator delete(data);
```

**Why not `new char[10]`?**

`new char[10]` allocates memory **and** value-initializes all 10 characters. For a string with `size = 0` and `capacity = 10`, you don't need 10 initialized characters — you want empty slots ready for future use.

**Think of it like this:**
- `::operator new` → reserving empty shelf space (just space)
- `new char[10]` → reserving shelf space and filling every slot with a blank book

---

### Placement New: `new (&data[i]) char(c)`

Placement new **constructs a value at a specific memory address** that has already been allocated. It does **not** allocate memory — it only writes the value.

```cpp
// Construct the character 'H' at the address &data_[0]
new (&data_[0]) char('H');

// Construct the character 'i' at the address &data_[1]
new (&data_[1]) char('i');
```

**Breaking it down:**

| Part             | Meaning                                           |
|------------------|---------------------------------------------------|
| `new`            | Placement new operator (does NOT allocate memory)  |
| `(&data_[i])`   | The memory address to write the value at           |
| `char(c)`        | Initialize a `char` with value `c`                 |

**Why use it?**

Because `::operator new` gives you raw, uninitialized memory. Placement new is how you **bring a value to life** at that address. For simple types like `char`, a direct assignment (`data_[i] = c`) also works because `char` is trivial — but placement new is used here for consistency with the `DynamicArray<T>` approach.

---

### Null Termination

C-style strings (and our `String`) use a **null terminator** (`'\0'`) to mark the end of the string. This is a `char` with value `0` placed immediately after the last character.

```
Buffer layout for "Hello" (size = 5, capacity = 10):

  data_[0]  data_[1]  data_[2]  data_[3]  data_[4]  data_[5]  ...  data_[9]
  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐       ┌──────┐
  │  'H' │  │  'e' │  │  'l' │  │  'l' │  │  'o' │  │ '\0' │  ...  │  ??? │
  └──────┘  └──────┘  └──────┘  └──────┘  └──────┘  └──────┘       └──────┘
                                                ▲         ▲
                                          last char   null terminator
```

**Why does this matter?**

- The null terminator is **not** counted in `size_`, but it **does** occupy a byte in the buffer.
- When checking if a resize is needed, you must always account for `size_ + 1` (the extra `1` is for `'\0'`).
- Every time you modify the string (`assign`, `append`, `+=`), the null terminator must be placed at `data_[size_]`.

**Common bug:** allocating exactly `size_` bytes and writing `'\0'` past the buffer boundary — this is a **buffer overrun** and causes heap corruption.

---

### The Rule of Five

Since `String` manages a raw `char*` pointer, the compiler-generated defaults would perform **shallow copies** — copying the pointer itself, not the data it points to. This leads to double-free bugs. The **Rule of Five** says: if you define any one of these, you should define all five:

| Special Member             | Purpose                                          |
|----------------------------|--------------------------------------------------|
| **Destructor** `~String()` | Frees the raw buffer with `::operator delete`     |
| **Copy constructor**       | Deep-copies another string's buffer               |
| **Copy assignment**        | Deep-copies via `assign()`                        |
| **Move constructor**       | Steals the buffer pointer from a temporary        |
| **Move assignment**        | Frees own buffer, steals from a temporary         |

#### Copy constructor

Creates a brand-new buffer and copies every character from the source:

```cpp
String(const String& string) {
    data_ = static_cast<char*>(::operator new(capacity * sizeof(char)));
    for (std::size_t i{}; i < size_; ++i) {
        new (&data_[i]) char(string.data_[i]);
    }
    data_[size_] = '\0';
}
```

This is an **O(n)** operation — every character must be copied one by one.

#### Move constructor

Steals the internal pointer from a dying (temporary) string. No data is copied — only three pointer/integer values are transferred:

```cpp
String(String&& string) noexcept :
    data_{string.data_},
    size_{string.size_},
    capacity_{string.capacity_}
{
    string.data_ = nullptr;  // Prevent double-free
    string.size_ = 0;
    string.capacity_ = 0;
}
```

This is an **O(1)** operation — no characters are touched.

**Why set the source to `nullptr`?**

After moving, both `this` and `string` would point to the same buffer. When `string` is destroyed, its destructor would call `::operator delete(data_)` — freeing the buffer that `this` is still using. Setting `string.data_ = nullptr` makes the source's destructor a harmless no-op (deleting `nullptr` is safe).

---

### Move Semantics: `String&&`

An **rvalue reference** (`String&&`) binds to **temporary objects** — values that are about to be destroyed and whose resources can be safely stolen.

```cpp
// Copy assignment (binds to existing, named strings)
String& operator=(const String& string);

// Move assignment (binds to temporaries / moved-from strings)
String& operator=(String&& string) noexcept;
```

**When does each get called?**

```cpp
String a = "hello";
String b = "world";

String c = a;              // Copy constructor — 'a' is an lvalue (it has a name)
String d = std::move(a);   // Move constructor — std::move casts 'a' to rvalue
String e = String("temp"); // Move constructor — the temporary is an rvalue
```

**Move assignment** frees the current buffer, then steals from the source:

```cpp
String& operator=(String&& string) noexcept {
    ::operator delete(data_);    // Free our own buffer

    data_ = string.data_;       // Steal the source's buffer
    size_ = string.size_;

    string.data_ = nullptr;     // Prevent double-free
    string.size_ = 0;

    return *this;
}
```

---

### Operator Overloading

The `String` class overloads several operators to make it behave like a built-in type:

| Operator   | Signature                                      | What it does                         |
|------------|-------------------------------------------------|--------------------------------------|
| `[]`       | `char operator[](std::size_t index) const`      | Access a character by index          |
| `=`        | `String& operator=(const char*)`                | Assign from a C-string               |
| `=`        | `String& operator=(const String&)`              | Copy assignment                      |
| `=`        | `String& operator=(String&&)`                   | Move assignment                      |
| `+=`       | `String& operator+=(const String&)`             | Append a string                      |
| `+=`       | `String& operator+=(const char*)`               | Append a C-string                    |
| `+=`       | `String& operator+=(char)`                      | Append a single character            |
| `==`       | `bool operator==(const String&) const`          | Check equality with another string   |
| `==`       | `bool operator==(const char*) const`            | Check equality with a C-string       |
| `!=`       | `bool operator!=(const String&) const`          | Check inequality                     |
| `!=`       | `bool operator!=(const char*) const`            | Check inequality with a C-string     |

**`const` on operators matters:**

The `const` after the parameter list promises the method won't modify `this`. Without it, you can't call the operator on `const String` objects:

```cpp
const String greeting = "Hello";
char c = greeting[0];    // Only works if operator[] is marked const
bool eq = (greeting == "Hello"); // Only works if operator== is marked const
```

---

### Friend Functions: `operator<<`

The stream insertion operator (`<<`) must be a **free (non-member) function**, not a member function. As a member, `this` would count as the first parameter, giving it 3 parameters instead of the required 2.

```cpp
// Inside the class — declare it as a friend
friend std::ostream& operator<<(std::ostream& os, const String& string);

// Outside the class — define it as a free function
std::ostream& operator<<(std::ostream& os, const String& string) {
    for (std::size_t i{}; i < string.size(); ++i) {
        os << string[i];
    }
    return os;
}
```

**Why `friend`?**

The `friend` declaration grants the free function access to the class's private members (`data_`, `size_`). In this case, since `size()` and `operator[]` are public, `friend` is optional but conventional.

**Why return `std::ostream&`?**

Returning the stream reference enables **chaining**: `std::cout << str1 << " " << str2;`

---

### Iterators

An **iterator** is an object that points to an element inside a container and lets you traverse it without knowing its internal structure. The `StringIterator` wraps a `const char*` pointer and exposes traversal through operator overloads.

#### How `StringIterator` works

```cpp
class StringIterator {
public:
    explicit StringIterator(const char* ptr) : ptr_{ptr} {}

private:
    const char* ptr_;  // Raw pointer to a character in the string
};
```

All iterator behavior comes from **operator overloads** on this pointer:

| Operator          | What it does                                  | Example            |
|-------------------|-----------------------------------------------|--------------------|
| `operator++`      | Move forward to the next character (`++ptr_`) | `++it`             |
| `operator--`      | Move backward to the previous character       | `--it`             |
| `operator+`       | Return a new iterator offset forward          | `it + 3`           |
| `operator-`       | Return a new iterator offset backward         | `it - 2`           |
| `operator*`       | Dereference — access the character            | `*it`              |
| `operator[]`      | Access character at offset from iterator      | `it[2]`            |
| `operator==`/`!=` | Compare two iterators for equality            | `it != str.end()`  |
| `operator<`/`>`   | Compare iterator positions                    | `it < other`       |
| `operator<=`/`>=` | Compare iterator positions (inclusive)         | `it <= other`      |

#### `begin()` and `end()`

The container exposes two methods that return iterators:

```cpp
Iterator begin() const { return Iterator{data_}; }          // Points to first character
Iterator end()   const { return Iterator{data_ + size_}; }  // Points one-past-last character
```

**Why one-past-the-end?**

The `end()` iterator does **not** point to a valid character (it points to the null terminator slot). It serves as a sentinel:

```
Buffer layout for "Hi!" (size = 3):

  data_[0]    data_[1]    data_[2]    data_[3]
  ┌──────┐    ┌──────┐    ┌──────┐    ┌──────┐
  │  'H' │    │  'i' │    │  '!' │    │ '\0' │
  └──────┘    └──────┘    └──────┘    └──────┘
    ▲                                    ▲
  begin()                              end()
```

#### Range-based `for` loop

When you write a range-based `for` loop, the compiler translates it into iterator calls:

```cpp
// What you write:
for (char c : str) {
    std::cout << c;
}

// What the compiler generates (conceptually):
for (auto it = str.begin(); it != str.end(); ++it) {
    char c = *it;
    std::cout << c;
}
```

For this to work, the container must provide `begin()` and `end()` methods returning an iterator type with `operator++`, `operator!=`, and `operator*`.

---

## Implementation

The full implementation lives in two header files: [`include/String.h`](include/String.h) and [`include/StringIterator.h`](include/StringIterator.h).

### Memory Lifecycle

```
assign(string):
    ┌──────────────────────────────────────────────────┐
    │ 1. Calculate string length                       │
    │ 2. Check if length + 1 > capacity_               │
    │    → Yes: resize(new_capacity)                   │
    │ 3. Copy each character into data_                │
    │ 4. Set size_ = length                            │
    │ 5. Place null terminator at data_[size_]         │
    └──────────────────────────────────────────────────┘

append(string):
    ┌──────────────────────────────────────────────────┐
    │ 1. Calculate new total size (size_ + str_len)    │
    │ 2. Check if new_size + 1 > capacity_             │
    │    → Yes: resize(capacity_ * 2 + str_len)        │
    │ 3. Placement new each character after size_      │
    │ 4. Set size_ = new_size                          │
    │ 5. Place null terminator at data_[size_]         │
    └──────────────────────────────────────────────────┘

resize(new_capacity):
    ┌──────────────────────────────────────────────────┐
    │ 1. ::operator new → allocate new raw buffer      │
    │ 2. Copy each character into new buffer           │
    │ 3. ::operator delete → free old buffer           │
    │ 4. Point data_ to new buffer                     │
    │ 5. Update capacity_ (size_ is unchanged)         │
    └──────────────────────────────────────────────────┘
```

### Methods

| Method                                 | Description                                             |
|----------------------------------------|---------------------------------------------------------|
| `assign(const char*)`                  | Replace contents with a C-string                        |
| `append(const char*)` / `append(char)` | Append characters to the end                            |
| `resize(std::size_t)`                  | Reallocate the buffer, preserving existing data          |
| `at(int)`                              | Access character at index                                |
| `front()` / `back()`                   | Access first / last character                            |
| `substr(int, int)`                     | Extract a substring starting at index with given length  |
| `find(const char*)` / `find(String&)`  | Find a substring within the string                       |
| `size()` / `capacity()`                | Query current size and allocated capacity                |
| `data()`                               | Return the raw `const char*` buffer                      |
| `is_empty()`                           | Check if the string has zero characters                  |
| `is_equal(...)` / `operator==`         | Compare two strings for equality                         |
| `strlen(const char*)`                  | Static utility to calculate C-string length              |

---

## Usage

```cpp
#include "String.h"

int main() {
    // Construction from C-string literal
    String str = "Hello world!";
    std::cout << str << std::endl;  // Hello world!

    // Copy construction
    String str2 = str;

    // Equality comparison
    if (str == str2) {
        std::cout << "Equal!" << std::endl;
    }

    // Append with +=
    str += " How are you?";
    std::cout << str << std::endl;  // Hello world! How are you?

    // Single character append
    str += '!';

    // Substring extraction
    String sub = str.substr(6, 5);
    std::cout << sub << std::endl;  // world

    // Find a substring
    String found = str.find("world");
    std::cout << found << std::endl;  // world! How are you?!

    // Iterate over characters
    for (char c : str) {
        std::cout << c;
    }
    std::cout << std::endl;

    // Query state
    std::cout << str.size() << std::endl;      // 26
    std::cout << str.capacity() << std::endl;   // depends on growth
    std::cout << str.is_empty() << std::endl;   // 0 (false)

    // Move semantics
    String str3 = std::move(str);  // str is now empty, str3 owns the buffer

    return 0;
}
```

