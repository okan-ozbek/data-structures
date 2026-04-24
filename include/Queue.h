//
// Created by Okan Ozbek on 4/24/2026
//

// TODO things to note: this would be a good place to add a ring buffer, read up on what that means.
// TODO also currently we are using Linked Lists as our solution, a basic array with keeping track of indexes
// TODO would probably work way faster, our current implementation is 2ms, while our dsa::Vector is 0.2ms

#ifndef QUEUE_H
#define QUEUE_H


#include <cstddef>

template<typename T>
class Queue {
public:
    Queue() = default;

    ~Queue() {
        if (size_ == 0) return;

        QueueNode* current{ head_ };
        QueueNode* pool{ pool_ };

        while (current) {
            const QueueNode* temp{ current };

            current = current->next;

            delete temp;
        }

        while (pool) {
            const QueueNode* temp{ pool };

            pool = pool->next;

            delete temp;
        }
    }

    void push(const T& item) {
        QueueNode* node{ allocate(item) };

        if (tail_ == nullptr) {
            head_ = node;
            tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }

        ++size_;
    }

    T pop() {
        QueueNode* popped{ head_ };
        const T value{ popped->value };
        
        head_ = head_->next;
        --size_;

        deallocate(popped);
        return value;
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

private:
    struct QueueNode {
        QueueNode() = default;

        explicit QueueNode(T value) : value{ value } {}

        ~QueueNode() = default;

        T value{};
        QueueNode* next{};
    };

    QueueNode* allocate(const T& item) {
        if (pool_ == nullptr) {
            return new QueueNode(item);
        }

        QueueNode* node{ pool_ };

        pool_ = pool_->next;
        node->value = item;
        node->next = nullptr;

        return node;
    }

    void deallocate(QueueNode* node) {
        node->next = pool_;
        pool_ = node;
    }

    QueueNode* head_{ nullptr };
    QueueNode* tail_{ nullptr };
    QueueNode* pool_{ nullptr };
    std::size_t size_{ 0 };
};


#endif // QUEUE_H