#include <cassert>
#include <iostream>
#include <string>
#include <tuple>

#include "AvlTree.hpp"

// NOLINTBEGIN(readability-magic-numbers)

int main(int /*unused*/, char** /*unused*/) {
    algos::AvlTree<std::string, int> ages;
    auto [pos, wasInserted] = ages.insert("Joe", 25);
    assert(wasInserted);
    std::tie(pos, wasInserted) = ages.insert("Ben", 99);
    assert(wasInserted);
    std::tie(pos, wasInserted) = ages.insert("Arthur", 42);
    assert(wasInserted);
    std::cout << "Contents of AVL tree:\n";
    for(const auto& [key, value] : ages) {
        std::cout << '(' << key << ", " << value << ")\n";
    }
    std::tie(pos, wasInserted) = ages.insert("Arthur", 142);
    assert(!wasInserted);
    assert(pos->first == "Arthur");
    assert(pos->second == 42);
    std::cout << "Contents of AVL tree:\n";
    for(const auto& [key, value] : ages) {
        std::cout << '(' << key << ", " << value << ")\n";
    }
    bool wasErased(ages.erase(pos));
    assert(wasErased);
    std::cout << "Contents of AVL tree:\n";
    for(const auto& [key, value] : ages) {
        std::cout << '(' << key << ", " << value << ")\n";
    }
    std::tie(pos, wasInserted) = ages.insert("Arthur", 142);
    assert(wasInserted);
    const auto found(ages.find("Arthur"));
    assert(found == pos);
    wasErased = ages.erase("Ben");
    assert(wasErased);
    std::cout << "Contents of AVL tree:\n";
    for(const auto& [key, value] : ages) {
        std::cout << '(' << key << ", " << value << ")\n";
    }
    wasErased = ages.erase("Benjamin");
    assert(!wasErased);
    wasErased = ages.erase("Joe");
    assert(wasErased);
    wasErased = ages.erase("Arthur");
    assert(wasErased);
    std::cout << "Contents of AVL tree:\n";
    for(const auto& [key, value] : ages) {
        std::cout << '(' << key << ", " << value << ")\n";
    }
    std::cout << "Size of AVL tree: " << ages.size() << '\n';
    assert(ages.empty());
    assert(ages.size() == 0);
    std::tie(pos, wasInserted) = ages.insert("Ben", 99);
    assert(wasInserted);
    std::tie(pos, wasInserted) = ages.insert("Arthur", 42);
    assert(wasInserted);
    assert(ages.size() == 2);
    assert(!ages.empty());
    ages.clear();
    assert(ages.size() == 0);
    assert(ages.empty());
    return 0;
}

// NOLINTEND(readability-magic-numbers)
