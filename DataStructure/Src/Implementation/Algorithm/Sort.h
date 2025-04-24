#pragma once
#include "Tool/Compare.h"
#include "Heap.h"
#include <omp.h>
#include <thread>
#include <vector>

namespace my_stl
{
	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void bubble_sort(RanIt first, RanIt last, Comp comp = Comp())
	{
		if (first == last) 
			return;

		for (RanIt i = first; i != last; ++i)
		{
			bool is_ranked = true;
			for (RanIt j = last - 1; j >= i + 1; --j)
			{
				if (comp(*j, *(j - 1)))
				{
					std::swap(*j, *(j - 1));
					is_ranked = false;
				}
			}
			if (is_ranked)
				return;
		}
	}

	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void selection_sort(RanIt first, RanIt last, Comp comp = Comp())
	{
		for (RanIt unordered = first; unordered != last; ++unordered)
		{
			RanIt selected = unordered;
			for (RanIt work = unordered + 1; work != last; ++work)
			{
				if (comp(*work, *selected))
				{
					selected = work;
				}
			}
			std::swap(*unordered, *selected);
		}
	}

	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void insertion_sort(RanIt first, RanIt last, Comp comp = Comp())
	{
		for (RanIt unordered = first; unordered != last; ++unordered)
		{
			RanIt to_insert = unordered;
			for (RanIt ordered = first; ordered != unordered; ++ordered)
			{
				if (comp(*to_insert, *ordered))
				{
					for (RanIt to_move = unordered; to_move != ordered; --to_move)
					{
						std::swap(*to_move, *(to_move - 1));
					}
					break;
				}
			}
		}
	}
	
	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void inplace_merge_sort(RanIt first, RanIt last, Comp comp = Comp())
	{
		if (first == last || first + 1 == last)
			return;
		RanIt mid = first + (last - first) / 2;
		inplace_merge_sort(first, mid, comp);
		inplace_merge_sort(mid, last, comp);
		
		// merge
		RanIt left = first;
		RanIt right = mid;
		while (left < right && right != last)
		{
			while (left < right && comp(*left, *right)) {
				++left;
			}
			RanIt index = right;
			while (right < last && comp(*right, *left)) {
				++right;
			}
			std::reverse(left, index);
			std::reverse(index, right);
			std::reverse(left, right);
			left += right - index;
		}
	}

	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void merge_sort(RanIt first, RanIt last, Comp comp = Comp())
	{
		using value_type = typename RanIt::value_type;
		using difference_type = typename RanIt::difference_type;
		if (first == last || first + 1 == last)
			return;
		RanIt mid = first + (last - first) / 2;
		merge_sort(first, mid, comp);	// [first, mid)
		merge_sort(mid, last, comp);	// [mid, last)
		std::vector<value_type> tmp;
		tmp.reserve(last - first);
		RanIt left = first;
		RanIt right = mid;
		while (left != mid && right != last)
		{
			if (comp(*left, *right))
			{
				tmp.push_back(*left);
				++left;
			}
			else
			{
				tmp.push_back(*right);
				++right;
			}
		}
		while (left != mid)
		{
			tmp.push_back(*left);
			++left;
		}
		while (right != last)
		{
			tmp.push_back(*right);
			++right;
		}
		for (difference_type i = 0; i != last - first; ++i)
		{
			*(first + i) = tmp[i];
		}
	}

	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void quick_sort(RanIt first, RanIt last, Comp comp = Comp())
	{
		if (first == last || first + 1 == last)
			return;
		RanIt mid = first + (last - first) / 2;
		[&](RanIt x, RanIt y, RanIt z)
			{ // let y be the median
				if (comp(*x, *z))
					std::swap(*x, *z); // now x ">=" z
				if (comp(*y, *z))
					std::swap(*y, *z); // now y ">=" z
				if (comp(*x, *y))
					std::swap(*x, *y); // now x ">=" y 
			}(first, mid, last - 1);
		std::swap(*mid, *first);
		RanIt pivot = first;
		RanIt i = first;
		
		for (RanIt work = first; work != last; ++work)
		{
			if (comp(*work, *pivot))
			{
				++i;
				std::swap(*work, *i);
			}
		}
		std::swap(*i, *pivot);
		quick_sort(first, i);
		quick_sort(i + 1, last);
	}

	template<typename RanIt, class Comp = less<typename RanIt::value_type>>
	void heap_sort(RanIt first, RanIt last, Comp comp = Comp())
	{
		my_stl::make_heap(first, last, comp);
		for (; last > first; --last)
		{
			my_stl::pop_heap(first, last, comp);
		}
	}
}