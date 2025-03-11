#pragma once

#include <cstdlib>
#include <memory>
#include <iostream>

#include "Tool/Macros.h"

namespace my_stl
{
	template<size_t N>
	concept PowerOf2 = (N != 0) && ((N & (N - 1)) == 0);

	// TODO: 在dataptr之前加入哨兵值，在释放时检查哨兵值是否被修改
	class RawAllocator
	{
	public:
		template<size_t alignment> requires PowerOf2<alignment>
		NODISCARD void* RawAllocate(size_t size)
		{
			size_t space = size + alignment - 1;
			void* originalptr = malloc(space + sizeof(uintptr_t));
			if (originalptr == nullptr)
			{
				throw std::bad_alloc();
			}
			void* dataptr = (char*)originalptr + sizeof(uintptr_t);
			std::align(alignment, size, dataptr, space);
			*((uintptr_t*)dataptr - 1) = (uintptr_t)originalptr;
			return dataptr;
		}
		void RawDeallocate(void* ptr)
		{
			if (!ptr) return;
			void* originalptr = (void*)*((uintptr_t*)ptr - 1);
			free(originalptr);
		}
	};

	template<typename Allocator, typename T>
	concept _Has_Allocator_Methods = requires(Allocator alloc, T const& value, T && other, size_t count, T* ptr, std::initializer_list<T> list)
	{
		{ alloc.Allocate(count) }->std::same_as<T*>;
		{ alloc.Deallocate(ptr, count) } -> std::same_as<void>;
	};

	template<typename T>
	class BasicAllocator
	{
	public:
		NODISCARD T* Allocate(size_t count)
		{
			RawAllocator rawAllocator;
			T* ptr = (T*)rawAllocator.RawAllocate<alignof(T)>(sizeof(T) * count);
			return ptr;
		}
		void Deallocate(T* ptr, size_t count = 1)
		{
			RawAllocator rawAllocator;
			rawAllocator.RawDeallocate(ptr);
		}
	};
}