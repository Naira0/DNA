#pragma once

#include <unordered_map>
#include <vector>

template<class T>
class Graph
{
    using Adjc = std::unordered_map<T, std::vector<T>>;
public:

    Graph() = default;

    void add_edge(const T &v, const T &w)
    {
        push_edge(v, w);
        push_edge(w, v);

        m_edges++;
    }

    size_t vertices() const 
    {
        return m_vertices;
    }

    size_t edges() const 
    {
        return m_edges;
    }

    const std::vector<T>& adjc(const T &v)
    {
        return m_adjc[v];
    }

private:
    size_t m_vertices = 0;
    size_t m_edges = 0;

    Adjc m_adjc;

    void push_edge(const T &v, const T &w)
    {
        auto iter = m_adjc.find(v);

        if (iter == m_adjc.end())
        {
            m_vertices++;
        }

        m_adjc[v].push_back(w);
    }
};