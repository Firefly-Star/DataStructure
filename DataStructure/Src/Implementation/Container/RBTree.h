#pragma once
#include "Tool/Compare.h"
#include "Implementation/Container/BSTree.h"

namespace my_stl
{
    enum class Color { Black, Red };

    template<typename T>
    struct RBTreeNode
    {
        T data;
        RBTreeNode* lchild = nullptr;
        RBTreeNode* rchild = nullptr;
        RBTreeNode* parent = nullptr;
        size_t height;
        Color color = Color::Red;
        RBTreeNode(T val = T()) : data(val) {}
    };

    template<typename T, class CompPred = std::less<T>>
    class RBTree : public BSTreeBase<RBTreeNode<T>, CompPred>
    {
    public:
        RBTree(CompPred comp = CompPred())
            : BSTreeBase<RBTreeNode<T>, CompPred>(comp)
        {
            this->sentinel->color = Color::Black;
        }

        using Base = BSTreeBase<RBTreeNode<T>, CompPred>;
        using Base::rotate_left;
        using Base::rotate_right;
        using Base::sentinel;

        void insert(T val)
        {
            Base::insert(val, [this](RBTreeNode<T>*& node) {
                fix_insertion(node);
                });
        }

        void erase(T val)
        {
            Base::erase(val, [this](RBTreeNode<T>*& node) {
                fix_deletion(node);
                });
        }

    private:
        void fix_insertion(RBTreeNode<T>* node)
        {
            while (node != this->root && node->parent->color == Color::Red)
            {
                auto parent = node->parent;
                auto grandpa = parent->parent;
                auto uncle = (parent == grandpa->lchild)
                    ? grandpa->rchild
                    : grandpa->lchild;

                // Case 1: ����ڵ��Ǻ�ɫ
                if (uncle && uncle->color == Color::Red)
                {
                    parent->color = Color::Black;
                    uncle->color = Color::Black;
                    grandpa->color = Color::Red;
                    node = grandpa;
                }
                else
                {
                    // Case 2/3: ����ڵ��Ǻ�ɫ
                    if (parent == grandpa->lchild)
                    {
                        if (node == parent->rchild) // Case 2: LR��
                        {
                            rotate_left(parent);
                            node = parent;
                            parent = node->parent;
                        }
                        // Case 3: LL��
                        rotate_right(grandpa);
                    }
                    else
                    {
                        if (node == parent->lchild) // Case 2: RL��
                        {
                            rotate_right(parent);
                            node = parent;
                            parent = node->parent;
                        }
                        // Case 3: RR��
                        rotate_left(grandpa);
                    }
                    parent->color = Color::Black;
                    grandpa->color = Color::Red;
                }
            }
            this->root->color = Color::Black;
        }

        void fix_deletion(RBTreeNode<T>* node)
        {
            while (node != this->root && node->color == Color::Black)
            {
                auto parent = node->parent;
                auto sibling = (node == parent->lchild)
                    ? parent->rchild
                    : parent->lchild;

                if (sibling->color == Color::Red) // Case 1: �ֵ��Ǻ�ɫ
                {
                    sibling->color = Color::Black;
                    parent->color = Color::Red;
                    if (sibling == parent->rchild)
                        rotate_left(parent);
                    else
                        rotate_right(parent);
                }
                else // �ֵ��Ǻ�ɫ
                {
                    bool bothBlack = true;
                    if (sibling->lchild && sibling->lchild->color == Color::Red)
                        bothBlack = false;
                    if (sibling->rchild && sibling->rchild->color == Color::Red)
                        bothBlack = false;

                    if (bothBlack) // Case 2: �ֵܵ������ӽڵ㶼�Ǻ�ɫ
                    {
                        sibling->color = Color::Red;
                        if (parent->color == Color::Red)
                        {
                            parent->color = Color::Black;
                            break;
                        }
                        else
                        {
                            node = parent;
                        }
                    }
                    else // Case 3/4: �ֵ�������һ����ɫ�ӽڵ�
                    {
                        if (sibling == parent->rchild)
                        {
                            if (!sibling->rchild || sibling->rchild->color == Color::Black) // Case 3
                            {
                                sibling->lchild->color = Color::Black;
                                sibling->color = Color::Red;
                                rotate_right(sibling);
                                sibling = parent->rchild;
                            }
                            // Case 4
                            sibling->color = parent->color;
                            parent->color = Color::Black;
                            sibling->rchild->color = Color::Black;
                            rotate_left(parent);
                        }
                        else
                        {
                            if (!sibling->lchild || sibling->lchild->color == Color::Black)
                            {
                                sibling->rchild->color = Color::Black;
                                sibling->color = Color::Red;
                                rotate_left(sibling);
                                sibling = parent->lchild;
                            }
                            sibling->color = parent->color;
                            parent->color = Color::Black;
                            sibling->lchild->color = Color::Black;
                            rotate_right(parent);
                        }
                        break;
                    }
                }
            }
            node->color = Color::Black;
            this->root->color = Color::Black;
        }

        // ���ָ��ڵ���ȷ�Ե���ת����
        void rotate_left(RBTreeNode<T>*& node)
        {
            Base::rotate_left(node);
            node->parent = node->lchild->parent;
            node->lchild->parent = node;
        }

        void rotate_right(RBTreeNode<T>*& node)
        {
            Base::rotate_right(node);
            node->parent = node->rchild->parent;
            node->rchild->parent = node;
        }
    };
};