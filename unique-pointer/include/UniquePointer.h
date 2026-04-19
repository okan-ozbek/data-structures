#ifndef UNIQUE_POINTER_H
#define UNIQUE_POINTER_H


#include <memory>
#include <utility>


namespace dsa {
    template<typename TValueType, typename TDeleter = std::default_delete<TValueType>>
    class UniquePointer : TDeleter {
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
        explicit UniquePointer(TValueType* pointer) 
            : pointer_{ pointer } 
        {}

        /**
        * Construct with a pointer and a custom deleter.
        *
        * @param pointer
        * @param deleter
        */
        UniquePointer(TValueType* pointer, TDeleter deleter) 
            : TDeleter{ std::move(deleter) }
            , pointer_{ pointer } 
        {}

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
            : pointer_{ other.pointer_ } 
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
            if (pointer_ == nullptr) {
                delete pointer_;
                return;
            }

            get_deleter()(pointer_);
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

            get_deleter()(stale);
        }

        /**
        * Return the pointer
        */
        TValueType* get() const noexcept {
            return pointer_;
        }

        /**
        * Return the deleter (access the base class)
        */
        TDeleter& get_deleter() noexcept {
            return static_cast<TDeleter&>(*this);
        }

        const TDeleter& get_deleter() const noexcept {
            return static_cast<const TDeleter&>(*this);
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
        TValueType* pointer_{nullptr};
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
}


#endif // UNIQUE_POINTER_H