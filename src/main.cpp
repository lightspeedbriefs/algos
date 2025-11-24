#include <cassert>
#include <iostream>
#include <string>
#include <tuple>

#include "AvlTree.hpp"

int main(int, char**) {
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
    wasErased = ages.erase("Ben");
    assert(wasErased);
    std::cout << "Contents of AVL tree:\n";
    for(const auto& [key, value] : ages) {
        std::cout << '(' << key << ", " << value << ")\n";
    }
}
