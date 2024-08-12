#include "SplayTree.h"

#include <cassert>
#include <functional>

struct SplayTree::Node
{

    Node * left;
    Node * right;
    Node * parent;
    int value;

    explicit Node(int value, Node * parent = nullptr)
        : left(nullptr)
        , right(nullptr)
        , parent(parent)
        , value(value)
    {
    }
    Node *& get_reference_son(bool is_left)
    {
        return (is_left ? left : right);
    }
    ~Node()
    {
        delete left;
        delete right;
    }
};

bool SplayTree::contains(int value) const
{
    if (root == nullptr) {
        return false;
    }
    Node * nearest_to_value_node = find(value);
    splay(nearest_to_value_node);
    assert(root == nearest_to_value_node);
    return root->value == value;
}

bool SplayTree::insert(int value)
{
    if (root == nullptr) {
        root = new Node(value);
        sz = 1;
        return true;
    }
    Node * nearest_to_value_node = find(value);
    if (nearest_to_value_node->value == value) {
        splay(nearest_to_value_node);
        return false;
    }
    ++sz;
    Node *& referenceSon = nearest_to_value_node->get_reference_son(nearest_to_value_node->value > value) = new Node(value, nearest_to_value_node);
    splay(referenceSon);
    return true;
}

bool SplayTree::remove(int value)
{
    if (!contains(value)) {
        return false;
    }
    assert(root->value == value);
    --sz;
    Node * temp = root;
    root = merge(root->left, root->right);
    if (root != nullptr) {
        root->parent = nullptr;
    }
    temp->left = temp->right = nullptr;
    delete temp;
    return true;
}

std::size_t SplayTree::size() const
{
    return sz;
}

bool SplayTree::empty() const
{
    return sz == 0;
}

std::vector<int> SplayTree::values() const
{
    std::vector<int> result;
    result.reserve(sz);

    static std::function<void(std::vector<int> &, Node *)> recursive_fill_result = [](std::vector<int> & a, Node * current) -> void {
        if (current == nullptr) {
            return;
        }
        recursive_fill_result(a, current->left);
        a.push_back(current->value);
        recursive_fill_result(a, current->right);
    };

    recursive_fill_result(result, root);
    return result;
}

SplayTree::~SplayTree()
{
    delete root;
}

void SplayTree::splay(SplayTree::Node * v) const
{
    assert(v != nullptr);
    while (v->parent != nullptr) {
        Node * p = v->parent;
        if (p->parent != nullptr) {
            Node * g = p->parent;
            if ((p->left == v) == (g->left == p)) {
                rotate(p);
            }
            else {
                rotate(v);
            }
        }
        rotate(v);
    }
    const_cast<SplayTree *>(this)->root = v;
}

SplayTree::Node * SplayTree::merge(SplayTree::Node * left, SplayTree::Node * right) const
{
    if (left == nullptr || right == nullptr) {
        return (left == nullptr ? right : left);
    }
    left->parent = nullptr;
    while (left->right) {
        left = left->right;
    }
    splay(left);
    assert(left->right == nullptr);
    left->right = right;
    right->parent = left;
    return left;
}

void SplayTree::rotate(SplayTree::Node * v)
{
    Node * p = v->parent;
    Node * g = p->parent;
    v->parent = g;
    p->parent = v;
    if (g) {
        g->get_reference_son(g->left == p) = v;
    }
    bool is_left = (p->left == v);
    p->get_reference_son(is_left) = v->get_reference_son(!is_left);
    Node *& v_son = v->get_reference_son(!is_left);
    if (v_son) {
        v_son->parent = p;
    }
    v_son = p;
}

SplayTree::Node * SplayTree::find(int value) const
{
    Node * current = root;
    while (true) {
        if (current->value == value) {
            return current;
        }
        auto newCurrent = (current->value < value ? current->right : current->left);
        if (newCurrent == nullptr) {
            return current;
        }
        current = newCurrent;
    }
    assert(false);
}
