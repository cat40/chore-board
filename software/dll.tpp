template <typename T> uint32_t DoubleLinkedList<T>::length(void)
{
    return find_length(head);
}

template <typename T> uint32_t DoubleLinkedList<T>::find_length(DoubleLinkedListNode<T> start_node)
{
    if (start_node == NULL)
    {
        return 0;
    }
    return 1 + find_length(start_node->next);
}

template <typename T> T DoubleLinkedList<T>::get_item_at_index(uint32_t index)
{
    DoubleLinkedListNode<T> node = traverse_list(index, 0, head);
    return node.data;
}

template <typename T> DoubleLinkedListNode<T> DoubleLinkedList<T>::traverse_list(uint32_t target_index, uint32_t current_index, DoubleLinkedListNode<T> node)
{
    if (node == NULL || target_index < 0)
    {
        return NULL;
    }
    if (target_index == current_index)
    {
        return node;
    }
    return traverse_list(target_index, current_index+1, node->next);
}

template <typename T> void DoubleLinkedList<T>::put_item_at_index(uint32_t index, T item)
{
    if (index == 0)
    {
        DoubleLinkedListNode<T> new_node(item, NULL, head);
        head->previous = &new_node;
        this->head = &new_node;
    }
    else if (index < 0)
    {
        // currently an invalid case. May implement backwards indexing in Python style
    }
    else
    {
        DoubleLinkedListNode<T>* leading_node = traverse_list(index-1);
        DoubleLinkedListNode<T>* trailing_node = leading_node->next;
        DoubleLinkedListNode<T> new_node(item, leading_node, trailing_node);
        leading_node->next = &new_node;
        trailing_node->previous = &new_node;
    }
}

template <typename T> T DoubleLinkedList<T>::peek(void)
{
    return head->data;
}

template <typename T> void DoubleLinkedList<T>::put (T item)
{
    DoubleLinkedListNode<T> new_node(item, head, NULL);
    this->head = &new_node;
}

        // uint32_t length;
        // template <typename T> class DoubleLinkedListNode
template <typename T> DoubleLinkedListNode<T>::DoubleLinkedListNode(T data, DoubleLinkedListNode<T>* previous_node, DoubleLinkedListNode<T>* next_node)
{
    this->data = data;
    this->previous = previous_node;
    this->next = next_node;
}
