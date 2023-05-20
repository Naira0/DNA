#pragma once

#include <cstddef>
#include <functional>
#include <vector>
#include <list>
#include <optional>
#include <initializer_list>

#include "record.hpp"

// a hash table implementation

template<class K, class V>
class Map
{
public:
    using Chain = std::list<Record<K, V>>;

    Map(std::initializer_list<Record<K&&, V&&>> list)
    {
        m_size = list.size();

        construct(m_size * 2);

        for (auto &[key, value] : list)
            set(std::forward<K>(key), std::forward<V>(value));
    }

    Map()
    {
        construct(10);
    }

    Map(Map<K, V> &&map) noexcept
    {
        move(std::forward<Map<K, V>>(map));
    }

    Map(const Map<K, V> &map)
    {
        copy(map);
    }

    ~Map()
    {
        delete[] m_bucket;
    }

    Map<K, V>& operator=(Map<K, V> &&map) noexcept
    {
        move(std::forward<Map<K, V>>(map));
        return *this;
    }

    Map<K, V>& operator=(const Map<K, V> &map)
    {
        copy(map);
        return *this;
    }

    [[nodiscard]]
    constexpr inline
    size_t size() const
    {
        return m_size;
    }

    [[nodiscard]]
    constexpr inline
    size_t capacity() const
    {
        return m_capacity;
    }

    [[nodiscard]]
    constexpr inline
    bool empty() const
    {
        return !m_size;
    }

    Record<K, V>& set(K &&key, V &&value)
    {
        Record<K, V> item(std::forward<K>(key), std::forward<V>(value));
        return set_item(item);
    }

    Record<K, V>& set(const K &key, V &&value)
    {
        Record<K, V> item(key, value);
        return set_item(item);
    }

    template<class ...A>
    Record<K, V>& emplace(A &&...a)
    {
        Record<K, V> item(a...);
        return set_item(item);
    }

    V* get(const K &key) const
    {
        return search(key);
    }

    V& operator[](const K &key)
    {
        V *value = search(key);

        if (!value)
            return set(key, V()).value;

        return *value;
    }

    V& get(const K &key, const V &def_value) const
    {
        V *value = search(key);
        return value ? *value : const_cast<V&>(def_value);
    }

    std::vector<V*> get_all(const K &key) const
    {
        size_t h = hash(key);

        std::vector<V*> output;

        for (auto &item : m_bucket[h])
        {
            if (item->key == key)
                output.push_back(&item->value);
        }

        return output;
    }

    bool contains(const K &key) const
    {
        return search(key);
    }

    // returns true if the entry was erased
    bool erase(const K &key)
    {
        size_t h = hash(key);
        Chain &chain = m_bucket[h];

        for (auto it = chain.begin(); it != chain.end(); it++)
        {
            if ((*it)->key == key)
            {
                chain.erase(it);
                m_size--;
                return true;
            }
        }
        return false;
    }

    // removes all entries in map
    void clear()
    {
        delete[] m_bucket;

        m_size = 0;
        construct(10);
    }

    auto begin() const
    {
        return m_bucket;
    }

    auto end() const
    {
        return m_bucket + m_capacity;
    }

private:
    size_t m_size{};
    size_t m_capacity{};
    Chain *m_bucket{};
    std::hash<K> m_hash;

    Record<K, V>& set_item(Record<K, V> &item)
    {
        if (++m_size >= m_capacity)
            rehash();

        size_t h = hash(item.key);

        Chain &chain = m_bucket[h];

        return chain.emplace_back(std::move(item));;
    }

    constexpr inline
    size_t hash(const K &k) const
    {
        return m_hash(k) % m_capacity;
    }

    V* search(const K &key) const
    {
        size_t h = hash(key);

        for (auto &record : m_bucket[h])
        {
            if (record.key == key)
                return &record.value;
        }
        return nullptr;
    }

    inline void construct(int n)
    {
        m_capacity = n;
        m_bucket   = new Chain[m_capacity];

        for (int i = 0; i < m_capacity; i++)
            m_bucket[i] = Chain();
    }

    void rehash()
    {
        const size_t n = m_capacity;

        m_capacity *= 2;

        auto temp = new Chain[m_capacity];

        size_t i;

        for (i = 0; i < n; i++)
        {
            //temp[i] = Chain();

            Chain &old = m_bucket[i];

            if (old.empty())
                continue;

            for (auto &record : old)
            {
                size_t h = hash(record.key);
                Chain &chain = temp[h];

                // if (!chain)
                //     temp[h] = Chain();

                chain.push_back(record);
            }
        }

        delete[] m_bucket;

        m_bucket = temp;
    }

    void move(Map<K, V> &&map) noexcept
    {
        m_bucket = map.m_bucket;
        map.m_bucket = nullptr;

        m_size = map.m_size;
        m_capacity = map.m_capacity;
    }

    void copy(const Map<K, V> &map)
    {
        m_capacity = map.m_capacity;
        m_size = map.m_size;

        m_bucket = new Chain[m_capacity];

        for (size_t i = 0; i < m_capacity; i++)
            m_bucket[i] = map.m_bucket[i];
    }
};
