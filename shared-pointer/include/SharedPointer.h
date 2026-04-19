//
// Created by Okan Ozbek on 4/18/2026
//

#ifndef SHARED_POINTER_H
#define SHARED_POINTER_H


#include <utility>


namespace dsa {
    struct ControlBlock {
        std::size_t count;

        ControlBlock() {
            count = 1;
        }
    };

    template<typename TValueType>
    class SharedPointer {
    public:
        SharedPointer() 
            : pointer_{ nullptr }
            , control_{ nullptr }
        {}

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

        void reset(TValueType* pointer = nullptr) {
            remove_shared();

            if (pointer) {
                pointer_ = pointer;
                control_ = new ControlBlock();
            } else {
                pointer_ = nullptr;
                control_ = nullptr;
            }
        }

        void swap(SharedPointer& other) {
            std::swap(pointer_, other.pointer_);
            std::swap(control_, other.control_);
        }

        TValueType* get() const {
            return pointer_;
        }

        bool is_unique() const {
            return control_ && control_->count == 1;
        }

        std::size_t share_count() const {
            return control_ 
                ? control_->count
                : 0;
        }

    private:
        TValueType* pointer_{nullptr};
        ControlBlock* control_{nullptr};

        void remove_shared() {
            if (!control_) {
                return;
            }

            if (--control_->count == 0) {
                delete pointer_;
                delete control_;
            }
        }
    };
}


#endif // SHARED_POINTER_H