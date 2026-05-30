#include "Treap.hpp"

#include <random>

Treap::Node::Node(int value, int priority)
    : value(value), priority(priority), left(nullptr), right(nullptr) {}

int Treap::randomPriority() {
    static std::mt19937 engine(std::random_device{}());
    return std::uniform_int_distribution<int>(1, 1000000000)(engine);
}

void Treap::split(Node* root, int key, Node*& left, Node*& right) {
    if (root == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }

    if (root->value < key) {
        split(root->right, key, root->right, right);
        left = root;
    } else {
        split(root->left, key, left, root->left);
        right = root;
    }
}

Treap::Node* Treap::merge(Node* left, Node* right) {
    if (left == nullptr) {
        return right;
    }
    if (right == nullptr) {
        return left;
    }

    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        return left;
    }

    right->left = merge(left, right->left);
    return right;
}

void Treap::destroy(Node* node) {
    if (node == nullptr) {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}

bool Treap::containsNode(Node* node, int value) {
    if (node == nullptr) {
        return false;
    }
    if (value == node->value) {
        return true;
    }
    if (value < node->value) {
        return containsNode(node->left, value);
    }
    return containsNode(node->right, value);
}

void Treap::collectValues(Node* node, std::vector<int>& result) {
    if (node == nullptr) {
        return;
    }
    collectValues(node->left, result);
    result.push_back(node->value);
    collectValues(node->right, result);
}

Treap::Treap() : root_(nullptr), size_(0) {}

Treap::~Treap() {
    destroy(root_);
}

bool Treap::contains(int value) const {
    return containsNode(root_, value);
}

bool Treap::insert(int value) {
    if (contains(value)) {
        return false;
    }

    Node* left = nullptr;
    Node* right = nullptr;
    split(root_, value, left, right);

    Node* node = new Node(value, randomPriority());
    root_ = merge(merge(left, node), right);
    ++size_;
    return true;
}

bool Treap::remove(int value) {
    Node* left = nullptr;
    Node* right = nullptr;
    split(root_, value, left, right);

    Node* middle = nullptr;
    Node* rightRest = nullptr;
    split(right, value + 1, middle, rightRest);

    if (middle == nullptr) {
        root_ = merge(left, rightRest);
        return false;
    }

    Node* middleChildren = merge(middle->left, middle->right);
    delete middle;

    root_ = merge(left, merge(middleChildren, rightRest));
    --size_;
    return true;
}

std::size_t Treap::size() const {
    return size_;
}

bool Treap::empty() const {
    return size_ == 0;
}

std::vector<int> Treap::values() const {
    std::vector<int> result;
    result.reserve(size_);
    collectValues(root_, result);
    return result;
}
