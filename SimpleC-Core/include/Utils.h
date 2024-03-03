#pragma once

#include "Forward.h"

namespace SimpleC
{
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

    struct FormatHelper : StringView
    {
        using StringView::StringView;

        template <class... Args>
        [[nodiscard]]
        String operator()(Args&&... args) const
        {
            return std::vformat(*this, std::make_wformat_args(std::forward<Args>(args)...));
        }
    };

    inline constexpr FormatHelper operator ""_fmt(const wchar_t* s, const size_t length)
    {
        return FormatHelper{s, length};
    }
}
