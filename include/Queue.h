//
// Created by Dorza on 4/29/2026.
//

#ifndef DATA_STRUCTURES_QUEUE_H
#define DATA_STRUCTURES_QUEUE_H


#include "Deque.h"


namespace dsa {
    /**
     * Queue is a data structure that follows the First-In-First-Out (FIFO) principle,
     * where the first element added to the queue. Under the hood this class is a wrapper
     * for the Deque class, limiting its scope to only allow Queue defined operations
     *
     * @tparam TValue
     */
    template<typename TValue>
    class Queue {
    public:
        /**
         * Default constructor
         */
        Queue()
            : m_deque{} {}

        /**
         * Copy constructor
         *
         * Time complexity: O(n) as it has to copy all the elements from the other Queue&
         * @param other
         */
        Queue(const Queue& other)
            : m_deque{ other.m_deque } {}

        /**
         * Move constructor
         *
         * Time complexity: O(1) as it only needs to move the Deque
         * @param other
         */
        Queue(Queue&& other) noexcept
            : m_deque{ std::move(other.m_deque) } {}

        /**
         * Default destructor
         */
        ~Queue() = default;

        /**
         * Copy assignment operator
         *
         * Time complexity: O(n) as it has to copy all the elements from the other Queue&
         * @param other
         * @return Queue&
         */
        Queue& operator=(const Queue& other) {
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
         * @return Queue&
         */
        Queue& operator=(Queue&& other) noexcept {
            if (&other == this) {
                return *this;
            }

            m_deque = std::move(other.m_deque);

            return *this;
        }

        /**
         * Check if the queue is empty
         *
         * Time complexity: O(1)
         * @return bool
         */
        bool IsEmpty() const {
            return m_deque.IsEmpty();
        }

        /**
         * Get the size of the queue
         *
         * @return std::size_t
         */
        std::size_t Size() const {
            return m_deque.Size();
        }

        /**
         * Get the first element in the queue
         *
         * Time complexity: O(1)
         * @return TValue&
         */
        TValue& Front() const {
            return m_deque.Front();
        }

        /**
         * Get the last element in the queue
         *
         * Time complexity: O(1)
         * @return TValue&
         */
        TValue& Back() const {
            return m_deque.Back();
        }

        /**
         * Push a new element to the back of the queue
         *
         * Time complexity: O(1) if the deque is not full, and O(n) if the deque is full since it needs to resize the deque.
         * @param item
         */
        void Push(const TValue& item) {
            m_deque.PushBack(item);
        }

        /**
         * Push a new element to the back of the queue
         *
         * Time complexity: O(1) if the deque is not full, and O(n) if the deque is full since it needs to resize the deque.
         * @param args
         */
        template<typename... TArgs>
        void Emplace(TArgs&&... args) {
            m_deque.EmplaceBack(std::forward<TArgs>(args)...);
        }

        /**
         * Pop the oldest element (the front) from the queue and return it. If the queue is empty,
         * it will throw an out_of_range exception.
         *
         * Time complexity: O(1)
         * @return TValue
         */
        TValue Pop() {
            return m_deque.PopFront();
        }

        /**
         * Swap the two queues
         *
         * Time complexity: O(1) as it only moves data from and to each other using std::swap()
         * @param other
         */
        void Swap(Queue& other) {
            m_deque.Swap(other.m_deque);
        }

    private:
        Deque<TValue> m_deque;
    };
}


#endif //DATA_STRUCTURES_QUEUE_H