#pragma once
#include "Tokenizer.h"
#include "Utils.h"

namespace SimpleC
{
    class NodeKind : public EnumValue<int>
    {
    public:
        using EnumValue::EnumValue;
    };

    class NodeType;

    struct NodeBranch
    {
        std::unique_ptr<NodeType> lhs;
        std::unique_ptr<NodeType> rhs;
        NodeBranch(std::unique_ptr<NodeType> lhs, std::unique_ptr<NodeType> rhs);
    };

    struct NodeAdd : NodeBranch
    {
        using NodeBranch::NodeBranch;
    };

    struct NodeSub : NodeBranch
    {
        using NodeBranch::NodeBranch;
    };

    struct NodeMul : NodeBranch
    {
        using NodeBranch::NodeBranch;
    };

    struct NodeDiv : NodeBranch
    {
        using NodeBranch::NodeBranch;
    };

    struct NodeNumber
    {
        int value;
    };

    namespace Nodes
    {
        inline constexpr NodeKind Add{0};
        inline constexpr NodeKind Sub{1};
        inline constexpr NodeKind Mul{2};
        inline constexpr NodeKind Div{3};
        inline constexpr NodeKind Number{4};
    }

    class NodeType : public std::variant<
            NodeAdd,
            NodeSub,
            NodeMul,
            NodeDiv,
            NodeNumber>
    {
    public:
        using variant::variant;
        using variant::operator=;
    };

    NodeType ExecuteParse(const TokenizedResult& input);
}
