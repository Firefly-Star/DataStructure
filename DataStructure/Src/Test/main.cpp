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

class alignas(256) A
{
public:
	A(double y):x(y){}
	A(A&& other) noexcept :x(other.x) { other.x = 0.0; std::cout << "Move"; }
	A(A const& other) :x(other.x) { std::cout << "Copy"; }
	~A() { std::cout << "Destruct " << x << "!\n"; }
	double x;
};

int main() {
	my_stl::BasicAllocator<A> alloc;
}


