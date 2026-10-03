#ifndef EXPLANATION_BUILDER_HPP
#define EXPLANATION_BUILDER_HPP

#include <string>
#include <memory>
#include <vector>
#include "ASTNode.hpp"

class ExplanationBuilder {
public:
    static std::string constantRule(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& result);
    static std::string variableRule(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& result);
    static std::string powerRule(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& exponent, const std::shared_ptr<ASTNode>& result);
    static std::string linearityRule(const std::shared_ptr<ASTNode>& node, const std::string& leftSteps, const std::string& rightSteps, const std::shared_ptr<ASTNode>& result);
    static std::string constantMultipleRule(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& constantNode, const std::shared_ptr<ASTNode>& innerNode, const std::string& innerSteps, const std::shared_ptr<ASTNode>& result);
    static std::string ibpRule(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& u, const std::shared_ptr<ASTNode>& du, const std::shared_ptr<ASTNode>& dv, const std::shared_ptr<ASTNode>& v, const std::string& vSteps, const std::shared_ptr<ASTNode>& du_v, const std::string& remainderSteps, const std::shared_ptr<ASTNode>& result);
    static std::string standardLookup(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& result);
    static std::string reverseChainProduct(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& inside, const std::shared_ptr<ASTNode>& coeffNode, const std::shared_ptr<ASTNode>& result);
    static std::string reverseChainShift(const std::shared_ptr<ASTNode>& node, const std::shared_ptr<ASTNode>& inside, double scaleFactor, const std::shared_ptr<ASTNode>& result);
};

#endif
