#ifndef NODE_TESTS_H
#define NODE_TESTS_H

#include "TestsHelper.h"
#include "DataStructure/Node.h"

#define DECLARE_NODE_TEST(name) DECLARE_TEST(DATA_STRUCT, node_##name)

namespace Xale::Tests
{
	DECLARE_NODE_TEST(creation_as_leaf_success)
	{
		Xale::DataStructure::Node<int, std::string> node(true);
		return node.isLeaf() == true && node.getKeys().empty() && node.getValues().empty();
	}

	DECLARE_NODE_TEST(creation_as_inner_success)
	{
		Xale::DataStructure::Node<int, std::string> node(false);
		return node.isLeaf() == false && node.getKeys().empty() && node.getChildren().empty();
	}

	DECLARE_NODE_TEST(find_key_index_empty_node_success)
	{
		Xale::DataStructure::Node<int, std::string> node(true);
		const std::size_t index = node.findKeyIndex(5);
		return index == 0;
	}

	DECLARE_NODE_TEST(find_key_index_single_key_smaller_success)
	{
		Xale::DataStructure::Node<int, std::string> node(true);
		node.insertLeaf(10, "ten");
		const std::size_t index = node.findKeyIndex(5);
		return index == 0;
	}

	DECLARE_NODE_TEST(find_key_index_single_key_larger_success)
	{
		Xale::DataStructure::Node<int, std::string> node(true);
		node.insertLeaf(10, "ten");
		const std::size_t index = node.findKeyIndex(15);
		return index == 1;
	}

	DECLARE_NODE_TEST(find_key_index_multiple_keys_success)
	{
		Xale::DataStructure::Node<int, std::string> node(true);
		node.insertLeaf(10, "ten");
		node.insertLeaf(20, "twenty");
		node.insertLeaf(30, "thirty");
		
		const std::size_t index1 = node.findKeyIndex(5);
		const std::size_t index2 = node.findKeyIndex(15);
		const std::size_t index3 = node.findKeyIndex(25);
		const std::size_t index4 = node.findKeyIndex(35);
		
		return index1 == 0 && index2 == 1 && index3 == 2 && index4 == 3;
	}

	DECLARE_NODE_TEST(insert_leaf_single_key_success)
	{
		Xale::DataStructure::Node<int, std::string> node(true);
		const bool result = node.insertLeaf(10, "ten");
		const auto& keys = node.getKeys();
		const auto& values = node.getValues();
		return result == true && keys.size() == 1 && keys[0] == 10 && values[0] == "ten";
	}

	DECLARE_NODE_TEST(insert_leaf_multiple_keys_sorted_success)
	{
		Xale::DataStructure::Node<int, std::string> node(true);
		node.insertLeaf(20, "twenty");
		node.insertLeaf(10, "ten");
		node.insertLeaf(30, "thirty");
		node.insertLeaf(15, "fifteen");
		
		const auto& keys = node.getKeys();
		const auto& values = node.getValues();
		
		return keys.size() == 4 && 
		       keys[0] == 10 && values[0] == "ten" &&
		       keys[1] == 15 && values[1] == "fifteen" &&
		       keys[2] == 20 && values[2] == "twenty" &&
		       keys[3] == 30 && values[3] == "thirty";
	}

	DECLARE_NODE_TEST(insert_leaf_on_inner_node_fail)
	{
		Xale::DataStructure::Node<int, std::string> node(false);
		const bool result = node.insertLeaf(10, "ten");
		return result == false;
	}

	DECLARE_NODE_TEST(insert_inner_single_key_success)
	{
		Xale::DataStructure::Node<int, std::string> innerNode(false);
		Xale::DataStructure::Node<int, std::string> rightChild(true);
		
		const bool result = innerNode.insertInner(10, &rightChild);
		const auto& keys = innerNode.getKeys();
		const auto& children = innerNode.getChildren();
		
		return result == true && keys.size() == 1 && keys[0] == 10 && 
		       children.size() == 2 && children[1] == &rightChild;
	}

	DECLARE_NODE_TEST(insert_inner_multiple_keys_success)
	{
		Xale::DataStructure::Node<int, std::string> innerNode(false);
		Xale::DataStructure::Node<int, std::string> child1(true);
		Xale::DataStructure::Node<int, std::string> child2(true);
		Xale::DataStructure::Node<int, std::string> child3(true);
		
		innerNode.insertInner(10, &child1);
		innerNode.insertInner(20, &child2);
		innerNode.insertInner(15, &child3);
		
		const auto& keys = innerNode.getKeys();
		const auto& children = innerNode.getChildren();
		
		return keys.size() == 3 && children.size() == 4 &&
		       keys[0] == 10 && keys[1] == 15 && keys[2] == 20;
	}

	DECLARE_NODE_TEST(insert_inner_on_leaf_node_fail)
	{
		Xale::DataStructure::Node<int, std::string> leafNode(true);
		Xale::DataStructure::Node<int, std::string> child(true);
		
		const bool result = leafNode.insertInner(10, &child);
		return result == false;
	}

	DECLARE_NODE_TEST(insert_inner_sets_parent_pointer_success)
	{
		Xale::DataStructure::Node<int, std::string> innerNode(false);
		Xale::DataStructure::Node<int, std::string> childNode(true);
		
		innerNode.insertInner(10, &childNode);
		
		return childNode.getParent() == &innerNode;
	}

	DECLARE_NODE_TEST(leaf_with_string_keys_success)
	{
		Xale::DataStructure::Node<std::string, int> node(true);
		node.insertLeaf("apple", 1);
		node.insertLeaf("banana", 2);
		node.insertLeaf("cherry", 3);
		
		const auto& keys = node.getKeys();
		const auto& values = node.getValues();
		
		return keys.size() == 3 && 
		       keys[0] == "apple" && values[0] == 1 &&
		       keys[1] == "banana" && values[1] == 2 &&
		       keys[2] == "cherry" && values[2] == 3;
	}

	DECLARE_NODE_TEST(leaf_with_double_keys_success)
	{
		Xale::DataStructure::Node<double, std::string> node(true);
		node.insertLeaf(3.14, "pi");
		node.insertLeaf(2.71, "e");
		node.insertLeaf(1.41, "sqrt2");
		
		const auto& keys = node.getKeys();
		const auto& values = node.getValues();
		
		return keys.size() == 3 && 
		       keys[0] == 1.41 && values[0] == "sqrt2" &&
		       keys[1] == 2.71 && values[1] == "e" &&
		       keys[2] == 3.14 && values[2] == "pi";
	}

}

#endif // NODE_TESTS_H