#pragma once

#include <type_traits>

namespace my_stl
{
	template<typename Container>
	class Enumerate_t
	{
	public:
		struct Iterator
		{
			size_t index;
			typename Container::iterator_type it;
			Iterator& operator++() { ++index; ++it; return *this; }
			Iterator operator++(int) { Iterator other(*this); ++(*this); return other; }
			bool operator==(Iterator const& other) const { return index == other.index && it == other.it; }
			bool operator!=(Iterator const& other) const { return !(*this == other); }
		};
		Enumerate_t(Container&& c)
			:container(std::forward<Container>(c))
		{}
		Iterator begin() { return { 0, c.begin() }; }
		Iterator end() { return { container.size(), end() }; }
	private:
		Container container;
	};

	template<typename Container>
	Enumerate_t<Container> enumeric(Container&& c) { return Enumeric_t(std::forward<Container>(c)); }
}