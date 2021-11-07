#pragma once

#include <initializer_list>
#include <cstdint>
#include <iostream>
#include <functional>
template<class T>
class Forward_List
{
private:

    struct Node
    {
        T     data;
        Node  *next;
    };

public:

    class Iterator;

    Forward_List(std::initializer_list<T> init_list)
    {
        m_head = nullptr;
        m_tail = nullptr;

        for(const auto data : init_list)
        {
            push_back(data);
        }

        m_size = init_list.size();
    }

    Forward_List() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    ~Forward_List()
    {
        Node *temp = m_head;

        while(temp != nullptr)
        {
            Node *next = temp->next;
            delete temp;
            temp = next;
        }
    }

    void push_back(T data)
    {
        Node *node = new Node{data, nullptr};

        if(m_head == nullptr)
        {
            m_head = node;
            return;
        }

        // links the head to the tail in some edge cases
        if(m_head->next == nullptr)
            m_head->next = node;

        if(m_tail != nullptr)
            m_tail->next = node;

        m_tail = node;

        m_size++;
    }

    void push(T data)
    {
        if(m_head != nullptr && m_head->next == nullptr)
            m_tail = m_head;

        Node *node = new Node{data, m_head};
        m_head = node;
        m_size++;
    }

    void insert(size_t index, T data)
    {
        if(index > m_size)
            return;
        if(index == 0)
            return push(data);
        if(index == m_size)
            return push_back(data);

        Node *node = new Node{data, nullptr};
        Node *temp = m_head;

        for(size_t i = 0; i < index && temp != nullptr; i++)
            temp = temp->next;

        node->next = temp->next;
        temp->next = node;

        m_size++;
    }

    void erase(size_t index)
    {
        if(index > m_size)
            return;

        Node *temp = m_head;
        for(size_t i = 0; i < index-1; i++)
        {
            if(temp != nullptr)
                temp = temp->next;
        }

        Node* next = temp->next->next;
        delete temp->next;
        temp->next = next;
        m_size--;
    }

    void pop()
    {
        Node *next = m_head->next;
        delete m_head;
        m_head = next;
        m_size--;
    }

    void pop_back()
    {
        Node *temp = m_head;

        while(temp->next->next != nullptr)
        {
            temp = temp->next;
        }

        delete temp->next;
        temp->next = nullptr;
        m_size--;
    }

    T operator[](size_t index)
    {
        if(index >= m_size)
            throw std::out_of_range("Out of range");

        Node *temp = m_head;
        for(size_t i = 1; i < index; i++)
        {
            if(m_head->next != nullptr)
                temp = temp->next;
        }
        return temp->data;
    }

    size_t size() { return m_size; }

    Iterator begin() { return Iterator(m_head); }
    Iterator end() { return Iterator(m_tail); }

    T& front() { return m_head != nullptr ? reinterpret_cast<T&>(m_head->data) : throw std::out_of_range("Out of range"); }
    T& back()  { return m_tail != nullptr ? reinterpret_cast<T&>(m_tail->data) : throw std::out_of_range("Out of range"); }

    void foreach(std::function<void(T&)> fn)
    {
        Iterator ptr(m_head);
        Iterator end(m_tail);

        while(ptr != end)
        {
            fn(ptr.node_ptr->data);
            ptr++;
        }
    }

private:

    size_t  m_size;
    Node*   m_head;
    Node*   m_tail;
};

template<typename T>
class Forward_List<T>::Iterator
{
public:
    explicit Iterator(Node *node) : node_ptr(node) {}
    Iterator() : node_ptr(nullptr) {}

    friend bool operator!=(const Iterator& a, const Iterator& b)
    {
        if(b.node_ptr == nullptr)
            return false;
        return a.node_ptr != b.node_ptr->next;
    }

    friend bool operator==(const Iterator& a, const Iterator& b) { return a.node_ptr == b.node_ptr; }

    T& operator*() { return node_ptr->data; }

    Iterator operator++(int)
    {
        Iterator temp = *this;
        node_ptr = node_ptr->next;
        return temp;
    }

    Iterator& operator++()
    {
        node_ptr = node_ptr->next;
        return *this;
    }

public:
    Node *node_ptr;
};

