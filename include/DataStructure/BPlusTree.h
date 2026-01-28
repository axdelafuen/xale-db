#ifndef DATA_STRUCTURE_PLUS_BTREE_H 
#define DATA_STRUCTURE_PLUS_BTREE_H

#include "DataStructure/Node.h"

#include <memory>

namespace Xale::DataStructure
{
    /*
	 * @brief BPlus Tree implementation
     */
	template <typename TKey, typename TValue>
	class BPlusTree
	{
		public:
			BPlusTree(int maxKeys);
			bool insert(TKey key, TValue* value);
			bool remove(TKey key);
			TValue* search(TKey key);
		private:
			Node<TKey, TValue>* _root;
			int _keysMax;

			void splitChild(Node<TKey, TValue>* parent, int index, Node<TKey, TValue>* child);
			void insertNonFull(Node<TKey, TValue>* node, TKey key, TValue* value);
			void remove(Node<TKey, TValue>* node, TKey key);
			void merge(Node<TKey, TValue>* parent, int index);
			void borrowFromPrev(Node<TKey, TValue>* parent, int index);
			void borrowFromNext(Node<TKey, TValue>* parent, int index);
	};

	/*
	 * @brief B+Tree constructor
	 * @param maxKeys Maximum number of keys per node
	 */
	template <typename TKey, typename TValue>
	BPlusTree<TKey, TValue>::BPlusTree(int maxKeys):
		_root(nullptr),
		_keysMax(maxKeys)
	{}

	/*
	 * @brief Insert a key-value pair into the B+ Tree
	 * @param key The key to insert
	 * @param value The value associated with the key
	 * @return True if insertion is successful, false otherwise
	 */
	template <typename TKey, typename TValue>
	bool BPlusTree<TKey, TValue>::insert(TKey key, TValue* value)
	{
		// TODO: Implementation of insert method
	}

	/*
	 * @brief Remove a key from the B+ Tree
	 * @param key The key to remove
	 * @return True if removal is successful, false otherwise
	 */
	template <typename TKey, typename TValue>
	bool BPlusTree<TKey, TValue>::remove(TKey key)
	{
		// TODO: Implementation of remove method
	}

	/*
	 * @brief Search for a key in the B+ Tree
	 * @param key The key to search for
	 * @return Pointer to the value associated with the key, or nullptr if not found
	 */
	template <typename TKey, typename TValue>
	TValue* BPlusTree<TKey, TValue>::search(TKey key)
	{
		// TODO: Implementation of search method
	}

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::splitChild(
		Node<TKey, TValue>* parent, 
		int index,
		Node<TKey, TValue>* child)
	{
		Node<TKey, TValue>* newChild = std::make_unique<Node<TKey, TValue>>(child->isLeaf);
		
		parent->children.insert(
			parent->children.begin() + index + 1, 
			newChild);
		
		parent->keys.insert(
			parent->keys.begin() + index,
			child->keys[t - 1]);

		newChild->keys.assign(
			child->keys.begin() + t,
			child->keys.end());

		child->keys.resize(t - 1);

		if (!child->isLeaf)
		{
			newChild->children.assign(
				child->children.begin() + t,
				child->children.end());
			child->children.resize(t);
		}

		if (child->isLeaf)
		{
			newChild->next = child->next;
			child->next = newChild;
		}
	}

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::insertNonFull(
		Node<TKey, TValue>* node, 
		TKey key, 
		TValue* value)
	{
		// TODO: Implementation of insertNonFull method
	}

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::remove(
		Node<TKey, TValue>* node, 
		TKey key)
	{
		// TODO: Implementation of remove method
	}

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::merge(
		Node<TKey, TValue>* parent, 
		int index)
	{
		// TODO: Implementation of merge method
	}

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::borrowFromPrev(
		Node<TKey, TValue>* parent,
		int index)
	{
		// TODO: Implementation of borrowFromPrev method
	}

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::borrowFromNext(
		Node<TKey, TValue>* parent,
		int index)
	{
		// TODO: Implementation of borrowFromNext method
	}
}

#endif // DATA_STRUCTURE_PLUS_BTREE_H
