//
// Created by Okan Ozbek on 4/18/2026
//

#ifndef SHARED_POINTER_H
#define SHARED_POINTER_H


#include <utility>


namespace dsa {
    template<typename TValueType>
    class SharedPointer {
    public:
        /**
         * Default constructor
         */
        SharedPointer() = default;

        /**
         * Default value constructor
         *
         * @param pointer
         */
        explicit SharedPointer(TValueType* pointer)
            : pointer_{ pointer } 
            , control_{ pointer ? new ControlBlock() : nullptr }
        {}

        /**
         * Copy constructor
         *
         * @param other
         */
        SharedPointer(const SharedPointer& other)
            : pointer_{ other.pointer_ }
            , control_{ other.control_ }
        {
            if (control_) {
                ++control_->count;
            }
        }

        /**
         * Copy assignment operator, handles self-assignment and properly manages reference counting
         *
         * Time complexity: O(1) for self-assignment, O(1) for non-self-assignment (amortized)
         * @param other
         * @return
         */
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

        /**
         * Deleting move constructor
         */
        SharedPointer(SharedPointer&&) = delete;

        /**
         * Deleting move-assignment operator
         *
         * @param SharedPointer&&
         * @return SharedPointer&
         */
        SharedPointer& operator=(SharedPointer&&) = delete;

        ~SharedPointer() {
            remove_shared();
        }

        /**
         * Reset the shared pointer to manage a new raw pointer, properly handling reference counting and memory
         * deallocation of the previously managed object if necessary.
         *
         * Time complexity: O(1) for all cases
         * @tparam TDeleter
         * @param pointer
         * @param deleter
         */
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

        /**
         * Swaps the managed object and control block with another SharedPointer, allowing for efficient transfer
         * of ownership without copying or reference counting adjustments.
         *
         * Time complexity: O(1) std::swap is in constant time
         * @param other
         */
        void swap(SharedPointer& other) noexcept {
            std::swap(pointer_, other.pointer_);
            std::swap(control_, other.control_);
        }

        /**
         * Get the pointer were managing
         *
         * @return TValueType*
         */
        TValueType* get() const {
            return pointer_;
        }

        /**
         * See if there are no other SharedPointer instances sharing ownership of the same object
         *
         * @return bool
         */
        [[nodiscard]] bool is_unique() const {
            return control_ && control_->count == 1;
        }

        /**
         * Return the number of SharedPointer instances sharing ownership of the same object, or 0 if there is no managed object.
         *
         * @return std::size_t
         */
        [[nodiscard]] std::size_t share_count() const {
            return control_ 
                ? control_->count
                : 0;
        }

    private:
        struct ControlBlock {
            std::size_t count;

            ControlBlock() : count{1} {}
            ~ControlBlock() = default;
        };

        TValueType* pointer_{nullptr};
        ControlBlock* control_{nullptr};

        /**
         * Helper function to decrement the reference count and delete the managed object and control block
         * if the count reaches zero, ensuring proper memory management and preventing memory leaks.
         *
         * @tparam TDeleter
         * @param deleter
         */
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