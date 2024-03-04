#include "pch.h"
#include "GenerateCode.h"

using namespace SimpleC;

namespace
{
    void generateCode(String& code, const NodeObject& node)
    {
        if (const auto n = dynamic_cast<const NodeNumber*>(&node))
        {
            code += L"  push {}\n"_fmt(n->value);
            return;
        }

        const auto branch = dynamic_cast<const NodeBranch*>(&node);
        assert(branch);
    }
}

namespace SimpleC
{
    String GenerateCode(const NodeObject& node)
    {
        String code{};
        generateCode(code, node);
        return code;
    }
}
