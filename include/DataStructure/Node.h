#ifndef DATA_STRUCTURE_NODE_H
#define DATA_STRUCTURE_NODE_H

#include "Core/ExceptionHandler.h"

#include <vector>
#include <exception>
#include <string>

namespace Xale::DataStructure
{
    /**
     * @brief Node struct for B+Tree implementation
     */
    template<typename TKey, typename TValue>
    struct Node {
        explicit Node(bool leaf) : parent(nullptr), next(nullptr), isLeaf(leaf) {}

        std::vector<TKey> keys;
        std::vector<TValue> values;       // leaf
        std::vector<Node*> children;      // inner
        Node* parent;
        Node* next;
        bool isLeaf;
    };
}

#endif // DATA_STRUCTURE_NODE_H
