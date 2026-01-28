#ifndef DATA_STRUCTURE_NODE_H
#define DATA_STRUCTURE_NODE_H

#include "Core/ExceptionHandler.h"

#include <vector>
#include <exception>
#include <string>

namespace Xale::DataStructure
{
    /*
	 * @brief Node class for B+Tree implementation
     */
    template <typename TKey, typename TValue>
    class Node
    {
        public:
            explicit Node(bool isLeaf);
            const std::vector<TKey>& getKeys() const;
            const std::vector<TValue>& getValues() const;
            const std::vector<Node<TKey, TValue>*>& getChildren() const;
            Node<TKey, TValue>* getParent() const;
            bool isLeaf() const;
            std::size_t findKeyIndex(const TKey& key) const;
            bool insertLeaf(const TKey& key, const TValue& value);
            bool insertInner(const TKey& key, Node<TKey, TValue>* rightChild);
        private:
            std::vector<TKey> _keys;
            std::vector<Node<TKey, TValue>*> _children; // inner nodes
            std::vector<TValue> _values;                // leaf nodes
            Node<TKey, TValue>* _next;
            Node<TKey, TValue>* _parent;
            bool _isLeaf;
    };


    /*
	 * @brief Constructor for Node class
	 * @param isLeaf True if Node is a Leaf, False otherwise
     */
    template <typename TKey, typename TValue>
	Node<TKey, TValue>::Node(bool isLeaf = true) : 
		_next(nullptr),
		_parent(nullptr),
        _isLeaf(isLeaf)
    {
        _keys = {};
        _children = {};
        _values = {};
    } 
  
    /*
	 * @brief Get keys of the node
     */
    template <typename TKey, typename TValue>
    const std::vector<TKey>& Node<TKey, TValue>::getKeys() const 
    { 
        return _keys; 
    }

    /*
     * @brief Get values of the node
     */
	template <typename TKey, typename TValue>
    const std::vector<TValue>& Node<TKey, TValue>::getValues() const 
    { 
        return _values; 
    }

    /*
     * @brief Get children of the node
     */
	template <typename TKey, typename TValue>
    const std::vector<Node<TKey, TValue>*>& Node<TKey, TValue>::getChildren() const
    { 
        return _children; 
    }

    /*
     * @brief Get parent of the node
     */
	template <typename TKey, typename TValue>
    Node<TKey, TValue>* Node<TKey, TValue>::getParent() const 
    { 
        return _parent; 
    }

    /*
     * @brief True if Node is a Lead, False otherwise
     */
	template <typename TKey, typename TValue>
    bool Node<TKey, TValue>::isLeaf() const 
    { 
        return _isLeaf; 
    }

    /*
	 * @brief Find index of the key in the node
	 * @param key Key to find
	 * @return Index of the key
     */
    template <typename TKey, typename TValue>
    std::size_t Node<TKey, TValue>::findKeyIndex(const TKey& key) const
    {
        std::size_t i = 0;

        while (i < _keys.size() && _keys[i] < key)
            ++i;

        return i;
    }

    /*
	 * @brief Insert key-value pair into leaf node
	 * @param key Key to insert
	 * @param value Value to insert
	 * @return True if successful, False otherwise
     */
    template <typename TKey, typename TValue>
    bool Node<TKey, TValue>::insertLeaf(const TKey& key, const TValue& value)
    {
        if(!_isLeaf)
            return false;

        std::size_t i = findKeyIndex(key);
        _keys.insert(_keys.begin() + i, key);
        _values.insert(_values.begin() + i, value);
        
        return true;
    }

    /*
	 * @brief Insert key and right child into inner node
	 * @param key Key to insert
	 * @param rightChild Right child node to insert
	 * @return True if successful, False otherwise
     */
    template <typename TKey, typename TValue>
    bool Node<TKey, TValue>::insertInner(const TKey& key, Node<TKey, TValue>* rightChild)
    {
        if(_isLeaf)
            return false;

        if(_children.empty()) // Ensure there is at least one element
            _children.push_back(nullptr);

        std::size_t i = findKeyIndex(key);
        _keys.insert(_keys.begin() + i, key);
        _children.insert(_children.begin() + i + 1, rightChild);
        
		rightChild->_parent = this;

        return true;
    }
}

#endif // DATA_STRUCTURE_NODE_H
