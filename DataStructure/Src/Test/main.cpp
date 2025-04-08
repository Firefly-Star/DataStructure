#include "Tree.h"
#include <set>
#include <functional>
#include "crtdbg.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	my_stl::AVLTree<int> t = { 1,2,3,4,5 };
	t.insert(7);
	t.erase(4);
	t.erase(10);
}