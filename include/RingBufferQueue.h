//
// Created by Okan Ozbek on 4/24/2026
//

#ifndef QUEUE_H
#define QUEUE_H


#include <cstddef>

namespace dsa {
    template<typename TValue>
    class RingBufferQueue {
    public:
        /**
         * Default constructor
         *
         * Time complexity: O(n) since it needs to populate each reserved memory slot with a default value.
         */
        RingBufferQueue()
            : m_data{ static_cast<TValue*>(::operator new(m_capacity * sizeof(TValue))) }
        {
            Populate();
        }

        /**
         * Capacity constructor
         *
         * Time complexity: O(n) since it needs to populate each reserved memory slot with a default value.
         * @param capacity
         */
        explicit RingBufferQueue(const std::size_t capacity)
            : m_capacity{ capacity }
            , m_data{ static_cast<TValue*>(::operator new(m_capacity * sizeof(TValue))) }
        {
            Populate();
        }

        /**
         * Copy constructor
         *
         * Time complexity: O(n) since it needs to populate each reserved memory slot with a default value.
         * @param other
         */
        RingBufferQueue(const RingBufferQueue& other)
            : m_capacity{ other.m_capacity }
            , m_data{ static_cast<TValue*>(::operator new(m_capacity * sizeof(TValue))) }
            , m_front{ other.m_front }
            , m_back{ other.m_back }
            , m_size{ other.m_size }
        {
            Populate(other);
        }

        /**
         * Move constructor
         *
         * Time complexity: O(1) as it simply moves the data from the moved object into this object
         * @param other
         */
        RingBufferQueue(RingBufferQueue&& other) noexcept
            : m_capacity{ other.m_capacity }
            , m_data{ other.m_data }
            , m_front{ other.m_front }
            , m_back{ other.m_back }
            , m_size( other.m_size )
        {
            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_front = 0;
            other.m_back = 0;
            other.m_size = 0;
        }

        /**
         * Destructor
         *
         * Time complexitY: O(n) as it needs to deallocate each memory slot by calling the destructor of the
         * value type and then deallocate the memory used for the queue.
         */
        ~RingBufferQueue() {
            Deallocate();

            m_capacity = 0;
            m_front = 0;
            m_back = 0;
            m_size = 0;
        }

        /**
         * Copy assignment operator
         *
         * Time complexity: O(n + m) where n is the size of the current queue and m is the size of the other queue.
         * It needs to deallocate the current queue and then populate the new queue with the values from the other queue.
         * @param other
         * @return RingBufferQueue&
         */
        RingBufferQueue& operator=(const RingBufferQueue& other) {
            if (&other == this) {
                return *this;
            }

            Deallocate();

            m_data = static_cast<TValue*>(::operator new(other.m_capacity * sizeof(TValue)));
            m_capacity = other.m_capacity;
            m_front = other.m_front;
            m_back = other.m_back;
            m_size = other.m_size;

            Populate(other);

            return *this;
        }

        /**
         * Move assignment operator
         *
         * Time complexity: O(n) where n is the size of the current queue. It needs to deallocate the current
         * queue and then move the data from the other queue into this queue.
         * @param other
         * @return RingBufferQueue&
         */
        RingBufferQueue& operator=(RingBufferQueue&& other) noexcept {
            if (&other == this) {
                return *this;
            }

            Deallocate();

            m_data = other.m_data;
            m_capacity = other.m_capacity;
            m_front = other.m_front;
            m_back = other.m_back;
            m_size = other.m_size;

            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_front = 0;
            other.m_back = 0;
            other.m_size = 0;

            return *this;
        }

        /**
         * Pushes an item to the back of the queue. If the queue is full, it will overwrite the oldest item in the queue.
         *
         * Time complexity: O(1)
         * @param item
         */
        void Push(const TValue& item) {
            new (&m_data[m_back]) TValue(item);

            ++m_back;
            if (m_back == m_capacity) {
                m_back = 0;
            }
            ++m_size;
        }

        /**
         * Pops the oldest item from the front of the queue. If the queue is empty it will throw an exception.
         *
         * Time complexity: O(1)
         * @return TValue
         */
        TValue Pop() {
            if (IsEmpty()) {
                throw std::out_of_range("Queue is empty");
            }

            std::size_t index{ m_front };

            ++m_front;
            if (m_front == m_capacity) {
                m_front = 0;
            }
            --m_size;

            TValue value = std::move(m_data[index]);
            m_data[index].~TValue();

            return value;
        }

        /**
         * If the queue is empty, it will return true. Otherwise, it will return false.
         * It does this by comparing the location of the m_front and m_back pointer. If they are both
         * pointing to the same adress it means it's currently empty.
         *
         * @return bool
         */
        [[nodiscard]] bool IsEmpty() const {
            return m_front == m_back;
        }

        [[nodiscard]] std::size_t Capacity() const {
            return m_capacity;
        }

    private:
        constexpr static std::size_t DEFAULT_CAPACITY{ 10 };

        std::size_t m_capacity{ DEFAULT_CAPACITY };
        TValue* m_data{ nullptr };
        std::size_t m_front{ 0 };
        std::size_t m_back{ 0 };
        std::size_t m_size{ 0 };

        /**
         * Deallocate the memory reserved for the ring-buffer queue
         *
         * Time complexity: O(n) as it needs to call the destructor for each element in m_data
         */
        void Deallocate() {
            for (std::size_t i{}; i < m_size; ++i) {
                m_data[i].~TValue();
            }

            m_size = 0;
            ::operator delete(m_data);
        }

        /**
         * Default Populate(), responsible for default constructing each element in the reserved memory for the queue.
         *
         * Time complexity: O(n) as it needs to default construct each element.
         */
        void Populate() const {
            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) TValue{};
            }
        }

        /**
         * Populate(const RingBufferQueue& other), responsible for copy constructing each element in the
         * reserved memory for the queue with the values from the other queue.
         *
         * Time complexity: O(n) as it needs to copy over each element.
         * @param other
         */
        void Populate(const RingBufferQueue& other) const {
            for (std::size_t i{}; i < m_size; ++i) {
                std::size_t index{ m_front + i };
                if (index == m_capacity) {
                    index = 0;
                }

                std::size_t otherIndex{ other.m_front + i};
                if (otherIndex == other.m_capacity) {
                    otherIndex = 0;
                }

                new (&m_data[index]) TValue(other.m_data[otherIndex]);
            }
        }
    };
}


#endif // QUEUE_H