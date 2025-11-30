#include <functional>
#define BOOST_TEST_MODULE HeapTests

#include <boost/test/unit_test.hpp>

#include "Heap.hpp"

#include <string>
#include <vector>
#include <deque>

using namespace algos;

BOOST_AUTO_TEST_SUITE(HeapSuite)

// NOLINTBEGIN(readability-magic-numbers)

BOOST_AUTO_TEST_CASE(EmptyHeapBehavior)
{
    Heap<int> minHeap;
    BOOST_TEST(minHeap.empty());
    BOOST_TEST(minHeap.size() == 0);
}

BOOST_AUTO_TEST_CASE(SingleElementMaxHeap)
{
    Heap<int, std::vector<int>, std::less<>> maxHeap;
    maxHeap.push(42);
    BOOST_TEST(!maxHeap.empty());
    BOOST_TEST(maxHeap.size() == 1);
    BOOST_TEST(maxHeap.top() == 42);
}

BOOST_AUTO_TEST_CASE(SingleElementMinHeap)
{
    Heap<int, std::vector<int>, std::less<>> minHeap;
    minHeap.push(42);
    BOOST_TEST(!minHeap.empty());
    BOOST_TEST(minHeap.size() == 1);
    BOOST_TEST(minHeap.top() == 42);
}

BOOST_AUTO_TEST_CASE(PushPopMaxHeap)
{
    Heap<int, std::vector<int>, std::less<>> maxHeap;
    maxHeap.push(10);
    maxHeap.push(20);
    maxHeap.push(5);
    maxHeap.push(30);

    BOOST_TEST(maxHeap.size() == 4);
    BOOST_TEST(maxHeap.top() == 30);

    maxHeap.pop();
    BOOST_TEST(maxHeap.top() == 20);

    maxHeap.pop();
    BOOST_TEST(maxHeap.top() == 10);

    maxHeap.pop();
    BOOST_TEST(maxHeap.top() == 5);

    maxHeap.pop();
    BOOST_TEST(maxHeap.empty());
}

BOOST_AUTO_TEST_CASE(PushPopMinHeap)
{
    Heap<int, std::vector<int>, std::greater<>> minHeap;
    minHeap.push(10);
    minHeap.push(20);
    minHeap.push(5);
    minHeap.push(30);

    BOOST_TEST(minHeap.size() == 4);
    BOOST_TEST(minHeap.top() == 5);

    minHeap.pop();
    BOOST_TEST(minHeap.top() == 10);

    minHeap.pop();
    BOOST_TEST(minHeap.top() == 20);

    minHeap.pop();
    BOOST_TEST(minHeap.top() == 30);

    minHeap.pop();
    BOOST_TEST(minHeap.empty());
}

BOOST_AUTO_TEST_CASE(ConstructorFromRangeMinHeap)
{
    std::vector<int> initValues = {15, 10, 20, 5, 30};
    Heap<int, std::vector<int>, std::greater<>> minHeap(initValues.begin(), initValues.end());

    BOOST_TEST(minHeap.size() == 5);
    // min heap should have minimum at top
    BOOST_TEST(minHeap.top() == 5);

    std::vector<int> orderedVals;
    while (!minHeap.empty()) {
        orderedVals.push_back(minHeap.top());
        minHeap.pop();
    }
    std::vector<int> expectedOrder = {5, 10, 15, 20, 30};
    BOOST_TEST(orderedVals == expectedOrder);
}

BOOST_AUTO_TEST_CASE(ConstructorFromRangeMaxHeap)
{
    std::vector<int> initValues = {15, 10, 20, 5, 30};
    Heap<int, std::vector<int>, std::less<>> maxHeap(
        initValues.begin(),
        initValues.end(),
        std::less<>{});

    BOOST_TEST(maxHeap.size() == 5);
    // max heap should have maximum at top
    BOOST_TEST(maxHeap.top() == 30);

    std::vector<int> orderedVals;
    while (!maxHeap.empty()) {
        orderedVals.push_back(maxHeap.top());
        maxHeap.pop();
    }
    std::vector<int> expectedOrder = {30, 20, 15, 10, 5};
    BOOST_TEST(orderedVals == expectedOrder);
}

BOOST_AUTO_TEST_CASE(ConstructorFromContainerAndComparatorMinHeap)
{
    std::vector<int> initValues = {12, 8, 25, 3, 18};
    Heap<int, std::vector<int>, std::greater<>> minHeap(std::greater<>{}, initValues);

    BOOST_TEST(minHeap.size() == 5);
    BOOST_TEST(minHeap.top() == 3);
}

BOOST_AUTO_TEST_CASE(ConstructorFromRValueContainerAndComparatorMaxHeap)
{
    std::vector<int> initValues = {12, 8, 25, 3, 18};
    Heap<int, std::vector<int>, std::less<>> maxHeap(
        std::less<>{},
        std::move(initValues));

    BOOST_TEST(maxHeap.size() == 5);
    BOOST_TEST(maxHeap.top() == 25);
}

BOOST_AUTO_TEST_CASE(ConstructorExplicitComparator)
{
    Heap<int, std::vector<int>, std::less<>> maxHeap(std::less<>{});
    BOOST_TEST(maxHeap.empty());

    maxHeap.push(5);
    maxHeap.push(10);
    BOOST_TEST(maxHeap.top() == 10);
}

BOOST_AUTO_TEST_CASE(StringTypeHeap)
{
    Heap<std::string, std::vector<std::string>, std::greater<>> strHeap;
    strHeap.push("charlie");
    strHeap.push("alice");
    strHeap.push("bob");

    BOOST_TEST(strHeap.size() == 3);
    BOOST_TEST(strHeap.top() == "alice");
    strHeap.pop();
    BOOST_TEST(strHeap.top() == "bob");
    strHeap.pop();
    BOOST_TEST(strHeap.top() == "charlie");
}

BOOST_AUTO_TEST_CASE(LargeSequencePushPopMinHeap)
{
    Heap<int, std::vector<int>, std::greater<>> minHeap;
    std::vector<int> inputVals = {50, 30, 70, 20, 80, 10, 90, 40, 60};

    for (int valItem : inputVals) {
        minHeap.push(valItem);
    }

    BOOST_TEST(minHeap.size() == 9);

    std::vector<int> outputVals;
    while (!minHeap.empty()) {
        outputVals.push_back(minHeap.top());
        minHeap.pop();
    }

    // Should be in ascending order
    std::vector<int> expectedOrder = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    BOOST_TEST(outputVals == expectedOrder);
}

BOOST_AUTO_TEST_CASE(LargeSequencePushPopMaxHeap)
{
    Heap<int, std::vector<int>, std::less<>> maxHeap;
    std::vector<int> inputVals = {50, 30, 70, 20, 80, 10, 90, 40, 60};

    for (int valItem : inputVals) {
        maxHeap.push(valItem);
    }

    BOOST_TEST(maxHeap.size() == 9);

    std::vector<int> outputVals;
    while (!maxHeap.empty()) {
        outputVals.push_back(maxHeap.top());
        maxHeap.pop();
    }

    // Should be in descending order
    std::vector<int> expectedOrder = {90, 80, 70, 60, 50, 40, 30, 20, 10};
    BOOST_TEST(outputVals == expectedOrder);
}

BOOST_AUTO_TEST_CASE(AlternateContainerDequeMinHeap)
{
    std::deque<int> initDeque = {15, 8, 25, 3, 18};
    Heap<int, std::deque<int>, std::greater<>> dequeHeap(initDeque.begin(), initDeque.end());

    BOOST_TEST(dequeHeap.size() == 5);
    BOOST_TEST(dequeHeap.top() == 3);

    dequeHeap.push(1);
    BOOST_TEST(dequeHeap.size() == 6);
    BOOST_TEST(dequeHeap.top() == 1);
}

BOOST_AUTO_TEST_CASE(DuplicateElementsMinHeap)
{
    Heap<int, std::vector<int>, std::greater<>> minHeap;
    minHeap.push(5);
    minHeap.push(5);
    minHeap.push(5);

    BOOST_TEST(minHeap.size() == 3);
    BOOST_TEST(minHeap.top() == 5);

    minHeap.pop();
    BOOST_TEST(minHeap.top() == 5);

    minHeap.pop();
    BOOST_TEST(minHeap.top() == 5);

    minHeap.pop();
    BOOST_TEST(minHeap.empty());
}

BOOST_AUTO_TEST_CASE(MixedIntegralTypesHeap)
{
    Heap<unsigned int, std::vector<unsigned int>, std::greater<>> unsignedHeap;
    unsignedHeap.push(100);
    unsignedHeap.push(50);
    unsignedHeap.push(200);

    BOOST_TEST(unsignedHeap.top() == 50);
}

BOOST_AUTO_TEST_CASE(TopConstMethodMinHeap)
{
    Heap<int, std::vector<int>, std::greater<>> minHeap;
    minHeap.push(42);

    const auto& constRef = minHeap;
    BOOST_TEST(constRef.top() == 42);
}

BOOST_AUTO_TEST_CASE(InterleavedPushPopMaxHeap)
{
    Heap<int, std::vector<int>, std::less<>> maxHeap;

    maxHeap.push(10);
    maxHeap.push(20);
    BOOST_TEST(maxHeap.top() == 20);

    maxHeap.pop();
    BOOST_TEST(maxHeap.top() == 10);

    maxHeap.push(15);
    BOOST_TEST(maxHeap.top() == 15);

    maxHeap.push(25);
    BOOST_TEST(maxHeap.top() == 25);

    maxHeap.pop();
    BOOST_TEST(maxHeap.top() == 15);

    maxHeap.pop();
    BOOST_TEST(maxHeap.top() == 10);

    maxHeap.pop();
    BOOST_TEST(maxHeap.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AlgorithmSuite)

BOOST_AUTO_TEST_CASE(MakeHeapEmptyRange)
{
    std::vector<int> vecEmpty;
    algos::make_heap(vecEmpty.begin(), vecEmpty.end(), std::less<>{});
    BOOST_TEST(vecEmpty.empty());
}

BOOST_AUTO_TEST_CASE(MakeHeapSingleElement)
{
    std::vector<int> vecSingle = {42};
    algos::make_heap(vecSingle.begin(), vecSingle.end(), std::less<>{});
    BOOST_TEST(vecSingle.size() == 1);
    BOOST_TEST(vecSingle[0] == 42);
}

BOOST_AUTO_TEST_CASE(MakeHeapTwoElements)
{
    std::vector<int> vecTwo = {5, 10};
    algos::make_heap(vecTwo.begin(), vecTwo.end(), std::less<>{});
    BOOST_TEST(vecTwo.size() == 2);
    // With less<> (max-heap), 10 should be at root
    BOOST_TEST(vecTwo[0] == 10);
}

BOOST_AUTO_TEST_CASE(MakeHeapUnorderedToMinHeap)
{
    std::vector<int> vecMin = {3, 1, 4, 1, 5, 9, 2, 6};
    algos::make_heap(vecMin.begin(), vecMin.end(), std::less<>{});

    // Verify heap property: parent >= children for min-heap (greater<>)
    for (size_t parIdx = 0; parIdx < vecMin.size(); ++parIdx) {
        const size_t leftIdx = (parIdx * 2) + 1;
        const size_t rightIdx = (parIdx * 2) + 2;

        if (leftIdx < vecMin.size()) {
            const auto leftOkay(std::greater<>{}(vecMin[parIdx], vecMin[leftIdx]) || vecMin[parIdx] == vecMin[leftIdx]);
            BOOST_TEST(leftOkay);
        }
        if (rightIdx < vecMin.size()) {
            const auto rightOkay(std::greater<>{}(vecMin[parIdx], vecMin[rightIdx]) || vecMin[parIdx] == vecMin[rightIdx]);
            BOOST_TEST(rightOkay);
        }
    }
}

BOOST_AUTO_TEST_CASE(MakeHeapUnorderedToMaxHeap)
{
    std::vector<int> vecMax = {3, 1, 4, 1, 5, 9, 2, 6};
    algos::make_heap(vecMax.begin(), vecMax.end(), std::greater<>{});

    // Verify heap property: parent <= children for max-heap (less<>)
    for (size_t parIdx = 0; parIdx < vecMax.size(); ++parIdx) {
        const size_t leftIdx = (parIdx * 2) + 1;
        const size_t rightIdx = (parIdx * 2) + 2;

        if (leftIdx < vecMax.size()) {
            const auto leftOkay(std::less<>{}(vecMax[parIdx], vecMax[leftIdx]) || vecMax[parIdx] == vecMax[leftIdx]);
            BOOST_TEST(leftOkay);
        }
        if (rightIdx < vecMax.size()) {
            const auto rightOkay(std::less<>{}(vecMax[parIdx], vecMax[rightIdx]) || vecMax[parIdx] == vecMax[rightIdx]);
            BOOST_TEST(rightOkay);
        }
    }
}

BOOST_AUTO_TEST_CASE(PushHeapSinglePush)
{
    std::vector<int> vecPush = {10};
    vecPush.push_back(5);
    algos::push_heap(vecPush.begin(), vecPush.end(), std::less<>{});

    // After push into max-heap, should have 10 at root
    BOOST_TEST(vecPush[0] == 10);
}

BOOST_AUTO_TEST_CASE(PushHeapMultiplePushesMaxHeap)
{
    std::vector<int> vecPush = {10};
    algos::make_heap(vecPush.begin(), vecPush.end(), std::less<>{});

    vecPush.push_back(5);
    algos::push_heap(vecPush.begin(), vecPush.end(), std::less<>{});
    BOOST_TEST(vecPush[0] == 10);

    vecPush.push_back(20);
    algos::push_heap(vecPush.begin(), vecPush.end(), std::less<>{});
    BOOST_TEST(vecPush[0] == 20);

    vecPush.push_back(15);
    algos::push_heap(vecPush.begin(), vecPush.end(), std::less<>{});
    BOOST_TEST(vecPush[0] == 20);

    vecPush.push_back(8);
    algos::push_heap(vecPush.begin(), vecPush.end(), std::less<>{});
    BOOST_TEST(vecPush[0] == 20);
}

BOOST_AUTO_TEST_CASE(PushHeapMultiplePushesMinHeap)
{
    std::vector<int> vecPush = {10};
    algos::make_heap(vecPush.begin(), vecPush.end(), std::greater<>{});

    vecPush.push_back(15);
    algos::push_heap(vecPush.begin(), vecPush.end(), std::greater<>{});
    BOOST_TEST(vecPush[0] == 10);

    vecPush.push_back(5);
    algos::push_heap(vecPush.begin(), vecPush.end(), std::greater<>{});
    BOOST_TEST(vecPush[0] == 5);

    vecPush.push_back(20);
    algos::push_heap(vecPush.begin(), vecPush.end(), std::greater<>{});
    BOOST_TEST(vecPush[0] == 5);

    vecPush.push_back(3);
    algos::push_heap(vecPush.begin(), vecPush.end(), std::greater<>{});
    BOOST_TEST(vecPush[0] == 3);
}

BOOST_AUTO_TEST_CASE(PopHeapEmptyAfterLastPop)
{
    std::vector<int> vecPop = {10, 5};
    algos::make_heap(vecPop.begin(), vecPop.end(), std::less<>{});

    algos::pop_heap(vecPop.begin(), vecPop.end(), std::less<>{});
    vecPop.pop_back();

    algos::pop_heap(vecPop.begin(), vecPop.end(), std::less<>{});
    vecPop.pop_back();

    BOOST_TEST(vecPop.empty());
}

BOOST_AUTO_TEST_CASE(PopHeapSingleElement)
{
    std::vector<int> vecPop = {42};
    algos::make_heap(vecPop.begin(), vecPop.end(), std::less<>{});

    algos::pop_heap(vecPop.begin(), vecPop.end(), std::less<>{});
    vecPop.pop_back();

    BOOST_TEST(vecPop.empty());
}

BOOST_AUTO_TEST_CASE(PopHeapSequenceMaxHeap)
{
    std::vector<int> vecPop = {20, 10, 15, 5};
    algos::make_heap(vecPop.begin(), vecPop.end(), std::less<>{});

    // First pop should remove the largest
    const int first = vecPop[0];
    algos::pop_heap(vecPop.begin(), vecPop.end(), std::less<>{});
    const int firstVal = vecPop.back();
    BOOST_TEST(firstVal == first);
    vecPop.pop_back();

    // After first pop, remaining should still be a valid max-heap
    const int secondTop = vecPop[0];
    algos::pop_heap(vecPop.begin(), vecPop.end(), std::less<>{});
    const int secondVal = vecPop.back();
    BOOST_TEST(secondVal == secondTop);
    vecPop.pop_back();
}

BOOST_AUTO_TEST_CASE(PopHeapSequenceMinHeap)
{
    std::vector<int> vecPop = {5, 10, 8, 15};
    algos::make_heap(vecPop.begin(), vecPop.end(), std::greater<>{});

    // First pop should remove the smallest
    const int first = vecPop[0];
    algos::pop_heap(vecPop.begin(), vecPop.end(), std::greater<>{});
    const int firstVal = vecPop.back();
    BOOST_TEST(firstVal == first);
    vecPop.pop_back();

    // After first pop, remaining should still be a valid min-heap
    const int secondTop = vecPop[0];
    algos::pop_heap(vecPop.begin(), vecPop.end(), std::greater<>{});
    const int secondVal = vecPop.back();
    BOOST_TEST(secondVal == secondTop);
    vecPop.pop_back();
}

BOOST_AUTO_TEST_CASE(IntegrationMakeHeapThenPushAndPopMaxHeap)
{
    std::vector<int> vecInteg = {10, 5, 15};
    algos::make_heap(vecInteg.begin(), vecInteg.end(), std::less<>{});

    BOOST_TEST(vecInteg[0] == 15);

    vecInteg.push_back(25);
    algos::push_heap(vecInteg.begin(), vecInteg.end(), std::less<>{});
    BOOST_TEST(vecInteg[0] == 25);

    algos::pop_heap(vecInteg.begin(), vecInteg.end(), std::less<>{});
    const int popVal = vecInteg.back();
    BOOST_TEST(popVal == 25);
    vecInteg.pop_back();

    BOOST_TEST(vecInteg[0] == 15);
}

BOOST_AUTO_TEST_CASE(IntegrationMakeHeapThenPushAndPopMinHeap)
{
    std::vector<int> vecInteg = {10, 20, 5};
    algos::make_heap(vecInteg.begin(), vecInteg.end(), std::greater<>{});

    BOOST_TEST(vecInteg[0] == 5);

    vecInteg.push_back(2);
    algos::push_heap(vecInteg.begin(), vecInteg.end(), std::greater<>{});
    BOOST_TEST(vecInteg[0] == 2);

    algos::pop_heap(vecInteg.begin(), vecInteg.end(), std::greater<>{});
    const int popVal = vecInteg.back();
    BOOST_TEST(popVal == 2);
    vecInteg.pop_back();

    BOOST_TEST(vecInteg[0] == 5);
}

BOOST_AUTO_TEST_CASE(HeapPropertyAfterMultiplePushesMaxHeap)
{
    std::vector<int> vecProps = {50};
    algos::make_heap(vecProps.begin(), vecProps.end(), std::less<>{});

    std::vector<int> pushVals = {30, 70, 20, 80, 10, 90};
    for (int val : pushVals) {
        vecProps.push_back(val);
        algos::push_heap(vecProps.begin(), vecProps.end(), std::less<>{});
    }

    // Verify heap property: parent >= children for max-heap
    for (size_t parIdx = 0; parIdx < vecProps.size(); ++parIdx) {
        const size_t leftIdx = (parIdx * 2) + 1;
        const size_t rightIdx = (parIdx * 2) + 2;

        if (leftIdx < vecProps.size()) {
            BOOST_TEST(vecProps[parIdx] >= vecProps[leftIdx]);
        }
        if (rightIdx < vecProps.size()) {
            BOOST_TEST(vecProps[parIdx] >= vecProps[rightIdx]);
        }
    }
}

BOOST_AUTO_TEST_CASE(HeapPropertyAfterMultiplePushesMinHeap)
{
    std::vector<int> vecProps = {50};
    algos::make_heap(vecProps.begin(), vecProps.end(), std::greater<>{});

    std::vector<int> pushVals = {30, 70, 20, 80, 10, 90};
    for (int val : pushVals) {
        vecProps.push_back(val);
        algos::push_heap(vecProps.begin(), vecProps.end(), std::greater<>{});
    }

    // Verify heap property: parent <= children for min-heap
    for (size_t parIdx = 0; parIdx < vecProps.size(); ++parIdx) {
        const size_t leftIdx = (parIdx * 2) + 1;
        const size_t rightIdx = (parIdx * 2) + 2;

        if (leftIdx < vecProps.size()) {
            BOOST_TEST(vecProps[parIdx] <= vecProps[leftIdx]);
        }
        if (rightIdx < vecProps.size()) {
            BOOST_TEST(vecProps[parIdx] <= vecProps[rightIdx]);
        }
    }
}

// NOLINTEND(readability-magic-numbers)

BOOST_AUTO_TEST_SUITE_END()
