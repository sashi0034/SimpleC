#include "pch.h"
#include "..\include\CodeGenerate.h"

using namespace SimpleC;

namespace
{
    void genLval(String& code, const NodeObject& node)
    {
        const auto lvar = dynamic_cast<const NodeLvar*>(&node);
        if (not lvar) throw CompileException(L"代入の左辺値が変数ではありません");

        code += L"  mov rax, rbp\n";
        code += L"  sub rax, {}\n"_fmt(lvar->offset);
        code += L"  push rax\n";
    }

    void gen(String& code, const NodeObject& node)
    {
        if (const auto n = dynamic_cast<const NodeNumber*>(&node))
        {
            // 定数
            code += L"  push {}\n"_fmt(n->value);
            return;
        }
        else if (dynamic_cast<const NodeLvar*>(&node))
        {
            // 変数
            genLval(code, node);
            code += L"  pop rax\n";
            code += L"  mov rax, [rax]\n";
            code += L"  push rax\n";
            return;
        }
        else if (const auto assign = dynamic_cast<const NodeAssign*>(&node))
        {
            // 代入文
            genLval(code, *assign->lhs);
            gen(code, *assign->rhs);
            code += L"  pop rdi\n";
            code += L"  pop rax\n";
            code += L"  mov [rax], rdi\n";
            code += L"  push rdi\n";
            return;
        }

        const auto branch = dynamic_cast<const NodeBranch*>(&node);
        assert(branch);

        // 二項演算式
        gen(code, *branch->lhs);
        gen(code, *branch->rhs);

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
        case NodeKind::Eq:
            code += L"  cmp rax, rdi\n";
            code += L"  sete al\n";
            code += L"  movzb rax, al\n";
            break;
        case NodeKind::Ne:
            code += L"  cmp rax, rdi\n";
            code += L"  setne al\n";
            code += L"  movzb rax, al\n";
            break;
        case NodeKind::Lt:
            code += L"  cmp rax, rdi\n";
            code += L"  setl al\n";
            code += L"  movzb rax, al\n";
            break;
        case NodeKind::Le:
            code += L"  cmp rax, rdi\n";
            code += L"  setle al\n";
            code += L"  movzb rax, al\n";
            break;
        default: ;
            assert(false);
        }

        code += L"  push rax\n";
    }
}

namespace SimpleC
{
    String CodeGenerate(const ProgramNodes& program)
    {
        String code{};
        code += LR"(
.intel_syntax noprefix
.globl main

main:
)";
        // プロローグ 変数26個分の領域を確保
        code += L"  push rbp\n";
        code += L"  mov rbp, rsp\n";
        code += L"  sub rsp, 208\n";

        for (auto&& prg : program)
        {
            gen(code, *prg);
            code += L"  pop rax\n"; // 式の評価結果としてスタックに一つの値が残っているはず
        }

        // エピローグ 最後の式の結果が RAX
        code += L"  mov rsp, rbp\n";
        code += L"  pop rbp\n";
        code += L"  ret\n";
        return code;
    }
}
