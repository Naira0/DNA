#include <bits/chrono.h>
#include <initializer_list>
#include <iostream>
#include <chrono>
#include <concepts>
#include <array>
#include <random>
#include <map>
#include <unordered_map>

#include "bst.hpp"
#include "rbt.hpp"

#include "util.hpp"
#include "map.hpp"
#include "graph.hpp"
#include "trie.hpp"

int main()
{
    Trie<int> trie;
    // {
    //     {"hello", 5},
    //     {"hello world", 10}
    // };

    trie.set("hello", 5);
    trie.set("hello world", 10);

    for (auto string : trie.keys_with_prefix("hello"))
    {
        std::cout << "key:" << string << '\n';
    }

    //trie.erase("hello");

    std::cout << "value: " << *trie.get("hello") << " " << *trie.get("hello world") << '\n';
}
