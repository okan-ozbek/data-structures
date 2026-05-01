//
// Created by Dorza on 5/1/2026.
//

#ifndef DATA_STRUCTURES_LINKEDLIST_H
#define DATA_STRUCTURES_LINKEDLIST_H


#include <cstddef>
#include "LinkedListIterator.h"


namespace dsa {
    /**
     * ListNode to keep track of the value, previous attached node and next attached node
     *
     * @tparam TValue
     */
    template<typename TValue>
    struct ListNode {
        TValue value;
        ListNode* next{ nullptr };
        ListNode* prev{ nullptr };

        /**
         * Value constructor, a list node should always have a value.
         *
         * @param value
         */
        explicit ListNode(const TValue& value)
            : value{ value }
        {}

        /**
         * Value next constructor, a list node can also immediately attach itself to a next node.
         *
         * @param value
         * @param next
         */
        explicit ListNode(const TValue& value, ListNode* next)
            : value{ value }
            , next{ next }
        {}

        /**
         * Value next previous constructor, list node is doubly-linked allowing it to have a previous and next.
         *
         * @param value
         * @param next
         * @param prev
         */
        explicit ListNode(const TValue& value, ListNode* next, ListNode* prev)
            : value{ value }
            , next{ next }
            , prev{ prev }
        {}

        ~ListNode() {
            next = nullptr;
            prev = nullptr;
        }
    };

    template<typename TValue>
    class LinkedList {
    public:
        using Iterator = LinkedListIterator<LinkedList>;
        using ValueType = TValue;

        /**
         * Default constructor
         */
        LinkedList() = default;

        /**
         * Copy constructor, default allows for shallow copying of the linked list,
         * which means that the new linked list will point to the same nodes as the original linked list.
         * This can lead to issues if one of the linked lists is modified,
         * as it will affect the other linked list as well.
         *
         * @param other
         */
        LinkedList(const LinkedList& other) = default;

        /**
         * Move constructor, move all linked list elements over from the other linked list to the new linked list,
         * and set the other linked list to an empty state.
         *
         * @param other
         */
        LinkedList(LinkedList&& other) noexcept
            : m_head{ other.m_head }
            , m_tail{ other.m_tail }
            , m_size{ other.m_size }
        {
            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_size = 0;
        }

        /**
         * Copy assignment operator, allows for shallow copying of the linked list,
         * which means that the new linked list will point to the same nodes as the original linked list.
         * This can lead to issues if one of the linked lists is modified,
         * as it will affect the other linked list as well.
         *
         * Time complexity: O(1)
         * @param other
         * @return LinkedList&
         */
        LinkedList& operator=(const LinkedList& other) {
            if (this == &other) {
                return *this;
            }

            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;

            return *this;
        }

        /**
         * Move constructor, move all linked list elements over from the other linked list to the new linked list,
         * and set the other linked list to an empty state.
         *
         * Time complexity: O(1)
         * @param other
         * @return LinkedList&
         */
        LinkedList& operator=(LinkedList&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;

            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_size = 0;

            return *this;
        }

        /**
         * Returns the value at the specified index.
         *
         * Time complexity: O(n) since we need to loop through each element in the linked list.
         * @param index
         * @return TValue&
         */
        TValue& operator[](std::size_t index) {
            return At(index);
        }

        /**
         * Starting point iterator, pointing to the first element
         *
         * @return Iterator
         */
        [[nodiscard]] Iterator begin() const {
            return Iterator{ m_head };
        }

        /**
         * Ending point iterator, pointing to the element after the last element, which is nullptr
         *
         * @return Iterator
         */
        [[nodiscard]] Iterator end() const {
            if (m_tail == nullptr) {
                return Iterator{ nullptr };
            }

            return Iterator{ m_tail->next };
        }

        /**
         * Find the list node at the given index, if there is no list node present return nullptr.
         *
         * Time complexity: O(n) since we need to loop through each node.
         * @param index
         * @return ListNode<TValue>*
         */
        ListNode<TValue>* Find(std::size_t index) {
            auto* current{ m_head };

            while (index > 0 && current != nullptr) {
                current = current->next;
                --index;
            }

            return current;
        }

        /**
         * Return the head list node pointer.
         *
         * Time complexity: O(1) since we are just returning the head pointer.
         * @return ListNode<TValue>*
         */
        ListNode<TValue>* Head() {
            return m_head;
        }

        /**
         * Return the tail list node pointer
         *
         * Time complexity: O(1) since we are just returning the tail pointer.
         * @return ListNode<TValue>*
         */
        ListNode<TValue>* Tail() {
            return m_tail;
        }

        /**
         * Returns the value at the given index
         *
         * Time complexity: O(n) since we need to loop through each node.
         * @param index
         * @return TValue&
         */
        TValue& At(std::size_t index) {
            auto* current{ m_head };

            while (index > 0 && current != nullptr) {
                current = current->next;
                --index;
            }

            return current->value;
        }

        /**
         * Returns the value at the head list node pointer.
         *
         * Time complexity: O(1) since we are just returning the head pointer.
         * @return TValue&
         */
        TValue& Front() {
            return m_head->value;
        }

        /**
         * Return the value at the tail list node pointer
         *
         * Time complexity: O(1) since we are just returning the tail pointer.
         * @return TValue&
         */
        TValue& Back() {
            return m_tail->value;
        }

        /**
         * Appends a new node at the start of the linked list
         *
         * Time complexity: O(1) since we are just adding a new node at the start of the linked list,
         * and updating the head pointer.
         * @param value
         */
        void PushFront(const TValue& value) {
            if (m_head == nullptr) {
                m_head = new ListNode(value);
                m_tail = m_head;
                ++m_size;

                return;
            }

            auto* newNode{ new ListNode(value, m_head) };

            m_head->prev = newNode;
            m_head = newNode;
            ++m_size;
        }

        /**
         * Appends a new node at the end of the linked list
         *
         * Time complexity: O(1) since we are just adding a new node at the end of the linked list,
         * and updating the tail pointer.
         * @param value
         */
        void PushBack(const TValue& value) {
            if (m_tail == nullptr) {
                m_tail = new ListNode(value);
                m_head = m_tail;
                ++m_size;

                return;
            }

            auto* newNode{ new ListNode<TValue>(value, nullptr, m_tail) };

            m_tail->next = newNode;
            m_tail = newNode;
            ++m_size;
        }

        /**
         * Appends a new node in between two nodes of the linked list
         *
         * Time complexity: O(1) since we are just adding a new node in between two nodes of the linked list,
         * and updating the next and previous pointers of the new node, and the adjacent nodes.
         * @param node
         * @param value
         */
        void PushAfter(ListNode<TValue>* node, const TValue& value) {
            auto* newNode{ new ListNode(value, node->next, node) };

            node->next = newNode;
            ++m_size;
        }

        /**
         * Appends a new node in between two nodes of the linked list
         *
         * Time complexity: O(1) since we are just adding a new node in between two nodes of the linked list,
         * and updating the next and previous pointers of the new node, and the adjacent nodes.
         * @param index
         * @param value
         */
        void PushAfter(const std::size_t index, const TValue& value) {
            auto* node{ Find(index) };
            auto* newNode{ new ListNode(value, node->next, node) };

            node->next = newNode;
            ++m_size;
        }

        /**
         * Pops the first element from the linked list, returning it in the process
         *
         * Time complexity: O(1) since we simply get the head, and update the new head to be the next node
         * in the linked list.
         * @return TValue
         */
        TValue PopFront() {
            if (m_head == nullptr) {
                return TValue{};
            }

            auto* current{ m_head };
            auto value = current->value;

            m_head = current->next;
            --m_size;

            if (m_head == nullptr) {
                m_tail = nullptr;
            } else {
                m_head->prev = nullptr;
            }

            delete current;
            return value;
        }

        /**
         * Pops the last element from the linked list, returning it in the process
         *
         * Time complexity: O(1) since we simply get the tail, and update the new tail to be the previous node
         * in the linked list.
         * @return TValue
         */
        TValue PopBack() {
            if (m_tail == nullptr) {
                return TValue{};
            }

            auto* current{ m_tail };
            auto value = current->value;

            m_tail = current->prev;
            --m_size;

            if (m_tail == nullptr) {
                m_head = nullptr;
            } else {
                m_tail->next = nullptr;
            }

            delete current;
            return value;
        }

        /**
         * Removes an element based on the iterator, and returns an iterator
         * pointing to the next element after the removed element.
         *
         * Time complexity: O(1)
         * @param iterator
         * @return Iterator
         */
        Iterator Erase(const Iterator& iterator) {
            if (IsEmpty() || iterator == end()) {
                return end();
            }

            auto* node{ iterator.pointer() };
            auto* next{ node->next };

            if (node->prev != nullptr) {
                node->prev->next = next;
            } else {
                m_head = node->next;
            }

            if (node->next != nullptr) {
                node->next->prev = next;
            } else {
                m_tail = node->prev;
            }

            delete node;
            --m_size;

            return Iterator{ next };
        }

        /**
         * Returns the size of the linked list
         *
         * @return std::size_t
         */
        [[nodiscard]] std::size_t Size() const {
            return m_size;
        }

        /**
         * Returns wether the linked list is empty or not
         *
         * @return bool
         */
        [[nodiscard]] bool IsEmpty() const {
            return m_size == 0 || m_head == nullptr || m_tail == nullptr;
        }

    private:
        ListNode<TValue>* m_head{ nullptr };
        ListNode<TValue>* m_tail{ nullptr };

        std::size_t m_size{ 0 };
    };
}


#endif //DATA_STRUCTURES_LINKEDLIST_H