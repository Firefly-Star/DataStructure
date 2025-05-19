#pragma once

#include <optional>
#include <queue>
#include <stack>
#include <iostream>
#include <functional>
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
        template<typename CoreType>
        class BaseIterator
        {
        public:
            auto& operator*() { return node->data; }
            bool operator==(BaseIterator const& other) const { return node == other.node; }
            bool operator!=(BaseIterator const& other) const { return node != other.node; }
        protected:
            BaseIterator(CoreType* node): node(node){}
            CoreType* node;
        };

        template<class CoreType> 
        class LNRIteratorBase : public BaseIterator<CoreType>
        {
        public:
            using BaseIterator<CoreType>::BaseIterator;
            using BaseIterator<CoreType>::node;
            LNRIteratorBase(CoreType* node): BaseIterator<CoreType>(node){}
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
        using lnr_iterator = LNRIteratorBase<NodeType>;
        using lnr_const_iterator = LNRIteratorBase<const NodeType>;

        template<class CoreType>
        class LNRReverseIteratorBase : public BaseIterator<CoreType>
        {
        public:
            using BaseIterator<CoreType>::BaseIterator;
            using BaseIterator<CoreType>::node;
            LNRReverseIteratorBase(CoreType* node) : BaseIterator<CoreType>(node) {}
            LNRReverseIteratorBase& operator++()
            {
                if (node->lchild)
                {
                    node = node->lchild;
                    while (this->node->rchild != nullptr)
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
            LNRReverseIteratorBase operator++(int)
            {
                LNRReverseIteratorBase tmp = *this;
                ++(*this);
                return tmp;
            }
            LNRReverseIteratorBase& operator--()
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
            LNRReverseIteratorBase operator--(int)
            {
                LNRReverseIteratorBase tmp = *this;
                --(*this);
                return tmp;
            }
        };
        using lnr_reverse_iterator = LNRReverseIteratorBase<NodeType>;
        using lnr_const_reverse_iterator = LNRReverseIteratorBase<const NodeType>;
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