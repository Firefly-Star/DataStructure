#pragma once
#include <functional>
#include "Tool/Compare.h"

namespace my_stl
{
	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	bool heapify(RanIt& node, const RanIt first, const RanIt last, Comp comp = Comp())
	{
		if ((node - first) * 2 + 1 >= last - first)
			return false;
		auto lchild = first + (node - first) * 2 + 1;
		RanIt max = lchild;
		if ((node - first) * 2 + 2 < last - first)
		{
			auto rchild = first + (node - first) * 2 + 2;
			if (comp(*lchild, *rchild))
				max = rchild;
		}
		if (comp(*node, *max))
		{
			std::swap(*node, *max);
			node = max;
			return true;
		}
		return false;
	}

	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void sink_down(RanIt node, const RanIt first, const RanIt last, Comp comp = Comp())
	{
		while (true)
		{
			if ((node - first) * 2 + 1 >= last - first)
				return;
			auto lchild = first + (node - first) * 2 + 1;
			RanIt max = lchild;
			if ((node - first) * 2 + 2 < last - first)
			{
				auto rchild = first + (node - first) * 2 + 2;
				if (comp(*lchild, *rchild))
					max = rchild;
			}
			if (comp(*node, *max))
			{
				std::swap(*node, *max);
				node = max;
			}
			else
			{
				return;
			}
		}
	}
	

	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void push_heap(RanIt first, RanIt last, Comp comp = Comp())
	{
		if (last - first < 2)
			return;
		RanIt node = last - 1;
		while (true)
		{
			auto offset = node - first;
			RanIt parent = first + (offset - 1) / 2;
			if (comp(*parent, *node))
			{
				std::swap(*parent, *node);
				node = parent;
			}
			else
			{
				break;
			}
		}
	}

	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void pop_heap(RanIt first, RanIt last, Comp comp = Comp())
	{
		if (last - first < 2)
			return;
		std::swap(*first, *(last - 1));
		RanIt node = first;
		RanIt newlast = last - 1;
		sink_down(node, first, newlast, comp);
	}

	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void make_heap(RanIt first, RanIt last, Comp comp = Comp())
	{
		if (last <= first + 1)
			return;
		for (RanIt node = first + (last - first) / 2 - 1; node > first; --node)
		{
			sink_down(node, first, last, comp);
		}
		RanIt node = first;
		sink_down(first, first, last, comp);
	}

}