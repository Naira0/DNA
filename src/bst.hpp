#pragma once

#include <cstddef>
#include <iostream>
#include <utility>

// binary search tree implementation

// TODO add key

template<class K, class V>
struct BinaryNode
{
    K key;
    V value;
    BinaryNode<K, V> 
        *left{}, 
        *right{},
        *parent{};
};

template<class K, class V>
class BST
{
private:
public:
    // used for brevity
    using Node = BinaryNode<K, V>;
    using Pair = std::pair<K, V>;

    typedef void(*ForeachFN)(K&, V&);

    BST() = default;

    BST(std::initializer_list<Pair> items)
    {
        for (auto &[key, value]: items)
            insert(key, std::forward<V>(value));
    }

    ~BST()
    {
        destroy(m_root);
    }

    void insert(const K& key, V &&value)
    {
        auto node = new Node { key, std::forward<V>(value) };
        set_node(node);
    }

    void insert(const K &key, const V &value)
    {
        auto node = new Node { key, value };
        set_node(node);
    }

    void foreach(ForeachFN fn)
    {
        foreach_node(m_root, fn);
    }

    Node* find(const K &item) const
    {
        Node *node = m_root;

        while (node && item != node->item)
        {
            if (item < node->item)
                node = node->left;
            else
                node = node->right;
        }

        return node;
    }

    void clear()
    {
        destroy(m_root);
        m_node_count = 0;
    }

    Node* min_node(Node *node = nullptr) const
    {
        
        if (!node)
            node = m_root;

        while (node->left)
            node = node->left;

        return node;
    }

    Node* max_node(Node *node = nullptr) const
    {
        if (!node)
            node = m_root;

        while (node->right)
            node = node->right;

        return node;
    }

    inline V& min() const
    {
        return min_node()->item;
    }

    inline V& max() const
    {
        return max_node()->item;
    }

    void erase(Node *node)
    {
        if (!node->left)
            transplant(node, node->right);
        else if (!node->right)
            transplant(node, node->left);
        else
        {
            Node *smallest = min_node();

            if (smallest->parent != node)
            {
                transplant(smallest, smallest->right);

                smallest->right = node->right;
                smallest->right->parent = smallest;
            }

            transplant(node, smallest);

            smallest->left = node->left;
            smallest->left->parent = node;
        }

        m_node_count--;
        delete node;
    }

    inline void erase(const K &item)
    {
        Node *node = find(item);
        erase(node);
    }

    inline void erase_max()
    {
        erase(max_node());
    }

    inline void erase_min()
    {
        erase(min_node());
    }

    constexpr inline
    size_t node_count() const
    {
        return m_node_count;
    }

private:
    Node *m_root = nullptr;

    size_t m_node_count{};

    void foreach_node(Node *node, ForeachFN fn)
    {
        if (!node)
            return;

        foreach_node(node->left, fn);

        fn(node->item);

        foreach_node(node->right, fn);
    }

    void destroy(Node *node)
    {
        if (!node)
            return;

        destroy(node->left);
        destroy(node->right);

        delete node;
    }

    void set_node(Node *node)
    {
        m_node_count++;

        Node *p = nullptr;
        Node *x = m_root;

        while (x)
        {
            p = x;

            if (node->item < x->item)
                x = x->left;
            else
                x = x->right;
        }

        node->parent = p;

        if (!p)
            m_root = node;
        else if (node->item < p->item)
            p->left = node;
        else
            p->right = node;
    }

    void transplant(Node *target, Node *value)
    {
        if (!target->parent)
            m_root = value;
        else if (target == target->parent->left)
            target->parent->left = value;
        else
            target->parent->right = value;

        if (value)
            value->parent = target->parent;
    }
};
