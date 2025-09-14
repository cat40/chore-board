#ifndef DLL_H
#define DLL_H

#include <stdint.h>

template <typename T> class DoubleLinkedListNode
{
    public:
        DoubleLinkedListNode(T data, DoubleLinkedListNode<T>* previous_node, DoubleLinkedListNode<T>* next_node);
        T data;
        DoubleLinkedListNode<T>* previous;
        DoubleLinkedListNode<T>* next;
};

// todo implement proper iterator
template <typename T> class DoubleLinkedList
{
    public:
        DoubleLinkedList(void);
        uint32_t length(void);
        T get_item_at_index(uint32_t index);
        void put_item_at_index(uint32_t index, T item);
        T peek(void);
        void put (T item);
    private:
        DoubleLinkedListNode<T>* head;
        DoubleLinkedListNode<T>* tail;
        static uint32_t find_length(DoubleLinkedListNode<T> node);
        DoubleLinkedListNode<T> traverse_list(uint32_t target_index, uint32_t current_index, DoubleLinkedListNode<T> node);
        // uint32_t length;
};

#include "dll.tpp"

#endif