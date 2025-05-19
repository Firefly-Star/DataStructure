#pragma once
#include "Tool/Compare.h"

namespace my_stl
{
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
		AVLTree(Compare comp = Compare())
			:comp(comp), root(nullptr){ }
		~AVLTree()
		{

		}
		void insert(T t)
		{
			_insert_with_func(t, root, [](AVLTreeNode* node){});
		}
		template<typename Func>
		void insert_with_func(T t, Func func)
		{
			_insert_with_func(t, root, func);
		}
		void erase(T t)
		{
			_erase_with_func(t, root, [](AVLTreeNode* node){});
		}
		template<typename Func>
		void erase_with_func(T t, Func func)
		{
			_erase_with_func(t, root, func);
		}
		int count(T t)
		{
			AVLTreeNode* node = root;
			while (node != nullptr)
			{
				if (comp(node->data, t))
					node = node->right;
				else if (comp(t, node->data))
					node = node->left;
				else
					return 1;
			}
			return 0;
		}
		AVLTreeNode* data()
		{
			return root;
		}
	private:
		Compare comp;
		AVLTreeNode* root;
	private:
		void copy(AVLTree const& other)
		{

		}
		void swap(AVLTree&& other)
		{
		}
		static int height(AVLTreeNode* node)
		{
			return node == nullptr ? 0 : node->height;
		}
		static void update_height(AVLTreeNode* node)
		{
			node->height = std::max(height(node->left), height(node->right)) + 1;
		}
		void LL(AVLTreeNode*& node)
		{
			AVLTreeNode* A = node;
			AVLTreeNode* B = node->left;
			AVLTreeNode* BR = B->right;
			A->left = BR;
			A->height -= 2;
			B->right = A;
			node = B;
		}
		void RR(AVLTreeNode*& node)
		{
			AVLTreeNode* A = node;
			AVLTreeNode* B = node->right;
			AVLTreeNode* BL = B->left;
			A->right = BL;
			A->height -= 2;
			B->left = A;
			node = B;
		}
		void RL(AVLTreeNode*& node)
		{
			LL(node->right);
			RR(node);
		}
		void LR(AVLTreeNode*& node)
		{
			RR(node->left);
			LL(node);
		}
		template<typename Func>
		void _insert_with_func(T t, AVLTreeNode*& node, Func func)
		{
			if (node == nullptr)
			{
				node = new AVLTreeNode{ t, nullptr, nullptr, 1 };
				func(node);
				return;
			}
			if (!comp(t, node->data) && !comp(node->data, t))
				return;
			if (comp(t, node->data))
			{
				_insert_with_func(t, node->left, func);
				update_height(node);
				func(node);
				if (height(node->left) == height(node->right) + 2)
				{
					if (height(node->left->left) == height(node->left->right) + 1)
					{
						LL(node);
					}
					else
					{
						LR(node);
					}
				}
			}
			else
			{
				_insert_with_func(t, node->right, func);
				update_height(node);
				func(node);
				if (height(node->right) > height(node->left) + 1)
				{
					if (height(node->right->left) == height(node->right->right) + 1)
					{
						RL(node);
					}
					else
					{
						RR(node);
					}
				}
			}
		}
		template<typename Func>
		void _erase_with_func(T t, AVLTreeNode*& node, Func func)
		{
			if (node == nullptr)
				return;
			if (comp(node->data, t))
			{
				_erase_with_func(t, node->right, func);
			}
			else if (comp(t, node->data))
			{
				_erase_with_func(t, node->left, func);
			}
			else
			{
				if (node->left == nullptr && node->right == nullptr)
				{
					delete node;
					node = nullptr;
				}
				else if (node->left == nullptr && node->right != nullptr)
				{
					AVLTreeNode* R = node->right;
					delete node;
					node = R;
				}
				else if (node->left != nullptr && node->right == nullptr)
				{
					AVLTreeNode* L = node->left;
					delete node;
					node = L;
				}
				else
				{
					// 删除右子树的最左结点或左子树的最右结点
					AVLTreeNode* next = node->right;
					while (next->left != nullptr)
					{
						next = next->left;
					}
					node->data = next->data;
					_erase_with_func(next->data, node->right, func);
				}
			}

			if (node == nullptr)
				return;
			update_height(node);
			func(node);
			if (height(node->left) > height(node->right) + 1)
			{
				if (height(node->left->left) > height(node->left->right))
					LL(node);
				else
					LR(node);
			}
			else if (height(node->left) < height(node->right) - 1)
			{
				if (height(node->right->left) > height(node->right->right))
					RL(node);
				else
					RR(node);
			}
		}

	};
}