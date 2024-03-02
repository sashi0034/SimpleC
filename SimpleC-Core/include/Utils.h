#pragma once

namespace SimpleC{
	template <typename T>
	class EnumValue
	{
		static_assert(std::is_integral<T>::value);

	public:
		explicit constexpr EnumValue() : value{0} { return; }
		explicit constexpr EnumValue(T v) : value{v} { return; }
		constexpr operator T() const { return value; }

	private:
		T value;
	};
}
