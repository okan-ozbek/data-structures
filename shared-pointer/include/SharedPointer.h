//
// Created by Okan Ozbek on 4/18/2026
//

#ifndef SHARED_POINTER_H
#define SHARED_POINTER_H


#include <utility>


namespace dsa {
    struct ControlBlock {
        std::size_t count;

        ControlBlock() : count{1} {}
        ~ControlBlock() = default;
    };

    template<typename TValueType>
    class SharedPointer {
    public:
        SharedPointer() = default;

        explicit SharedPointer(TValueType* pointer)
            : pointer_{ pointer } 
            , control_{ pointer ? new ControlBlock() : nullptr }
        {}

        SharedPointer(const SharedPointer& other) 
            : pointer_{ other.pointer_ }
            , control_{ other.control_ }
        {
            if (control_) {
                ++control_->count;
            }
        }

        SharedPointer& operator=(const SharedPointer& other) {
            if (this == &other) {
                return *this;
            }

            remove_shared();

            control_ = other.control_;
            pointer_ = other.pointer_;

            if (control_) {
                ++control_->count;
            }

            return *this;
        }

        SharedPointer(SharedPointer&&) = delete;

        SharedPointer& operator=(SharedPointer&&) = delete;

        ~SharedPointer() {
            remove_shared();
        }

        template<typename TDeleter = std::default_delete<TValueType>>
        void reset(TValueType* pointer = nullptr, TDeleter deleter = TDeleter{}) {
            remove_shared<TDeleter>(deleter);

            if (pointer) {
                pointer_ = pointer;
                control_ = new ControlBlock();
            } else {
                pointer_ = nullptr;
                control_ = nullptr;
            }
        }

        void swap(SharedPointer& other) noexcept {
            std::swap(pointer_, other.pointer_);
            std::swap(control_, other.control_);
        }

        TValueType* get() const {
            return pointer_;
        }

        [[nodiscard]] bool is_unique() const {
            return control_ && control_->count == 1;
        }

        [[nodiscard]] std::size_t share_count() const {
            return control_ 
                ? control_->count
                : 0;
        }

    private:
        TValueType* pointer_{nullptr};
        ControlBlock* control_{nullptr};

        template<typename TDeleter = std::default_delete<TValueType>>
        void remove_shared(TDeleter deleter = TDeleter{}) const {
            if (!control_) {
                return;
            }

            if (--control_->count == 0) {
                deleter(pointer_);
                delete control_;
            }
        }
    };
}


#endif // SHARED_POINTER_H