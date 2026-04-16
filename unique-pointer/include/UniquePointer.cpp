#ifndef UNIQUE_POINTER_H
#define UNIQUE_POINTER_H


#include <utility>


template<typename T>
class UniquePointer {
public:
    UniquePointer() {}

    explicit UniquePointer(T* pointer) : pointer_{pointer} {}

    UniquePointer(const UniquePointer& other) = delete;

    UniquePointer(UniquePointer&& other) noexcept 
        : pointer_{other.pointer_} 
    { 
        other.pointer_ = nullptr;
    }

    ~UniquePointer() {
        delete pointer_;
    }

    UniquePointer& operator=(const UniquePointer& other) = delete;

    UniquePointer& operator=(UniquePointer&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete pointer_;

        pointer_ = other.pointer_;
        other.pointer_ = nullptr;

        return *this;
    }

    T* operator->() const {
        return pointer_;
    }

    T& operator*() const {
        return *pointer_;
    }

    operator bool() const {
        return (get() != nullptr);
    }

    T* release() {
        auto* stale = pointer_;
        pointer_ = nullptr;

        return stale;
    }

    void reset(T* other = nullptr) {
        auto* stale = pointer_;
        pointer_ = other;
        
        delete stale;
    }

    T* get() {
        return pointer_;
    }

    void swap(UniquePointer& other) noexcept {
        std::swap(pointer_, other.pointer_);
    }

private:
    T* pointer_{nullptr};

};


template<typename T, typename... Args>
UniquePointer<T> make_unique(Args&&... args) {
    return UniquePointer<T>(new T(std::forward<Args>(args)...));
}


#endif // UNIQUE_POINTER_H