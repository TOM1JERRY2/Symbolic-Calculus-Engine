#include "ExplanationBuilder.hpp"
#include "Printer.hpp"
#include <sstream>

std::string ExplanationBuilder::constantRule(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& result) {
    std::stringstream ss;
    ss << "  ■ Rule: ∫ k dx = k * x\n"
       << "    ∫ " << Printer::toFormulaString(node) << " dx  =  " << Printer::toFormulaString(result) << "\n\n";
    return ss.str();
}

std::string ExplanationBuilder::variableRule(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& result) {
    std::stringstream ss;
    ss << "  ■ Rule: ∫ x dx = 0.5 * x^2\n"
       << "    ∫ " << node->name << " dx  =  " << Printer::toFormulaString(result) << "\n\n";
    return ss.str();
}

std::string ExplanationBuilder::powerRule(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& exponent, const std::shared_ptr<ASTNode>& result) {
    std::stringstream ss;
    ss << "  ■ Power Rule: ∫ x^n dx = (x^(n+1)) / (n+1)\n"
       << "    ∫ " << Printer::toFormulaString(node) << " dx  =  " << Printer::toFormulaString(result) << "\n\n";
    return ss.str();
}

std::string ExplanationBuilder::linearityRule(const std::shared_ptr<ASTNode>& node, const std::string& leftSteps, const std::string& rightSteps, const std::shared_ptr<ASTNode>& result) {
    std::stringstream ss;
    ss << "  ■ Sum Rule: ∫(f + g)dx = ∫f dx + ∫g dx\n"
       << "    1. Solve: ∫ " << Printer::toFormulaString(node->children[0]) << " dx:\n" << leftSteps
       << "    2. Solve: ∫ " << Printer::toFormulaString(node->children[1]) << " dx:\n" << rightSteps
       << "    Combined: " << Printer::toFormulaString(result) << "\n\n";
    return ss.str();
}

std::string ExplanationBuilder::constantMultipleRule(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& constantNode, const std::shared_ptr<ASTNode>& innerNode, const std::string& innerSteps, const std::shared_ptr<ASTnode>& result) {
    std::stringstream ss;
    ss << "  ■ Constant Rule: ∫ c*f(x)dx = c * ∫ f(x)dx\n"
       << "    Pull out " << Printer::toFormulaString(constantNode) << " ->  " << Printer::toFormulaString(constantNode) << " * ∫ " << Printer::toFormulaString(innerNode) << " dx\n"
       << innerSteps
       << "    Evaluated: " << Printer::toFormulaString(result) << "\n\n";
    return ss.str();
}

std::string ExplanationBuilder::ibpRule(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& u, const std::shared_ptr<ASTNode>& du, const std::shared_ptr<ASTNode>& dv, const std::shared_ptr<ASTNode>& v, const std::string& vSteps, const std::shared_ptr<ASTNode>& du_v, const std::string& remainderSteps, const std::shared_ptr<ASTNode>& result) {
    std::stringstream ss;
    ss << "  ■ Integration by Parts: ∫ u dv = u*v - ∫ v du\n"
       << "    [Setup]\n"
       << "      u  = " << Printer::toFormulaString(u) << "   => du = " << Printer::toFormulaString(du) << " dx\n"
       << "      dv = " << Printer::toFormulaString(dv) << " dx\n"
       << "    [Find v]\n"
       << "      v  = ∫ " << Printer::toFormulaString(dv) << " dx\n" << vSteps
       << "      v  = " << Printer::toFormulaString(v) << "\n"
       << "    [Substitute]\n"
       << "      = (" << Printer::toFormulaString(u) << " * " << Printer::toFormulaString(v) << ") - ∫ (" << Printer::toFormulaString(du_v) << ") dx\n"
       << "    [Solve Remaining Integral]\n" << remainderSteps
       << "    Result Pass: " << Printer::toFormulaString(result) << "\n\n";
    return ss.str();
}

std::string ExplanationBuilder::standardLookup(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& result) {
    std::stringstream ss;
    ss & node, const std::shared_ptr<ASTNode>& inside, const std::shared_ptr<ASTNode>& coeffNode, const std::shared_ptr<ASTNode>& result) {
    std::stringstream ss;
    ss << "  ■ Reverse Chain Rule (Substitution):\n"
       << "    ∫ f(a*x) dx = (1/a) * F(a*x)\n"
       << "    ∫ " << Printer::toFormulaString(node) << " dx  =  " << Printer::toFormulaString(result) << "\n\n";
    return ss.str();
}

std::string ExplanationBuilder::reverseChainShift(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& inside, double scaleFactor, const std::shared_ptr<ASTNode>& result) {
    std::stringstream ss;
    ss << "  ■ Reverse Chain Rule (Linear Shift):\n"
       << "    Scale Factor (a): " << scaleFactor << "\n"
       << "    ∫ f(a*x + b) dx = (1/a) * F(a*x + b)\n"
       << "    ∫ " << Printer::toFormulaString(node) << " dx  =  " << Printer::toFormulaString(result) << "\n\n";
    return ss.str();
}
