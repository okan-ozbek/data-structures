//
// Created by Okan Ozbek on 4/18/2026
//

#ifndef SHARED_POINTER_H
#define SHARED_POINTER_H


#include <utility>


namespace dsa {
    template<typename TValue>
    class SharedPointer {
    public:
        /**
         * Default constructor
         */
        SharedPointer() = default;

        /**
         * Default value constructor
         *
         * @param pointer
         */
        explicit SharedPointer(TValue* pointer)
            : m_pointer{ pointer }
            , m_control{ pointer ? new ControlBlock() : nullptr }
        {}

        /**
         * Copy constructor
         *
         * @param other
         */
        SharedPointer(const SharedPointer& other)
            : m_pointer{ other.m_pointer }
            , m_control{ other.m_control }
        {
            if (m_control) {
                ++m_control->count;
            }
        }

        /**
         * Copy assignment operator, handles self-assignment and properly manages reference counting
         *
         * Time complexity: O(1) for self-assignment, O(1) for non-self-assignment (amortized)
         * @param other
         * @return
         */
        SharedPointer& operator=(const SharedPointer& other) {
            if (this == &other) {
                return *this;
            }

            RemoveShared();

            m_control = other.m_control;
            m_pointer = other.m_pointer;

            if (m_control) {
                ++m_control->count;
            }

            return *this;
        }

        /**
         * Deleting move constructor
         */
        SharedPointer(SharedPointer&&) = delete;

        /**
         * Deleting move-assignment operator
         *
         * @param SharedPointer&&
         * @return SharedPointer&
         */
        SharedPointer& operator=(SharedPointer&&) = delete;

        ~SharedPointer() {
            RemoveShared();
        }

        /**
         * Reset the shared pointer to manage a new raw pointer, properly handling reference counting and memory
         * deallocation of the previously managed object if necessary.
         *
         * Time complexity: O(1) for all cases
         * @tparam TDeleter
         * @param pointer
         * @param deleter
         */
        template<typename TDeleter = std::default_delete<TValue>>
        void Reset(TValue* pointer = nullptr, TDeleter deleter = TDeleter{}) {
            RemoveShared<TDeleter>(deleter);

            if (pointer) {
                m_pointer = pointer;
                m_control = new ControlBlock();
            } else {
                m_pointer = nullptr;
                m_control = nullptr;
            }
        }

        /**
         * Swaps the managed object and control block with another SharedPointer, allowing for efficient transfer
         * of ownership without copying or reference counting adjustments.
         *
         * Time complexity: O(1) std::swap is in constant time
         * @param other
         */
        void Swap(SharedPointer& other) noexcept {
            std::swap(m_pointer, other.m_pointer);
            std::swap(m_control, other.m_control);
        }

        /**
         * Get the pointer were managing
         *
         * @return TValueType*
         */
        TValue* Get() const {
            return m_pointer;
        }

        /**
         * See if there are no other SharedPointer instances sharing ownership of the same object
         *
         * @return bool
         */
        [[nodiscard]] bool IsUnique() const {
            return m_control && m_control->count == 1;
        }

        /**
         * Return the number of SharedPointer instances sharing ownership of the same object, or 0 if there is no managed object.
         *
         * @return std::size_t
         */
        [[nodiscard]] std::size_t ShareCount() const {
            return m_control
                ? m_control->count
                : 0;
        }

    private:
        struct ControlBlock {
            std::size_t count;

            ControlBlock() : count{ 1 } {}
            ~ControlBlock() = default;
        };

        TValue* m_pointer{ nullptr };
        ControlBlock* m_control{ nullptr };

        /**
         * Helper function to decrement the reference count and delete the managed object and control block
         * if the count reaches zero, ensuring proper memory management and preventing memory leaks.
         *
         * @tparam TDeleter
         * @param deleter
         */
        template<typename TDeleter = std::default_delete<TValue>>
        void RemoveShared(TDeleter deleter = TDeleter{}) const {
            if (!m_pointer) {
                return;
            }

            if (--m_control->count == 0) {
                deleter(m_pointer);
                delete m_control;
            }
        }
    };
}


#endif // SHARED_POINTER_H