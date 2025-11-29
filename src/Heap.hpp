#pragma once

#include "Algorithm.hpp"

#include <functional>
#include <iterator>
#include <vector>

namespace algos {

/**
 * @brief Implementation of a heap analagous to std::priority_queue
 *
 * Recommended reading:
 * https://en.wikipedia.org/wiki/Heap_(data_structure)
 * https://en.cppreference.com/w/cpp/container/priority_queue.html
 *
 * @tparam T
 * @tparam Container
 * @tparam std::less<typename Container::value_type>
 */
template<typename T, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class Heap {
public:
    [[nodiscard]]
    auto& top() const {
        return storage.front();
    }

    [[nodiscard]]
    bool empty() const {
        return std::empty(storage);
    }

    [[nodiscard]]
    auto size() const {
        return std::size(storage);
    }

    using value_type = Container::value_type;

    void push(const value_type& value) {
        storage.push_back(value);
        algos::push_heap(std::begin(storage), std::end(storage), Compare{});
    }

    void pop() {
        algos::pop_heap(std::begin(storage), std::end(storage), Compare{});
        storage.pop_back();
    }

private:
    Container storage;
};

}