//
// Created by Dorza on 4/28/2026.
//

#ifndef DEQUE_H
#define DEQUE_H


namespace dsa {
    template<typename TValue>
    class Deque {
    public:
        Deque()
            : m_data{ static_cast<TValue*>(::operator new(m_capacity * sizeof(TValue))) }
        {
            for (std::size_t i{}; i < m_capacity; ++i) {
                new (&m_data[i]) T{};
            }
        }

        Deque(const Deque& other)
            : m_data{ static_cast<TValue*>(::operator new(other.m_capacity * sizeof(TValue))) }
        , m_capacity{ other.m_capacity }
        , m_size{ other.m_size }
        {
            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data) TValue(other.m_data[i]);
            }
        }

        Deque(Deque&& other) noexcept
            : m_data{ std::move(other.m_data) }
        , m_capacity{ other.m_capacity }
        , m_size{ other.m_size }
        {
            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
        }

        ~Deque() {
            ::operator delete(m_data);

            m_capacity = 0;
            m_size = 0;
        }

        // Revisit for ring buffer impl
        Deque& operator=(const Deque& other) {
            if (&other == this) {
                return *this;
            }

            Clear();
            ::operator delete(m_data);

            m_data = static_cast<TValue*>(::operator new(m_capacity * sizeof(TValue)));
            m_capacity = other.m_capacity;
            m_size = other.m_size;

            for (std::size_t i{}; i < m_size; ++i) {
                new (&m_data[i]) TValue(other.m_data[i]);
            }

            return *this;
        }

        // Revisit for ring buffer impl
        Deque& operator=(const Deque&& other) {
            if (&other == this) {
                return *this;
            }

            Clear();
            ::operator delete(m_data);

            m_data = std::move(other.m_data);
            m_size = other.m_size;
            m_capacity = other.m_capacity;

            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;

            return *this;
        }

        // Wont work with ring buffer impl
        TValue& operator[](std::size_t index) {
            if (IsOutOfBounds(index)) {
                throw std::out_of_range("Index out of range");
            }

            return m_data[index];
        }

        // Wont work with ring buffer impl
        TValue& At(std::size_t index) {
            if (IsOutOfBounds(index)) {
                throw std::out_of_range("Index out of range");
            }

            return m_data[index];
        }

        // Wont work with ring buffer impl
        TValue& Front() {
            if (IsEmpty()) {
                throw std::out_of_range("Index out of range");
            }

            return m_data[0];
        }

        // Wont work with ring buffer impl
        TValue& Back() {
            if (IsEmpty()) {
                throw std::out_of_range("Index out of range");
            }

            // TODO figure out why this is being cringe..
            return m_data[m_size - 1];
        }

        std::size_t Size() {
            return m_size;
        }

        bool IsEmpty() {
            return m_size > 0;
        }

        // We need to implement a hybrid dynamic-array / ring-buffer style resize
        // The goal is that during the resizing we also linearize all our data
        // Meaning if our buffer was pointing at 4-12 it would now be reset to 0-8
        void Resize() {}
        void ShrinkToFit() {}

        // Push back will just append to our end pointer + 1
        void PushBack() {

        }

        // Push front will just append to our front pointer - 1
        void PushFront() {}

        // Pop back will pop and remove the element from our end pointer
        // End pointer will now be x -= 1
        void PopBack() {

        }

        // Pop front will pop and remove the element from our front pointer
        // Front pointer will now be x += 1
        void PopFront() {}
        void EmplaceFront() {}
        void EmplaceBack() {}
        void Swap() {}

        void Clear() {
            for (std::size_t i{}; i < m_size; ++i) {
                m_data[i].~T();
            }

            m_size = 0;
        }


    private:
        constexpr static int DEFAULT_CAPACITY{ 10 };

        TValue* m_data{ nullptr };
        std::size_t m_capacity{ DEFAULT_CAPACITY };
        std::size_t m_size{ 0 };
        std::size_t m_front{ 0 };
        std::size_t m_back{ 0 };

        bool IsOutOfBounds(std::size_t& index) {
            return index >= m_size;
        }
    };
}



#endif //DATA_STRUCTURES_DEQUE_H