#pragma once

#include <iterator>
#include <utility>

namespace algos {

/**
 * @brief Implementation of std::push_heap().
 *
 * @tparam RandomIt
 * @tparam Compare
 * @param first
 * @param last
 * @param cmp
 */
template<typename RandomIt, class Compare = std::less<>>
void push_heap(RandomIt first, RandomIt last, Compare cmp = Compare{}) {
    while(first != last) {
        const auto distance(std::distance(first, --last));
        if(distance == 0) {
            break;
        }
        const auto parentPos((distance - 1) / 2);
        const auto parent = std::next(first, parentPos);
        if(cmp(*last, *parent)) {
            break;
        }
        std::swap(*parent, *last);
        last = std::next(parent, 1);
    }
}

/**
 * @brief Implementation of std::pop_heap().
 *
 * @tparam RandomIt
 * @tparam Compare
 * @param first
 * @param last
 * @param cmp
 */
template<typename RandomIt, class Compare = std::less<>>
void pop_heap(RandomIt first, RandomIt last, Compare cmp = Compare{}) {
    if(first != last) {
        std::swap(*first, *(--last));
    }
    const size_t numElems(std::distance(first, last));
    for(size_t parentIdx(0), childIdx(1); childIdx < numElems; parentIdx = std::exchange(childIdx, (childIdx * 2) + 1)) {
        auto child(std::next(first, childIdx));
        if(const auto rightChild(std::next(child, 1)); rightChild != last) {
            // Right child is valid.  We will compare the parent value
            // to whichever child value is "greater"
            if(cmp(*child, *rightChild)) {
                ++childIdx;
                child = rightChild;
            }
        }
        const auto parent(std::next(first, parentIdx));
        if(cmp(*child, *parent)) {
            break;
        }
        std::swap(*parent, *child);
    }
}

/**
 * @brief Implementation of std::make_heap().
 *
 * @tparam RandomIt
 * @tparam Compare
 * @param first
 * @param last
 * @param cmp
 */
template<typename RandomIt, class Compare = std::less<>>
void make_heap(RandomIt first, RandomIt last, Compare cmp = Compare{}) {
    const size_t numElems(std::distance(first, last));
    if(numElems <= 1) {
        return;
    }
    for(int64_t i=((numElems - 2) / 2); i>=0; --i) {
        for(size_t parentIdx(i), childIdx((i * 2) + 1); childIdx < numElems; parentIdx = std::exchange(childIdx, (childIdx * 2) + 1)) {
            const auto parent(std::next(first, parentIdx));
            auto child(std::next(first, childIdx));
            if(const auto rightChild(std::next(child, 1)); rightChild != last) {
                // Right child is valid.  We will compare the parent value
                // to whichever child value is "greater"
                if(cmp(*child, *rightChild)) {
                    ++childIdx;
                    child = rightChild;
                }
            }
            if(cmp(*child, *parent)) {
                break;
            }
            std::swap(*parent, *child);
        }
    }
}

}
