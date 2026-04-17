#ifndef UNIQUE_POINTER_H
#define UNIQUE_POINTER_H


#include <utility>


template<typename T>
class UniqueDeleter {
public:
    void operator()(const T* pointer) noexcept {
        delete pointer;

        deleted_ = true;
    }

    [[nodiscard]] bool deleted() const noexcept {
        return deleted_;
    }

private:
    bool deleted_{false};
};


template<typename T, typename D = UniqueDeleter<T>>
class UniquePointer {
public:
    /**
     * Default constructor. Initializes with nullptr (no ownership).
     */
    UniquePointer() {}

    /**
     * Assign pointer of value T* to our UniquePointer.
     * 
     * @param pointer
     */
    explicit UniquePointer(T* pointer) : pointer_{pointer} {}

    /**
     * Delete the copy constructor.
     */
    UniquePointer(const UniquePointer&) = delete;

    /**
     * Delete the copy assignment operator.
     */
    UniquePointer& operator=(const UniquePointer&) = delete;

    /**
     * Move constructor to move the data from UniquePointer&& over to this object.
     *
     * Time complexity: O(1)
     * @param other
     */
    UniquePointer(UniquePointer&& other) noexcept 
        : pointer_{other.pointer_} 
    { 
        other.pointer_ = nullptr;
    }

    /**
     * Move assignment operator to move the data from UniquePointer&& over to this object
     * If UniquePointer&& is equal to this object it wont do anything and return UniquePointer&
     * 
     * Time complexity: O(1)
     * @param other
     * @return UniquePointer&
     */
    UniquePointer& operator=(UniquePointer&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        reset(other.release());

        return *this;
    }

    /**
     * Destructor. Deletes the owned pointer if not null.
     */
    ~UniquePointer() {
        deleter(pointer_);
    }

    /**
     * Arrow operator. Provides access to the underlying pointer.
     * 
     * @return T*
     */
    T* operator->() const noexcept {
        return pointer_;
    }

    /** 
     * Dereference operator. Returns a reference to the managed object.
     * 
     * @return T&
     */
    T& operator*() const noexcept {
        return *pointer_;
    }

    /**
     * Bool operator to make conditionals work.
     *
     * @return bool
     */
    explicit operator bool() const {
        return (pointer_ != nullptr);
    }

    /**
     * Removes ownership from this object, and returns the pointer to transfer ownership.
     *
     * Time complexity: O(1).
     * @return T*
     */
    T* release() {
        auto* stale = pointer_;
        pointer_ = nullptr;

        return stale;
    }

    /**
     * Changes the ownership of the given pointer to be other, deletes the previous pointer.
     *
     * Time complexity: O(1).
     * @param other
     */
    void reset(T* other = nullptr) noexcept {
        if (pointer_ == other) {
            return;
        }

        auto* stale = pointer_;
        pointer_ = other;
        
        deleter(stale);
    }

    /**
     * Return the pointer
     */
    T* get() const noexcept {
        return pointer_;
    }

    /**
     * Return the deleter
     */
    D get_deleter() const noexcept {
        return deleter;
    }

    /**
     * Swaps the ownership between this pointer and the other pointer.
     * 
     * Time complexity: O(1).
     * @param other
     */
    void swap(UniquePointer& other) noexcept {
        std::swap(pointer_, other.pointer_);
    }

private:
    /**
     * The pointer we are protecting ownership of.
     */
    T* pointer_{nullptr};
    D deleter{};
};

/**
 * Factory function that constructs an object and returns a UniquePointer.
 * Uses perfect forwarding and avoids manual new usage.
 *
 * Time complexity: O(1).
 * @param ...args
 * @return UniquePointer<T>
 */
template<typename T, typename... Args>
UniquePointer<T> make_unique(Args&&... args) {
    return UniquePointer<T>(new T(std::forward<Args>(args)...));
}


#endif // UNIQUE_POINTER_H