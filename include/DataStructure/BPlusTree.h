#ifndef DATA_STRUCTURE_PLUS_BTREE_H 
#define DATA_STRUCTURE_PLUS_BTREE_H

#include "DataStructure/Node.h"

#include <algorithm>

namespace Xale::DataStructure
{
    /**
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
			void merge(Node<TKey, TValue>* node, int index);
			void borrowFromPrev(Node<TKey, TValue>* node, int index);
			void borrowFromNext(Node<TKey, TValue>* node, int index);
	};

	/**
	 * @brief B+Tree constructor
	 * @param maxKeys Maximum number of keys per node
	 */
	template <typename TKey, typename TValue>
	BPlusTree<TKey, TValue>::BPlusTree(int maxKeys):
		_root(nullptr),
		_keysMax(maxKeys)
	{}

	/**
	 * @brief Insert a key-value pair into the B+ Tree
	 * @param key The key to insert
	 * @param value The value associated with the key
	 * @return True if insertion is successful, false otherwise
	 */
	template <typename TKey, typename TValue>
	bool BPlusTree<TKey, TValue>::insert(TKey key, TValue* value)
	{
        if (_root == nullptr)
        {
            _root = new Node<TKey, TValue>(true);
            _root->keys.push_back(key);
            _root->values.push_back(*value);
            return true;
        }
        else {
            if (_root->keys.size() == 2 * _keysMax - 1)
            {
                Node<TKey, TValue>* newRoot = new Node<TKey, TValue>(false);
                newRoot->children.push_back(_root);
                splitChild(newRoot, 0, _root);
                _root = newRoot;
            }
            insertNonFull(_root, key, value);
            return true;
        }
	}

	/**
	 * @brief Remove a key from the B+ Tree
	 * @param key The key to remove
	 * @return True if removal is successful, false otherwise
	 */
	template <typename TKey, typename TValue>
	bool BPlusTree<TKey, TValue>::remove(TKey key)
	{
        if (_root == nullptr) {
            return false;
        }
        remove(_root, key);
        if (_root->keys.empty() && !_root->isLeaf) {
            Node<TKey, TValue>* tmp = _root;
            _root = _root->children[0];
            delete tmp;
            return true;
        }
        return false;
	}

	/**
	 * @brief Search for a key in the B+ Tree
	 * @param key The key to search for
	 * @return Pointer to the value associated with the key, or nullptr if not found
	 */
	template <typename TKey, typename TValue>
	TValue* BPlusTree<TKey, TValue>::search(TKey key)
	{

        if (!_root)
            return nullptr;

        Node<TKey, TValue>* current = _root;

        while (!current->isLeaf)
        {
            size_t i = 0;
            while (i < current->keys.size() && key >= current->keys[i])
                ++i;
            current = current->children[i];
        }

        for (size_t i = 0; i < current->keys.size(); ++i)
        {
            if (current->keys[i] == key)
                return &current->values[i];
        }

        return nullptr;
    }

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::splitChild(
		Node<TKey, TValue>* parent, 
		int index,
        Node<TKey, TValue>* child)
    {
        Node<TKey, TValue>* newChild = new Node<TKey, TValue>(child->isLeaf);

        parent->children.insert(
                parent->children.begin() + index + 1, 
                newChild);

        parent->keys.insert(
                parent->keys.begin() + index,
                child->keys[_keysMax - 1]);

        newChild->keys.assign(
                child->keys.begin() + _keysMax,
                child->keys.end());

        child->keys.resize(_keysMax - 1);

        if (!child->isLeaf)
        {
            newChild->children.assign(
                    child->children.begin() + _keysMax,
                    child->children.end());
            child->children.resize(_keysMax);
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
        if (node->isLeaf)
        {
            auto pos = std::upper_bound(
                node->keys.begin(),
                node->keys.end(),
                key);
            size_t index = std::distance(node->keys.begin(), pos);
            node->keys.insert(pos, key);
            node->values.insert(node->values.begin() + index, *value);        
        }
        else
        {
            int i = node->keys.size() - 1;
            
            while (i >= 0 && key < node->keys[i])
                i--;

            i++;

            if (node->children[i]->keys.size() == 2 * _keysMax - 1)
            {
                splitChild(node, i, node->children[i]);

                if (key > node->keys[i])
                    i++;
            }

            insertNonFull(node->children[i], key, value); 
        }
	}

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::remove(
		Node<TKey, TValue>* node, 
		TKey key)
	{
        if (node->isLeaf)
        {
            auto it = std::find(
                node->keys.begin(),
                node->keys.end(),
                key);

            if (it != node->keys.end())
            {
                size_t index = std::distance(node->keys.begin(), it);
                node->keys.erase(it);
                node->values.erase(node->values.begin() + index);
            }               
        }
        else
        {
            int index = std::distance(
                node->keys.begin(),
                std::lower_bound(
                    node->keys.begin(),
                    node->keys.end(),
                    key));

            if (index < node->keys.size() && node->keys[index] == key)
            {
                if (node->children[index]->keys.size() >= _keysMax)
                {
                    Node<TKey, TValue>* predecessorNode = node->children[index];
                    while (!predecessorNode->isLeaf)
                        predecessorNode = predecessorNode->children.back();

                    TKey predecessorKey = predecessorNode->keys.back();
                    TValue predecessorVal = predecessorNode->values.back();
                    node->keys[index] = predecessorKey;
                    node->values[index] = predecessorVal;

                    remove(node->children[index], predecessorKey);
                }
                else if (node->children[index + 1]->keys.size() >= _keysMax)
                {
                    Node<TKey, TValue>* successorNode = node->children[index + 1];
                    while(!successorNode->isLeaf)
                        successorNode = successorNode->children.front();

                    TKey successorKey = successorNode->keys.front();
                    TValue successorVal = successorNode->values.front();
                    node->keys[index] = successorKey;
                    node->values[index] = successorVal;

                    remove(node->children[index + 1], successorKey);
                }
                else
                {
                    merge(node, index);
                    remove(node->children[index], key);
                }
            }
            else 
            {
                if (node->children[index]->keys.size() < _keysMax)
                {
                    if (index > 0 && 
                            node->children[index - 1]->keys.size() >= _keysMax)
                        borrowFromPrev(node, index);
                    else if (index < node->children.size() - 1 && 
                            node->children[index + 1]->keys.size() >= _keysMax)
                        borrowFromNext(node, index);
                    else
                    {
                        if (index < node->children.size() -1)
                            merge(node, index);
                        else
                            merge(node, index - 1);
                    }
                }
                remove(node->children[index], key);
            }
        }
	}

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::merge(
		Node<TKey, TValue>* node, 
		int index)
	{
        Node<TKey, TValue>* child = node->children[index];
        Node<TKey, TValue>* sibling = node->children[index + 1];

        child->keys.push_back(node->keys[index]);
        child->values.push_back(node->values[index]);

        child->keys.insert(
            child->keys.end(),
            sibling->keys.begin(),
            sibling->keys.end());
        child->values.insert(
            child->values.end(),
            sibling->values.begin(),
            sibling->values.end());

        if (!child->isLeaf)
        {
            child->children.insert(child->children.end(),
                sibling->children.begin(),
                sibling->children.end());
        }

        node->keys.erase(node->keys.begin() + index);
        node->values.erase(node->values.begin() + index);
        node->children.erase(node->children.begin() + index + 1);

        delete sibling;
	}

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::borrowFromPrev(
		Node<TKey, TValue>* node,
		int index)
	{
        Node<TKey, TValue>* child = node->children[index];
        Node<TKey, TValue>* sibling = node->children[index - 1];

        child->keys.insert(child->keys.begin(), node->keys[index - 1]);
        child->values.insert(child->values.begin(), node->values[index - 1]);

        node->keys[index - 1] = sibling->keys.back();
        node->values[index - 1] = sibling->values.back();

        sibling->keys.pop_back();
        sibling->values.pop_back();

        if (!child->isLeaf) {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }
    }

	template <typename TKey, typename TValue>
	void BPlusTree<TKey, TValue>::borrowFromNext(
		Node<TKey, TValue>* node,
		int index)
	{
        Node<TKey, TValue>* child = node->children[index];
        Node<TKey, TValue>* sibling = node->children[index + 1];

        child->keys.push_back(node->keys[index]);
        child->values.push_back(node->values[index]);

        node->keys[index] = sibling->keys.front();
        node->values[index] = sibling->values.front();

        sibling->keys.erase(sibling->keys.begin());
        sibling->values.erase(sibling->values.begin());

        if (!child->isLeaf) {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }
    }
}

#endif // DATA_STRUCTURE_PLUS_BTREE_H
