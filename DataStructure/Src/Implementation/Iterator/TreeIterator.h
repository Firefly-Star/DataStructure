#pragma once

#include "IteratorTag.h"

namespace my_stl
{
    template<typename NodeType>
    struct NodeTraits 
    {
        using value_type = decltype(NodeType::data);
        using pointer = value_type*;
        using reference = value_type&;
    };

    template<typename NodeType>
    struct NodeTraits<const NodeType>
    {
        using value_type = decltype(NodeType::data);
        using pointer = const value_type*;
        using reference = const value_type&;
    };

    template<typename CoreType>
    class BaseIterator
    {
    public:
        using node_traits = NodeTraits<CoreType>;
        using difference_type = ptrdiff_t;
        using value_type = typename node_traits::value_type;
        using pointer = typename node_traits::pointer;
        using reference = typename node_traits::reference;
        using iterator_category = bidirectional_iterator_tag;

        auto& operator*() { return node->data; }
        bool operator==(BaseIterator const& other) const { return node == other.node; }
        bool operator!=(BaseIterator const& other) const { return node != other.node; }
    protected:
        BaseIterator(CoreType* node) : node(node) {}
        CoreType* node;
    };

    template<class ForwardIterator>
    class ReverseIterator : public ForwardIterator
    {
    public:
        using ForwardIterator::ForwardIterator;
        ReverseIterator& operator++()
        {
            ForwardIterator::operator--();
            return *this;
        }
        ReverseIterator& operator--()
        {
            ForwardIterator::operator++();
            return *this;
        }
        ReverseIterator operator++(int)
        {
            ReverseIterator old = *this;
            ForwardIterator::operator--();
            return old;
        }
        ReverseIterator& operator--(int)
        {
            ReverseIterator old = *this;
            ForwardIterator::operator++();
            return old;
        }
    };

    // Implementation
    
    // LNR
    template<class CoreType>
    class LNRIteratorBase : public BaseIterator<CoreType>
    {
    public:
        using BaseIterator<CoreType>::BaseIterator;
        using BaseIterator<CoreType>::node;
        LNRIteratorBase(CoreType* node) : BaseIterator<CoreType>(node) {}
        LNRIteratorBase& operator++()
        {
            if (node->rchild)
            {
                node = node->rchild;
                while (node->lchild != nullptr)
                {
                    node = node->lchild;
                }
            }
            else
            {
                while (node->parent != nullptr && node != node->parent->lchild)
                {
                    node = node->parent;
                }
                node = node->parent;
            }
            return *this;
        }
        LNRIteratorBase operator++(int)
        {
            LNRIteratorBase tmp = *this;
            ++(*this);
            return tmp;
        }
        LNRIteratorBase& operator--()
        {
            if (node->lchild)
            {
                node = node->lchild;
                while (node->rchild != nullptr)
                {
                    node = node->rchild;
                }
            }
            else
            {
                while (node->parent != nullptr && node != node->parent->rchild)
                {
                    node = node->parent;
                }
                node = node->parent;
            }
            return *this;
        }
        LNRIteratorBase operator--(int)
        {
            LNRIteratorBase tmp = *this;
            --(*this);
            return tmp;
        }
    };

    template<typename CoreType>
    using LNRReverseIteratorBase = ReverseIterator<LNRIteratorBase<CoreType>>;
    
    // NLR
    template<class CoreType>
    class NLRIteratorBase : public BaseIterator<CoreType>
    {
    public:
        using BaseIterator<CoreType>::BaseIterator;
        using BaseIterator<CoreType>::node;
        NLRIteratorBase(CoreType* node) : BaseIterator<CoreType>(node) {}

        NLRIteratorBase& operator++()
        {
            if (node->lchild) 
            {
                node = node->lchild;
            }
            else if (node->rchild) 
            {
                node = node->rchild;
            }
            else 
            {
                // Go up until we find a node that has a right child we haven't visited
                while (node->parent &&
                    (node->parent->rchild == node || node->parent->rchild == nullptr)) 
                {
                    node = node->parent;
                }
                if (node->parent) 
                {
                    node = node->parent->rchild;
                }
                else 
                {
                    node = nullptr; // end of traversal
                }
            }
            return *this;
        }

        NLRIteratorBase operator++(int)
        {
            NLRIteratorBase tmp = *this;
            ++(*this);
            return tmp;
        }

        NLRIteratorBase& operator--()
        {
            if (node->parent == nullptr) {
                // At begin(), can't go back further
                node = nullptr;
                return *this;
            }

            if (node == node->parent->rchild) {
                if (node->parent->lchild) {
                    node = node->parent->lchild;
                    while (true) {
                        if (node->rchild) {
                            node = node->rchild;
                        }
                        else if (node->lchild) {
                            node = node->lchild;
                        }
                        else {
                            break;
                        }
                    }
                }
                else {
                    node = node->parent;
                }
            }
            else {
                node = node->parent;
            }
            return *this;
        }

        NLRIteratorBase operator--(int)
        {
            NLRIteratorBase tmp = *this;
            --(*this);
            return tmp;
        }
    };

    template<class CoreType>
    using NLRReverseIteratorBase = ReverseIterator<NLRIteratorBase<CoreType>>;

    
    // LRN
    template<class CoreType>
    class LRNIteratorBase : public BaseIterator<CoreType>
    {
    public:
        using BaseIterator<CoreType>::BaseIterator;
        using BaseIterator<CoreType>::node;
        LRNIteratorBase(CoreType* node) : BaseIterator<CoreType>(node) {}

        LRNIteratorBase& operator++()
        {
            if (node->parent == nullptr) 
            {
                node = nullptr;
            }
            else if (node == node->parent->rchild) 
            {
                node = node->parent;
            }
            else 
            {
                // node is left child
                if (node->parent->rchild) 
                {
                    node = node->parent->rchild;
                    while (true) 
                    {
                        if (node->lchild) 
                        {
                            node = node->lchild;
                        }
                        else if (node->rchild) 
                        {
                            node = node->rchild;
                        }
                        else 
                        {
                            break;
                        }
                    }
                }
                else 
                {
                    node = node->parent;
                }
            }
            return *this;
        }

        LRNIteratorBase& operator--()
        {
            if (node->rchild)
            {
                node = node->rchild;
            }
            else if (node->lchild)
            {
                node = node->lchild;
            }
            else
            {
                while (node->parent &&
                    (node->parent->lchild == node || node->parent->lchild == nullptr))
                {
                    node = node->parent;
                }
                if (node->parent)
                {
                    node = node->parent->lchild;
                }
                else
                {
                    node = nullptr;
                }
            }
            return *this;

        }

        LRNIteratorBase operator++(int)
        {
            LRNIteratorBase tmp = *this;
            ++(*this);
            return tmp;
        }

        LRNIteratorBase operator--(int)
        {
            LRNIteratorBase tmp = *this;
            --(*this);
            return tmp;
        }
    };

    template<class CoreType>
    using LRNReverseIteratorBase = ReverseIterator<LRNIteratorBase<CoreType>>;
}