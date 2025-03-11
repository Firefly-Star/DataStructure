#pragma once

#include "Tool/Macros.h"
#include "Implementation/Iterator/IteratorTag.h"
#include "Implementation/Allocator/BasicAllocator.h"

namespace my_stl
{
	template<typename T, class Allocator = BasicAllocator<T>>
		requires _Has_Allocator_Methods<Allocator, T>
	class vector
	{
	public:
		// TODO: 在调试配置下进行越界判断
		class iterator : public random_access_iterator_t<iterator>
		{
		public:
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using difference_type = std::ptrdiff_t;
		public:
			iterator(pointer ptr) :m_Current(ptr){}

			iterator(iterator const& other) :m_Current(other.m_Current){}
			
			reference operator*() { return *m_Current; }
			iterator& operator++() { ++m_Current; return *this;	}
			iterator& operator--() { --m_Current; return *this; }
			iterator operator++(int) { iterator tmp = *this; ++m_Current; return tmp;	}
			iterator operator--(int) { iterator tmp = *this; --m_Current; return tmp; }
			reference operator[](difference_type n) const { return m_Current[n]; }
			iterator& operator+=(difference_type n) { m_Current += n; return *this; }
			iterator& operator-=(difference_type n) { m_Current -= n; return *this; }

			friend bool operator==(iterator const& lhs, iterator const& rhs) { return (lhs.m_Current == rhs.m_Current); }
			friend bool operator!=(iterator const& lhs, iterator const& rhs) { return (lhs.m_Current != rhs.m_Current); }
			friend iterator operator+(iterator lhs, difference_type n) { lhs.m_Current += n; return lhs; }
			friend iterator operator+(difference_type n, iterator rhs) { rhs.m_Current += n; return rhs; }
			friend iterator operator-(iterator lhs, difference_type n) { lhs.m_Current -= n; return lhs; }
			friend difference_type operator-(iterator lhs, iterator rhs) { return lhs.m_Current - rhs.m_Current; }
			friend bool operator<(iterator const& lhs, iterator const& rhs) { return lhs.m_Current < rhs.m_Current; }
			friend bool operator<=(iterator const& lhs, iterator const& rhs) { return lhs.m_Current <= rhs.m_Current; }
			friend bool operator>(iterator const& lhs, iterator const& rhs) { return lhs.m_Current > rhs.m_Current; }
			friend bool operator>=(iterator const& lhs, iterator const& rhs) { return lhs.m_Current >= rhs.m_Current; }
		private:
			pointer m_Current;
		};

		class const_iterator : public random_access_iterator_t<const_iterator>
		{
		public:
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using difference_type = std::ptrdiff_t;
		public:
			const_iterator(pointer ptr) :m_Current(ptr) {}

			const_iterator(const_iterator const& other) :m_Current(other.m_Current) {}

			reference operator*() { return *m_Current; }
			const_iterator& operator++() { ++m_Current; return *this; }
			const_iterator& operator--() { --m_Current; return *this; }
			const_iterator operator++(int) { const_iterator tmp = *this; ++m_Current; return tmp; }
			const_iterator operator--(int) { const_iterator tmp = *this; --m_Current; return tmp; }
			reference operator[](difference_type n) const { return m_Current[n]; }
			const_iterator& operator+=(difference_type n) { m_Current += n; return *this; }
			const_iterator& operator-=(difference_type n) { m_Current -= n; return *this; }

			friend bool operator==(const_iterator const& lhs, const_iterator const& rhs) { return (lhs.m_Current == rhs.m_Current); }
			friend bool operator!=(const_iterator const& lhs, const_iterator const& rhs) { return (lhs.m_Current != rhs.m_Current); }
			friend const_iterator operator+(const_iterator lhs, difference_type n) { lhs.m_Current += n; return lhs; }
			friend const_iterator operator+(difference_type n, const_iterator rhs) { rhs.m_Current += n; return rhs; }
			friend const_iterator operator-(const_iterator lhs, difference_type n) { lhs.m_Current -= n; return lhs; }
			friend difference_type operator-(const_iterator lhs, const_iterator rhs) { return lhs.m_Current - rhs.m_Current; }
			friend bool operator<(const_iterator const& lhs, const_iterator const& rhs) { return lhs.m_Current < rhs.m_Current; }
			friend bool operator<=(const_iterator const& lhs, const_iterator const& rhs) { return lhs.m_Current <= rhs.m_Current; }
			friend bool operator>(const_iterator const& lhs, const_iterator const& rhs) { return lhs.m_Current > rhs.m_Current; }
			friend bool operator>=(const_iterator const& lhs, const_iterator const& rhs) { return lhs.m_Current >= rhs.m_Current; }
		private:
			pointer m_Current;
		};

		class reverse_iterator : public random_access_iterator_t<reverse_iterator>
		{
		public:
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using difference_type = std::ptrdiff_t;
		public:
			reverse_iterator(pointer ptr) :m_Current(ptr) {}

			reverse_iterator(reverse_iterator const& other) :m_Current(other.m_Current) {}

			reference operator*() { return *m_Current; }
			reverse_iterator& operator++() { --m_Current; return *this; }
			reverse_iterator& operator--() { ++m_Current; return *this; }
			reverse_iterator operator++(int) { reverse_iterator tmp = *this; --m_Current; return tmp; }
			reverse_iterator operator--(int) { reverse_iterator tmp = *this; ++m_Current; return tmp; }
			reference operator[](difference_type n) const { return m_Current[-n]; }
			reverse_iterator& operator+=(difference_type n) { m_Current -= n; return *this; }
			reverse_iterator& operator-=(difference_type n) { m_Current += n; return *this; }

			friend bool operator==(reverse_iterator const& lhs, reverse_iterator const& rhs) { return (lhs.m_Current == rhs.m_Current); }
			friend bool operator!=(reverse_iterator const& lhs, reverse_iterator const& rhs) { return (lhs.m_Current != rhs.m_Current); }
			friend reverse_iterator operator+(reverse_iterator lhs, difference_type n) { lhs.m_Current -= n; return lhs; }
			friend reverse_iterator operator+(difference_type n, reverse_iterator rhs) { rhs.m_Current -= n; return rhs; }
			friend reverse_iterator operator-(reverse_iterator lhs, difference_type n) { lhs.m_Current += n; return lhs; }
			friend difference_type operator-(reverse_iterator lhs, reverse_iterator rhs) { return lhs.m_Current + rhs.m_Current; }
			friend bool operator<(reverse_iterator const& lhs, reverse_iterator const& rhs) { return lhs.m_Current > rhs.m_Current; }
			friend bool operator<=(reverse_iterator const& lhs, reverse_iterator const& rhs) { return lhs.m_Current >= rhs.m_Current; }
			friend bool operator>(reverse_iterator const& lhs, reverse_iterator const& rhs) { return lhs.m_Current < rhs.m_Current; }
			friend bool operator>=(reverse_iterator const& lhs, reverse_iterator const& rhs) { return lhs.m_Current <= rhs.m_Current; }
		private:
			pointer m_Current;
		};

		class const_reverse_iterator : public random_access_iterator_t<const_reverse_iterator>
		{
		public:
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using difference_type = std::ptrdiff_t;
		public:
			const_reverse_iterator(pointer ptr) :m_Current(ptr) {}

			const_reverse_iterator(const_reverse_iterator const& other) :m_Current(other.m_Current) {}

			reference operator*() { return *m_Current; }
			const_reverse_iterator& operator++() { --m_Current; return *this; }
			const_reverse_iterator& operator--() { ++m_Current; return *this; }
			const_reverse_iterator operator++(int) { const_reverse_iterator tmp = *this; --m_Current; return tmp; }
			const_reverse_iterator operator--(int) { const_reverse_iterator tmp = *this; ++m_Current; return tmp; }
			reference operator[](difference_type n) const { return m_Current[-n]; }
			const_reverse_iterator& operator+=(difference_type n) { m_Current -= n; return *this; }
			const_reverse_iterator& operator-=(difference_type n) { m_Current += n; return *this; }

			friend bool operator==(const_reverse_iterator const& lhs, const_reverse_iterator const& rhs) { return (lhs.m_Current == rhs.m_Current); }
			friend bool operator!=(const_reverse_iterator const& lhs, const_reverse_iterator const& rhs) { return (lhs.m_Current != rhs.m_Current); }
			friend const_reverse_iterator operator+(const_reverse_iterator lhs, difference_type n) { lhs.m_Current -= n; return lhs; }
			friend const_reverse_iterator operator+(difference_type n, const_reverse_iterator rhs) { rhs.m_Current -= n; return rhs; }
			friend const_reverse_iterator operator-(const_reverse_iterator lhs, difference_type n) { lhs.m_Current += n; return lhs; }
			friend difference_type operator-(const_reverse_iterator lhs, const_reverse_iterator rhs) { return lhs.m_Current + rhs.m_Current; }
			friend bool operator<(const_reverse_iterator const& lhs, const_reverse_iterator const& rhs) { return lhs.m_Current > rhs.m_Current; }
			friend bool operator<=(const_reverse_iterator const& lhs, const_reverse_iterator const& rhs) { return lhs.m_Current >= rhs.m_Current; }
			friend bool operator>(const_reverse_iterator const& lhs, const_reverse_iterator const& rhs) { return lhs.m_Current < rhs.m_Current; }
			friend bool operator>=(const_reverse_iterator const& lhs, const_reverse_iterator const& rhs) { return lhs.m_Current <= rhs.m_Current; }
		private:
			pointer m_Current;
		};

	public:
		vector(Allocator const& alloc = Allocator())
			:m_Allocator(alloc), m_Data(nullptr), m_Capability(0), m_Size(0) {}
		vector(size_t count, Allocator const& alloc = Allocator())
			:m_Allocator(alloc), m_Data(m_Allocator.Allocate(count)), m_Capability(count), m_Size(count) {}
		vector(size_t count, T const& value, Allocator const& alloc = Allocator())
			:m_Allocator(alloc), m_Data(m_Allocator.Allocate(count, value)), m_Capability(count), m_Size(count)	{}
		~vector(){ m_Allocator.Deallocate(m_Data, m_Capability); }
		vector(vector const& other, Allocator const& alloc = Allocator()){}

		iterator begin() { return iterator(m_Data); }
		iterator end() { return iterator(m_Data + m_Size); }
		const_iterator cbegin() { return const_iterator(m_Data); }
		const_iterator cend() { return const_iterator(m_Data + m_Size); }
		reverse_iterator rbegin() { return reverse_iterator(m_Data + m_Size - 1); }
		reverse_iterator rend() { return reverse_iterator(m_Data - 1); }
		const_reverse_iterator crbegin() { return const_reverse_iterator(m_Data + m_Size - 1); }
		const_reverse_iterator crend() { return const_reverse_iterator(m_Data - 1); }


	private:
		Allocator m_Allocator;
		T* m_Data;
		size_t m_Capability, m_Size;

	private:
		void copy_from(vector const& other)
		{
		}
		void swap(vector& other)
		{

		}
	};
}