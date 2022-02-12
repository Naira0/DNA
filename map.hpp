#pragma once

#include <stdexcept>
#include <functional>
#include <initializer_list>
#include <utility>
#include <vector>

template<typename K, typename V>
class Map
{
public:
    struct Entry
    {
        K key;
        V value;

        Entry() = default;
        Entry(const K& key, const V& value) : key(key), value(value) {}
    };
private:
    struct Chain
    {
        Chain *next = nullptr;
        Entry entry;

        bool is_empty = true;

        Chain() = default;
        Chain(K& key, V& value)
        :   entry(std::move(key), std::move(value)),
            is_empty(false)
        {}
    };

public:

    Map()
    {
        m_capacity = 20;
        m_size     = 0;
        m_table    = new Chain*[m_capacity];

        for(size_t i = 0; i < m_capacity; i++)
            m_table[i] = nullptr;
    }

    Map(std::initializer_list<Entry> items)
    {
        m_size      = items.size();
        m_capacity  = m_size < 10 ? 10 : m_size * 2;
        m_table     = new Chain*[m_capacity];

        for(size_t i = 0; i < m_capacity; i++)
            m_table[i] = nullptr;

        for(auto &[key, value] : items)
            set(std::move(key), std::move(value));
    }

    ~Map()
    {
        destroy();
    }

    std::vector<K> keys() const
    {
        std::vector<K> output;

        output.reserve(m_size+1);

        for(size_t i = 0; i < m_capacity; i++)
        {
            Chain *chain = m_table[i];

            while(chain)
            {
                if(!chain->is_empty)
                    output.push_back(chain->entry.key);
                chain = chain->next;
            }
        }

        return output;
    }

    std::vector<V> values() const
    {
        std::vector<V> output;

        output.reserve(m_size+1);

        for(size_t i = 0; i < m_capacity; i++)
        {
            Chain *chain = m_table[i];

            while(chain)
            {
                if(!chain->is_empty)
                    output.push_back(chain->entry.value);
                chain = chain->next;
            }
        }

        return output;
    }

    void set(const K& key, const V& value)
    {
        if(m_size+1 >= m_capacity)
            rehash(m_capacity * 2);

        size_t h = hash(key);

        auto chain = new Chain(const_cast<K&>(key), const_cast<V&>(value));

        chain->next = m_table[h];
        m_table[h] = chain;

        m_size++;
    }

    void remove(const K& key)
    {
        size_t h = hash(key);

        Chain *chain = m_table[h];

        if(chain->is_empty)
            return;

        Chain *last = nullptr;

        while(chain)
        {
            if(chain->entry.key == key)
            {
                Chain *temp = chain->next;

                delete chain;

                if(last == nullptr)
                    m_table[h] = new Chain();
                else
                    last->next = temp;

                break;
            }

            last = chain;
            chain = chain->next;
        }

        m_size--;
    }

    void clear()
    {
        destroy();

        m_size  = 0;
        m_table = new Chain*[m_capacity];

        for(size_t i = 0; i < m_capacity; i++)
            m_table[i] = nullptr;
    }

    bool contains(const K& key) const
    {
        return search_key(key) != nullptr;
    }

    V* get(const K& key) const
    {
        return search_key(key);
    }

    V& get(const K& key, const V& def_value)
    {
        V *value = search_key(key);
        return value ? *value : const_cast<V&>(def_value);
    }

    V& operator[](const K& key) const
    {
        V *value = search_key(key);

        if(value == nullptr)
            throw std::out_of_range("Key not found");

        return *value;
    }

    [[nodiscard]]
    constexpr inline
    size_t capacity() const
    {
        return m_capacity;
    }

    [[nodiscard]]
    constexpr inline
    size_t size() const
    {
        return m_size;
    }

    [[nodiscard]]
    constexpr inline
    bool empty() const
    {
        return m_size == 0;
    }

private:

    size_t  m_capacity;
    size_t  m_size;
    Chain **m_table;

    std::hash<K> hasher;

    constexpr inline
    size_t hash(const K& key)
    {
        return hasher(key) % m_capacity;
    }

    V* search_key(const K& key)
    {
        Chain *chain = m_table[hash(key)];

        while(chain)
        {
            if(chain->entry.key == key)
                return &chain->entry.value;
            chain = chain->next;
        }

        return nullptr;
    }

    void destroy()
    {
        for(size_t i = 0; i < m_capacity; i++)
        {
            Chain *chain = m_table[i];

            while(chain)
            {
                Chain *next = chain->next;
                delete chain;
                chain = next;
            }
        }

        delete[] m_table;
    }

    void rehash(size_t new_capacity)
    {
        size_t old_capacity = m_capacity;
        m_capacity = new_capacity;

        auto new_table = new Chain*[m_capacity];

        for(size_t i = 0; i < m_capacity; i++)
            new_table[i] = nullptr;

        for(size_t i = 0; i < old_capacity; i++)
        {
            Chain *chain = m_table[i];

            while(chain && !chain->is_empty)
            {
                Chain *next = chain->next;

                size_t h = hash(chain->entry.key);

                chain->next  = new_table[h];
                new_table[h] = chain;

                chain = next;
            }
        }

        delete[] m_table;

        m_table = new_table;
    }
};