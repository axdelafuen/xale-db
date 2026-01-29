#ifndef B_PLUS_TREE_TESTS_H
#define B_PLUS_TREE_TESTS_H

#include "TestsHelper.h"
#include "DataStructure/BPlusTree.h"

#define DECLARE_B_PLUS_TREE_TEST(name) DECLARE_TEST(DATA_STRUCT, b_plus_tree_##name)

namespace Xale::Tests
{
    DECLARE_B_PLUS_TREE_TEST(search_empty_tree)
    {
        Xale::DataStructure::BPlusTree<int, int> tree(2);
        return tree.search(42) == nullptr;
    }

    DECLARE_B_PLUS_TREE_TEST(insert_single)
    {
        Xale::DataStructure::BPlusTree<int, int> tree(2);
        int value = 10;

        if (!tree.insert(1, &value))
            return false;

        int* result = tree.search(1);
        return result != nullptr && *result == 10;
    }

    DECLARE_B_PLUS_TREE_TEST(insert_multiple_no_split)
    {
        Xale::DataStructure::BPlusTree<int, int> tree(3);
        int v1 = 10, v2 = 20;

        tree.insert(1, &v1);
        tree.insert(2, &v2);

        return tree.search(1) != nullptr
            && tree.search(2) != nullptr
            && *tree.search(1) == 10
            && *tree.search(2) == 20;
    }

    DECLARE_B_PLUS_TREE_TEST(root_split)
    {
        Xale::DataStructure::BPlusTree<int, int> tree(2);
        int v1 = 10, v2 = 20, v3 = 30;

        tree.insert(1, &v1);
        tree.insert(2, &v2);
        tree.insert(3, &v3);

        return tree.search(1) != nullptr
            && tree.search(2) != nullptr
            && tree.search(3) != nullptr
            && *tree.search(3) == 30;
    }

    DECLARE_B_PLUS_TREE_TEST(search_missing_key)
    {
        Xale::DataStructure::BPlusTree<int, int> tree(2);
        int value = 10;

        tree.insert(1, &value);
        return tree.search(99) == nullptr;
    }

    DECLARE_B_PLUS_TREE_TEST(remove_leaf_key)
    {
        Xale::DataStructure::BPlusTree<int, int> tree(2);
        int v1 = 10, v2 = 20;

        tree.insert(1, &v1);
        tree.insert(2, &v2);

        tree.remove(1);

        return tree.search(1) == nullptr
            && tree.search(2) != nullptr
            && *tree.search(2) == 20;
    }

    DECLARE_B_PLUS_TREE_TEST(remove_merge)
    {
        Xale::DataStructure::BPlusTree<int, int> tree(2);
        int v1 = 10, v2 = 20, v3 = 30;

        tree.insert(1, &v1);
        tree.insert(2, &v2);
        tree.insert(3, &v3);

        tree.remove(1);
        tree.remove(2);

        return tree.search(1) == nullptr
            && tree.search(2) == nullptr
            && tree.search(3) != nullptr 
            && *tree.search(3) == 30;
    }

    DECLARE_B_PLUS_TREE_TEST(insert_reverse_order)
    {
        Xale::DataStructure::BPlusTree<int, int> tree(2);
        int v1 = 10, v2 = 20, v3 = 30;

        tree.insert(3, &v3);
        tree.insert(2, &v2);
        tree.insert(1, &v1);

        return tree.search(1) != nullptr
            && tree.search(2) != nullptr
            && tree.search(3) != nullptr
            && *tree.search(1) == 10;
    }

    DECLARE_B_PLUS_TREE_TEST(remove_all_keys)
    {
        Xale::DataStructure::BPlusTree<int, int> tree(2);
        int v1 = 10, v2 = 20;

        tree.insert(1, &v1);
        tree.insert(2, &v2);

        tree.remove(1);
        tree.remove(2);

        return tree.search(1) == nullptr
            && tree.search(2) == nullptr;
    }
}

#endif // B_PLUS_TREE_TESTS_H
