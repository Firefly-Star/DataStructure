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
		using rvalue_reference = value_type&&;
		using const_reference = const value_type&;
		using pointer = allocator_traits<Allocator>::pointer;
		using const_pointer = allocator_traits<Allocator>::const_pointer;
	public:
		// TODO: Debug模式下的边界检查和迭代器有效性检查
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
		protected:
			pointer m_Current;
		};

		class const_iterator : public iterator
		{
		public:
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using difference_type = std::ptrdiff_t;

			using iterator::iterator;
		public:
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
		protected:
			pointer m_Current;
		};

		class const_reverse_iterator : public reverse_iterator
		{
		public:
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using difference_type = std::ptrdiff_t;

			using reverse_iterator::reverse_iterator;
		public:
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
		iterator begin() noexcept { return iterator(m_Data); }
		const_iterator begin() const noexcept { return const_iterator(m_Data); }
		iterator end() noexcept { return iterator(m_Data + m_Size); }
		const_iterator end() const noexcept { return const_iterator(m_Data + m_Size); }
		const_iterator cbegin() const noexcept { return begin(); }
		const_iterator cend() const noexcept { return end(); }
		reverse_iterator rbegin() noexcept { return reverse_iterator(m_Data + m_Size - 1); }
		const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(m_Data + m_Size - 1); }
		reverse_iterator rend() noexcept { return reverse_iterator(m_Data - 1); }
		const_reverse_iterator rend() const noexcept { return const_reverse_iterator(m_Data - 1); }
		const_reverse_iterator crbegin() noexcept { return rbegin(); }
		const_reverse_iterator crend() const noexcept { return rend(); }

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
		NODISCARD size_type max_size() const noexcept 
		{
			return std::min(std::numeric_limits<size_type>::max(), allocator_traits<allocator_type>::max_size(m_Allocator));
		}
		NODISCARD size_type capacity() const noexcept { return m_Capacity; }

		void reserve(size_type new_cap)
		{
			if (new_cap <= m_Capacity) return;
			pointer new_data = allocator_traits<allocator_type>::allocate(m_Allocator, new_cap);
			size_type constructed = 0;
			try
			{
				for (; constructed < m_Size; ++constructed)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, std::move(m_Data[constructed]));
				}
			}
			catch (...)
			{
				for (size_type i = 0; i < constructed; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, new_data + i);
				}
				allocator_traits<allocator_type>::deallocate(m_Allocator, new_data, new_cap);
				throw;
			}
			for (size_type i = 0; i < m_Size; ++i)
			{
				allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
			}
			allocator_traits<allocator_type>::deallocate(m_Allocator, m_Data, m_Capacity);
			m_Data = new_data;
			m_Capacity = new_cap;
		}
		void shrink_to_fit()
		{
			if (m_Size == m_Capacity) return;
			pointer new_data = allocator_traits<allocator_type>::allocate(m_Allocator, m_Size);
			size_type constructed = 0;
			try
			{
				for (; constructed < m_Size; ++constructed)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, std::move(m_Data[constructed]));
				}
			}
			catch (...)
			{
				for (size_type i = 0; i < constructed; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, new_data + i);
				}
				allocator_traits<allocator_type>::deallocate(m_Allocator, new_data, m_Size);
				throw;
			}
			for (size_type i = 0; i < m_Size; ++i)
			{
				allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
			}
			allocator_traits<allocator_type>::deallocate(m_Allocator, m_Data, m_Capacity);
			m_Data = new_data;
			m_Capacity = m_Size;
		}

		void resize(size_type count)
		{
			if (count < m_Size)
			{
				for (size_type i = count; i < m_Size; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
				}
				m_Size = count;
			}
			else if (count > m_Size)
			{
				if (count > m_Capacity)
				{
					reserve(count);
				}
				for (size_type i = m_Size; i < count; ++i)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + i);
				}
				m_Size = count;
			}
		}
		void resize(size_type count, const_reference value)
		{
			if (count < m_Size)
			{
				for (size_type i = count; i < m_Size; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
				}
				m_Size = count;
			}
			else if (count > m_Size)
			{
				if (count > m_Capacity)
				{
					reserve(count);
				}
				for (size_type i = m_Size; i < count; ++i)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + i, value);
				}
				m_Size = count;
			}
		}

		template<typename... Args>
		iterator emplace(const_iterator pos, Args&&... args)
		{
			difference_type index = pos - cbegin();
			if (index < 0 || index > m_Size) throw std::out_of_range("Iterator out of range.");
			if (m_Size == m_Capacity)
			{
				size_type new_capacity = m_Capacity == 0 ? 1 : m_Capacity * 2;
				pointer new_data = allocator_traits<allocator_type>::allocate(m_Allocator, new_capacity);
				size_type constructed = 0;
				try
				{
					for (; constructed < index; ++constructed)
					{
						allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, std::move(m_Data[constructed]));
					}
					allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, std::forward<Args>(args)...);
					++constructed;
					for (; constructed < m_Size; ++constructed)
					{
						allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, std::move(m_Data[constructed - 1]));
					}
				}
				catch (...)
				{
					for (size_type i = 0; i < constructed; ++i)
					{
						allocator_traits<allocator_type>::destroy(m_Allocator, new_data + i);
					}
					allocator_traits<allocator_type>::deallocate(m_Allocator, new_data, new_capacity);
					throw;
				}
				for (size_type i = 0; i < m_Size; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
				}
				allocator_traits<allocator_type>::deallocate(m_Allocator, m_Data, m_Capacity);
				m_Data = new_data;
				m_Capacity = new_capacity;
				++m_Size;
			}
			else
			{
				for (size_type i = m_Size; i > index; --i)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + i, std::move(m_Data[i - 1]));
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + (i - 1));
				}
				allocator_traits<allocator_type>::construct(m_Allocator, m_Data + index, std::forward<Args>(args)...);
				++m_Size;
			}
			return iterator(m_Data + index);
		}

		iterator insert(const_iterator pos, const_reference value)
		{
			return emplace(pos, value);
		}
		iterator insert(const_iterator pos, rvalue_reference value)
		{
			return emplace(pos, std::move(value));
		}
		iterator insert(const_iterator pos, size_type count, const_reference value)
		{
			difference_type index = pos - cbegin();
			if (index < 0 || index > m_Size) throw std::out_of_range("Iterator out of range.");
			if (m_Size + count > m_Capacity)
			{
				size_type new_capacity = m_Capacity == 0 ? 1 : m_Capacity * 2;
				pointer new_data = allocator_traits<allocator_type>::allocate(m_Allocator, new_capacity);
				size_type constructed = 0;
				try
				{
					for (; constructed < index; ++constructed)
					{
						allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, std::move(m_Data[constructed]));
					}
					for (size_type i = 0; i < count; ++i)
					{
						allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, value);
						++constructed;
					}
					for (; constructed < m_Size + count; ++constructed)
					{
						allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, std::move(m_Data[constructed - count]));
					}
				}
				catch (...)
				{
					for (size_type i = 0; i < constructed; ++i)
					{
						allocator_traits<allocator_type>::destroy(m_Allocator, new_data + i);
					}
					allocator_traits<allocator_type>::deallocate(m_Allocator, new_data, new_capacity);
					throw;
				}
				for (size_type i = 0; i < m_Size; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
				}
				allocator_traits<allocator_type>::deallocate(m_Allocator, m_Data, m_Capacity);
				m_Data = new_data;
				m_Capacity = new_capacity;
				m_Size += count;
			}
			else
			{
				for (size_type i = m_Size + count - 1; i > index + count - 1; --i)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + i, std::move(m_Data[i - count]));
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + (i - count));
				}
				for (size_type i = 0; i < count; ++i)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + index + i, value);
				}
				m_Size += count;
			}
			return iterator(m_Data + index);
		}
		template<typename InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last)
		{
			difference_type index = pos - cbegin();
			if (index < 0 || index > m_Size) throw std::out_of_range("Iterator out of range.");
			size_type count = last - first; // TODO: distance
			if (m_Size + count > m_Capacity)
			{
				size_type new_capacity = m_Capacity == 0 ? 1 : m_Capacity * 2;
				pointer new_data = allocator_traits<allocator_type>::allocate(m_Allocator, new_capacity);
				size_type constructed = 0;
				try
				{
					for (; constructed < index; ++constructed)
					{
						allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, std::move(m_Data[constructed]));
					}
					for (InputIt it = first; it != last; ++it)
					{
						allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, *it);
						++constructed;
					}
					for (; constructed < m_Size + count; ++constructed)
					{
						allocator_traits<allocator_type>::construct(m_Allocator, new_data + constructed, std::move(m_Data[constructed - count]));
					}
				}
				catch (...)
				{
					for (size_type i = 0; i < constructed; ++i)
					{
						allocator_traits<allocator_type>::destroy(m_Allocator, new_data + i);
					}
					allocator_traits<allocator_type>::deallocate(m_Allocator, new_data, new_capacity);
					throw;
				}
				for (size_type i = 0; i < m_Size; ++i)
				{
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + i);
				}
				allocator_traits<allocator_type>::deallocate(m_Allocator, m_Data, m_Capacity);
				m_Data = new_data;
				m_Capacity = new_capacity;
				m_Size += count;
			}
			else
			{
				for (size_type i = m_Size + count - 1; i > index + count - 1; --i)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + i, std::move(m_Data[i - count]));
					allocator_traits<allocator_type>::destroy(m_Allocator, m_Data + (i - count));
				}
				size_type i = index;
				InputIt it = first;
				for (; it != last; ++it, ++i)
				{
					allocator_traits<allocator_type>::construct(m_Allocator, m_Data + i, *it);
				}
			}
			return iterator(m_Data + index);
		}

		void swap(vector& other)
		{
			std::swap(m_Allocator, other.m_Allocator);
			std::swap(m_Data, other.m_Data);
			std::swap(m_Capacity, other.m_Capacity);
			std::swap(m_Size, other.m_Size);
		}
		NODISCARD constexpr allocator_type get_allocator() const { return m_Allocator; }
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

	};
}