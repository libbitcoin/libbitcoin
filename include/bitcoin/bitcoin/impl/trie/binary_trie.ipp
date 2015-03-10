/*
 * Copyright 2011-2015
 *
 * Modified from https://github.com/BoostGSoC13/boost.trie
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#ifndef LIBBITCOIN_BINARY_TRIE_IPP
#define LIBBITCOIN_BINARY_TRIE_IPP

namespace libbitcoin {

// binary_trie implementation
template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::binary_trie(
    structure_node_allocator structure_allocator,
    value_node_allocator value_allocator)
    : structure_allocator_(structure_allocator),
        value_allocator_(value_allocator), root_(create_structure_node())
{
    root_->next = root_;
    root_->previous = root_;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::~binary_trie()
{
    destroy();
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
void binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::destroy()
{
    erase_subtree(root_);
    root_ = nullptr;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::create_structure_node()
{
    auto node = structure_allocator_.allocate(1);

    if (node == nullptr)
    {
        throw std::bad_alloc();
    }

    new (node) structure_node_type();

    return node;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::create_structure_node(
    const binary_type& key)
{
    auto node = create_structure_node();

    node->label = key;

    return node;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::create_structure_node(
    const binary_type& key, const value_node_type* value_node)
{
    auto node = create_structure_node(key);

    append_value(node, value_node);

    return node;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::create_structure_node(
    const binary_type& key, const value_type& value)
{
    auto node = create_structure_node(key);
    auto value_node = create_value_node(value);

    append_value(node, value_node);

    return node;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
bool binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::destroy_structure_node(
    structure_node_type* node)
{
    bool result = false;

    if (node != nullptr)
    {
        erase_values(node);
        structure_allocator_.destroy(node);
        structure_allocator_.deallocate(node, 1);
        result = true;
    }

    return result;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
void binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::erase_values(
    structure_node_type* node)
{
    if (node != nullptr)
    {
        value_node_type* value = node->value_head;

        while (value != nullptr)
        {
            value_node_type* tmp = value->next;
            destroy_value_node(value);
            value = tmp;
        }

        node->value_head = nullptr;
        node->value_tail = nullptr;
    }
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
void binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::erase_subtree(
    structure_node_type* node)
{
    if (node != nullptr)
    {
        // identify previous/next pointers which need reset to separate
        // the subtree from the rest of the tree
        structure_node_type* first_in_subtree = node;
        structure_node_type* previous_outside_subtree = nullptr;

        if (node->value_leftmost != nullptr)
        {
            first_in_subtree = node->value_leftmost->anchor;
            previous_outside_subtree = first_in_subtree->previous;
        }

        structure_node_type* last_in_subtree = node;
        structure_node_type* next_outside_subtree = nullptr;

        if (node->value_rightmost != nullptr)
        {
            last_in_subtree = node->value_rightmost->anchor;
            next_outside_subtree = last_in_subtree->next;
        }

        // sever the subtree from the remaining tree by resetting
        // previous, next, parent and child pointers
        structure_node_type* parent_from_tree = node->parent;

        if (parent_from_tree != nullptr)
        {
            parent_from_tree->set_child(node->label[0], nullptr);
            node->parent = nullptr;
        }

        first_in_subtree->previous = node;
        last_in_subtree->next = node;

        if (previous_outside_subtree != nullptr)
        {
            previous_outside_subtree->next = next_outside_subtree;
        }

        if (next_outside_subtree != nullptr)
        {
            next_outside_subtree->previous = previous_outside_subtree;
        }

        // should be done, can't do it now without propegating nullptrs
        // update_left_and_right_branch(parent_from_tree);

        // descend first children to leaf
        structure_node_type* current = get_leftmost_leaf(node);

        // erase node, remembering parent and which child was matching
        // if not first child, current is parent
        // otherwise descend first children of last child until leaf
        while (current != nullptr)
        {
            structure_node_type* next = current->parent;

            if (next != nullptr)
            {
                if (next->get_last_child() != current)
                {
                    next = get_leftmost_leaf(next->get_last_child());
                }

                current->parent->set_child(current->label[0], nullptr);
            }

            // destroy the leaf, regardless of contained values
            destroy_structure_node(current);

            current = next;
        }
    }
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::compress_branch(
    structure_node_type* node)
{
    if (node != nullptr)
    {
        bool update = true;

        // while not the trie root (designated by null parent), having no value
        // and having no children, delete leaf and replace reference with its parent
        while ((node->parent != nullptr) && !node->has_value())
        {
            auto parent = node->parent;

            if (node->has_children())
            {
                // Can we collapse this node out of the trie before termination?
                if (node->get_first_child() == node->get_last_child())
                {
                    auto replacement = node->get_last_child();
                    replacement->label.prepend(node->label);
                    attach_child(parent, replacement);
                    destroy_structure_node(node);
                    node = replacement;
                    update = false; // was triggered by attach_child
                }

                break;
            }

            // remove child reference within parent, delete leaf
            parent->set_child(node->label[0], nullptr);
            unlink_node(node);
            destroy_structure_node(node);
            node = parent;
        }

        if (update)
        {
            update_left_and_right_branch(node);
        }
    }

    return node;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::value_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::create_value_node(
    const value_type& value)
{
    value_node_type* node = value_allocator_.allocate(1);

    if (node == nullptr)
    {
        throw std::bad_alloc();
    }

    new (node) value_node_type(value);

    return node;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
bool binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::destroy_value_node(
    value_node_type* node)
{
    bool result = false;

    if (node != nullptr)
    {
        value_allocator_.destroy(node);
        value_allocator_.deallocate(node, 1);
        result = true;
    }

    return result;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::value_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::append_value(
    structure_node_type* node, value_node_type* value_node)
{
    bool update = false;

    value_node->anchor = node;
    // value_node->next = node->value_head;

    value_node_type* previous = node->value_tail;

    while ((previous != nullptr)
        && !value_comparer_(previous->value, value_node->value))
    {
        previous = previous->previous;
    }

    if (previous != nullptr)
    {
        value_node_type* next = previous->next;

        previous->next = value_node;
        value_node->previous = previous;
        value_node->next = next;

        if (next != nullptr)
        {
            next->previous = value_node;
        }
    }

    if (previous == node->value_tail)
    {
        update = true;
        node->value_tail = value_node;
    }

    if (previous == nullptr)
    {
        update = true;
        value_node->next = node->value_head;
        node->value_head = value_node;
    }

    // fixup left/right pointers
    if (update)
    {
        update_left_and_right_branch(node);
    }

    return value_node;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::value_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::append_value(
    structure_node_type* node, const value_type& value)
{
    value_node_type* value_node = create_value_node(value);
    return append_value(node, value_node);
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::get_leftmost_leaf(
    structure_node_type* origin) const
{
    structure_node_type* current = origin;

    while (current->has_children())
    {
        current = current->get_first_child();
    }

    return current;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::get_rightmost_leaf(
    structure_node_type* origin) const
{
    structure_node_type* current = origin;

    while (current->has_children())
    {
        current = current->get_last_child();
    }

    return current;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::get_leftmost_node(
    structure_node_type* origin) const
{
    structure_node_type* current = origin;

    while (current->has_children() && !(current->has_value()))
    {
        current = current->get_first_child();
    }

    return current;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::get_rightmost_node(
    structure_node_type* origin) const
{
    return get_rightmost_leaf(origin);
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
void binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::update_left_and_right(
    structure_node_type* node)
{
    if (!node->has_children())
    {
        node->value_leftmost = node->value_head;
        node->value_rightmost = node->value_tail;
    }
    else
    {
        if (node->has_value())
        {
            node->value_leftmost = node->value_head;
        }
        else
        {
            node->value_leftmost = node->get_first_child()->value_leftmost;
        }

        node->value_rightmost = node->get_last_child()->value_rightmost;
    }
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
void binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::update_left_and_right_branch(
    structure_node_type* node)
{
    if (node != nullptr)
    {
        // fixup left/right pointers
        auto temp = node;

        while (temp != nullptr)
        {
            update_left_and_right(temp);
            temp = temp->parent;
        }
    }
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::next_node_with_value(
    structure_node_type* node)
{
    // if at root (designated by null parent), terminate
    if (node->parent == nullptr)
        return node;

    auto next = node;

    if (next->has_children())
    {
        // if this node has a child, then at least one value will be located
        // within the subtree, walk the subtree favoring first child until
        // a value is reached
        do
        {
            next = next->get_first_child();
        } while (!next->has_value());
    }
    else
    {
        // if a leaf, back up until a sibling is reached
        while (next->parent != nullptr)
        {
            auto parent = next->parent;

            // if sibling found, decend first children until value
            if (parent->get_last_child() != next)
            {
                next = get_leftmost_node(parent->get_last_child());

                break;
            }

            next = parent;
        }
    }

    return next;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
void binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::link_node(
    structure_node_type* node)
{
    unlink_node(node);

    auto next = next_node_with_value(node);
    auto previous = next->previous;
    node->next = next;
    node->previous = previous;
    next->previous = node;
    previous->next = node;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
void binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::unlink_node(
    structure_node_type* node)
{
    // only unlink linked nodes (note that the root node is linked)
    if ((node->next != nullptr) && (node->previous != nullptr))
    {
        auto next = node->next;
        auto previous = node->previous;
        previous->next = next;
        next->previous = previous;
        node->previous = nullptr;
        node->next = nullptr;
    }
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::begin()
{
    auto value = root_->value_leftmost;
    return (value != nullptr) ? (iterator) value : (iterator) root_;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::end()
{
    return (iterator) root_;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::const_iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::begin() const
{
    auto value = root_->value_leftmost;
    return (value != nullptr) ? (const_iterator) value : (const_iterator) root_;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::const_iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::end() const
{
    return (const_iterator) root_;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::const_iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::cbegin() const
{
    auto value = root_->value_leftmost;
    return (value != nullptr) ? (const_iterator) value : (const_iterator) root_;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::const_iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::cend() const
{
    return (const_iterator) root_;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::reverse_iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::rbegin()
{
    return static_cast<reverse_iterator>(end());
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::reverse_iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::rend()
{
    return static_cast<reverse_iterator>(begin());
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::const_reverse_iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::rbegin() const
{
    return static_cast<const_reverse_iterator>(end());
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::const_reverse_iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::rend() const
{
    return static_cast<const_reverse_iterator>(begin());
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::const_reverse_iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::crbegin() const
{
    return rbegin();
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::const_reverse_iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::crend() const
{
    return rend();
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
void binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::attach_child(
    structure_node_type* parent, structure_node_type* child)
{
    // note: method trusts that the parent's child is safely overwriteable
    child->parent = parent;
    parent->set_child(child->label[0], child);

    if (child->has_value())
    {
        link_node(child);

        update_left_and_right_branch(child);
    }
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::insert_at(
    structure_node_type* current, const binary_type& key)
{
    auto host = create_structure_node(key);

    attach_child(current, host);

    return host;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::structure_node_type*
binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::insert_at(
    structure_node_type* current, const binary_type& key,
    const value_type& value)
{
    auto host = create_structure_node(key, value);

    attach_child(current, host);

    return host;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::pair_iterator_bool binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::insert(
    structure_node_type*& current, const binary_type& key,
    const value_type& value)
{
    binary_type::size_type key_offset = 0;

    for (; key_offset < key.size();)
    {
        auto initial_match = current->get_child(key[key_offset]);

        if (initial_match == nullptr)
        {
            binary_type subkey = key.get_substring(key_offset,
                key.size() - key_offset);

            return std::make_pair(
                (iterator) (insert_at(current, subkey, value)->value_head),
                true);
        }

        // scan for substring mismatch with label, note scan begins at
        // second character as first character was redundantly encoded
        // and checked during child choice for the loop's examination
        bool matches_label = true;
        bool initial_match_label_exceeds_key = false;
        binary_type::size_type label_offset = 1;

        for (; (label_offset < initial_match->label.size()); label_offset++)
        {
            if ((key_offset + label_offset) >= key.size())
            {
                initial_match_label_exceeds_key = true;
                break;
            }

            if (initial_match->label[label_offset]
                != key[key_offset + label_offset])
            {
                matches_label = false;
                break;
            }
        }

        if (!matches_label || initial_match_label_exceeds_key
            || (label_offset != initial_match->label.size()))
        {
            // if there is a disagreement, introduce intermediary node
            // and insert the new branch
            binary_type intermediary_key = initial_match->label.get_substring(0,
                label_offset);

            binary_type trailing_initial_key =
                initial_match->label.get_substring(label_offset);

            // unlink/remove the initial_match from the tree
            unlink_node(initial_match);
            initial_match->parent = nullptr;

            // add intermediary to tree
            auto intermediary = insert_at(current, intermediary_key);

            // add back initial_match with reduced label
            initial_match->label = trailing_initial_key;
            attach_child(intermediary, initial_match);

            if (key.size() > (key_offset + label_offset))
            {
                // if the key has remaining length, insert a sibling
                binary_type remaining_key = key.get_substring(
                    key_offset + label_offset);

                return std::make_pair(
                    (iterator) (insert_at(intermediary, remaining_key, value)->value_head),
                    true);
            }
            else
            {
                // otherwise intermediary label must be key, so add value to 
                // the intermediary which was uniquely added and link_node
                value_node_type* inserted = append_value(intermediary, value);
                link_node(intermediary);

                current = intermediary;
                return std::make_pair((iterator) inserted, true);
            }
        }
        else
        {
            // otherwise, loop decending the tree
            current = initial_match;
            key_offset += label_offset;
        }
    }

    return std::make_pair((iterator) current, false);
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::pair_iterator_bool binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::insert_equal(
    const binary_type& key, const value_type& value)
{
    auto current = root_;
    auto result = insert(current, key, value);

    // if we haven't created a node yet, but exhausted our key
    // we must have matched an existing node, we can append here
    // though we aren't guaranteed to be unique
    if (!result.second && (current != root_))
    {
        bool linked = current->has_value();

        auto inserted = append_value(current, value);

        if (!linked)
        {
            link_node(current);
        }

        return std::make_pair((iterator) inserted, true);
    }

    return result;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::pair_iterator_bool binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::insert_unique(
    const binary_type& key, const value_type& value)
{
    auto current = root_;
    auto result = insert(current, key, value);

    // if we haven't created a node yet, but exhausted our key
    // we must have matched an existing node, if it has no value
    // we can introduce one here
    if (!result.second && !current->has_value() && (current != root_))
    {
        auto inserted = append_value(current, value);

        link_node(current);

        return std::make_pair((iterator) inserted, true);
    }

    return result;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::pair_node_size binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::find_closest_subkey_matching_node(
    structure_node_type* start, const binary_type& key)
{
    auto current = start;
    binary_type::size_type key_offset = 0;
    bool match = true;

    for (; match && (current != nullptr) && (key_offset < key.size());)
    {
        auto initial_match = current->get_child(key[key_offset]);

        if (initial_match != nullptr)
        {
            for (binary_type::size_type label_offset = 0;
                (label_offset < initial_match->label.size())
                    && (key_offset + label_offset < key.size()); label_offset++)
            {
                if (key[key_offset + label_offset]
                    != initial_match->label[label_offset])
                {
                    match = false;
                    break;
                }
            }

            key_offset += initial_match->label.size();
        }

        current = initial_match;
    }

    if (!match)
    {
        current = nullptr;
    }

    return std::make_pair(current, key_offset);
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::iterator_range binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::find_equal(
    const binary_type& key)
{
    auto find_pair = find_closest_subkey_matching_node(root_, key);

    if ((find_pair.first == nullptr) || (find_pair.second != key.size())
        || (find_pair.second == 0))
    {
        return std::make_pair(end(), end());
    }

    iterator begin = (iterator) (find_pair.first->value_head);
    iterator end = (iterator) (find_pair.first->value_tail);
    ++end;

    return std::make_pair(begin, end);
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::iterator_range binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::find_prefix(
    const binary_type& key)
{
    auto find_pair = find_closest_subkey_matching_node(root_, key);

    if ((find_pair.first == nullptr) || (find_pair.second == 0))
    {
        return std::make_pair(end(), end());
    }

    iterator begin = (find_pair.first->value_leftmost);
    iterator end = (find_pair.first->value_rightmost);
    ++end;

    return std::make_pair(begin, end);
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
bool binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::remove_equal(
    const binary_type& key)
{
    auto find_pair = find_closest_subkey_matching_node(root_, key);

    bool nonremovable = ((find_pair.first == nullptr)
        || (find_pair.second != key.size()) || (find_pair.second == 0));

    if (!nonremovable)
    {
        auto node = find_pair.first;

        erase_values(node);
        compress_branch(node);
    }

    return !nonremovable;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
bool binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::remove_prefix(
    const binary_type& key)
{
    auto find_pair = find_closest_subkey_matching_node(root_, key);

    bool nonremovable = ((find_pair.first == nullptr)
        || (find_pair.second < key.size()) || (find_pair.second == 0));

    if (!nonremovable)
    {
        auto node = find_pair.first;
        auto parent = node->parent;

        erase_subtree(node);
        compress_branch(parent);
    }

    return !nonremovable;
}

template<typename Value, typename StructureNodeAllocator,
    typename ValueNodeAllocator, typename Comparer>
typename binary_trie<Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::iterator binary_trie<
    Value, StructureNodeAllocator, ValueNodeAllocator, Comparer>::remove_value(
    iterator it)
{
    auto value_node = it.value_node_;

    if (value_node == nullptr)
    {
        return it;
    }

    auto anchor = value_node->anchor;

    if (anchor->value_head != anchor->value_tail)
    {
        // if the value can be removed without removing emptying the node
        auto next = value_node->next;
        auto previous = value_node->previous;

        if (next != nullptr)
        {
            next->previous = previous;
        }

        if (previous != nullptr)
        {
            previous->next = next;
        }

        value_node->next = nullptr;
        value_node->previous = nullptr;

        bool update = false;

        if (anchor->value_head == value_node)
        {
            anchor->value_head = next;
            update = true;
        }

        if (anchor->value_tail == value_node)
        {
            anchor->value_tail = previous;
            update = true;
        }

        destroy_value_node(value_node);

        if (update)
        {
            update_left_and_right_branch(anchor);
        }

        if (next != nullptr)
        {
            return (iterator) (next);
        }
        else
        {
            return (iterator) (anchor->next);
        }
    }
    else
    {
        auto next = anchor->next;
        // otherwise, remove all values and attempt to remove the node
        erase_values(anchor);
        compress_branch(anchor);

        return (iterator) (next);
    }
}

}

#endif
