#pragma once

#include <type_traits>
#include <tuple>
#include <unordered_map>
#include <iostream>
#include <functional>

namespace my_stl
{
	inline namespace hash
	{
		inline constexpr size_t combine_param = 0x9e3779b9; // from boost

		inline void _hash_combine(size_t& seed) {} // 0 param

		template<typename T>
		void _hash_combine(size_t& seed, const T& val) // 1 param
		{
			std::hash<T> hasher;
			seed ^= hasher(val) + combine_param + (seed << 6) + (seed >> 2);
		}

		template<typename T, typename... Args>
		void _hash_combine(size_t& seed, T const& t, Args const&... args) // >1 params
		{
			_hash_combine(seed, t);
			_hash_combine(seed, args...);
		}

		template<typename... Args>
		struct TupleHash
		{
			size_t operator()(std::tuple<Args...> const& t) const
			{
				size_t seed = 0;
				std::apply([&](Args const&... args) {
					_hash_combine(seed, args...);
					}, t);
				return seed;
			}
		};
	}

	template<typename Func>
	class Memorizer;

	template<typename ReturnType, typename... Args>
	class Memorizer<ReturnType(Args...)>
	{
	public:
		using func_type = std::function<ReturnType(Args...)>;
		using args_type = std::tuple<Args...>;
	public:
		Memorizer(){}
		explicit Memorizer(func_type func): func(func){}
		explicit Memorizer(func_type&& func): func(std::move(func)){}

		void clear() { cache.clear(); }
		static void print()
		{
			std::cout << "ReturnType: " << typeid(ReturnType).name() << "\nArgs...: ";
			((std::cout << typeid(Args).name() << " "), ...);
			std::cout << "\n";
		}

		ReturnType operator()(Args... args)
		{
			auto wrapped_args = wrap(args...);
			if (cache.count(wrapped_args) != 0)
				return cache[wrapped_args];
			ReturnType res = func(args...);
			cache[wrapped_args] = res;
			return res;
		}
	private:
		func_type func;
		std::unordered_map<args_type, ReturnType, TupleHash<Args...>> cache;
	private:
		args_type wrap(Args... args)
		{
			return std::make_tuple(args...);
		}
	};
}