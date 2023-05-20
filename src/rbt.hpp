#pragma once

#include <cstdint>
#include <cstddef>
#include <stdexcept>

// red-black tree implementation

enum class RBColor : uint8_t
{
    Red, Black
};

template<class K, class V>
struct RBTNode
{
    RBColor color;

    K key;
    V value;

    RBTNode<K, V>
            *left{},
            *right{},
            *parent{};

    bool is_red() const
    {
        return color == RBColor::Red;
    }
};

template<class K, class V>
class RBT
{
public:
    using Node = RBTNode<K, V>;

    RBT() = default;

    Node* find_node(const K &key) const
    {
        Node *node = m_root;

        while (node && key != node->key)
        {
            if (key < node->key)
                node = node->left;
            else
                node = node->right;
        }

        return node;
    }

    bool contains(const K &k) const
    {
        return find_node(k);
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

    V& find(const K &key) const
    {
        Node *node = find_node(key);

        if (!node)
            throw std::out_of_range("node does not exist in tree");

        return node->value;
    }

    void insert(const K &key, const V &value)
    {
        set_node(new Node
        {
            RBColor::Red, 
            key, 
            value
        });
    }

    void erase(const K &key)
    {
        Node *n = find_node(key);
        Node *y = n;

        RBColor color = y->color;

        Node *x = nullptr;

        if (!n->left)
        {
            x = n->right;
            transplant(n, n->right);
        }
        else if (!n->right)
        {
            x = n->left;
            transplant(n, n->left);
        }
        else
        {
            y = min_node(n->right);

            color = y->color;

            x = y->right;

            if (x && y->parent == n)
            {
                x->parent = y;
            } 
            else
            {
                transplant(y, y->right);

                y->right = n->right;
                
                if (y->right)
                    y->right->parent = y;
            }

            transplant(n, y);

            y->left = n->left;
            y->left->parent = y;
            y->color = n->color;
        }

        if (color == RBColor::Black)
        {
            erase_fixup(x);
        }
    }

private:
    Node  *m_root = nullptr;
    size_t m_node_count;

    void transplant(Node *u, Node *v)
    {
        if (!u->parent)
        {
            m_root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;;
        }

        if (v && u)
            v->parent = u->parent;
    }

    void set_node(Node *n)
    {
        m_node_count++;

        Node *p = nullptr;
        Node *x = m_root;

        while (x)
        {
            p = x;

            if (n->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        n->parent = p;

        if (!p)
            m_root = n;
        else if (n->key < p->key)
            p->left = n;
        else
            p->right = n;

        balance(n);
    }

    void rotate_left(Node *n)
    {
        Node *y = n->right;

        n->right = y->left;

        if (y->left)
        {
            y->left->parent = n;
        } 

        y->parent = n->parent;

        if (!n->parent)
        {
            m_root = y;
        }
        else if (n == n->parent->left)
        {
            n->parent->left = y;
        }
        else 
        {
            n->parent->right = y;
        }

        y->left   = n;
        n->parent = y;
    }

    void rotate_right(Node *n)
    {
        Node *y = n->left;

        n->right = y->left;

        if (y->right)
        {
            y->right->parent = n;
        } 

        y->parent = n->parent;

        if (!n->parent)
        {
            m_root = y;
        }
        else if (n == n->parent->left)
        {
            n->parent->left = y;
        }
        else 
        {
            n->parent->right = y;
        }

        y->left   = n;
        n->parent = y;
    }

    void balance(Node *n)
    {
        Node *p = n->parent;

        while (p && p->is_red())
        {
            if (p->parent->left == p)
            {
                Node *y = p->parent->right;

                if (y->is_red())
                {
                    p->parent->color = RBColor::Red;

                    p->color         = RBColor::Black;
                    y->color         = RBColor::Black;

                    n = p->parent;
                }
                else if (n == p->right)
                {
                    n = p;
                    rotate_left(n);
                }
                else
                {
                    p->color         = RBColor::Black;
                    p->parent->color = RBColor::Red;

                    rotate_right(p->parent);
                }
            }
            else
            {
                if (p->parent->left && p->parent->left->is_red())
                {
                    p->parent->color = RBColor::Red;

                    p->parent->left->color  = RBColor::Black;
                    p->parent->right->color = RBColor::Black;

                    n = p->parent;
                }
                else if (n == p->left)
                {
                    n = p;
                    rotate_right(n);
                }
                else
                {
                    p->color         = RBColor::Black;
                    p->parent->color = RBColor::Red;

                    rotate_left(p->parent);
                }
            }
            
        }

        m_root->color = RBColor::Black;
    }

    void erase_fixup(Node *n)
    {
        while (n != m_root && n->color == RBColor::Black)
        {
            if (n == n->parent->left)
            {
                Node *w = n->parent->right;

                if (w->is_red())
                {
                    w->color = RBColor::Black;
                    n->parent->color = RBColor::Red;

                    rotate_left(n->parent);

                    w = n->parent->right;
                }
                if (!w->is_red() && !w->right->is_red())
                {
                    w->color = RBColor::Red;
                    n = n->parent;
                }
                else 
                {
                    if (!w->right->is_red())
                    {
                        w->left->color = RBColor::Black;
                        w->color = RBColor::Red;

                        rotate_right(w);

                        w = n->parent->right;
                    }

                    w->color = n->parent->color;
                    n->parent->color = RBColor::Black;
                    w->right->color = RBColor::Black;

                    rotate_left(n->parent);

                    n = m_root;
                }
            }
            else
            {
                Node *w = n->parent->left;

                if (w->is_red())
                {
                    w->color = RBColor::Black;
                    n->parent->color = RBColor::Red;

                    rotate_right(n->parent);

                    w = n->parent->left;
                }
                if (!w->is_red() && !w->left->is_red())
                {
                    w->color = RBColor::Red;
                    n = n->parent;
                }
                else 
                {
                    if (!w->left->is_red())
                    {
                        w->left->color = RBColor::Black;
                        w->color = RBColor::Red;

                        rotate_left(w);

                        w = n->parent->left;
                    }

                    w->color = n->parent->color;
                    n->parent->color = RBColor::Black;
                    w->left->color = RBColor::Black;

                    rotate_right(n->parent);

                    n = m_root;
                }
            }
        }

        n->color = RBColor::Black;
    }
};
