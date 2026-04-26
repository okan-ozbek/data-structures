//
// Created by Okan Ozbek on 4/24/2026
//

// TODO things to note: this would be a good place to add a ring buffer, read up on what that means.
// TODO also currently we are using Linked Lists as our solution, a basic array with keeping track of indexes
// TODO would probably work way faster, our current implementation is 2ms, while our dsa::Vector is 0.2ms

#ifndef QUEUE_H
#define QUEUE_H


#include <cstddef>

namespace dsa {
    template<typename TValue>
    class Queue {
    public:
        Queue()
            : m_data{ static_cast<TValue*>(::operator new(m_capacity * sizeof(TValue))) }
        {
            Populate();
        }

        explicit Queue(const std::size_t capacity)
            : m_capacity{ capacity }
            , m_data{ static_cast<TValue*>(::operator new(m_capacity * sizeof(TValue))) }
        {
            Populate();
        }

        Queue(const Queue& other)
            : m_data{ static_cast<TValue*>(::operator new(m_capacity * sizeof(TValue))) }
            , m_capacity{ other.m_capacity }
            , m_front{ other.m_front }
            , m_back{ other.m_back }
        {
            Populate(other);
        }

        Queue(Queue&& other) noexcept
            : m_data{ other.m_data }
            , m_capacity{ other.m_capacity }
            , m_front{ other.m_front }
            , m_back{ other.m_back }
        {
            Destroy(other);
        }

        ~Queue() {
            Deallocate();

            m_capacity = 0;
            m_front = 0;
            m_back = 0;
            m_size = 0;
        }

        Queue& operator=(Queue& other) {
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

        Queue& operator=(Queue&& other) noexcept {
            if (&other == this) {
                return *this;
            }

            Deallocate();

            m_data = other.m_data;
            m_capacity = other.m_capacity;
            m_front = other.m_front;
            m_back = other.m_back;
            m_size = other.m_size;

            Destroy(other);

            return *this;
        }

        void Push(const TValue& item) {
            new (&m_data[m_back]) TValue(item);

            m_back = (m_back + 1) % m_capacity;
            ++m_size;
        }

        TValue Pop() {
            if (IsEmpty()) {
                throw std::out_of_range("Queue is empty");
            }

            std::size_t index{ m_front };

            m_front = (m_front + 1) % m_capacity;
            --m_size;

            return std::move(m_data[index]);
        }

        [[nodiscard]] bool IsEmpty() const {
            return (m_front == m_back);
        }

    private:
        constexpr static std::size_t DEFAULT_CAPACITY{ 10 };

        TValue* m_data{ nullptr };
        std::size_t m_front{ 0 };
        std::size_t m_back{ 0 };
        std::size_t m_size{ 0 };
        std::size_t m_capacity{ DEFAULT_CAPACITY };

        void Deallocate() {
            for (std::size_t i{}; i < m_size; ++i) {
                m_data[i].~TValue();
            }

            m_size = 0;
            ::operator delete(m_data);
        }

        void Populate() {
            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) TValue{};
            }
        }

        void Populate(Queue& other) {
            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) TValue(other.m_data[i]);
            }
        }

        void Destroy(Queue&& other) {
            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_front = 0;
            other.m_back = 0;
            other.m_size = 0;
        }
    };
}


#endif // QUEUE_H