//
// Created by Dorza on 4/30/2026.
//

#ifndef DATA_STRUCTURES_STACK_H
#define DATA_STRUCTURES_STACK_H


#include "Deque.h"


namespace dsa {
    template<typename TValue>
    class Stack {
    public:
        /**
         * Default constructor
         */
        Stack()
            : m_deque{} {}

        /**
         * Copy constructor
         *
         * Time complexity: O(n) as it has to copy all the elements from the other Stack&
         * @param other
         */
        Stack(const Stack& other)
            : m_deque{ other.m_deque } {}

        /**
         * Move constructor
         *
         * Time complexity: O(1) as it only needs to move the Deque
         * @param other
         */
        Stack(Stack&& other) noexcept
            : m_deque{ std::move(other.m_deque) } {}

        /**
         * Default destructor
         */
        ~Stack() = default;

        /**
         * Copy assignment operator
         *
         * Time complexity: O(n) as it has to copy all the elements from the other Stack&
         * @param other
         * @return Stack&
         */
        Stack& operator=(const Stack& other) {
            if (&other == this) {
                return *this;
            }

            m_deque = other.m_deque;

            return *this;
        }

        /**
         * Move assignment operator
         *
         * Time complexity: O(1) as it only needs to move the Deque
         * @param other
         * @return Stack&
         */
        Stack& operator=(Stack&& other) noexcept {
            if (&other == this) {
                return *this;
            }

            m_deque = std::move(other.m_deque);

            return *this;
        }

        /**
         * Check if the stack is empty
         *
         * Time complexity: O(1)
         * @return bool
         */
        bool IsEmpty() const {
            return m_deque.IsEmpty();
        }

        /**
         * Get the size of the stack
         *
         * @return std::size_t
         */
        std::size_t Size() const {
            return m_deque.Size();
        }

        /**
         * Get the first element in the stack
         *
         * Time complexity: O(1)
         * @return TValue&
         */
        TValue& Front() const {
            return m_deque.Front();
        }

        /**
         * Get the last element in the stack
         *
         * Time complexity: O(1)
         * @return TValue&
         */
        TValue& Back() const {
            return m_deque.Back();
        }

        /**
         * Push a new element to the back of the stack
         *
         * Time complexity: O(1) if the deque is not full, and O(n) if the deque is full since it needs to resize the deque.
         * @param item
         */
        void Push(const TValue& item) {
            m_deque.PushBack(item);
        }

        /**
         * Push a new element to the back of the stack
         *
         * Time complexity: O(1) if the deque is not full, and O(n) if the deque is full since it needs to resize the deque.
         * @param args
         */
        template<typename ...TArgs>
        void Emplace(TArgs&&... args) {
            m_deque.EmplaceBack(std::forward<TArgs>(args)...);
        }

        /**
         * Pop the newest element (the back) from the stack and return it. If the stack is empty,
         * it will throw an out_of_range exception.
         *
         * Time complexity: O(1)
         * @return TValue
         */
        TValue Pop() {
            return m_deque.PopBack();
        }

        /**
         * Swap the two stacks
         *
         * Time complexity: O(1) as it only moves data from and to each other using std::swap()
         * @param other
         */
        void Swap(Stack& other) {
            m_deque.Swap(other.m_deque);
        }

    private:
        Deque<TValue> m_deque;
    };
}


#endif //DATA_STRUCTURES_STACK_H