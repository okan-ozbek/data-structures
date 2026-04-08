//
// Created by Dorza on 4/7/2026.
//

#ifndef TEMP_CPP_DYNAMICARRAY_H
#define TEMP_CPP_DYNAMICARRAY_H


#include <cstddef>

template<typename T>
class DynamicArray {
public:
    DynamicArray() :
        data_{static_cast<T*>(::operator new(DEFAULT_CAPACITY * sizeof(T)))},
        capacity_{DEFAULT_CAPACITY}
    {
    }

    explicit DynamicArray(const std::size_t capacity) :
        data_{static_cast<T*>(::operator new(capacity * sizeof(T)))},
        size_{capacity},
        capacity_{capacity}
    {
        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) T{};
        }
    }

    explicit DynamicArray(const std::size_t capacity, const T& value) :
        data_{static_cast<T*>(::operator new(capacity * sizeof(T)))},
        size_{capacity},
        capacity_{capacity}
    {
        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) T(value);
        }
    }

    DynamicArray(const DynamicArray& other) :
        data_{static_cast<T*>(::operator new(other.capacity_ * sizeof(T)))},
        size_{other.size_},
        capacity_{other.capacity_}
    {
        for (std::size_t i{}; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    ~DynamicArray() {
        clear();
        ::operator delete(data_, capacity_ * sizeof(T));
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (&other == this) {
            return *this;
        }

        clear();
        ::operator delete(data_, capacity_ * sizeof(T));

        data_ = static_cast<T*>(::operator new(other.capacity_ * sizeof(T)));
        capacity_ = other.capacity_;
        size_ = other.size_;

        for (std::size_t i{}; i < size_; ++i) {
            new (&data_[i]) T(other.data_[i]);
        }

        return *this;
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (&other == this) {
            return *this;
        }

        clear();
        ::operator delete(data_, capacity_ * sizeof(T));

        data_ = std::move(other.data_);
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    const T& operator[](const int index) const {
        return data_[index];
    }

    T& operator[](const int index) {
        return data_[index];
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            resize(capacity_ * 2);
        }

        new (&data_[size_]) T(value);
        ++size_;
    }

    /**
     * A quick note on what the (&&) actually mean in this context:
     * && means that the function takes an rvalue reference, which allows us to move the value instead of copying it.
     * @param value
     */
    void push_back(T&& value) {
        if (size_ == capacity_) {
            resize(capacity_ * 2);
        }

        new (&data_[size_]) T(std::move(value));
        ++size_;
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            resize(capacity_ * 2);
        }

        new (&data_[size_]) T(std::forward<Args>(args)...);
        ++size_;
    }

    [[maybe_unused]] void pop_back() {
        decrement_size();
        data_[size_].~T();
    }

    void clear() {
        for (std::size_t i{}; i < size_; ++i) {
            data_[i].~T();
        }

        size_ = 0;
    }

    [[nodiscard]] constexpr bool is_empty() const {
        return size_ == 0;
    }

    [[nodiscard]] constexpr std::size_t size() const {
        return size_;
    }

    [[nodiscard]] constexpr std::size_t capacity() const {
        return capacity_;
    }

private:
    constexpr static std::size_t DEFAULT_CAPACITY = 10;

    T* data_{nullptr};
    std::size_t size_{};
    std::size_t capacity_{};

    void resize(const std::size_t capacity) {
        T* data = static_cast<T*>(::operator new(capacity * sizeof(T)));

        const std::size_t new_size = (capacity < size_)
            ? capacity
            : size_;

        for (std::size_t i{}; i < new_size; ++i) {
            new (&data[i]) T(std::move(data_[i]));
            data_[i].~T();
        }

        for (std::size_t i{new_size}; i < size_; ++i) {
            data_[i].~T();
        }

        ::operator delete(data_, capacity_ * sizeof(T));

        data_ = data;
        size_ = new_size;
        capacity_ = capacity;
    }

    void decrement_size() {
        --size_;

        if (size_ < 0) {
            size_ = 0;
        }
    }
};


#endif //TEMP_CPP_DYNAMICARRAY_H