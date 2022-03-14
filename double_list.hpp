#pragma once

#include <initializer_list>
#include <stdexcept>

// i got kinda lazy on this one so it lacks a few methods and features from forward_list

template<typename T>
class List
{
private:

    struct Node
    {
        T     data;
        Node* last;
        Node* next;
    };

public:

    List(std::initializer_list<T> init_list)
    {
        m_head = nullptr;
        m_tail = nullptr;

        for(const auto data : init_list)
        {
            push_back(data);
        }

        m_size = init_list.size();
    }

    List() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    ~List()
    {
        Node *temp = m_head;

        while(temp != nullptr)
        {
            Node *next = temp->next;
            delete temp;
            temp = next;
        }
    }

    void push(T data)
    {
        if(m_head != nullptr && m_tail == nullptr)
            m_tail = m_head;

        Node *node = new Node{data, nullptr, m_head};

        if(m_head != nullptr)
            m_head->last = node;

        m_head = node;
        m_size++;
    }

    void push_back(T data)
    {
        Node *node = new Node{data, m_tail, nullptr};

        if(m_head == nullptr)
            m_head = node;
        if(m_tail == nullptr)
            m_tail = node;

        m_tail->next = node;
        m_tail       = node;
        m_size++;
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
        Node *tail = m_tail;
        m_tail = m_tail->last;
        delete tail;
        m_tail->next = nullptr;
        m_size--;
    }

    T operator[](size_t index)
    {
        if(index >= m_size)
            throw std::out_of_range("Out of range");

        Node *temp = m_head;
        for(size_t i = 0; i < index && temp->next != nullptr; i++)
                temp = temp->next;
        return temp->data;
    }

    T front() { return m_head; }
    T back()  { return m_tail; }

    size_t size() { return m_size; }

private:

    Node *m_head;
    Node *m_tail;
    size_t m_size;
};