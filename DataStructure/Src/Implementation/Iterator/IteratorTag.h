#pragma once

#include <concepts>

namespace my_stl
{
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};
	
	// �������ͱ���
	template<class _Value_type, class _Pointer, class _Reference, class _Difference_type>
	class iterator_type_defination
	{
	public:
		using value_type = _Value_type;
		using pointer = _Pointer;
		using reference = _Reference;
		using difference_type = _Difference_type;
	};

	// �ж��Ƿ��������ͱ���
	template<typename Iter>
	concept _Has_iterator_aliases = requires
	{
		typename Iter::value_type;
		typename Iter::pointer;
		typename Iter::reference;
		typename Iter::difference_type;
	};

	// �������������Ҫ�ķ���
	template<typename Iter>
	concept _Has_unidirectional_iterator_methods = requires (Iter it1, Iter it2)
	{
		// ����
		{ *it1 } -> std::same_as<typename Iter::reference>;
		{ ++it1 } -> std::same_as<Iter&>;
		{ it1++ } -> std::same_as<Iter>;
		// ��Ԫ
		{ it1 == it2 } -> std::same_as<bool>;
		{ it1 != it2 } -> std::same_as<bool>;
	};

	// ˫�����������Ҫ�ķ���
	template<typename Iter>
	concept _Has_bidirectional_iterator_methods = requires (Iter it1, Iter it2)
	{
		// ����
		{ *it1 } -> std::same_as<typename Iter::reference>;
		{ ++it1 } -> std::same_as<Iter&>;
		{ --it1 } -> std::same_as<Iter&>;
		{ it1++ } -> std::same_as<Iter>;
		{ it1-- } -> std::same_as<Iter>;
		// ��Ԫ
		{ it1 == it2 } -> std::same_as<bool>;
		{ it1 != it2 } -> std::same_as<bool>;
	};

	// �����������Ҫ�ķ���
	template<typename Iter>
	concept _Has_random_access_iterator_methods = requires (Iter it1, Iter it2, typename Iter::difference_type n)
	{
		// ����
		{ *it1 } -> std::same_as<typename Iter::reference>;
		{ ++it1 } -> std::same_as<Iter&>;
		{ --it1 } -> std::same_as<Iter&>;
		{ it1++ } -> std::same_as<Iter>;
		{ it1-- } -> std::same_as<Iter>;
		{ it1[n] } -> std::same_as<typename Iter::reference>;
		{ it1 += n } -> std::same_as<Iter&>;
		{ it1 -= n } -> std::same_as<Iter&>;
		// ��Ԫ
		{ it1 == it2 } -> std::same_as<bool>;
		{ it1 != it2 } -> std::same_as<bool>;
		{ it1 + n } -> std::same_as<Iter>;
		{ n + it1 } -> std::same_as<Iter>;
		{ it1 - n } -> std::same_as<Iter>;
		{ it1 - it2 } -> std::same_as<typename Iter::difference_type>;
		{ it1 < it2 } -> std::same_as<bool>;
		{ it1 <= it2 } -> std::same_as<bool>;
		{ it1 > it2 } -> std::same_as<bool>;
		{ it1 >= it2 } -> std::same_as<bool>;
	};

	// �������������ģ��
	template<typename Iter>
	class input_iterator_t
	{
	public:
		using iterator_category = input_iterator_tag;

		input_iterator_t()
		{
			static_assert(
				_Has_iterator_aliases<Iter> && 
				_Has_unidirectional_iterator_methods<Iter>,
				"Input iterator must has specified aliases and methods!");
		}
	};

	// �������������ģ��
	template<typename Iter>
	class output_iterator_t
	{
	public:
		using iterator_category = output_iterator_tag;

		output_iterator_t()
		{
			static_assert(
				_Has_iterator_aliases<Iter> && 
				_Has_unidirectional_iterator_methods<Iter>,
				"Output iterator must has specified aliases and methods!");
		}
	};

	// ˫�����������ģ��
	template<typename Iter>
	class bidirectional_iterator_t
	{
	public:
		using iterator_category = bidirectional_iterator_tag;
		
		bidirectional_iterator_t()
		{
			static_assert(
				_Has_iterator_aliases<Iter> && 
				_Has_bidirectional_iterator_methods<Iter>,
				"Bidirectionalput iterator must has specified aliases and methods!");
		}
	};

	// ������ʵ���������ģ��
	template<typename Iter>
	class random_access_iterator_t
	{
	public:
		using iterator_category = random_access_iterator_tag;
		
		random_access_iterator_t()
		{
			static_assert(
				_Has_iterator_aliases<Iter> && 
				_Has_random_access_iterator_methods<Iter>,
				"Input iterator must has specified aliases and methods!");
		}
	};

	template<typename Iter>
	constexpr bool is_input_iterator = std::derived_from<Iter, input_iterator_t<Iter>>;

	template<typename Iter>
	constexpr bool is_output_iterator = std::derived_from<Iter, output_iterator_t<Iter>>;

	template<typename Iter>
	constexpr bool is_bidirectional_iterator = std::derived_from<Iter, bidirectional_iterator_t<Iter>>;

	template<typename Iter>
	constexpr bool is_random_access_iterator = std::derived_from<Iter, random_access_iterator_t<Iter>>;

	template<typename Iter>
	class iterator_traits
	{
	public:
		using value_type = typename decltype([]()
			{
				if constexpr (_Has_iterator_aliases<Iter>)
				{
					return std::type_identity<typename Iter::value_type>{};
				}
				else
				{
					return std::type_identity<char>{};
				}
			}())::type;
		using pointer = typename decltype([]()
			{
				if constexpr (_Has_iterator_aliases<Iter>)
				{
					return std::type_identity<typename Iter::pointer>{};
				}
				else
				{
					return std::type_identity<value_type*>{};
				}
			}())::type;
		using reference = typename decltype([]()
			{
				if constexpr (_Has_iterator_aliases<Iter>)
				{
					return std::type_identity<typename Iter::reference>{};
				}
				else
				{
					return std::type_identity<value_type&>{};
				}
			}())::type;
		using difference_type = typename decltype([]()
			{
				if constexpr (_Has_iterator_aliases<Iter>)
				{
					return std::type_identity<typename Iter::difference_type>{};
				}
				else
				{
					return std::type_identity<std::ptrdiff_t>{};
				}
			}())::type;
		using iterator_category = typename decltype([]()
			{
				if constexpr (is_input_iterator<Iter>)
				{
					return std::type_identity<input_iterator_tag>{};
				}
				else if constexpr (is_output_iterator<Iter>)
				{
					return std::type_identity<output_iterator_tag>{};
				}
				else if constexpr (is_bidirectional_iterator<Iter>)
				{
					return std::type_identity<bidirectional_iterator_tag>{};
				}
				else if constexpr (is_random_access_iterator<Iter>)
				{
					return std::type_identity<random_access_iterator_tag>{};
				}
				else
				{
					return std::type_identity<void>{};
				}
			}())::type;
	};


	template<typename Iter>
	NODISCARD constexpr typename Iter::difference_type distance(Iter first, Iter last)
	{
		if constexpr (is_random_access_iterator<Iter>)
		{
			return last - first;
		}
		else
		{
			typename Iter::difference_type n = 0;
			while (first != last)
			{
				++first;
				++n;
			}
			return n;
		}
	}


}