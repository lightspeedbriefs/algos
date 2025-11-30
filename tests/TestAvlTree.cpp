#define BOOST_TEST_MODULE AvlTreeTests

#include <boost/test/unit_test.hpp>

#include "AvlTree.hpp"

#include <string>
#include <vector>

using namespace algos;

BOOST_AUTO_TEST_SUITE(AvlTreeSuite)

// NOLINTBEGIN(readability-magic-numbers)

BOOST_AUTO_TEST_CASE(EmptyTreeBehavior)
{
    AvlTree<int, std::string> tree;
    BOOST_TEST(tree.empty());
    BOOST_TEST(tree.size() == 0);
    BOOST_TEST(tree.begin() == tree.end());
    BOOST_TEST(tree.cbegin() == tree.cend());
}

BOOST_AUTO_TEST_CASE(SimpleInsertFindAndDuplicate)
{
    AvlTree<int, std::string> tree;
    auto insertRes = tree.insert(10, "ten");
    BOOST_TEST(insertRes.second);
    BOOST_TEST(tree.size() == 1);
    auto iter = tree.find(10);
    BOOST_TEST(iter != tree.end());
    BOOST_TEST(iter->second == "ten");

    // duplicate insert should not change size and should return false
    auto insertRes2 = tree.insert(10, "TEN");
    BOOST_TEST(!insertRes2.second);
    BOOST_TEST(tree.size() == 1);
    BOOST_TEST(tree.find(10)->second == "ten");
}

BOOST_AUTO_TEST_CASE(InOrderTraversalAndIterators)
{
    AvlTree<int, int> tree;
    std::vector<int> values = {20, 10, 30, 5, 15, 25, 35};
    for(auto valItem : values) { (void) tree.insert(valItem, valItem*10); }

    // collect in-order
    std::vector<int> outVec;
    for(const auto& [key, value] : tree) {
        outVec.push_back(key);
    }
    std::vector<int> expected = {5,10,15,20,25,30,35};
    BOOST_TEST(outVec == expected);

    // test post-increment
    auto iter = tree.begin();
    auto iterOld = iter++;
    BOOST_TEST(iterOld->first == 5);
    BOOST_TEST(iter->first == 10);

    // test pre-increment
    ++iter;
    BOOST_TEST(iter->first == 15);
}

BOOST_AUTO_TEST_CASE(RotationCases)
{
    // LL rotation: insert descending
    AvlTree<int,int> tree1;
    (void) tree1.insert(30,0);
    (void) tree1.insert(20,0);
    (void) tree1.insert(10,0);
    // in-order should be sorted
    std::vector<int> out1Vec;
    for(const auto& iter : tree1) { out1Vec.push_back(iter.first); }
    BOOST_TEST(out1Vec == std::vector<int>({10,20,30}));

    // RR rotation: insert ascending
    AvlTree<int,int> tree2;
    (void) tree2.insert(10,0);
    (void) tree2.insert(20,0);
    (void) tree2.insert(30,0);
    std::vector<int> out2Vec;
    for(const auto& iter : tree2) { out2Vec.push_back(iter.first); }
    BOOST_TEST(out2Vec == std::vector<int>({10,20,30}));

    // LR rotation
    AvlTree<int,int> tree3;
    (void) tree3.insert(30,0);
    (void) tree3.insert(10,0);
    (void) tree3.insert(20,0);
    std::vector<int> out3Vec;
    for(const auto& iter : tree3) { out3Vec.push_back(iter.first); }
    BOOST_TEST(out3Vec == std::vector<int>({10,20,30}));

    // RL rotation
    AvlTree<int,int> tree4;
    (void) tree4.insert(10,0);
    (void) tree4.insert(30,0);
    (void) tree4.insert(20,0);
    std::vector<int> out4Vec;
    for(const auto& iter : tree4) { out4Vec.push_back(iter.first); }
    BOOST_TEST(out4Vec == std::vector<int>({10,20,30}));
}

BOOST_AUTO_TEST_CASE(EraseLeafOneChildTwoChildren)
{
    AvlTree<int,int> tree;
    // build tree
    (void) tree.insert(20,0);
    (void) tree.insert(10,0);
    (void) tree.insert(30,0);
    (void) tree.insert(5,0);
    (void) tree.insert(15,0);
    (void) tree.insert(25,0);
    (void) tree.insert(35,0);
    BOOST_TEST(tree.size() == 7);

    // erase leaf
    BOOST_TEST(tree.erase(5));
    BOOST_TEST(tree.size() == 6);
    BOOST_TEST(tree.find(5) == tree.end());

    // erase node with one child: create a situation by inserting a child only on one side
    (void) tree.insert(27,0);
    // erase 25 which now may have one child (27)
    BOOST_TEST(tree.erase(25));
    BOOST_TEST(tree.find(25) == tree.end());

    // erase node with two children: erase 20 (root) and ensure remaining keys are present
    BOOST_TEST(tree.erase(20));
    BOOST_TEST(tree.find(20) == tree.end());
    std::vector<int> remainingVec;
    for(const auto& iter : tree) { remainingVec.push_back(iter.first); }
    // ensure size matches and contains known values
    BOOST_TEST(tree.size() == remainingVec.size());
    for(int key : {10,15,27,30,35}) {
        BOOST_TEST(tree.find(key) != tree.end());
    }

    // erase non-existent
    BOOST_TEST(!tree.erase(999));
}

BOOST_AUTO_TEST_CASE(EraseByIteratorAndClear)
{
    AvlTree<int,std::string> tree;
    (void) tree.insert(1, "a");
    (void) tree.insert(2, "b");
    (void) tree.insert(3, "c");
    auto iter = tree.find(2);
    BOOST_TEST(iter != tree.end());
    BOOST_TEST(tree.erase(iter));
    BOOST_TEST(tree.find(2) == tree.end());
    BOOST_TEST(tree.size() == 2);

    tree.clear();
    BOOST_TEST(tree.empty());
    BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(ConstFindAndConstIterator)
{
    AvlTree<int,int> tree;
    (void) tree.insert(5,50);
    (void) tree.insert(3,30);
    (void) tree.insert(7,70);
    const AvlTree<int,int>& constTree = tree;
    auto constIter = constTree.find(5);
    BOOST_TEST(constIter != constTree.cend());
    BOOST_TEST(constIter->second == 50);

    // const cbegin
    auto constBegin = constTree.cbegin();
    BOOST_TEST(constBegin != constTree.cend());
}

// NOLINTEND(readability-magic-numbers)

BOOST_AUTO_TEST_SUITE_END()
