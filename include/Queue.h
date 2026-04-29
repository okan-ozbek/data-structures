//
// Created by Dorza on 4/29/2026.
//

#ifndef DATA_STRUCTURES_QUEUE_H
#define DATA_STRUCTURES_QUEUE_H


#include "Deque.h"


namespace dsa {
    template<typename TValue>
    class Queue {
    public:
        Queue()
            : m_deque{} {}

        Queue(const Queue& other)
            : m_deque{ other.m_deque } {}

        Queue(Queue&& other) noexcept
            : m_deque{ std::move(other.m_deque) } {}

        ~Queue() = default;

        Queue& operator=(const Queue& other) {
            if (&other == this) {
                return *this;
            }

            m_deque = other.m_deque;

            return *this;
        }

        Queue& operator=(Queue&& other) noexcept {
            if (&other == this) {
                return *this;
            }

            m_deque = std::move(other.m_deque);

            return *this;
        }

        bool IsEmpty() const {
            return m_deque.IsEmpty();
        }

        std::size_t Size() const {
            return m_deque.Size();
        }

        TValue& Front() const {
            return m_deque.Front();
        }

        TValue& Back() const {
            return m_deque.Back();
        }

        void Push(const TValue& item) {
            m_deque.PushBack(item);
        }

        template<typename... TArgs>
        void Emplace(TArgs&&... args) {
            m_deque.EmplaceBack(std::forward<TArgs>(args)...);
        }

        TValue Pop() {
            return m_deque.PopFront();
        }

        void Swap(Queue& other) {
            m_deque.Swap(other.m_deque);
        }

    private:
        Deque<TValue> m_deque;
    };
}


#endif //DATA_STRUCTURES_QUEUE_H