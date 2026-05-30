#pragma once

#include <cstddef>
#include <vector>

class Treap {
public:
    Treap();
    ~Treap();

    Treap(const Treap&) = delete;
    Treap& operator=(const Treap&) = delete;

    bool contains(int value) const;
    bool insert(int value);
    bool remove(int value);

    std::size_t size() const;
    bool empty() const;

    std::vector<int> values() const;

private:
    struct Node {
        int value;
        int priority;
        Node* left;
        Node* right;

        Node(int value, int priority);
    };

    Node* root_;
    std::size_t size_;

    static void split(Node* root, int key, Node*& left, Node*& right);
    static Node* merge(Node* left, Node* right);
    static void destroy(Node* node);
    static bool containsNode(Node* node, int value);
    static void collectValues(Node* node, std::vector<int>& result);
    static int randomPriority();
};
