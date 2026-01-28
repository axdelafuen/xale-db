#ifndef DATA_STRUCTURE_NODE_H
#define DATA_STRUCTURE_NODE_H

#include "Core/ExceptionHandler.h"

#include <vector>
#include <exception>
#include <string>

namespace Xale::DataStructure
{
    /*
	 * @brief Node class for B+ tree
     */
    template <typename TKey, typename TValue>
    class Node
    {
		using NodePtr = Node<TKey, TValue>*;    // Pointer to templated Node type
        
        public:
            explicit Node(bool isLeaf);
            std::size_t findKeyIndex(const TKey& key) const;
            bool insertLeaf(const TKey& key, const TValue& value);
            bool insertInner(const TKey& key, NodePtr rightChild);
        private:
            std::vector<TKey> _keys;
            std::vector<NodePtr> _children;     // inner nodes
            std::vector<TValue> _values;        // leaf nodes
            NodePtr _next;
            NodePtr _parent;
            bool _isLeaf;
    };


    /*
	 * @brief Constructor for Node class
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
  
    template <typename TKey, typename TValue>
    std::size_t Node<TKey, TValue>::findKeyIndex(const TKey& key) const
    {
        std::size_t i = 0;

        while (i < _keys.size() && _keys[i] < key)
            ++i;

        return i;
    }

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

    template <typename TKey, typename TValue>
    bool Node<TKey, TValue>::insertInner(const TKey& key, NodePtr rightChild)
    {
        if(_isLeaf)
            return false;

        std::size_t i = findKeyIndex(key);
        _keys.insert(_keys.begin() + i, key);
		_children.resize(_children.size() + 1);
        _children.insert(_children.begin() + i + 1, rightChild);
        
		rightChild->_parent = this;

        return true;
    }
}

#endif // DATA_STRUCTURE_NODE_H
