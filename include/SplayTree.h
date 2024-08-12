#pragma once

#include <cassert>
#include <vector>

class SplayTree
{
public:
    bool contains(int value) const;
    bool insert(int value);
    bool remove(int value);

    std::size_t size() const;
    bool empty() const;

    std::vector<int> values() const;

    ~SplayTree();

    SplayTree()
        : root(nullptr)
        , sz(0)
    {
    }

private:
    struct Node;

    Node * root;
    std::size_t sz;

    void splay(Node * x) const;
    static void rotate(Node * x);
    Node * merge(Node * left, Node * right) const;
    Node * find(int value) const;
};
