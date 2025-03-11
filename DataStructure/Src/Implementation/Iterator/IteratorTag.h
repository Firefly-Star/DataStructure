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
}