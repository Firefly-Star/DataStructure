#pragma once

#include <vector>
#include <numeric>

namespace my_stl
{
	class DSU
	{
	public:
		DSU(int n)
			:parent(n), rank(n, 1)
		{
			std::iota(parent.begin(), parent.end(), 0);
		}
		int Find(int i)
		{
			if (parent[i] != i)
			{
				parent[i] = Find(parent[i]);
			}
			return parent[i];
		}
		void Union(int x, int y)
		{
			int rootx = Find(x);
			int rooty = Find(y);
			if (rank[rootx] <= rank[rooty])
			{
				parent[rootx] = rooty;
				if (rank[rootx] == rank[rooty])
				{
					++rooty;
				}
			}
			else
			{
				parent[rooty] = rootx;
			}
		}
	private:
		std::vector<int> parent;
		std::vector<int> rank;
	};
}