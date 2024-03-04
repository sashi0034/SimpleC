﻿#include "pch.h"
#include "..\include\CodeGenerate.h"

using namespace SimpleC;

namespace
{
    void generate(String& code, const NodeObject& node)
    {
        if (const auto n = dynamic_cast<const NodeNumber*>(&node))
        {
            code += L"  push {}\n"_fmt(n->value);
            return;
        }

        const auto branch = dynamic_cast<const NodeBranch*>(&node);
        assert(branch);

        generate(code, *branch->lhs);
        generate(code, *branch->rhs);

        code += L"  pop rdi\n";
        code += L"  pop rax\n";

        switch (branch->Kind())
        {
        case NodeKind::Add:
            code += L"  add rax, rdi\n";
            break;
        case NodeKind::Sub:
            code += L"  sub rax, rdi\n";
            break;
        case NodeKind::Mul:
            code += L"  imul rax, rdi\n";
            break;
        case NodeKind::Div:
            code += L"  cqo\n";
            code += L"  idiv rdi\n";
            break;
        default: ;
            assert(false);
        }

        code += L"  push rax\n";
    }
}

namespace SimpleC
{
    String CodeGenerate(const NodeObject& node)
    {
        String code{};
        code += LR"(
.intel_syntax noprefix
.globl main

main:
)";
        generate(code, node);
        code += LR"(
  pop rax
  ret
)";
        return code;
    }
}