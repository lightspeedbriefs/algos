#pragma once

#include <functional>
#include <memory>

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

    void insert(const K& key, const V& value);

    [[nodiscard]]
    bool erase(const K& key);

private:
    struct Node {
        K key;
        V value;
        Node* parent{nullptr};
        std::unique_ptr<Node> left, right;
    };

    std::unique_ptr<Node> root;

    void insertHelper(const K& key, const V& value, std::unique_ptr<Node>& root, Node* parent = nullptr) {
        if(root == nullptr) {
            root = std::make_unique<Node>(key, value, parent);
            return;
        }
        if(Compare{}(key, root->key)) {
            insertHelper(key, value, root->left, root.get());
        } else if(Compare{}(root->key, key)) {
            insertHelper(key, value, root->right, root.get());
        } else {
            // Values are equal
        }
    }
};

}