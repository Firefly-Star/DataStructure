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
	double x;
};

int main() {
    std::initializer_list<int> x = { 1, 2, 3 };
}


