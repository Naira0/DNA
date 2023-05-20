#pragma once

#include <utility>

template<typename K, typename V>
struct Record
{
    K key;
    V value;

    Record(K &&key, V &&value) :
            key(std::move(key)),
            value(std::move(value))
    {}

    Record(const K &key, const V &value) :
            key(key),
            value(value)
    {}

    Record(Record &&item) noexcept :
            key(std::move(item.key)),
            value(std::move(item.value))
    {}

    Record(const Record &item) noexcept :
            key(item.key),
            value(item.value)
    {}

    Record& operator=(const Record &item)
    {
        key = item.key;
        value = item.value;
        return *this;
    }

    Record& operator=(Record &&item) noexcept
    {
        key = std::move(item.key);
        value = std::move(item.value);
        return *this;
    }
};