#pragma once

#include "Tool/Compare.h"

namespace my_stl
{
	template<typename T, class Pred = less<T>>
	class RBTree
	{
	public:
		enum class Color{ Black, Red };
		struct RBTreeNode
		{
			T data;
			RBTreeNode* left, right;
			Color color;
		};
		RBTree(Pred pred)
			:pred(pred), root(nullptr){ }
		~RBTree()
		{

		}
	private:
		RBTreeNode* root;
		Pred pred;
	private:
		void insert(T val, RBTreeNode*& parent, RBTreeNode*& cur)
		{
			if (cur == nullptr)
			{
				cur = new RBTreeNode({ val, nullptr, nullptr, Color::Red });
				if (parent != nullptr && parent->color == Color::Red)
			}
		}
	};
}