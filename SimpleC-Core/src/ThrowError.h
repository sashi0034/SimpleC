#pragma once
#include "Forward.h"

namespace SimpleC
{
    [[noreturn]]
    void ThrowErrorAt(StringView initialInput, StringView input, StringView desc);
}
