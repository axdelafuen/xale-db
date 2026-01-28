#ifndef B_PLUS_TREE_TESTS_H
#define B_PLUS_TREE_TESTS_H

#include "TestsHelper.h"
#include "DataStructure/BPlusTree.h"

#define DECLARE_B_PLUS_TREE_TEST(name) DECLARE_TEST(DATA_STRUCT, b_plus_tree_##name)

namespace Xale::Tests
{
	DECLARE_B_PLUS_TREE_TEST(insert_value)
	{
		// TODO: Implement B+ Tree delete tests
		return false;
	}

	DECLARE_B_PLUS_TREE_TEST(delete_value)
	{
		// TODO: Implement B+ Tree delete tests
		return false;
	}

	DECLARE_B_PLUS_TREE_TEST(search_value)
	{
		// TODO: Implement B+ Tree search tests
		return false;
	}
}

#endif // B_PLUS_TREE_TESTS_H