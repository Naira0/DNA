#pragma once

#include <cstddef>
#include <vector>

template<class T, size_t N = 126>
class Trie
{
public:
    struct Node 
    {
        Node *data[N] {};
        T value;
        bool end;
    };

    Trie()
    {
        m_root = new Node();
        m_size = 0;
    }

    Trie(std::initializer_list<std::pair<const std::string_view, T>> list)
    {
        m_root = new Node();
        m_size = 0;

        for (auto [key, value] : list)
        {
            set(key, std::move(value));
        }
    }

    ~Trie()
    {
        destroy(m_root);
    }
    
    std::vector<std::string> keys_with_prefix(std::string_view prefix)
    {
        std::vector<std::string> output;
        std::string buff = std::string{prefix};

        output.reserve(m_size);

        collect(get_node(m_root, prefix), output, buff);

        return output;
    }

    Node* set(std::string_view key, T &&value)
    {
        auto node = m_root;

        for (char c : key)
        {
            auto data = node->data[c];

            if (data == nullptr)
            {
                node->data[c] = new Node();
            }

            node = node->data[c];
        }

        node->value = std::forward<T>(value);
        node->end = true;

        m_size++;

        return node;
    }

    Node* get_node(Node *node, std::string_view key) const 
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        for (char c : key)
        {
            node = node->data[c];
        }

        return node;
    }

    T* get(std::string_view key) const
    {
        auto node = get_node(m_root, key);
        return node ? &node->value : nullptr;
    }

    void erase(std::string_view key)
    {
        auto node = m_root;
        auto last = node;

        int i;

        for (i = 0; i < N; i++)
        {
            if (node != nullptr)
            {
                last = node;
                node = node->data[key[i]];
            }
        }

        last->data[i] = nullptr;

        delete node;

        m_size--;
    }

    size_t size() const 
    {
        return m_size;
    }

private:
    Node *m_root;
    size_t m_size;

    void collect(Node *node, std::vector<std::string> &vec, std::string buff)
    {
        if (node == nullptr)
        {
            return;
        }

        if (node->end)
        {
            vec.push_back(std::move(buff));
        }

        for (int i = 0; i < N; i++)
        {
            collect(node->data[i], vec, buff + (char)i);
        }
    }

    void destroy(Node *node)
    {
        for (int i = 0; i < N; i++)
        {
            if (node->data[i] != nullptr)
            {
                destroy(node->data[i]);
            }
        }

        delete node;
    }
};