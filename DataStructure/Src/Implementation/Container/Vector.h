#pragma once

#include "Tool/Macros.h"
#include "Implementation/Iterator/IteratorTag.h"
#include "Implementation/Allocator/BasicAllocator.h"

namespace my_stl
{
	template<typename T, class Allocator = BasicAllocator<T>>
		requires std::same_as<T, typename allocator_traits<Allocator>::value_type>
	class vector
	{
	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = allocator_traits<Allocator>::pointer;
		using const_pointer = allocator_traits<Allocator>::const_pointer;

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
		vector(allocator_type const& alloc = allocator_type())
			:m_Allocator(alloc), m_Data(nullptr), m_Capacity(0), m_Size(0) 
		{}
		vector(size_type count, allocator_type const& alloc = allocator_type())
			:m_Allocator(alloc), m_Data(allocator_traits<allocator_type>::allocate(m_Allocator, count)), m_Capacity(count), m_Size(count) 
		{
			size_type constructed = 0;
			try
			{
				for (; constructed < count; ++constructed)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + constructed);
				}
			}
			catch (...)
			{
				for (size_type i = 0; i < constructed; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
				}
				allocator_traits<allocator_type>::deallocate(m_Allocator, m_Data, m_Capacity);
				throw;
			}
		}
		vector(size_type count, const_reference value, allocator_type const& alloc = allocator_type())
			:m_Allocator(alloc), m_Data(allocator_traits<allocator_type>::allocate(m_Allocator, count)), m_Capacity(count), m_Size(count)
		{
			size_type constructed = 0;
			try
			{
				for (; constructed < count; ++constructed)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + constructed, value);
				}
			}
			catch (...)
			{
				for (size_type i = 0; i < constructed; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
				}
				allocator_traits<allocator_type>::deallocate(m_Allocator, m_Data, m_Capacity);
				throw;
			}
		}
		vector(std::initializer_list<T> list, allocator_type const& alloc = allocator_type())
			:m_Allocator(alloc), m_Data(allocator_traits<allocator_type>::allocate(m_Allocator, list.size())), m_Capacity(list.size()), m_Size(list.size())
		{
			int constructed = 0;
			try
			{
				for (; constructed < list.size(); ++constructed)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + constructed, (list.begin())[constructed]);
				}
			}
			catch (...)
			{
				for (size_type i = 0; i < constructed; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
				}
				allocator_traits<allocator_type>::deallocate(m_Allocator, m_Data, m_Capacity);
				throw;
			}
		}
		
		~vector() 
		{ 
			for (size_type i = 0; i < m_Size; ++i)
			{
				allocator_traits<Allocator>::destroy(m_Allocator, m_Data + i);
			}
			allocator_traits<Allocator>::deallocate(m_Allocator, m_Data, m_Capacity);
		}
		
		vector(vector const& other, allocator_type const& alloc = allocator_type())
			:m_Allocator(alloc), m_Data(nullptr), m_Capacity(0), m_Size(0) { copy_from(other); }
		vector(vector&& other, allocator_type const& alloc = allocator_type()) noexcept
			:m_Allocator(alloc), m_Data(nullptr), m_Capacity(0), m_Size(0) { swap(other); }

		vector& operator=(vector const& other) { vector tmp = other; swap(tmp); return *this; }
		vector& operator=(vector && other) noexcept { swap(other); return *this; }

	public:
		iterator begin() { return iterator(m_Data); }
		iterator end() { return iterator(m_Data + m_Size); }
		const_iterator cbegin() { return const_iterator(m_Data); }
		const_iterator cend() { return const_iterator(m_Data + m_Size); }
		reverse_iterator rbegin() { return reverse_iterator(m_Data + m_Size - 1); }
		reverse_iterator rend() { return reverse_iterator(m_Data - 1); }
		const_reverse_iterator crbegin() { return const_reverse_iterator(m_Data + m_Size - 1); }
		const_reverse_iterator crend() { return const_reverse_iterator(m_Data - 1); }

	public:
		NODISCARD reference operator[](size_type n) noexcept { return m_Data[n]; }
		NODISCARD const_reference operator[](size_type n) const noexcept { return m_Data[n]; }
		
		NODISCARD reference at(size_type n) { if (n >= m_Size) throw std::out_of_range(); return m_Data[n]; }
		NODISCARD const_reference at(size_type n) const { if (n >= m_Size) throw std::out_of_range(); return m_Data[n]; }
		
		NODISCARD reference front() { if (m_Size == 0) throw std::out_of_range("front() called from a empty vector."); return m_Data[0]; }
		NODISCARD const_reference front() const { if (m_Size == 0) throw std::out_of_range("front() called from a empty vector."); return m_Data[0]; }
		NODISCARD reference back() { if (m_Size == 0) throw std::out_of_range("back() called from a empty vector."); return m_Data[m_Size - 1]; }
		NODISCARD const_reference back() const { if (m_Size == 0) throw std::out_of_range("back() called from a empty vector."); return m_Data[m_Size - 1]; }
		
		NODISCARD pointer data() noexcept { return m_Data; }
		NODISCARD const_pointer data() const noexcept { return m_Data; }

		NODISCARD size_type size() const noexcept { return m_Size; }
		NODISCARD size_type max_size() const noexcept { return std::numeric_limits<size_type>::max() / sizeof(value_type); }
		NODISCARD size_type capacity() const noexcept { return m_Capacity; }

	private:
		allocator_type m_Allocator;
		pointer m_Data;
		size_type m_Capacity, m_Size;

	private:
		void copy_from(vector const& other)
		{
			allocator_traits<allocator_type>::allocate(m_Allocator, other.m_Capacity);
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;
			size_type constructed = 0;
			try
			{
				for (; constructed < other.m_Size; ++constructed)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + constructed, other.m_Data[constructed]);
				}
			}
			catch (...)
			{
				for (size_type i = 0; i < constructed; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
				}
				allocator_traits<allocator_type>::deallocate(m_Allocator, m_Data, m_Capacity);
				throw;
			}
		}

		void swap(vector& other)
		{
			std::swap(m_Allocator, other.m_Allocator);
			std::swap(m_Data, other.m_Data);
			std::swap(m_Capacity, other.m_Capacity);
			std::swap(m_Size, other.m_Size);
		}
	};
}