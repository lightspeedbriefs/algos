#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>

namespace algos {

/**
 * @brief Implementation of a red-black tree https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
 *
 * @tparam K The type of keys used to identifty elements in the tree.
 * @tparam V The type of value associated with each key.
 * @tparam Compare A function used for ordering keys.
 */
template<typename K, typename V, typename Compare=std::less<K>>
class RedBlackTree {
public:
    using value_type = std::pair<K, V>;
private:
    struct Node {
        value_type value;
        Node* parent{nullptr};
        bool red{false};
        std::unique_ptr<Node> left, right;
    };
public:
    template<typename N=Node>
    struct Iterator {
        explicit Iterator(N* node) : node(node) {}
        Iterator& operator++() {
            if(node->right) {
                node = node->right.get();
                while(node->left) {
                    node = node->left.get();
                }
                return *this;
            }
            bool done(false);
            while(!done) {
                auto* const parent(node->parent);
                done = (parent == nullptr || node == parent->left.get());
                node = parent;
            }
            return *this;
        }
        Iterator operator++(int) {
            const Iterator old(*this);
            ++(*this);
            return old;
        }
        [[nodiscard]]
        auto& operator*() const {
            return node->value;
        }
        auto* operator->() const {
            return &node->value;
        }
        // Allow comparing iterator with const_iterator
        template<typename U>
        [[nodiscard]]
        bool operator==(const Iterator<U>& rhs) const {
            return node == rhs.node;
        }
    private:
        friend class RedBlackTree<K, V, Compare>;
        N* node{nullptr};
    };

    using iterator = Iterator<>;
    using const_iterator = Iterator<const Node>;

    [[nodiscard]]
    std::pair<iterator, bool> insert(const K& key, const V& value) {
        return insertHelper(key, value, root);
    }

    [[nodiscard]]
    bool erase(const K& key) {
        return eraseHelper(key, root);
    }

    [[nodiscard]]
    bool erase(iterator i) {
        return eraseHelper(i.node->value.first, root);
    }

    [[nodiscard]]
    auto cbegin() const {
        return beginHelper<const_iterator>();
    }

    [[nodiscard]]
    auto begin() {
        return beginHelper<iterator>();
    }

    [[nodiscard]]
    auto end() {
        return iterator{nullptr};
    }

    [[nodiscard]]
    auto cend() const {
        return const_iterator{nullptr};
    }

    [[nodiscard]]
    iterator find(const K& key) {
        return findHelper<iterator>(key, root.get());
    }

    [[nodiscard]]
    const_iterator find(const K& key) const {
        return findHelper<const_iterator>(key, root.get());
    }

    [[nodiscard]]
    bool empty() const {
        return root == nullptr;
    }

    [[nodiscard]]
    auto size() const {
        return numElems;
    }

    void clear() {
        root.reset();
        numElems = 0;
    }

private:
    std::unique_ptr<Node> root;
    size_t numElems{0};

    template<typename Iter>
    [[nodiscard]]
    auto beginHelper() const {
        if(!root) {
            return Iter{nullptr};
        }
        auto* tmp(root.get());
        while(tmp->left) {
            tmp = tmp->left.get();
        }
        return Iter{tmp};
    }

    [[nodiscard]]
    std::pair<iterator, bool> insertHelper(const K& key, const V& value, std::unique_ptr<Node>& root, Node* parent = nullptr) {
        if(root == nullptr) {
            ++numElems;
            root = std::make_unique<Node>(value_type{key, value}, parent);
            // The root node must be black
            if(parent != nullptr) {
                root->red = true;
            }
            return {iterator{root.get()}, true};
        }
        bool left;
        if(Compare{}(key, root->value.first)) {
            left = true;
        } else if(Compare{}(root->value.first, key)) {
            left = false;
        } else {
            // Values are equal
            return {iterator{root.get()}, false};
        }
        const auto ret(insertHelper(key, value, left ? root->left : root->right, root.get()));
        rotate(root);
        return ret;
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
        oldRoot = std::move(newRoot);
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
        oldRoot = std::move(newRoot);
    }

    /**
     * @brief Determine if a tree rooted at @p node is left
     *        leaning or right leaning, and rotate as-needed.
     *
     * @param node The root of a tree that may require rotation.
     */
    static void rotate(std::unique_ptr<Node>& node) {
        if(!node->red) {
            return;
        }
        if(node->left && node->left->red) {
            if(!node->right) {
                rotateRight(node);
                node->red = false;
            }
        }
        if(node->right && node->right->red) {
            if(!node->left) {
                rotateLeft(node);
                node->red = false;
            }
        }
    }

    [[nodiscard]]
    bool eraseHelper(const K& key, std::unique_ptr<Node>& root) {
        if(root == nullptr) {
            return false;
        }
        if(Compare{}(key, root->value.first)) {
            const auto wasErased(eraseHelper(key, root->left));
            rotate(root);
            return wasErased;
        }
        if(Compare{}(root->value.first, key)) {
            const auto wasErased(eraseHelper(key, root->right));
            rotate(root);
            return wasErased;
        }
        // Values are equal, we're removing this node.
        if(!root->left || !root->right) {
            auto promoted(root->left != nullptr ? std::move(root->left) : std::move(root->right));
            if(!promoted) {
                // This node has no children, just delete it and then we're done.
                root.reset();
            } else {
                promoted->parent = root->parent;
                root = std::move(promoted);
            }
            --numElems;
        } else {
            // 2 valid children; both left and right
            auto* tmp(root->right.get());
            while(tmp->left) {
                tmp = tmp->left.get();
            }
            root->value = tmp->value;
            (void)eraseHelper(tmp->value.first, root->right);
            rotate(root);
        }
        return true;
    }

    template<typename Iter>
    [[nodiscard]]
    static Iter findHelper(const K& key, Node* root) {
        if(root == nullptr) {
            return Iter{nullptr};
        }
        if(Compare{}(key, root->value.first)) {
            return findHelper<Iter>(key, root->left.get());
        }
        if(Compare{}(root->value.first, key)) {
            return findHelper<Iter>(key, root->right.get());
        }
        // Values are equal, we found what we're looking for
        return Iter{root};
    }
};

}
