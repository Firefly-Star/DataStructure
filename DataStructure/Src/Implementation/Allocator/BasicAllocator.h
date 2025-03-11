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
	concept _Has_Allocator_Methods = requires(Allocator alloc, T const& value, T && other, size_t count, T* ptr)
	{
		{ alloc.Allocate() } -> std::same_as<T*>;
		{ alloc.Allocate(count) }->std::same_as<T*>;
		{ alloc.Allocate(count, *&value) }->std::same_as<T*>;
		{ alloc.Allocate(std::move(other)) } -> std::same_as<T*>;
		{ alloc.Allocate(*&value) } -> std::same_as<T*>;
		{ alloc.Deallocate(ptr, count) } -> std::same_as<void>;
	};

	template<typename T>
	class BasicAllocator
	{
	public:
		NODISCARD T* Allocate()
		{
			RawAllocator rawAllocator;
			T* ptr = (T*)rawAllocator.RawAllocate<alignof(T)>(sizeof(T));
			try
			{
				new (ptr) T();
			}
			catch (...)
			{
				rawAllocator.RawDeallocate(ptr);
				throw;
			}
			return ptr;
		}
		template<typename... Args>
		NODISCARD T* Allocate(size_t count, Args&&... args)
		{
			RawAllocator rawAllocator;
			T* ptr = (T*)rawAllocator.RawAllocate<alignof(T)>(sizeof(T) * count);
			size_t constructed = 0;
			try
			{
				for (size_t i = 0; i < count; ++i)
				{
					new (ptr + i) T(std::forward<Args>(args)...);
					++constructed;
				}
			}
			catch (...)
			{
				for (size_t i = 0; i < constructed; ++i)
				{
					(ptr + i)->~T();
				}
				rawAllocator.RawDeallocate(ptr);
				throw;
			}
			return ptr;
		}
		NODISCARD T* Allocate(size_t count, T const& value)
		{
			RawAllocator rawAllocator;
			T* ptr = (T*)rawAllocator.RawAllocate<alignof(T)>(sizeof(T) * count);
			size_t constructed = 0;
			try
			{
				for (size_t i = 0; i < count; ++i)
				{
					new (ptr + i) T(value);
					++constructed;
				}
			}
			catch (...)
			{
				for (size_t i = 0; i < constructed; ++i)
				{
					(ptr + i)->~T();
				}
				rawAllocator.RawDeallocate(ptr);
				throw;
			}
			return ptr;
		}
		NODISCARD T* Allocate(T&& other)
		{
			RawAllocator rawAllocator;
			T* ptr = (T*)rawAllocator.RawAllocate<alignof(T)>(sizeof(T));
			try
			{
				new (ptr) T(std::move(other));
			}
			catch (...)
			{
				rawAllocator.RawDeallocate(ptr);
				throw;
			}
			return ptr;
		}
		NODISCARD T* Allocate(T const& other)
		{
			RawAllocator rawAllocator;
			T* ptr = (T*)rawAllocator.RawAllocate<alignof(T)>(sizeof(T));
			try
			{
				new (ptr) T(other);
			}
			catch (...)
			{
				rawAllocator.RawDeallocate(ptr);
				throw;
			}
			return ptr;
		}
		void Deallocate(T* ptr, size_t count = 1)
		{
			RawAllocator rawAllocator;
			for (size_t i = 0; i < count; ++i)
			{
				(ptr + i)->~T();
			}
			rawAllocator.RawDeallocate(ptr);
		}
	};
}