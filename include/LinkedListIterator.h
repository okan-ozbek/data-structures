//
// Created by Dorza on 5/1/2026.
//

#ifndef DATA_STRUCTURES_LINKEDLISTITERATOR_H
#define DATA_STRUCTURES_LINKEDLISTITERATOR_H


namespace dsa {
    template<typename TValue>
    struct ListNode;

    template<typename TLinkedList>
    class LinkedListIterator {
    public:
        using ValueType = TLinkedList::ValueType;

        /**
         * Initialize a linked list iterator, pointing to the provided node.
         *
         * @param node
         */
        explicit LinkedListIterator(ListNode<ValueType>* node) : m_node{ node } {}

        /**
         * Move the iterator to the next element in the array.
         *
         * @return LinkedListIterator&
         */
        LinkedListIterator& operator++() {
            if (m_node != nullptr) {
                m_node = m_node->next;
            }

            return *this;
        }

        /**
         * Move the iterator to the previous element in the array.
         *
         * @return LinkedListIterator&
         */
        LinkedListIterator& operator--() {
            if (m_node != nullptr) {
                m_node = m_node->prev;
            }

            return *this;
        }

        /**
         * (constant)
         * Increase the iterator by the specified offset, moving it forward in the array.
         * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
         *
         * @return LinkedListIterator
         */
        LinkedListIterator operator+(std::size_t offset) const {
            auto* node{ m_node };

            while (offset > 0 && node != nullptr) {
                node = node->next;
                --offset;
            }

            return LinkedListIterator{ node };
        }

        /**
         * (constant)
         * Decrease the iterator by the specified offset, moving it backward in the array.
         * The operation also causes the original iterator to be unchanged, and a new iterator to be returned.
         *
         * @param offset
         * @return LinkedListIterator
         */
        LinkedListIterator operator-(std::size_t offset) const {
            auto* node{ m_node };

            while (offset > 0 && node != nullptr) {
                node = node->prev;
                --offset;
            }

            return LinkedListIterator{ node };
        }

        /**
         * Return the node at the specified index.
         *
         * @param index
         * @return ValueType&
         */
        ValueType& operator[](std::size_t index) {
            while (index > 0 && m_node != nullptr) {
                m_node = m_node->next;
                --index;
            }

            return m_node->value;
        }

        /**
         * (constant)
         * Return the node at the specified index.
         *
         * @param index
         * @return ValueType&
         */
        ValueType& operator[](std::size_t index) const {
            while (index > 0 && m_node != nullptr) {
                m_node = m_node->next;
                --index;
            }

            return m_node->value;
        }

        /**
         * Return the dereferenced version of the iterator, which is the node it points to.
         *
         * @return ValueType&
         */
        ValueType& operator*() const {
            return m_node->value;
        }

        /**
         * Return the pointer version of the iterator, which is the node it points to.
         *
         * @return ValueType*
         */
        ValueType* operator->() const {
            return &m_node->value;
        }

        /**
         * (constant)
         * Return true if the iterators point to the same node in the linked list, and false otherwise.
         *
         * @return bool
         */
        bool operator==(const LinkedListIterator& other) const {
            return m_node == other.m_node;
        }

        /**
         * (constant)
         * Return true if the iterators do not point to the same node in the array, and false otherwise.
         *
         * @return bool
         */
        bool operator!=(const LinkedListIterator& other) const {
            return m_node != other.m_node;
        }

        ListNode<ValueType>* pointer() const {
            return m_node;
        }

    private:
        ListNode<ValueType>* m_node{ nullptr };
    };
}


#endif //DATA_STRUCTURES_LINKEDLISTITERATOR_H