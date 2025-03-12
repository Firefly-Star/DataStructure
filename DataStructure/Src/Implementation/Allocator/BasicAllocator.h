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

	template<typename T>
	class BasicAllocator
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using size_type = size_t;

	public:
		NODISCARD pointer allocate(size_type count)
		{
			RawAllocator rawAllocator;
			pointer ptr = (pointer)rawAllocator.RawAllocate<alignof(value_type)>(sizeof(value_type) * count);
			return ptr;
		}
		void deallocate(pointer ptr, size_type count = 1)
		{
			RawAllocator rawAllocator;
			rawAllocator.RawDeallocate(ptr);
		}
	};

	template<typename Allocator>
	class allocator_traits
	{
	public:
		using value_type = typename decltype([]() {
			if constexpr (requires { typename Allocator::value_type; })
			{
				return std::type_identity<typename Allocator::value_type>{};
			}
			else
			{
				return std::type_identity<char>{};
			}
			}())::type;
		using pointer = typename decltype([]() {
			if constexpr (requires { typename Allocator::pointer; })
			{
				return std::type_identity<typename Allocator::pointer>{};
			}
			else
			{
				return std::type_identity<value_type*>{};
			}
			}())::type;
		using const_pointer = typename decltype([]() {
			if constexpr (requires { typename Allocator::const_pointer; })
			{
				return std::type_identity<typename Allocator::const_pointer>{};
			}
			else
			{
				return std::type_identity<const value_type*>{};
			}
			}())::type;
		using size_type = typename decltype([]() {
			if constexpr (requires { typename Allocator::size_type; })
			{
				return std::type_identity<typename Allocator::size_type>{};
			}
			else
			{
				return std::type_identity<size_t>{};
			}
			}())::type;
		
	public:
		static pointer allocate(Allocator& alloc, size_type count)
		{
			if constexpr (requires { {alloc.allocate(count)} -> std::same_as<pointer>; })
			{
				return alloc.allocate(count);
			}
			else
			{
				RawAllocator rawAllocator;
				return rawAllocator.RawAllocate(count * sizeof(value_type));
			}
		}
		static void deallocate(Allocator& alloc, pointer ptr, size_type count = 1)
		{
			if constexpr (requires { {alloc.deallocate(ptr, count)} -> std::same_as<void>; })
			{
				alloc.deallocate(ptr, count);
			}
			else
			{
				RawAllocator rawAllocator;
				rawAllocator.RawDeallocate(ptr);
			}
		}
		static size_type max_size(Allocator const& alloc) noexcept
		{
			if constexpr (requires { {alloc.max_size()} -> std::same_as<size_type>; })
			{
				return alloc.max_size();
			}
			else
			{
				return std::numeric_limits<size_type>::max() / sizeof(value_type);
			}
		}
		template<typename... Args>
		static void construct(Allocator& alloc, pointer ptr, Args&&... args)
		{
			if constexpr (requires { alloc.construct(ptr, std::forward<Args>(args)...); })
			{
				alloc.construct(ptr, std::forward<Args>(args)...);
			}
			else
			{
				new(ptr) value_type(std::forward<Args>(args)...);
			}
		}
		static void destroy(Allocator& alloc, pointer ptr)
		{
			if constexpr (requires { alloc.destroy(ptr); })
			{
				alloc.destroy(ptr);
			}
			else
			{
				ptr->~value_type();
			}
		}
	};
}