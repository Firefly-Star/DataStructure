#pragma once

#define RULE_OF_FIVE(className) \
public: \
~className(); \
className(className const& other) \
{ \
	_Copy(other); \
} \
className(className && other) noexcept \
{ \
	_Swap(other); \
} \
className& operator=(className const& other) \
{ \
	_Copy(other); \
	return *this; \
} \
className& operator=(className && other) noexcept \
{ \
	_Swap(other); \
	return *this; \
} \
private: \
void _Swap(className& other) noexcept; \
void _Copy(className const& other);

#define SWAP_MEMBER(member) std::swap(this->member, other.member);

#define NODISCARD [[nodiscard]]