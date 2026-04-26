#ifndef UNIQUE_POINTER_H
#define UNIQUE_POINTER_H


#include <memory>
#include <utility>


namespace dsa {
    template<typename TValue, typename TDeleter = std::default_delete<TValue>>
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
        explicit UniquePointer(TValue* pointer)
            : m_pointer{ pointer }
        {}

        /**
        * Construct with a pointer and a custom deleter.
        *
        * @param pointer
        * @param deleter
        */
        UniquePointer(TValue* pointer, TDeleter deleter)
            : TDeleter{ std::move(deleter) }
            , m_pointer{ pointer }
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
            : m_pointer{ other.m_pointer }
        {
            other.m_pointer = nullptr;
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

            Reset(other.Release());

            return *this;
        }

        /**
        * Destructor. Deletes the owned pointer if not null.
        */
        ~UniquePointer() {
            if (m_pointer == nullptr) {
                delete m_pointer;
                return;
            }

            GetDeleter()(m_pointer);
        }

        /**
        * Arrow operator. Provides access to the underlying pointer.
        * 
        * @return TValueType*
        */
        TValue* operator->() const noexcept {
            return m_pointer;
        }

        /** 
        * Dereference operator. Returns a reference to the managed object.
        * 
        * @return TValueType&
        */
        TValue& operator*() const noexcept {
            return *m_pointer;
        }

        /**
        * Bool operator to make conditionals work.
        *
        * @return bool
        */
        explicit operator bool() const {
            return (m_pointer != nullptr);
        }

        /**
        * Removes ownership from this object, and returns the pointer to transfer ownership.
        *
        * Time complexity: O(1).
        * @return TValueType*
        */
        TValue* Release() {
            auto* stale = m_pointer;
            m_pointer = nullptr;

            return stale;
        }

        /**
        * Changes the ownership of the given pointer to be other, deletes the previous pointer.
        *
        * Time complexity: O(1).
        * @param other
        */
        void Reset(TValue* other = nullptr) noexcept {
            if (m_pointer == other) {
                return;
            }

            auto* stale = m_pointer;
            m_pointer = other;

            if (stale == nullptr) {
                return;
            }

            GetDeleter()(stale);
        }

        /**
        * Return the pointer
        */
        TValue* Get() const noexcept {
            return m_pointer;
        }

        /**
        * Return the deleter (access the base class)
        */
        TDeleter& GetDeleter() noexcept {
            return static_cast<TDeleter&>(*this);
        }

        [[nodiscard]] const TDeleter& GetDeleter() const noexcept {
            return static_cast<const TDeleter&>(*this);
        }

        /**
        * Swaps the ownership between this pointer and the other pointer.
        * 
        * Time complexity: O(1).
        * @param other
        */
        void Swap(UniquePointer& other) noexcept {
            std::swap(m_pointer, other.m_pointer);
        }

    private:
        /**
        * The pointer we are protecting ownership of.
        */
        TValue* m_pointer{ nullptr };
    };

    /**
    * Factory function that constructs an object and returns a UniquePointer.
    * Uses perfect forwarding and avoids manual new usage.
    *
    * Time complexity: O(1).
    * @param args
    * @return UniquePointer<TValueType>
    */
    template<typename TValue, typename... TArgs>
    UniquePointer<TValue> make_unique(TArgs&&... args) {
        return UniquePointer<TValue>(new TValue(std::forward<TArgs>(args)...));
    }
}


#endif // UNIQUE_POINTER_H