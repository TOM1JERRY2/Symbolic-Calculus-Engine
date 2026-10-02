#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <string>
#include <memory>
#include <vector>

// Enumeration to define what type of math element the node is
enum class NodeType {
    NUMBER,      // e.g., 5, 3.14
    VARIABLE,    // e.g., x
    ADD,         // +
    SUBTRACT,    // -
    MULTIPLY,    // *
    DIVIDE,      // /
    POWER,       // ^
    SIN,         
    COS,
    TAN,
    SEC,
    COSEC,
    COT,
    ARCSIN,
    ARCCOS,
    ARCTAN,
    ARCSEC,
    ARCCOSEC,
    ARCCOT,         // ALL FUNCTIONS RELATED TO TRIGONOMETRY
    EXP,         // e^(x)
    LN,
    LOG,
};

// The structural node of our Abstract Syntax Tree (AST)
struct ASTNode {
    NodeType type;
    double value = 0.0;          // Only used if type is NUMBER
    std::string name = "";       // Used for VARIABLE (e.g., "x")

    // Children nodes: 
    // - A NUMBER has 0 children
    // - A SIN node has 1 child (what's inside the sine)
    // - An ADD node has 2 children (left side and right side)
    std::vector<std::shared_ptr<ASTNode>> children;

    // Constructors for easy creation
    ASTNode(NodeType t) : type(t) {}
    ASTNode(double val) : type(NodeType::NUMBER), value(val) {}
    ASTNode(NodeType t, std::string varName) : type(t), name(varName) {}
};

#endif // AST_NODE_HPP
