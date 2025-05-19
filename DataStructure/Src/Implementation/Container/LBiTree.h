#pragma once

#include <optional>
#include <queue>
#include <stack>
#include <iostream>
#include <functional>
#include "Implementation/Iterator/TreeIterator.h"
#include "Tool/Compare.h"
#include "Tool/Exception.h"

namespace my_stl
{
    template<typename T>
    struct BaseTreeNode
    {
        T data;
        BaseTreeNode* parent = nullptr;
        BaseTreeNode* lchild = nullptr;
        BaseTreeNode* rchild = nullptr;
        BaseTreeNode(T data) :data(data), lchild(nullptr), rchild(nullptr){}
    };

    template<typename NodeType>
    class LBiTreeBase
    {
    public:
        using lnr_iterator = LNRIteratorBase<NodeType>;
        using lnr_const_iterator = LNRIteratorBase<const NodeType>;
        using lnr_reverse_iterator = LNRReverseIteratorBase<NodeType>;
        using lnr_const_reverse_iterator = LNRReverseIteratorBase<const NodeType>;

        using nlr_iterator = NLRIteratorBase<NodeType>;
        using nlr_const_iterator = NLRIteratorBase<const NodeType>;
        using nlr_reverse_iterator = NLRReverseIteratorBase<NodeType>;
        using nlr_const_reverse_iterator = NLRReverseIteratorBase<const NodeType>;

        using lrn_iterator = LRNIteratorBase<NodeType>;
        using lrn_const_iterator = LRNIteratorBase<const NodeType>;
        using lrn_reverse_iterator = LRNReverseIteratorBase<NodeType>;
        using lrn_const_reverse_iterator = LRNReverseIteratorBase<const NodeType>;
    public:
        ~LBiTreeBase()
        {
            if (root == nullptr)
                return;
            std::queue<NodeType*> q;
            q.push(root);
            while (!q.empty())
            {
                NodeType* node = q.front();
                q.pop();
                if (node->lchild)
                {
                    q.push(node->lchild);
                }
                if (node->rchild)
                {
                    q.push(node->rchild);
                }
                delete node;
            }
        }
        LBiTreeBase(LBiTreeBase const& other)
        {
            copy(other);
        }
        LBiTreeBase(LBiTreeBase&& other) noexcept
        {
            swap(other);
        }
        LBiTreeBase& operator=(LBiTreeBase const& other)
        {
            LBiTreeBase tmp(other);
            swap(tmp);
            return *this;
        }
        LBiTreeBase& operator=(LBiTreeBase&& other) noexcept
        {
            swap(other);
            return *this;
        }

        template<typename Func, typename... Args>
        void nlr(Func func, Args&&... args)
        {
            if (root == nullptr)
                throw std::exception("Try to traversal an empty tree.");
            std::stack<NodeType*> st;
            st.push(root);
            while (!st.empty())
            {
                NodeType* cur = st.top();
                st.pop();
                std::invoke(func, cur->data, std::forward<Args>(args)...);
                if (cur->rchild)
                    st.push(cur->rchild);
                if (cur->lchild)
                    st.push(cur->lchild);
            }
        }
        template<typename Func, typename... Args>
        void lnr(Func func, Args&&... args)
        {
            if (root == nullptr)
                throw std::exception("Try to traversal an empty tree.");
            struct CallFrame
            {
                NodeType* node;
                int stage; // 0: before left, 1: before node, 2: before right
            };
            std::stack<CallFrame> st;
            st.push({ root, 0 });
            while (!st.empty())
            {
                auto frame = st.top();
                st.pop();
                NodeType* node = frame.node;
                switch (frame.stage)
                {
                case 0:
                {
                    st.push({ node, 1 });
                    if (node->lchild != nullptr)
                        st.push({ node->lchild, 0 });
                    break;
                }
                case 1:
                {
                    std::invoke(func, node->data, std::forward<Args>(args)...);
                    st.push({ node, 2 });
                    break;
                }
                case 2:
                {
                    if (node->rchild!= nullptr)
                        st.push({ node->rchild, 0 });
                    break;
                }
                }
            }
        }
        template<typename Func, typename... Args>
        void lrn(Func func, Args&&... args)
        {
            if (root == nullptr)
                throw std::exception("Try to traversal an empty tree.");
            struct CallFrame
            {
                NodeType* node;
                int stage; // 0: before left, 1: before right, 2: before node
            };
            std::stack<CallFrame> st;
            st.push({ root, 0 });
            while (!st.empty())
            {
                auto frame = st.top();
                st.pop();
                NodeType* node = frame.node;
                switch (frame.stage)
                {
                case 0:
                {
                    st.push({ node, 1 });
                    if (node->lchild != nullptr)
                        st.push({ node->lchild, 0 });
                    break;
                }
                case 1:
                {
                    st.push({ node, 2 });
                    if (node->rchild != nullptr)
                        st.push({ node->rchild, 0 });
                    break;
                }
                case 2:
                {
                    std::invoke(func, node->data, std::forward<Args>(args)...);
                    break;
                }
                }
            }
        }
        template<typename Func, typename... Args>
        void level_order(Func func, Args&&... args)
        {
            if (root == nullptr)
                throw std::exception("Try to traversal an empty tree.");
            std::queue<NodeType*> q;
            q.push(root);
            while (!q.empty())
            {
                NodeType* cur = q.front();
                q.pop();
                std::invoke(func, cur->data, std::forward<Args>(args)...);
                if (cur->lchild != nullptr)
                {
                    q.push(cur->lchild);
                }
                if (cur->rchild != nullptr)
                {
                    q.push(cur->rchild);
                }
            }
        }

        lnr_iterator lnr_begin()
        {
            if (root == nullptr)
            {
                return lnr_iterator(nullptr);
            }
            NodeType* node = root;
            while (node->lchild != nullptr)
            {
                node = node->lchild;
            }
            return lnr_iterator(node);
        }
        lnr_const_iterator lnr_cbegin() const
        {
            if (root == nullptr)
            {
                return lnr_const_iterator(nullptr);
            }
            const NodeType* node = root;
            while (node->lchild != nullptr)
            {
                node = node->lchild;
            }
            return lnr_const_iterator(node);
        }
        lnr_const_iterator lnr_begin() const
        {
            return lnr_cbegin();
        }
        lnr_iterator lnr_end()
        {
            return lnr_iterator(nullptr);
        }
        lnr_const_iterator lnr_cend() const
        {
            return lnr_const_iterator(nullptr);
        }
        lnr_const_iterator lnr_end() const
        {
            return lnr_cend();
        }
        lnr_reverse_iterator lnr_rbegin()
        {
            if (root == nullptr)
            {
                return lnr_reverse_iterator(nullptr);
            }
            NodeType* node = root;
            while (node->rchild != nullptr)
            {
                node = node->rchild;
            }
            return lnr_reverse_iterator(node);
        }
        lnr_const_reverse_iterator lnr_crbegin() const
        {
            if (root == nullptr)
            {
                return lnr_const_reverse_iterator(nullptr);
            }
            const NodeType* node = root;
            while (node->rchild != nullptr)
            {
                node = node->rchild;
            }
            return lnr_const_reverse_iterator(node);
        }
        lnr_const_reverse_iterator lnr_rbegin() const
        {
            return lnr_crbegin();
        }
        lnr_reverse_iterator lnr_rend()
        {
            return lnr_reverse_iterator(nullptr);
        }
        lnr_const_reverse_iterator lnr_crend() const
        {
            return lnr_const_reverse_iterator(nullptr);
        }
        lnr_const_reverse_iterator lnr_rend() const
        {
            return lnr_crend();
        }

        nlr_iterator nlr_begin()
        {
            return nlr_iterator(root);
        }
        nlr_const_iterator nlr_cbegin() const
        {
            return nlr_const_iterator(root);
        }
        nlr_const_iterator nlr_begin() const
        {
            return nlr_cbegin();
        }
        nlr_iterator nlr_end()
        {
            return nlr_iterator(nullptr);
        }
        nlr_const_iterator nlr_cend() const
        {
            return nlr_const_iterator(nullptr);
        }
        nlr_const_iterator nlr_end() const
        {
            return nlr_cend();
        }
        nlr_reverse_iterator nlr_rbegin()
        {
            if (root == nullptr)
                return nlr_reverse_iterator(nullptr);

            // Find the last node in NLR traversal (rightmost leaf)
            NodeType* node = root;
            while (true) {
                if (node->rchild)
                    node = node->rchild;
                else if (node->lchild)
                    node = node->lchild;
                else
                    break;
            }
            return nlr_reverse_iterator(node);
        }
        nlr_const_reverse_iterator nlr_crbegin() const
        {
            if (root == nullptr)
                return nlr_const_reverse_iterator(nullptr);

            const NodeType* node = root;
            while (true) {
                if (node->rchild)
                    node = node->rchild;
                else if (node->lchild)
                    node = node->lchild;
                else
                    break;
            }
            return nlr_const_reverse_iterator(node);
        }
        nlr_const_reverse_iterator nlr_rbegin() const
        {
            return nlr_crbegin();
        }
        nlr_reverse_iterator nlr_rend()
        {
            return nlr_reverse_iterator(nullptr);
        }
        nlr_const_reverse_iterator nlr_crend() const
        {
            return nlr_const_reverse_iterator(nullptr);
        }
        nlr_const_reverse_iterator nlr_rend() const
        {
            return nlr_crend();
        }

        lrn_iterator lrn_begin()
        {
            if (root == nullptr)
                return lrn_iterator(nullptr);

            // Find the first node in LRN traversal (leftmost leaf)
            NodeType* node = root;
            while (true) {
                if (node->lchild)
                    node = node->lchild;
                else if (node->rchild)
                    node = node->rchild;
                else
                    break;
            }
            return lrn_iterator(node);
        }
        lrn_const_iterator lrn_cbegin() const
        {
            if (root == nullptr)
                return lrn_const_iterator(nullptr);

            const NodeType* node = root;
            while (true) {
                if (node->lchild)
                    node = node->lchild;
                else if (node->rchild)
                    node = node->rchild;
                else
                    break;
            }
            return lrn_const_iterator(node);
        }
        lrn_const_iterator lrn_begin() const
        {
            return lrn_cbegin();
        }
        lrn_iterator lrn_end()
        {
            return lrn_iterator(nullptr);
        }
        lrn_const_iterator lrn_cend() const
        {
            return lrn_const_iterator(nullptr);
        }
        lrn_const_iterator lrn_end() const
        {
            return lrn_cend();
        }
        lrn_reverse_iterator lrn_rbegin()
        {
            return lrn_reverse_iterator(root);
        }
        lrn_const_reverse_iterator lrn_crbegin() const
        {
            return lrn_const_reverse_iterator(root);
        }
        lrn_const_reverse_iterator lrn_rbegin() const
        {
            return lrn_crbegin();
        }
        lrn_reverse_iterator lrn_rend()
        {
            return lrn_reverse_iterator(nullptr);
        }
        lrn_const_reverse_iterator lrn_crend() const
        {
            return lrn_const_reverse_iterator(nullptr);
        }
        lrn_const_reverse_iterator lrn_rend() const
        {
            return lrn_crend();
        }

        NodeType* data() const { return root; }
    protected:
        LBiTreeBase() {}
        NodeType* root = nullptr;
    private:
        
        void copy(LBiTreeBase const& other)
        {
            std::queue<std::pair<NodeType*, NodeType*>> q;
            root = new NodeType(*(other.root));
            q.push({ other.root, root });
            while (!q.empty())
            {
                auto [o, t] = q.top();
                q.pop();
                if (o->lchild)
                {
                    NodeType* newnode = new NodeType(*(o->lchild));
                    t->lchild = newnode;
                    newnode->parent = t;
                    q.push({ o->lchild, newnode });
                }
                else
                {
                    t->lchild = nullptr;
                }
                if (o->rchild)
                {
                    NodeType* newnode = new NodeType(*(o->rchild));
                    t->rchild = newnode;
                    newnode->parent = t;
                    q.push({ o->rchild, newnode });
                }
                else
                {
                    t->rchild = nullptr;
                }
            }
        }
        void swap(LBiTreeBase& other)
        {
            std::swap(root, other.root);
        }
    };

    template<typename T>
    class LBiTree : public LBiTreeBase<BaseTreeNode<T>>
    {
    public:
        using LBiTreeBase<BaseTreeNode<T>>::LBiTreeBase;
        using NodeType = BaseTreeNode<T>;
        LBiTree(){}
        template<typename RanIt>
        LBiTree(RanIt first, RanIt last)
        {
            construct(first, last);
        }
        template<typename Range = std::initializer_list<std::optional<T>>>
        LBiTree(Range range = Range())
        {
            construct(range.begin(), range.end());
        }
    private:
        template<typename RanIt>
        void construct(RanIt first, RanIt last)
        {
            if (first == last || !(*first).has_value())
                return;
            std::queue<NodeType*> q;
            RanIt cur = first;
            RanIt child = ++first;
            this->root = new NodeType(cur->value());
            q.push( this->root );
            while (!q.empty())
            {
                auto node = q.front();
                q.pop();
                if (node == nullptr)
                {
                    ++child; ++child;
                    continue;
                }
                if (child < last)
                {
                    if (child->has_value())
                    {
                        NodeType* newnode = new NodeType(child->value());
                        node->lchild = newnode;
                        newnode->parent = node;
                        q.push( newnode );
                    }
                    else
                    {
                        q.push(nullptr);
                    }
                    ++child;
                }
                if (child < last)
                {
                    if (child->has_value())
                    {
                        NodeType* newnode = new NodeType(child->value());
                        node->rchild = newnode;
                        newnode->parent = node;
                        q.push(newnode);
                    }
                    else
                    {
                        q.push(nullptr);
                    }
                    ++child;
                }
            }
        }
    };
}