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
        Number,
    };

    struct NodeObject
    {
        virtual ~NodeObject() = default;
        virtual NodeKind Kind() = 0;
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
        NodeKind Kind() override { return NodeKind::Add; }
    };

    struct NodeSub : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() override { return NodeKind::Sub; }
    };

    struct NodeMul : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() override { return NodeKind::Mul; }
    };

    struct NodeDiv : NodeBranch
    {
        using NodeBranch::NodeBranch;
        NodeKind Kind() override { return NodeKind::Div; }
    };

    struct NodeNumber : NodeObject
    {
        int value;
        NodeKind Kind() override { return NodeKind::Number; }
    };

    std::unique_ptr<NodeObject> ExecuteParse(const TokenizedResult& input);
}
