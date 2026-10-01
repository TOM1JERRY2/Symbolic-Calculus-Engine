#include "Integrator.hpp"
#include "MathRules.hpp"
#include <stdexcept>

std::shared_ptr<ASTNode> Integrator::integrate(const std::shared_ptr<ASTNode>& node) {
    if (!node) return nullptr;

    // Rule 1: Constant numbers
    if (node->type == NodeType::NUMBER) {
        auto mult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
        mult->children.push_back(std::make_shared<ASTNode>(node->value)); 
        mult->children.push_back(std::make_shared<ASTNode>(NodeType::VARIABLE, "x"));
        return mult;
    }

    // Rule 2: Single variable variable
    if (node->type == NodeType::VARIABLE) {
        auto mult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
        auto power = std::make_shared<ASTNode>(NodeType::POWER);
        power->children.push_back(std::make_shared<ASTNode>(NodeType::VARIABLE, node->name));
        power->children.push_back(std::make_shared<ASTNode>(2.0)); 
        mult->children.push_back(std::make_shared<ASTNode>(0.5)); 
        mult->children.push_back(power);
        return mult;
    }

    // Rule 3: Linearity rule (+)
    if (node->type == NodeType::ADD) {
        auto result = std::make_shared<ASTNode>(NodeType::ADD);
        result->children.push_back(integrate(node->children[0]));
        result->children.push_back(integrate(node->children[1]));
        return result;
    }

    // Rule 4: Multiplication (Handling Constants AND Integration by Parts)
    if (node->type == NodeType::MULTIPLY) {
        auto leftChild = node->children[0];
        auto rightChild = node->children[1];

        if (leftChild->type == NodeType::NUMBER) {
            double constantValue = leftChild->value;
            auto baseIntegral = integrate(rightChild);
            auto mult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
            mult->children.push_back(std::make_shared<ASTNode>(constantValue));
            mult->children.push_back(baseIntegral);
            return mult;
        }

        if (rightChild->type == NodeType::NUMBER) {
            double constantValue = rightChild->value;
            auto baseIntegral = integrate(leftChild);
            auto mult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
            mult->children.push_back(std::make_shared<ASTNode>(constantValue));
            mult->children.push_back(baseIntegral);
            return mult;
        }

        // Heuristic IBP handling expressions like: x * sin(2 * x)
        if (leftChild->type == NodeType::VARIABLE) {
            auto u = leftChild;           
            auto dv = rightChild;         
            auto du = std::make_shared<ASTNode>(1.0); 
            auto v = integrate(dv);       

            auto u_times_v = std::make_shared<ASTNode>(NodeType::MULTIPLY);
            u_times_v->children.push_back(u);
            u_times_v->children.push_back(v);

            auto du_times_v = std::make_shared<ASTNode>(NodeType::MULTIPLY);
            du_times_v->children.push_back(du);
            du_times_v->children.push_back(v);

            auto remainingIntegral = integrate(du_times_v);
            auto ibpResult = std::make_shared<ASTNode>(NodeType::SUBTRACT);
            ibpResult->children.push_back(u_times_v);
            ibpResult->children.push_back(remainingIntegral);
            return ibpResult;
        }

        throw std::runtime_error("Product is too complex for basic Heuristic IBP!");
    }

    // Rule 5: Power Rule
    if (node->type == NodeType::POWER) {
        auto base = node->children[0];
        auto exponent = node->children[1];
        
        if (base->type == NodeType::VARIABLE && exponent->type == NodeType::NUMBER) {
            double n = exponent->value;
            auto mult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
            auto power = std::make_shared<ASTNode>(NodeType::POWER);
            power->children.push_back(std::make_shared<ASTNode>(NodeType::VARIABLE, base->name));
            power->children.push_back(std::make_shared<ASTNode>(n + 1.0)); 
            mult->children.push_back(std::make_shared<ASTNode>(1.0 / (n + 1.0))); 
            mult->children.push_back(power);
            return mult;
        }
    }

    // Advanced Composite Argument Router Pass (Supports x, a*x, and a*x + b)
    if (!node->children.empty()) {
        auto inside = node->children[0];
        
        // Scenario A: Bare Variable -> sin(x)
        if (inside->type == NodeType::VARIABLE) {
            auto basicRuleResult = MathRules::lookupIntegral(node->type, inside);
            if (basicRuleResult) return basicRuleResult;
        }
        
        // Scenario B: Linear Product Composite -> sin(2 * x) or sin(a * x)
        if (inside->type == NodeType::MULTIPLY && inside->children.size() == 2) {
            auto coeffNode = inside->children[0];
            if (coeffNode->type == NodeType::NUMBER) {
                double a = coeffNode->value;
                auto outerIntegral = MathRules::lookupIntegral(node->type, inside);
                if (outerIntegral) {
                    auto chainMult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
                    chainMult->children.push_back(std::make_shared<ASTNode>(1.0 / a));
                    chainMult->children.push_back(outerIntegral);
                    return chainMult;
                }
            }
        }
        
        // Scenario C: Full Polynomial Slope-Intercept Composite -> sin(2 * x + 5) or sin(a * x + b)
        if ((inside->type == NodeType::ADD || inside->type == NodeType::SUBTRACT) && inside->children.size() == 2) {
            auto leftPart = inside->children[0];
            double a = 1.0;
            bool isChainRuleApplicable = false;

            // Matches form: (a * x) + b
            if (leftPart->type == NodeType::MULTIPLY && leftPart->children.size() == 2) {
                auto coeff = leftPart->children[0];
                if (coeff->type == NodeType::NUMBER) {
                    a = coeff->value;
                    isChainRuleApplicable = true;
                }
            }
            // Matches form: x + b
            else if (leftPart->type == NodeType::VARIABLE) {
                a = 1.0;
                isChainRuleApplicable = true;
            }

            if (isChainRuleApplicable) {
                auto outerIntegral = MathRules::lookupIntegral(node->type, inside);
                if (outerIntegral) {
                    auto chainMult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
                    chainMult->children.push_back(std::make_shared<ASTNode>(1.0 / a));
                    chainMult->children.push_back(outerIntegral);
                    return chainMult;
                }
            }
        }
    }

    throw std::runtime_error("Expression format too complex or rule mapping missing!");
}
