#pragma once

#include <cstddef>
#include <functional>
#include <vector>
#include <list>
#include <optional>
#include <initializer_list>

#include "record.hpp"

// a hash table implementation that maintains insertion order using std::list

template<class K, class V>
class OMap
{
public:
    using IterType = typename std::list<Record<K, V>>::iterator;
    using Chain = std::list<IterType>;

    OMap(std::initializer_list<Record<K&&, V&&>> list)
    {
        m_size = list.size();

        construct(m_size * 2);

        for (auto &[key, value] : list)
            set(std::forward<K>(key), std::forward<V>(value));
    }

    OMap()
    {
        construct(10);
    }

    OMap(OMap<K, V> &&map) noexcept
    {
        move(std::forward<OMap<K, V>>(map));
    }

    OMap(const OMap<K, V> &OMap)
    {
        copy(OMap);
    }

    ~OMap()
    {
        delete[] m_bucket;
    }

    OMap<K, V>& operator=(OMap<K, V> &&map) noexcept
    {
        move(std::forward<OMap<K, V>>(map));
        return *this;
    }

    OMap<K, V>& operator=(const OMap<K, V> &OMap)
    {
        copy(OMap);
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

    // returns a pointer instead of an optional because realistically it would end in the same operation ie checking if its valid and using it
    // this just results in less verbose code
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

    // gets all values of duplicate keys
    std::vector<V*> get_all(const K &key) const
    {
        size_t h = hash(key);

        std::vector<V*> output;

        for (IterType item : m_bucket[h])
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
                m_items.erase(*it);
                chain.erase(it);
                m_size--;
                return true;
            }
        }
        return false;
    }

    // removes all entries in OMap
    void clear()
    {
        m_items.clear();

        delete[] m_bucket;

        m_size = 0;
        construct(10);
    }

    auto begin() const
    {
        return m_items.begin();
    }

    auto end() const
    {
        return m_items.end();
    }

private:
    size_t m_size{};
    size_t m_capacity{};
    Chain *m_bucket{};
    std::list<Record<K, V>> m_items;
    std::hash<K> m_hash;

    Record<K, V>& set_item(Record<K, V> &item)
    {
        if (++m_size >= m_capacity)
            rehash();

        size_t h = hash(item.key);

        Chain &chain = m_bucket[h];

        IterType iter = m_items.emplace(m_items.end(), std::move(item));
        chain.emplace_back(iter);

        return *iter;
    }

    constexpr inline
    size_t hash(const K &k) const
    {
        return m_hash(k) % m_capacity;
    }

    V* search(const K &key) const
    {
        size_t h = hash(key);

        for (IterType item : m_bucket[h])
        {
            if (item->key == key)
                return &item->value;
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
            temp[i] = Chain();
            Chain &old = m_bucket[i];

            if (old.empty())
                continue;

            for (IterType item : old)
            {
                size_t h = hash(item->key);
                Chain *chain = &temp[h];

                if (!chain)
                    temp[h] = Chain();

                chain->push_back(item);
            }
        }

        delete[] m_bucket;

        m_bucket = temp;
    }

    void move(OMap<K, V> &&OMap) noexcept
    {
        m_bucket = OMap.m_bucket;
        OMap.m_bucket = nullptr;

        m_size = OMap.m_size;
        m_capacity = OMap.m_capacity;

        m_items = std::move(OMap.m_items);
    }

    void copy(const OMap<K, V> &OMap)
    {
        m_items = OMap.m_items;

        m_capacity = OMap.m_capacity;
        m_size = OMap.m_size;

        m_bucket = new Chain[m_capacity];

        for (size_t i = 0; i < m_capacity; i++)
            m_bucket[i] = OMap.m_bucket[i];
    }
};
