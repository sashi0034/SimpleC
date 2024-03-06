#pragma once
#include "Tokenizer.h"
#include "Utils.h"

namespace SimpleC
{
    enum class NodeKind
    {
        Add,
        Sub,
        Mul,
        Div,
        Eq,
        Ne,
        Lt,
        Le,
        Number,
    };

    struct NodeObject
    {
        virtual ~NodeObject() = default;
        virtual NodeKind Kind() const = 0;
    };

    struct NodeBranch : NodeObject
    {
        std::unique_ptr<NodeObject> lhs;
        std::unique_ptr<NodeObject> rhs;
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
        NodeNumber(int value = 0): value(value) { return; }

        int value;
        NodeKind Kind() const override { return NodeKind::Number; }
    };

    std::unique_ptr<NodeObject> ExecuteParse(const TokenizedResult& input);
}
