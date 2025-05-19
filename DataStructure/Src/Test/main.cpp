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
#include <queue>
#include <unordered_set>
// #include <span>
#include <set>
// #include <ranges>
#include <random>
#include <map>
#include <chrono>
#include <future>
#include <iterator>
#include <stack>
//#include "Implementation/Algorithm/Sort.h"
//#include "Implementation/Algorithm/Heap.h"
//#include "Tool/Measure.h"
//#include "Tool/Memorizer.h"
#include "Implementation/Container/AVLTree.h"
#include "Implementation/Container/LBiTree.h"
using namespace std;

using node = my_stl::AVLTree<int>::AVLTreeNode*;

int f()
{
	if (1 != 2)
		std::abort();
}
#include "Tool/Exception.h"
int main()
{
	//my_stl::AVLTree<int> tree;
	//std::cout << tree.count(62);
	my_stl::LBiTree<int> tree({ 1,2,3,nullopt, 4,5 });
	for (auto it = tree.lnr_begin(); it != tree.lnr_end(); ++it)
	{
		std::cout << *it << " ";
	}
}