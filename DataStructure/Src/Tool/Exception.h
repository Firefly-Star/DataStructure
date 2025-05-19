#pragma once

#include <stdexcept>

namespace my_stl
{
	class iterator_out_of_range : public std::runtime_error
	{
		using Base = std::runtime_error;
		using Base::Base;
	};
}