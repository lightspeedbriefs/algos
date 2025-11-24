#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>

namespace algos {

/**
 * @brief Implementation of an AVL tree https://en.wikipedia.org/wiki/AVL_tree
 *
 * @tparam K The type of keys used to identifty elements in the tree.
 * @tparam V The type of value associated with each key.
 * @tparam Compare A function used for ordering keys.
 */
template<typename K, typename V, typename Compare=std::less<K>>
class AvlTree {
public:
    using value_type = std::pair<K, V>;
private:
    struct Node {
        value_type value;
        Node* parent{nullptr};
        int height{0};
        std::unique_ptr<Node> left, right;
    };
public:
    struct Iterator {
        explicit Iterator(const Node* node) : node(node) {}
        Iterator& operator++() {
            if(node->right) {
                node = node->right.get();
                return *this;
            }
            bool done(false);
            while(!done) {
                auto* const parent(node->parent);
                done = (parent == nullptr || node = parent->left.get());
                node = parent;
            }
            return *this;
        }
        Iterator operator++(int) {
            const Iterator old(*this);
            ++(*this);
            return old;
        }
        value_type& operator*() {
            return node->value;
        }
        bool operator==(const Iterator& rhs) const {
            return node == rhs.node;
        }
    private:
        Node* node{nullptr};
    };

    using iterator = Iterator;
    using const_iterator = const Node*;

    [[nodiscard]]
    std::pair<iterator, bool> insert(const K& key, const V& value) {
        return insertHelper(key, value, root);
    }

    [[nodiscard]]
    bool erase(const K& key) {
        return eraseHelper(key, root);
    }

    auto begin() const {
        if(!root) {
            return Iterator{nullptr};
        }
        auto* tmp(root.get());
        while(tmp->left) {
            tmp = tmp->left.get();
        }
        return Iterator{tmp};
    }
private:
    std::unique_ptr<Node> root;

    [[nodiscard]]
    static std::pair<iterator, bool> insertHelper(const K& key, const V& value, std::unique_ptr<Node>& root, Node* parent = nullptr) {
        if(root == nullptr) {
            root = std::make_unique<Node>(key, value, parent);
            return std::pair{root.get(), true};
        }
        bool left;
        if(Compare{}(key, root->value.first)) {
            left = true;
        } else if(Compare{}(root->value.first, key)) {
            left = false;
        } else {
            // Values are equal
            return {root.get(), false};
        }
        const auto ret(insertHelper(key, value, left ? root->left : root->right, root.get()));
        updateHeight(root.get());
        rotate(root);
        return ret;
    }

    static int getHeight(const Node* const node) {
        return node == nullptr ? -1 : node->height;
    }

    static void updateHeight(Node* const node) {
        node->height = std::max(getHeight(node->left.get()), getHeight(node->right.get())) + 1;
    }

    static int getBalanceFactor(const Node* const node) {
        return node == nullptr ? 0 : getHeight(node->left.get()) - getHeight(node->right.get());
    }

    /**
     * @brief Right rotate a left leaning tree rooted at @p oldRoot.
     *
     * @param oldRoot The root of a left leaning tree.
     */
    static void rotateRight(std::unique_ptr<Node>& oldRoot) {
        auto newRoot(std::move(oldRoot->left));
        auto* const parent(oldRoot->parent);
        oldRoot->left = std::move(newRoot->right);
        oldRoot->parent = newRoot.get();
        newRoot->right = std::move(oldRoot);
        newRoot->parent = parent;
        if(parent != nullptr) {
            parent->left = std::move(newRoot);
        }
        updateHeight(newRoot->right.get());
        updateHeight(newRoot.get());
    }

    /**
     * @brief Left rotate a right leaning tree rooted at @p oldRoot.
     *
     * @param oldRoot The root of a right leaning tree.
     */
    static void rotateLeft(std::unique_ptr<Node>& oldRoot) {
        auto newRoot(std::move(oldRoot->right));
        auto* const parent(oldRoot->parent);
        oldRoot->right = std::move(newRoot->left);
        oldRoot->parent = newRoot.get();
        newRoot->left = std::move(oldRoot);
        newRoot->parent = parent;
        if(parent != nullptr) {
            parent->right = std::move(newRoot);
        }
        updateHeight(newRoot->left.get());
        updateHeight(newRoot.get());
    }

    /**
     * @brief Determine if a tree rooted at @p node is left
     *        leaning or right leaning, and rotate as-needed.
     *
     * @param node The root of a tree that may require rotation.
     */
    static void rotate(std::unique_ptr<Node>& node) {
        const auto balanceFactor(getBalanceFactor(node.get()));
        if(balanceFactor > 1) {
            // Left leaning tree
            if(getBalanceFactor(node->left) < 0) {
                rotateLeft(node->left);
            }
            rotateRight(node);
        } else if(balanceFactor < -1) {
            // Right leaning tree
            if(getBalanceFactor(node->right) > 0) {
                rotateRight(node->right);
            }
            rotateLeft(node);
        }
    }

    static bool eraseHelper(const K& key, std::unique_ptr<Node>& root) {
        if(root == nullptr) {
            return false;
        }
        if(Compare{}(key, root->value.first)) {
            return eraseHelper(key, root->left);
        } else if(Compare{}(root->value.first, key)) {
            return eraseHelper(key, root->right);
        }
        // Values are equal
        auto* const parent(root->parent);
        if(root->right) {
            auto* tmp(root->right.get());
            while(tmp->left) {
                tmp = tmp->left.get();
            }
            (void)eraseHelper(tmp->value.first, root->right);
        }
        updateHeight(parent);
    }
};

}