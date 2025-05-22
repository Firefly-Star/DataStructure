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
#include "Implementation/Container/BSTree.h"
#include "Implementation/Container/RBTree.h"
using namespace std;

#include "Tool/Exception.h"

class X
{
protected:
	void f() { std::cout << "x\n"; }
};

class Y : public X
{
public:
	void y() { X::f(); }
};

template<typename T>
class A
{
	static_assert(false, "unsupported param.");
};

template<>
class A<int>
{

};
template<>
class A<double>
{

};


int main()
{
	my_stl::RBTree<int> tree;
	for (int i = 0; i < 10; ++i)
	{
		tree.insert(i);
	}
	for (auto i : tree)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";
	for (auto i : tree | my_stl::lrn)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";
	for (auto i : tree | my_stl::nlr)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";
}