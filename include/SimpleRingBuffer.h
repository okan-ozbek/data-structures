//
// Created by Okan Ozbek on 4/26/2026
//

#ifndef RING_BUFFER_H
#define RING_BUFFER_H


#include <iostream>

/**
 * Fixed size ring-buffer also known as a circular buffer. The reason that we often use this is when we want a
 * continuous block of memory for our queue. A couple methods to implement this is as follows:
 *
 * 1. When we only care about the latest data: We simply overwrite the old data as long as we dont use it.
 * 2. When we care about ALL data:
 *    a. We either refuse to rewrite until the old data is popped.
 *    b. We lock/await until data is popped and then push the new data.
 *
 * @tparam T
 */
template<typename T>
class SimpleRingBuffer {
public:
    SimpleRingBuffer() {
        m_data = { static_cast<T *>(::operator new(m_capacity * sizeof(T))) };

        for (std::size_t i{}; i < m_capacity; ++i) {
            new(&m_data[i]) T{};
        }
    }

    SimpleRingBuffer(SimpleRingBuffer& other) = delete;
    SimpleRingBuffer(SimpleRingBuffer&& other) noexcept = delete;

    ~SimpleRingBuffer() {
        ::operator delete(m_data);
    }

    T& operator=(SimpleRingBuffer& other) = delete;
    T& operator=(SimpleRingBuffer&& other) noexcept = delete;

    T& operator[](std::size_t index) {
        if (index >= m_size) {
            return m_data[index % m_capacity];
        }

        return m_data[index];
    }

    void Push(const T& value) {
        if (m_size == m_capacity) {
            return;
        }

        m_data[m_back] = value;

        m_back = (m_back + 1) % m_capacity;
        ++m_size;
    }

    T Pop() {
        if (m_size == 0) {
            return {};
        }

        T temp = m_data[m_front];
        m_data[m_front] = {};

        m_front = (m_front + 1) % m_capacity;
        --m_size;

        return temp;
    }

    T& At(std::size_t index) {
        return m_data[index];
    }

    [[nodiscard]] std::size_t Size() const {
        return m_size;
    }

private:
    constexpr static int DEFAULT_CAPACITY{ 5 };

    T* m_data{ nullptr };
    std::size_t m_front{ 0 }, m_back{ 0 }, m_size{ 0 }, m_capacity{ DEFAULT_CAPACITY };
};


#endif