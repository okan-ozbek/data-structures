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
            , m_capacity{ DEFAULT_CAPACITY }
            , m_size{ 0 }
            , m_front{ 0 }
            , m_back{ 0 }
        {
            for (std::size_t i{}; i < m_capacity; ++i) {
                new (&m_data[i]) TValue{};
            }
        }

        Deque(const Deque& other)
            : m_data{ static_cast<TValue*>(::operator new(other.m_capacity * sizeof(TValue))) }
            , m_capacity{ other.m_capacity }
            , m_size{ other.m_size }
            , m_front{ other.m_front }
            , m_back{ other.m_back }
        {
            for (std::size_t i{}; i < other.m_size; ++i) {
                std::size_t index{ (other.m_front + i) % other.m_capacity - 1 };

                new (&m_data[i]) TValue(other.m_data[index]);
            }
        }

        Deque(Deque&& other) noexcept
            : m_data{ other.m_data }
            , m_capacity{ other.m_capacity }
            , m_size{ other.m_size }
            , m_front{ other.m_front }
            , m_back{ other.m_back }
        {
            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
            other.m_front = 0;
            other.m_back = 0;
        }

        ~Deque() {
            ::operator delete(m_data);

            m_capacity = 0;
            m_size = 0;
            m_front = 0;
            m_back = 0;
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
            m_front = other.m_front;
            m_back = other.m_back;

            for (std::size_t i{}; i < other.m_size; ++i) {
                std::size_t index{ (other.m_front + i) % other.m_capacity - 1 };

                new (&m_data[i]) TValue(other.m_data[index]);
            }

            return *this;
        }

        // Revisit for ring buffer impl
        Deque& operator=(Deque&& other) noexcept {
            if (&other == this) {
                return *this;
            }

            Clear();
            ::operator delete(m_data);

            m_data = other.m_data;
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_front = other.m_front;
            m_back = other.m_back;

            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
            other.m_front = 0;
            other.m_back = 0;

            return *this;
        }

        // Wont work with ring buffer impl
        TValue& operator[](const std::size_t index) const {
            return m_data[(m_front + index) % m_capacity];
        }

        // Wont work with ring buffer impl
        TValue& At(const std::size_t index) const {
            return m_data[(m_front + index) % m_capacity];
        }

        TValue& Front() const {
            return m_data[m_front];
        }

        TValue& Back() const {
            return m_data[m_back];
        }

        std::size_t Size() {
            return m_size;
        }

        bool IsEmpty() {
            return m_size == 0;
        }

        // We need to implement a hybrid dynamic-array / ring-buffer style resize
        // The goal is that during the resizing we also linearize all our data
        // Meaning if our buffer was pointing at 4-12 it would now be reset to 0-8
        void Resize(const std::size_t newCapacity) {
            if (newCapacity == m_capacity) {
                return;
            }

            auto* newData{ static_cast<TValue*>(::operator new(newCapacity * sizeof(TValue))) };

            for (std::size_t i{}; i < m_size; ++i) {
                std::size_t index{ (m_front + i) % m_capacity };

                new (&newData[i]) TValue(std::move(m_data[index]));
            }

            ::operator delete(m_data);
            m_data = newData;

            m_capacity = newCapacity;
            m_front = 0;
            m_back = m_size;
        }

        // void ShrinkToFit() {} TODO extra

        // Push back will just append to our end pointer + 1
        void PushBack(const TValue& item) {
            if (IsFull()) {
                Resize(m_capacity * 2);
            }

            new (&m_data[m_back]) TValue(item);

            m_back = (m_back + 1) % m_capacity;
            ++m_size;
        }

        // Push front will just append to our front pointer - 1
        void PushFront(const TValue& item) {
            if (IsFull()) {
                Resize(m_capacity * 2);
            }

            m_front = (m_front + m_capacity - 1) % m_capacity;
            new (&m_data[m_front]) TValue(item);
            ++m_size;
        }

        // Pop back will pop and remove the element from our end pointer
        // End pointer will now be x -= 1
        TValue PopBack() {
            if (IsEmpty()) {
                throw std::out_of_range("Deque is empty");
            }

            std::size_t index{ m_back };
            m_back = (m_back + m_capacity - 1) % m_capacity;
            --m_size;

            TValue value = std::move(m_data[index]);
            m_data[index].~TValue();

            return value;
        }

        // Pop front will pop and remove the element from our front pointer
        // Front pointer will now be x += 1
        TValue PopFront() {
            if (IsEmpty()) {
                throw std::out_of_range("Deque is empty");
            }

            std::size_t index{ m_front };
            m_front = (m_front + 1) % m_capacity;
            --m_size;

            TValue value = std::move(m_data[index]);
            m_data[index].~TValue();

            return value;
        }

        void EmplaceFront() {}
        void EmplaceBack() {}

        void Swap(const Deque& other) {}

        void Clear() {
            for (std::size_t i{}; i < m_size; ++i) {
                std::size_t index{ (m_front + i) % m_capacity };

                m_data[index].~TValue();
            }

            m_size = 0;
        }


    private:
        constexpr static int DEFAULT_CAPACITY{ 10 };

        TValue* m_data{ nullptr };
        std::size_t m_capacity;
        std::size_t m_size;
        std::size_t m_front;
        std::size_t m_back;

        [[nodiscard]] constexpr bool IsOutOfBounds(const std::size_t& index) const {
            return index >= m_size;
        }

        [[nodiscard]] constexpr bool IsFull() const {
            return m_size == m_capacity;
        }
    };
}



#endif //DATA_STRUCTURES_DEQUE_H