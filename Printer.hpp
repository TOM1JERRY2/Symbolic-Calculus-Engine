#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <string>
#include <memory>
#include "ASTnode.hpp"

class Printer {
public:
    // Recursively converts any ASTNode tree structure back into a string formula
    static std::string toFormulaString(const std::shared_ptr<ASTNode>& node);
};

#endif // PRINTER_HPP
