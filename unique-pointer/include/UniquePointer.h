#ifndef UNIQUE_POINTER_H
#define UNIQUE_POINTER_H


#include <utility>


template<typename TValueType, typename TDeleter = std::default_delete<TValueType>>
class UniquePointer {
public:
    /**
     * Default constructor. Initializes with nullptr (no ownership).
     */
    UniquePointer() = default;

    /**
     * Assign pointer of value TValueType* to our UniquePointer.
     * 
     * @param pointer
     */
    explicit UniquePointer(TValueType* pointer) : pointer_{pointer} {}

    /**
     * Construct with a pointer and a custom deleter.
     *
     * @param pointer
     * @param deleter
     */
    UniquePointer(TValueType* pointer, TDeleter deleter) : pointer_{pointer}, deleter_{std::move(deleter)} {}

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
        , deleter_{std::move(other.deleter_)}
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
        deleter_ = std::move(other.deleter_);

        return *this;
    }

    /**
     * Destructor. Deletes the owned pointer if not null.
     */
    ~UniquePointer() {
        if (pointer_ == nullptr) {
            delete pointer_;
            return;
        }

        deleter_(pointer_);
    }

    /**
     * Arrow operator. Provides access to the underlying pointer.
     * 
     * @return TValueType*
     */
    TValueType* operator->() const noexcept {
        return pointer_;
    }

    /** 
     * Dereference operator. Returns a reference to the managed object.
     * 
     * @return TValueType&
     */
    TValueType& operator*() const noexcept {
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
     * @return TValueType*
     */
    TValueType* release() {
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
    void reset(TValueType* other = nullptr) noexcept {
        if (pointer_ == other) {
            return;
        }

        auto* stale = pointer_;
        pointer_ = other;

        if (stale == nullptr) {
            return;
        }

        deleter_(stale);
    }

    /**
     * Return the pointer
     */
    TValueType* get() const noexcept {
        return pointer_;
    }

    /**
     * Return the deleter
     */
    TDeleter& get_deleter() noexcept {
        return deleter_;
    }

    const TDeleter& get_deleter() const noexcept {
        return deleter_;
    }

    /**
     * Swaps the ownership between this pointer and the other pointer.
     * 
     * Time complexity: O(1).
     * @param other
     */
    void swap(UniquePointer& other) noexcept {
        std::swap(pointer_, other.pointer_);
        std::swap(deleter_, other.deleter_);
    }

private:
    /**
     * The pointer we are protecting ownership of.
     */
    TValueType* pointer_{nullptr};
    TDeleter deleter_{};
};

/**
 * Factory function that constructs an object and returns a UniquePointer.
 * Uses perfect forwarding and avoids manual new usage.
 *
 * Time complexity: O(1).
 * @param args
 * @return UniquePointer<TValueType>
 */
template<typename TValueType, typename... TArgs>
UniquePointer<TValueType> make_unique(TArgs&&... args) {
    return UniquePointer<TValueType>(new TValueType(std::forward<TArgs>(args)...));
}


#endif // UNIQUE_POINTER_H