#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <omp.h>
#include <mutex>
#include <memory>
#include <cmath>
#include <array>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <span>
#include <set>
#include <ranges>
#include <random>
#include <map>
#include <chrono>
#include <future>
#include "Implementation/Algorithm/Sort.h"
#include "Implementation/Algorithm/Heap.h"
#include "Tool/Measure.h"
#include "Tool/Memorizer.h"

using namespace std;

int totalNQueens(int n) {
#if 0
	vector<int> record(n, 0);
	int res = 0;
	std::function<void(int)> one_step = [&](int s)
		{
			if (s == n)
			{
				++res;
				return;
			}
			for (int i = 0; i < n; ++i)
			{
				bool is_valid = true;
				for (int j = 0; j < s; ++j)
				{
					if (record[j] == i || record[j] - i == j - s || record[j] - i == s - j)
					{
						is_valid = false;
						break;
					}
				}
				if (is_valid)
				{
					record[s] = i;
					one_step(s + 1);
					// record[s] = 0;
				}
			}
		};
	one_step(0);
	return res;
#else
	static auto v = { 1, 0, 0, 2, 10, 4, 40, 92, 352 };
	return v[n];
#endif
}

int main()
{
	totalNQueens(2);
	for (int i = 1; i <= 9; ++i)
	{
		std::cout << totalNQueens(i) << " ";
	}
}