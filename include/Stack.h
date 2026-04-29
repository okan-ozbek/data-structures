//
// Created by Dorza on 4/30/2026.
//

#ifndef DATA_STRUCTURES_STACK_H
#define DATA_STRUCTURES_STACK_H


#include "Deque.h"


namespace dsa {
    template<typename TValue>
    class Stack {
    public:
        Stack()
            : m_deque{} {}

        Stack(const Stack& other)
            : m_deque{ other.m_deque } {}

        Stack(Stack&& other) noexcept
            : m_deque{ std::move(other.m_deque) } {}

        ~Stack() = default;

        Stack& operator=(const Stack& other) {
            if (&other == this) {
                return *this;
            }

            m_deque = other.m_deque;

            return *this;
        }

        Stack& operator=(Stack&& other) noexcept {
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

        template<typename ...TArgs>
        void Emplace(TArgs&&... args) {
            m_deque.EmplaceBack(std::forward<TArgs>(args)...);
        }

        TValue Pop() {
            return m_deque.PopBack();
        }

        void Swap(Stack& other) {
            m_deque.Swap(other.m_deque);
        }

    private:
        Deque<TValue> m_deque;
    };
}


#endif //DATA_STRUCTURES_STACK_H