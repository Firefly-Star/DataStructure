#pragma once

#include <optional>
#include <queue>
#include <stack>
#include <iostream>
#include <functional>

namespace my_stl
{

    template <typename T>
    struct LBiTreeNode
    {
        T data;
        LBiTreeNode<T>* left;
        LBiTreeNode<T>* right;
        LBiTreeNode(std::initializer_list<std::optional<T>> list = {})
            : left(nullptr), right(nullptr)
        {
            if (list.size() == 0 || !*(list.begin())) return;
            using size_type = typename std::initializer_list<std::optional<T>>::size_type;
            std::queue<LBiTreeNode<T>*> q;
            auto it = list.begin();
            data = **it;
            q.push(this);
            while (!q.empty())
            {
                LBiTreeNode<T>* cur = q.front();
                q.pop();

                if (++it == list.end()) break;
                if (*it)
                {
                    LBiTreeNode<T>* l = new LBiTreeNode<T>;
                    l->data = **it;
                    cur->left = l;
                    q.push(l);
                }

                if (++it == list.end()) break;
                if (*it)
                {
                    LBiTreeNode<T>* r = new LBiTreeNode<T>;
                    r->data = **it;
                    cur->right = r;
                    q.push(r);
                }
            }
        }
    };

    enum class TravesalType
    {
        LRN, LNR, NLR
    };

    template<typename T>
    class LBiTree
    {
    public:
        template<TravesalType tt = TravesalType::NLR>
        struct Proxy
        {
            Proxy(LBiTree<T>& tree) : tree(tree) {}
            LBiTree<T>& tree;
        };

    public:
        using node_type = LBiTreeNode<T>;
        using pointer = node_type*;

        LBiTree(std::initializer_list<std::optional<T>> list = {})
            : root(new node_type(list)) {
        }
        LBiTree(pointer root)
            : root(root) {
        }
        LBiTree(LBiTree const& other)
            : root(nullptr)
        {
            _copy_from(other);
        }
        LBiTree(LBiTree&& other) noexcept
            : root(nullptr)
        {
            _swap(other);
        }
        ~LBiTree()
        {
            std::queue<LBiTreeNode<T>*> q;
            q.push(root);
            while (!q.empty())
            {
                LBiTreeNode<T>* cur = q.front();
                q.pop();
                if (cur->left)
                {
                    q.push(cur->left);
                }
                if (cur->right)
                {
                    q.push(cur->right);
                }
                delete cur;
            }
        }
        LBiTree& operator=(LBiTree const& other)
        {
            LBiTree temp = other;
            _swap(temp);
            return *this;
        }
        LBiTree& operator=(LBiTree&& other) noexcept
        {
            _swap(other);
            return *this;
        }
        template<typename Func = std::function<void(pointer const)>, typename... Args>
        void nlr(Func&& func = [](pointer const node) { std::cout << node->data << " "; }, Args&&... args) const
        {
            std::stack<LBiTreeNode<T>*> st;
            st.push(root);
            while (!st.empty())
            {
                LBiTreeNode<T>* cur = st.top();
                st.pop();
                func(cur, std::forward<Args>(args)...);
                if (cur->right)
                {
                    st.push(cur->right);
                }
                if (cur->left)
                {
                    st.push(cur->left);
                }
            }
        }
        template<typename Func, typename... Args>
        void nlr_with_stack(Func&& func, Args&&... args) const
        {
            std::stack<LBiTreeNode<T>*> st;
            st.push(root);
            while (!st.empty())
            {
                LBiTreeNode<T>* cur = st.top();
                st.pop();
                func(cur, st, std::forward<Args>(args)...);
                if (cur->right)
                {
                    st.push(cur->right);
                }
                if (cur->left)
                {
                    st.push(cur->left);
                }
            }
        }
        template<typename Func = std::function<void(pointer const)>, typename... Args>
        void lnr(Func&& func = [](pointer const node) { std::cout << node->data << " "; }, Args&&... args) const
        {
            std::stack<LBiTreeNode<T>*> st;
            LBiTreeNode<T>* cur = root;
            while (cur || !st.empty())
            {
                while (cur)
                {
                    st.push(cur);
                    cur = cur->left;
                }
                cur = st.top();
                st.pop();
                func(cur, std::forward<Args>(args)...);
                cur = cur->right;
            }
        }
        template<typename Func, typename... Args>
        void lnr_with_stack(Func&& func, Args&&... args) const
        {
            std::stack<LBiTreeNode<T>*> st;
            LBiTreeNode<T>* cur = root;
            while (cur || !st.empty())
            {
                while (cur)
                {
                    st.push(cur);
                    cur = cur->left;
                }
                cur = st.top();
                st.pop();
                func(cur, st, std::forward<Args>(args)...);
                cur = cur->right;
            }
        }
        template<typename Func, typename... Args>
        void lnr_with_stack_and_stage(Func&& func, Args&&... args) const
        {
            struct StackFrame
            {
                enum class Stage
                {
                    L, N, R
                };
                LBiTreeNode<T>* node;
                Stage stage;
            };
            std::stack<StackFrame> st;
            st.push({ root, StackFrame::Stage::L });
            while (!st.empty())
            {
                auto cur = st.top();
                st.pop();
                if (cur.node != nullptr)
                {
                    switch (cur.stage)
                    {
                    case StackFrame::Stage::L:
                    {
                        st.push({ cur.node, StackFrame::Stage::N });
                        st.push({ cur.node->left, StackFrame::Stage::L });
                        break;
                    }
                    case StackFrame::Stage::N:
                    {
                        func(cur.node, std::forward<Args>(args)...);
                        st.push({ cur.node, StackFrame::Stage::R });
                        break;
                    }
                    case StackFrame::Stage::R:
                    {
                        st.push({ cur.node->right, StackFrame::Stage::L });
                        break;
                    }
                    }
                }
            }
        }
        template<typename Func = std::function<void(pointer const)>, typename... Args>
        void lrn(Func&& func = [](pointer const node) { std::cout << node->data << " "; }, Args&&... args) const
        {
            std::stack<LBiTreeNode<T>*> st;
            LBiTreeNode<T>* cur = root;
            LBiTreeNode<T>* prev = nullptr;
            while (cur || !st.empty())
            {
                while (cur)
                {
                    st.push(cur);
                    cur = cur->left;
                }
                cur = st.top();
                if (cur->right && cur->right != prev)
                {
                    cur = cur->right;
                }
                else
                {
                    st.pop();
                    func(cur, std::forward<Args>(args)...);
                    prev = cur;
                    cur = nullptr;
                }
            }
        }
        template<typename Func, typename... Args>
        void lrn_with_stack(Func&& func, Args&&... args) const
        {
            std::stack<LBiTreeNode<T>*> st;
            LBiTreeNode<T>* cur = root;
            LBiTreeNode<T>* prev = nullptr;
            while (cur || !st.empty())
            {
                while (cur)
                {
                    st.push(cur);
                    cur = cur->left;
                }
                cur = st.top();
                if (cur->right && cur->right != prev)
                {
                    cur = cur->right;
                }
                else
                {
                    st.pop();
                    func(cur, st, std::forward<Args>(args)...);
                    prev = cur;
                    cur = nullptr;
                }
            }
        }
        template<typename Func = std::function<void(pointer const)>, typename... Args>
        void level_order(Func&& func = [](pointer const node) { std::cout << node->data << " "; }, Args&&... args) const
        {
            std::queue<LBiTreeNode<T>*> q;
            q.push(root);
            while (!q.empty())
            {
                LBiTreeNode<T>* cur = q.front();
                q.pop();
                func(cur, std::forward<Args>(args)...);
                if (cur->left)
                {
                    q.push(cur->left);
                }
                if (cur->right)
                {
                    q.push(cur->right);
                }
            }
        }
        template<typename Func = std::function<void(pointer const)>, typename... Args>
        void level_order_with_level(Func&& func = [](pointer const node) { std::cout << node->data << " "; }, Args&&... args) const
        {
            std::queue<std::pair<LBiTreeNode<T>*, int>> q;
            q.push({ root, 1 });
            while (!q.empty())
            {
                auto [cur, level] = q.front();
                q.pop();
                func(cur, level, std::forward<Args>(args)...);
                if (cur->left)
                {
                    q.push({ cur->left, level + 1 });
                }
                if (cur->right)
                {
                    q.push({ cur->right, level + 1 });
                }
            }
        }
        size_t height() const
        {
            std::stack<std::pair<LBiTreeNode<T>*, int>> st;
            st.push({ root, 1 });
            int height = 0;
            while (!st.empty())
            {
                auto [cur, h] = st.top();
                st.pop();
                if (cur->left == nullptr && cur->right == nullptr)
                {
                    if (h > height)
                    {
                        height = h;
                    }
                }
                else
                {
                    if (cur->left)
                    {
                        st.push({ cur->left, h + 1 });
                    }
                    if (cur->right)
                    {
                        st.push({ cur->right, h + 1 });
                    }
                }
            }
            return height;
        }
        LBiTreeNode<T>* const data() const { return root; }
        bool is_complete() const
        {
            bool require_leaf = false;
            bool result = true;
            auto func = [&](pointer const node) {
                if (require_leaf)
                {
                    if (node->left || node->right)
                    {
                        result = false;
                    }
                }
                if (node->left == nullptr)
                {
                    require_leaf = true;
                    if (node->right != nullptr)
                    {
                        result = false;
                    }
                }
                else
                {
                    if (node->right == nullptr)
                    {
                        require_leaf = true;
                    }
                }
                };
            level_order(func);
            return result;
        }
        void reverse()
        {
            auto func = [](pointer node)
                {
                    std::swap(node->left, node->right);
                };
            nlr(func);
        }
        T wpl() const
        {
            T result = 0;
            auto sum = [&](pointer node, int level)
                {
                    if (node->left == nullptr && node->right == nullptr)
                    {
                        result += level * node->data;
                    }
                };
            level_order_with_level(sum);
            return result;
        }
    private:
        pointer root;
        void _copy_from(LBiTree const& other)
        {
            root = new node_type;
            std::stack<pointer> this_st;
            auto insert = [&](pointer node)
                {
                    pointer this_cur = this_st.top();
                    this_st.pop();

                    this_cur->data = node->data;

                    if (node->right)
                    {
                        pointer newright = new node_type;
                        this_cur->right = newright;
                        this_st.push(newright);
                    }
                    if (node->left)
                    {
                        pointer newleft = new node_type;
                        this_cur->left = newleft;
                        this_st.push(newleft);
                    }
                };
            this_st.push(root);
            other.nlr(insert);
        }
        void _swap(LBiTree& other)
        {
            std::swap(root, other.root);
        }
    };

    class DSU
    {
    public:
        DSU(int n)
            :parent(n, -1), rank(n, 1)
        {
            for (int i = 0; i < n; ++i)
            {
                parent[i] = i;
            }
        }
        int Find(int a)
        {
            if (parent[a] != a)
            {
                parent[a] = Find(parent[a]);
            }
            return parent[a];
        }
        void Union(int a, int b)
        {
            int roota = Find(a);
            int rootb = Find(b);
            if (roota == rootb) return;
            if (rank[roota] == rank[rootb])
            {
                parent[roota] = rootb;
                ++rank[rootb];
            }
            else if (rank[roota] < rank[rootb])
            {
                parent[roota] = rootb;
            }
            else
            {
                parent[rootb] = roota;
            }
        }
    private:
        std::vector<int> parent;
        std::vector<int> rank;
    };

    template<typename T>
    struct less
    {
        bool operator()(T const& left, T const& right)
        {
            return left < right;
        }
    };

    template<typename T>
    struct greater
    {
        bool operator()(T const& left, T const& right)
        {
            return left > right;
        }
    };


    template<typename T, class Compare = less<T>>
    class AVLTree
    {
    public:
        struct AVLTreeNode
        {
            T data;
            AVLTreeNode* left;
            AVLTreeNode* right;
            int height;
        };
        using node_type = AVLTreeNode;
        using pointer = AVLTreeNode*;
    public:
        AVLTree(Compare comparator = Compare())
            :root(nullptr), comparator(comparator)
        {
        }

        AVLTree(std::initializer_list<T> list, Compare comparator = Compare())
            :root(nullptr), comparator(comparator)
        {
            for (auto ele : list)
            {
                insert(ele);
            }
        }
        ~AVLTree()
        {
            clear(root);
        }
        template<typename U>
        void insert(U&& ele)
        {
            _insert(root, std::forward<U>(ele));
        }
        template<typename U>
        void erase(U&& ele)
        {
            _erase(root, std::forward<U>(ele));
        }
        const pointer const data() const
        {
            return root;
        }
    private:
        pointer root;
        Compare comparator;
    private:
        template<typename U>
        void _insert(pointer& node, U&& ele)
        {
            if (node == nullptr)
            {
                node = new node_type{ std::forward<U>(ele), nullptr, nullptr, 1 };
                return;
            }
            if (node->data == ele) return;
            if (comparator(node->data, ele))
            {
                _insert(node->right, ele);
            }
            else
            {
                _insert(node->left, ele);
            }
            node->height = 1 + std::max(height(node->left), height(node->right));
            adjust(node);
        }
        template<typename U>
        void _erase(pointer& node, U&& ele)
        {
            if (node == nullptr) return;

            if (node->data == ele)
            {
                if (node->left == nullptr && node->right == nullptr)
                {
                    delete node;
                    node = nullptr;
                }
                else if (node->left != nullptr)
                { // nonempty left subtree
                    pointer tmp = node;
                    node = node->left;
                    node->right = tmp->right;
                    delete tmp;
                }
                else
                { // empty left subtree, nonempty right subtree
                    pointer tmp = node;
                    node = node->right;
                    delete tmp;
                }
            }
            else if (node->data < ele)
            {
                _erase(node->right, std::forward<U>(ele));
            }
            else
            {
                _erase(node->left, std::forward<U>(ele));
            }
        }
        static void adjust(pointer node)
        {
            if (get_balance(node) < -1)
            {
                if (get_balance(node->right) < 0)
                {
                    RR(node);
                }
                else
                {
                    RL(node);
                }
            }
            else if (get_balance(node) > 1)
            {
                if (get_balance(node->left) < 0)
                {
                    LR(node);
                }
                else
                {
                    LL(node);
                }
            }
        }
        static int height(pointer node)
        {
            return node == nullptr ? 0 : node->height;
        }
        static int get_balance(pointer node)
        {
            return node == nullptr ? 0 : height(node->left) - height(node->right);
        }
        static void LL(pointer node)
        {
            pointer A = node;
            pointer B = A->left;
            pointer BL = B->left;
            pointer BR = B->right;
            pointer AR = A->right;
            std::swap(A->data, B->data);
            A->right = B;
            B->right = AR;
            B->left = BR;
            A->left = BL;
        }
        static void RR(pointer node)
        {
            pointer A = node;
            pointer AL = A->left;
            pointer B = A->right;
            pointer BL = B->left;
            pointer BR = B->right;
            std::swap(A->data, B->data);
            B->left = AL;
            B->right = BL;
            A->right = BR;
            A->left = B;
        }
        static void LR(pointer node)
        {
            LL(node->left);
            RR(node);
        }
        static void RL(pointer node)
        {
            RR(node->right);
            LL(node);
        }
        static void clear(pointer node)
        {
            if (node != nullptr)
            {
                clear(node->left);
                clear(node->right);
                delete node;
            }
        }
    };
}