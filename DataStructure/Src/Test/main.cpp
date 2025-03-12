#include <utility>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ranges>

#include <crtdbg.h>
#include "Implementation/Allocator/BasicAllocator.h"
#include "Implementation/Container/Vector.h"
#include "Tool/Macros.h"

class alignas(1024) A
{
public:
	A(double y):x(y){}
	A(A&& other) noexcept :x(other.x) { other.x = 0.0; std::cout << "Move"; }
	A(A const& other) :x(other.x) { std::cout << "Copy"; }
	~A() { std::cout << "Destruct " << x << "!\n"; }
	double x;
};

class B
{
public:
	B(std::initializer_list<double> list, int x_ = 10)
		:y(x_)
	{
		memcpy(x, list.begin(), list.size() * sizeof(double));
	}
	B(B const&) { std::cout << "Copy Construct\n"; }
	B(B &&) { std::cout << "Move Construct\n"; }
	B& operator=(B const&) { std::cout << "Copy\n"; }
	B& operator=(B &&) { std::cout << "Move\n"; }
	~B() { std::cout << "Destruct\n"; }
	double x[10];
	int y;
};

class T
{
public:
	using value_type = A;
	using pointer_type = double;
	using const_pointer_type = double;
	using size_type = int;
};

template<typename T>
class Traits
{
private:
public:
	using value_type = typename decltype([](){
		if constexpr (requires{ typename T::value_type; })
		{
			return std::type_identity<typename T::value_type>{};
		}
		else
		{
			return std::type_identity<char>{};
		}
		}())::type;
};

using namespace my_stl;

int main() {
	using vt = Traits<A>::value_type;
	using vt2 = Traits<T>::value_type;
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
	std::vector<double> v;
	vector<double> v2 = { 1.0, 3.0, 6.43 };
	std::cout << std::hex << v.max_size() << "\n";
	std::cout << std::hex << v2.max_size() << "\n";
	for (auto i : v2)
	{
		std::cout << i << " ";
	}
}
