#pragma once
#include "Tool/Compare.h"
#include "Implementation/Container/BSTree.h"

namespace my_stl
{
    template<typename T>
    struct AVLTreeNode
    {
        T data;
        AVLTreeNode* lchild = nullptr;
        AVLTreeNode* rchild = nullptr;
        AVLTreeNode* parent = nullptr;
        size_t height = 1;
        AVLTreeNode(T val = T()) : data(val) {}
    };

    template<typename T, class CompPred = std::less<T>>
    class AVLTree : public BSTreeBase<AVLTreeNode<T>, CompPred> {
    public:
        AVLTree(CompPred comp = CompPred())
            : BSTreeBase<AVLTreeNode<T>, CompPred>(comp) {}

        using Base = BSTreeBase<AVLTreeNode<T>, CompPred>;
        using Base::rotate_left;
        using Base::rotate_right;

        void insert(T val) 
        {
            Base::insert(val, [this](AVLTreeNode<T>*& node) {
                update_height(node);
                balance(node);
                });
        }

        void erase(T val) 
        {
            Base::erase(val, [this](AVLTreeNode<T>*& node) {
                update_height(node);
                balance(node);
                });
        }

    private:
        static int height(AVLTreeNode<T>* node) 
        {
            return node ? node->height : 0;
        }

        void update_height(AVLTreeNode<T>* node) {
            node->height = std::max(height(node->lchild), height(node->rchild)) + 1;
        }

        void balance(AVLTreeNode<T>*& node) 
        {
            if (height(node->lchild) > height(node->rchild) + 1) 
            {
                if (height(node->lchild->lchild) >= height(node->lchild->rchild)) {
                    rotate_right(node); // LL
                }
                else {
                    rotate_left(node->lchild); // LR
                    rotate_right(node);
                }
            }
            else if (height(node->rchild) > height(node->lchild) + 1) 
            {
                if (height(node->rchild->rchild) >= height(node->rchild->lchild)) {
                    rotate_left(node); // RR
                }
                else {
                    rotate_right(node->rchild); // RL
                    rotate_left(node);
                }
            }
        }

        
    };
}