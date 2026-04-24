//
// Created by Okan Ozbek on 4/24/2026
//

// TODO things to note: this would be a good place to add a ring buffer, read up on what that means.
// TODO also currently we are using Linked Lists as our solution, a basic array with keeping track of indexes
// TODO would probably work way faster, our current implementation is 2ms, while our dsa::Vector is 0.2ms

#ifndef STACK_H
#define STACK_H


#include <cstddef>


namespace dsa {
    template<typename T>
    class Stack {
    public:
        Stack() = default;

        ~Stack() {
            if (size_ == 0) return;

            StackNode* current{ head_ };
            StackNode* pool{ pool_ };

            while (current) {
                const StackNode* temp{ current };

                current = current->next;

                delete temp;
            }

            while (pool) {
                const StackNode* temp{ pool };

                pool = pool->next;

                delete temp;
            }
        }

        void push(const T& item) {
            StackNode* node{ allocate(item) };

            if (tail_ == nullptr) {
                head_ = node;
                tail_ = node;
            } else {
                node->prev = tail_;
                tail_->next = node;
                tail_ = node;
            }

            ++size_;
        }

        T pop() {
            StackNode* popped{ tail_ };
            const T value{ popped->value };

            tail_ = tail_->prev;
            --size_;

            deallocate(popped);
            return value;
        }

        [[nodiscard]] std::size_t size() const {
            return size_;
        }

    private:
        struct StackNode {
            StackNode() = default;

            explicit StackNode(T value) : value{ value } {}

            ~StackNode() = default;

            T value{};
            StackNode* next{};
            StackNode* prev{};
        };

        StackNode* allocate(const T& item) {
            if (pool_ == nullptr) {
                return new StackNode(item);
            }

            StackNode* node{ pool_ };

            pool_ = pool_->next;
            node->value = item;
            node->next = nullptr;

            return node;
        }

        void deallocate(StackNode* node) {
            node->next = pool_;
            pool_ = node;
        }

        StackNode* head_{ nullptr };
        StackNode* tail_{ nullptr };
        StackNode* pool_{ nullptr };
        std::size_t size_{ 0 };
    };
}


#endif // STACK_H