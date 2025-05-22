#pragma once
#pragma once
#include "Tool/Compare.h"
#include "Tool/Exception.h"
#include "LBiTree.h"
#include <functional>
#include <queue>

namespace my_stl
{
    template<typename NodeType, class CompPred>
    class BSTreeBase : public LBiTreeBase<NodeType>
    {
    public:
        template<typename CoreType>
        class BSTreeIterator : public BaseIterator<CoreType>
        {
        public:
            using cont_type = BSTreeBase<NodeType, CompPred>;
            using BaseIterator<CoreType>::node;
            BSTreeIterator(CoreType* node, const cont_type* tree)
                :BaseIterator<CoreType>(node), tree(tree) {}
            BSTreeIterator& operator++()
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
                    if (node == nullptr)
                    {
                        node = tree->sentinel;
                    }
                }
                return *this;
            }
            BSTreeIterator operator++(int)
            {
                BSTreeIterator tmp = *this;
                ++(*this);
                return tmp;
            }
            BSTreeIterator& operator--()
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
                    if (node == nullptr)
                    {
                        node = tree->sentinel;
                    }
                }
                return *this;
            }
            BSTreeIterator operator--(int)
            {
                BSTreeIterator tmp = *this;
                ++(*this);
                return tmp;
            }
        private:
            const cont_type* tree;
        };
        template<typename CoreType>
        using BSTreeReverseIterator = ReverseIterator<BSTreeIterator<CoreType>>;

        using iterator = BSTreeIterator<NodeType>;
        using const_iterator = BSTreeIterator<const NodeType>;
        using reverse_iterator = BSTreeReverseIterator<NodeType>;
        using const_reverse_iterator = BSTreeReverseIterator<const NodeType>;
    public:
        using LBiTreeBase<NodeType>::root;
        using typename LBiTreeBase<NodeType>::lnr_iterator;
        using typename LBiTreeBase<NodeType>::lnr_const_iterator;
        using CallBackFunc = std::function<void(NodeType*&)>;

        template<typename T>
        void insert(T val, CallBackFunc on_insert = [](NodeType*&) {})
        {
            insert_impl(val, root, on_insert);
        }
        template<typename T>
        void erase(T val, CallBackFunc on_erase = [](NodeType*&) {})
        {
            erase_impl(val, root, on_erase);
        }
        template<typename T>
        typename iterator find(T val)
        {
            return iterator(find_impl(val), this);
        }
        template<typename T>
        typename const_iterator find(T val) const
        {
            return const_iterator(find_impl(val), this);
        }

        iterator begin() { return iterator(sentinel->rchild, this); }
        const_iterator cbegin() const { return const_iterator(sentinel->rchild, this); }
        const_iterator begin() const { return cbegin(); }
        iterator end() { return iterator(sentinel, this); }
        const_iterator cend() const { return const_iterator(sentinel, this); }
        const_iterator end() const { return cend(); }

        reverse_iterator rbegin() { return reverse_iterator(sentinel->lchild, this); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(sentinel->lchild, this); }
        const_reverse_iterator rbegin() const { return cbegin(); }
        reverse_iterator rend() { return reverse_iterator(sentinel, this); }
        const_reverse_iterator crend() const { return const_reverse_iterator(sentinel, this); }
        const_reverse_iterator rend() const { return cend(); }
        CompPred comp;
        NodeType* sentinel;
    protected:
        BSTreeBase(CompPred comp)
            :LBiTreeBase<NodeType>(), comp(comp)
        {
            sentinel = new NodeType();
            root = sentinel;
            sentinel->parent = nullptr;
        }
        ~BSTreeBase()
        {
            delete sentinel;
        }

        void rotate_left(NodeType*& node)
        {
            NodeType* A = node;
            NodeType* B = node->rchild;
            NodeType* BL = B->lchild;
            A->rchild = BL;
            if (BL)
                BL->parent = A;
            A->height -= 2;
            B->lchild = A;
            B->parent = A->parent;
            A->parent = B;
            node = B;
        }
        void rotate_right(NodeType*& node)
        {
            NodeType* A = node;
            NodeType* B = node->lchild;
            NodeType* BR = B->rchild;
            A->lchild = BR;
            if (BR)
                BR->parent = A;
            A->height -= 2;
            B->rchild = A;
            B->parent = A->parent;
            A->parent = B;
            node = B;
        }
    private:
        template<typename T>
        void insert_impl(T val, NodeType*& node, CallBackFunc on_insert)
        {
            if (node == nullptr || node == sentinel)
            {
                node = new NodeType(val);
                if (sentinel->lchild == nullptr || comp(sentinel->lchild->data, val))
                {
                    sentinel->lchild = node;
                }
                if (sentinel->rchild == nullptr || comp(val, sentinel->rchild->data))
                {
                    sentinel->rchild = node;
                }
                on_insert(node);
                return;
            }
            if (comp(val, node->data))
            {
                insert_impl(val, node->lchild, on_insert);
                node->lchild->parent = node;
            }
            else if (comp(node->data, val))
            {
                insert_impl(val, node->rchild, on_insert);
                node->rchild->parent = node;
            }
            on_insert(node);
        }
        template<typename T>
        void erase_impl(T val, NodeType*& node, CallBackFunc on_erase)
        {
            static auto find_prev = [](NodeType* node)
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
                    return node;
                };
            static auto find_next = [](NodeType* node)
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
                    return node;
                };
            if (node == nullptr)
                return;
            if (comp(val, node->data))
            {
                erase_impl(val, node->lchild, on_erase);
            }
            else if (comp(node->data, val))
            {
                erase_impl(val, node->rchild, on_erase);
            }
            else
            {
                if (node->lchild == nullptr)
                {
                    if (node == sentinel->lchild)
                    {
                        sentinel->lchild = find_prev(node);
                    }
                    if (node == sentinel->rchild)
                    {
                        sentinel->rchild = find_next(node);
                    }
                    NodeType* tmp = node;
                    node = node->rchild;
                    if (node)
                        node->parent = tmp->parent;
                    delete tmp;
                }
                else if (node->rchild == nullptr)
                {
                    if (node == sentinel->lchild)
                    {
                        sentinel->lchild = find_prev(node);
                    }
                    if (node == sentinel->rchild)
                    {
                        sentinel->rchild = find_next(node);
                    }
                    NodeType* tmp = node;
                    node = node->lchild;
                    if (node)
                        node->parent = tmp->parent;
                    delete tmp;
                }
                else
                {
                    NodeType* next = node->rchild;
                    while (next->lchild != nullptr)
                    {
                        next = next->lchild;
                    }
                    node->data = next->data;
                    erase_impl(next->data, node->rchild, on_erase);
                }
            }
            if (node) on_erase(node);
        }
        template<typename T>
        NodeType* find_impl(T val)
        {
            NodeType* cur = root;
            while (cur != nullptr)
            {
                if (comp(val, cur->data))
                    cur = cur->lchild;
                else if (comp(cur->data, val))
                    cur = cur->rchild;
                else
                    return cur;
            }
            return sentinel;
        }
    };

    template<typename T, typename CompPred = less<T>>
    class BSTree : public BSTreeBase<BaseTreeNode<T>, CompPred>
    {
    public:
        BSTree(CompPred comp = CompPred()): BSTreeBase<BaseTreeNode<T>, CompPred>(comp){}
    };
}