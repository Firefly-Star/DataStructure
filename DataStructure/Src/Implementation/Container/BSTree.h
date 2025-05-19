#pragma once
#pragma once
#include "Tool/Compare.h"
#include "LBiTree.h"
#include <functional>
#include <queue>

namespace my_stl
{
    template<typename NodeType, class CompPred>
    class BSTreeBase : public LBiTree<NodeType>
    {
    public:
        using LBiTree<NodeType>::root;
        using CallBackFunc = std::function<void(NodeType*)>;
        BSTreeBase(CompPred comp)
            :LBiTree<NodeType>(), comp(comp)
        {
        }
        template<typename T>
        void insert(T val)
        {
            insert_impl(val, root);
        }
        template<typename T>
        void erase(T val)
        {
            erase_impl(val, root);
        }
    protected:
        CompPred comp;
    protected:
        template<typename T>
        void insert_impl(T val, NodeType*& node)
        {
            if (node == nullptr)
            {
                node = new NodeType(val);
                return;
            }
            if (comp(val, node->data))
            {
                insert_impl(val, node->lchild);
                node->lchild->parent = node;
            }
            else if (comp(node->data, val))
            {
                insert_impl(val, node->rchild);
                node->rchild->parent = node;
            }
        }
        template<typename T>
        void erase_impl(T val, NodeType*& node)
        {
            if (node == nullptr)
                return;
            if (comp(val, node->data))
            {
                erase_impl(val, node->lchild);
            }
            else if (comp(node->data, val))
            {
                erase_impl(val, node->rchild);
            }
            else
            {
                if (node->lchild == nullptr)
                {
                    NodeType* tmp = node;
                    node = node->rchild;
                    node->parent = tmp->parent;
                    delete tmp;
                    return;
                }
                if (node->rchild == nullptr)
                {
                    NodeType* tmp = node;
                    node = node->lchild;
                    node->parent = tmp->parent;
                    delete tmp;
                    return;
                }
                NodeType* next = node->rchild;
                while (next->lchild != nullptr)
                {
                    next = next->lchild;
                }
                std::swap(*next, *node);
                [[likely]] if (next->parent->lchild == next)
                {
                    next->parent->lchild = nullptr;
                }
                else if (next->parent->rchild == next)
                {
                    next->parent->rchild = nullptr;
                }
                delete next;
            }
        }
    };
}