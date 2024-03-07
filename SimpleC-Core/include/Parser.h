#pragma once
#include "Tokenizer.h"
#include "Utils.h"

namespace SimpleC
{
    enum class NodeKind
    {
        Add, // +
        Sub, // -
        Mul, // *
        Div, // /
        Eq, // ==
        Ne, // !=
        Lt, // <
        Le, // <=
        Number, // 数値
        Lvar, // ローカル変数
        Assign, // =
    };

    struct NodeObject
    {
        virtual ~NodeObject() = default;
        virtual NodeKind Kind() const = 0;
    };

    using NodePtr = std::unique_ptr<NodeObject>;

    struct NodeBranch : NodeObject
    {
        NodePtr lhs;
        NodePtr rhs;

        NodeBranch(std::unique_ptr<NodeObject> lhs, std::unique_ptr<NodeObject> rhs);
    };

    struct NodeAdd : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() const override { return NodeKind::Add; }
    };

    struct NodeSub : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() const override { return NodeKind::Sub; }
    };

    struct NodeMul : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() const override { return NodeKind::Mul; }
    };

    struct NodeDiv : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() const override { return NodeKind::Div; }
    };

    struct NodeEq : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() const override { return NodeKind::Eq; }
    };

    struct NodeNe : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() const override { return NodeKind::Ne; }
    };

    struct NodeLt : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() const override { return NodeKind::Lt; }
    };

    struct NodeLe : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() const override { return NodeKind::Le; }
    };

    struct NodeNumber : NodeObject
    {
        int value;

        NodeNumber(int value = 0): value(value) { return; }
        NodeKind Kind() const override { return NodeKind::Number; }
    };

    struct NodeLvar : NodeObject
    {
        int offset;

        explicit NodeLvar(int offset): offset(offset) { return; }
        NodeKind Kind() const override { return NodeKind::Lvar; }
    };

    struct NodeAssign : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() const override { return NodeKind::Assign; }
    };

    using ProgramNodes = std::vector<NodePtr>;

    ProgramNodes ExecuteParse(const TokenizedResult& input);
}
