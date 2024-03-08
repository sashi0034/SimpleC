#pragma once
#include "Forward.h"

namespace SimpleC
{
    [[noreturn]]
    void ThrowErrorAt(StringView initialInput, StringView input, StringView desc);

    [[noreturn]]
    void ThrowErrorAt(StringView initialInput, int pos, StringView desc);
}
