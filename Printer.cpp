#include "Printer.hpp"

std::string Printer::toFormulaString(const std::shared_ptr<ASTNode>& node) {
    if (!node) return "";

    // Base Case 1: Numbers (e.g., 5 or 3.14)
    if (node->type == NodeType::NUMBER) {
        std::string str = std::to_string(node->value);
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.back() == '.') str.pop_back();
        return str;
    }

    // Base Case 2: Variables (e.g., "x")
    if (node->type == NodeType::VARIABLE) {
        return node->name;
    }

    // Unary Operators: ALL 15 SINGLE-PARAMETER FUNCTIONS
    if (node->type == NodeType::SIN    || node->type == NodeType::COS    || node->type == NodeType::TAN    || 
        node->type == NodeType::SEC    || node->type == NodeType::COSEC  || node->type == NodeType::COT    ||
        node->type == NodeType::ARCSIN || node->type == NodeType::ARCCOS || node->type == NodeType::ARCTAN ||
        node->type == NodeType::ARCSEC || node->type == NodeType::ARCCOSEC|| node->type == NodeType::ARCCOT ||
        node->type == NodeType::EXP    || node->type == NodeType::LN     || node->type == NodeType::LOG) {
        
        std::string funcName = "";
        if (node->type == NodeType::SIN)          funcName = "sin";
        else if (node->type == NodeType::COS)     funcName = "cos";
        else if (node->type == NodeType::TAN)     funcName = "tan";
        else if (node->type == NodeType::SEC)     funcName = "sec";
        else if (node->type == NodeType::COSEC)   funcName = "cosec";
        else if (node->type == NodeType::COT)     funcName = "cot";
        else if (node->type == NodeType::ARCSIN)  funcName = "arcsin";
        else if (node->type == NodeType::ARCCOS)  funcName = "arccos";
        else if (node->type == NodeType::ARCTAN)  funcName = "arctan";
        else if (node->type == NodeType::ARCSEC)  funcName = "arcsec";
        else if (node->type == NodeType::ARCCOSEC)funcName = "arccosec";
        else if (node->type == NodeType::ARCCOT)  funcName = "arccot";
        else if (node->type == NodeType::EXP)     funcName = "exp";
        else if (node->type == NodeType::LN)      funcName = "ln";
        else if (node->type == NodeType::LOG)     funcName = "log";
        
        return funcName + "(" + toFormulaString(node->children[0]) + ")";
    }

    // Binary Operators: +, -, *, /, ^ (Operators with a Left and Right side)
    if (node->children.size() == 2) {
        std::string leftStr = toFormulaString(node->children[0]);
        std::string rightStr = toFormulaString(node->children[1]);
        
        std::string opStr = "";
        switch (node->type) {
            case NodeType::ADD:      opStr = " + "; break;
            case NodeType::SUBTRACT: opStr = " - "; break;
            case NodeType::MULTIPLY: opStr = " * "; break;
            case NodeType::DIVIDE:   opStr = " / "; break;
            case NodeType::POWER:    opStr = "^";   break;
            default: opStr = " ? ";
        }
        
        return "(" + leftStr + opStr + rightStr + ")";
    }

    return "";
}
