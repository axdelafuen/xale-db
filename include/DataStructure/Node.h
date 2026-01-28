#ifndef DATA_STRUCTURE_NODE_H
#define DATA_STRUCTURE_NODE_H

#include "Core/ExceptionHandler.h"

#include <vector>
#include <exception>
#include <string>

namespace Xale::DataStructure
{
    template <typename TKey, typename TValue>
    class Node
    {
        public:
            Node();
            bool insertAtLeaf(T value);
            
        private:
            std::vector<TKey> _keys;
            std::vector<Node*> _children;   // inner nodes
            std::vector<TValue> _values     // leaf nodes
            Node* _next;
            Node* _parent;
            bool _isLeaf;
    };

    template <typename TKey, typename TValue>
    Node<T>::Node()
    {
        _keys = {};
        _children = {};
        _values = {};
        _next = nullptr;
        _parent = nullptr;
        _isLeaf = true;
    } 

    template <typename TKey, typename TValue>
    std::size_t Node<TKey, TValue>findKeyIndex(const TKey key) const
    {
        std::size_t i = 0;

        while (i < _keys.size() && _keys[i] < key)
            ++i;

        return i;
    }

    template <typename TKey, typename TValue>
    bool Node<TKey, TValue>::insertAtLeaf(const TKey key, const TValue value)
    {
        try
        {
            if(_isLeaf)
                return false;

            std::size_t i = findKeyIndex(key);
            _keys.insert(_keys.begin() + i, key);
            _values.insert(_values.begin() + i, value);
            return true;
        }
        catch (const std::exception& e)
        {
            std::string eStr(e.what());
            THROW_DB_EXCEPTION(
                Xale::Core::ExceptionCode::DataStruct,
                "Node vector error: " + eStr); 
        }
    }

    template <typename TKey, typename TValue>
    bool Node<TKey, TValue>::insertAtInner(const TKey& key, Node* rightChild) {
        if(_isLeaf)
            return false;

        std::size_t i = findKeyIndex(key);
        _keys.insert(_keys.begin() + i, key);
        _children.insert(_children.begin() + i + 1, rightChild);
        
        // not accessible?
        //rightChild->_parent = this;
        return true;
    }
}

#endif // DATA_STRUCTURE_NODE_H
