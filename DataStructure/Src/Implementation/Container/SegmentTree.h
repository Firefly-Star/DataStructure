#pragma once

#include <vector>
#include "Tool/Aggregate.h"

namespace my_stl
{
	template<typename T, class Aggregate = plus<T>>
	class SegmentTree
	{
	public:
		struct SegmentTreeNode
		{ // [low, high)
			size_t low, high;
			T val;
		};
		template<typename Range = std::initializer_list<T>>
		SegmentTree(Range range = Range(), Aggregate agg = Aggregate())
			:agg(agg)
		{
			SegmentTree other(range.begin(), other.end(), agg);
			*this = other;
		}
		SegmentTree(std::initializer_list<T> list = {}, Aggregate agg = Aggregate())
			:agg(agg)
		{
			SegmentTree other(list.begin(), list.end(), agg);
			*this = other;
		}
		template<typename RanIt>
		SegmentTree(RanIt first, RanIt last, Aggregate agg = Aggregate())
			: agg(agg), data((last - first) * 4, { 0, 0, agg.identity() }), _size(last - first)
		{
			build(first, last, first, 0);
		}
		size_t size() const { return _size; }
		T query(size_t begin, size_t end)
		{
			if (end >= _size)
				throw std::runtime_error("index out of range");
			return query_impl(begin, end, 0);
		}
		void update(size_t pos, T val)
		{
			update_impl(pos, val, 0);
		}
	private:
		std::vector<SegmentTreeNode> data;
		Aggregate agg;
		size_t _size;
	private:
		template<typename RanIt>
		void build(RanIt first, RanIt last, RanIt begin, size_t index)
		{
			if (first == last)
				return;
			if (last - first == 1)
			{
				data[index] = { (size_t)(first - begin), (size_t)(last - begin), *first };
			}
			else
			{
				RanIt mid = first + (last - first) / 2;
				build(first, mid, begin, 2 * index + 1);
				build(mid, last, begin, 2 * index + 2);
				data[index] = { (size_t)(first - begin), (size_t)(last - begin), agg(data[2 * index + 1].val, data[2 * index + 2].val) };
			}
		}
		T query_impl(size_t begin, size_t end, size_t index)
		{
			if (begin == data[index].low && end == data[index].high)
			{
				return data[index].val;
			}
			else
			{
				size_t mid = (data[index].low + data[index].high) / 2;
				if (mid <= begin)
					return query_impl(mid, end, 2 * index + 2);
				else if (mid >= end)
					return query_impl(begin, mid, 2 * index + 1);
				else
					return agg(query_impl(begin, mid, 2 * index + 1), query_impl(mid, end, 2 * index + 2));
			}
		}
		void update_impl(size_t pos, T val, size_t index)
		{
			if (data[index].low == pos && data[index].high == pos + 1)
			{
				data[index].val = val;
				return;
			}
			size_t mid = (data[index].low + data[index].high) / 2;
			if (mid <= pos)
			{
				update_impl(pos, val, 2 * index + 2);
				data[index].val = agg(data[2 * index + 1].val, data[2 * index + 2].val);
			}
			else
			{
				update_impl(pos, val, 2 * index + 1);
				data[index].val = agg(data[2 * index + 1].val, data[2 * index + 2].val);
			}
		}
	};
}
