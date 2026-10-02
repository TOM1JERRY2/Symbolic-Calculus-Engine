#include "Integrator.hpp"
#include "MathRules.hpp"
#include "ExplanationBuilder.hpp"
#include "Printer.hpp"
#include <stdexcept>
#include <sstream>

// Helper to determine LIATE priority score (Lower score = higher integration priority for 'u')
int getLIATEPriority(NodeType type) {
    switch (type) {
        case NodeType::LN:
        case NodeType::LOG:
            return 1; // L - Logarithmic
        case NodeType::ARCSIN:
        case NodeType::ARCCOS:
        case NodeType::ARCTAN:
        case NodeType::ARCSEC:
        case NodeType::ARCCOSEC:
        case NodeType::ARCCOT:
            return 2; // I - Inverse Trig
        case NodeType::VARIABLE:
        case NodeType::POWER:
            return 3; // A - Algebraic
        case NodeType::SIN:
        case NodeType::COS:
        case NodeType::TAN:
        case NodeType::SEC:
        case NodeType::COSEC:
        case NodeType::COT:
            return 4; // T - Trigonometric
        case NodeType::EXP:
            return 5; // E - Exponential
        default:
            return 6; // Unknown fallback
    }
}

IntegrationResult Integrator::integrate(const std::shared_ptr<ASTNode>& node) {
    if (!node) return { nullptr, "" };

    // Rule 1: Constant numbers
    if (node->type == NodeType::NUMBER) {
        auto result = std::make_shared<ASTNode>(NodeType::MULTIPLY);
        result->children.push_back(std::make_shared<ASTNode>(node->value)); 
        result->children.push_back(std::make_shared<ASTNode>(NodeType::VARIABLE, "x"));
        
        return { result, ExplanationBuilder::constantRule(node, result) };
    }

    // Rule 2: Single variable (x)
    if (node->type == NodeType::VARIABLE) {
        auto mult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
        auto power = std::make_shared<ASTNode>(NodeType::POWER);
        power->children.push_back(std::make_shared<ASTNode>(NodeType::VARIABLE, node->name));
        power->children.push_back(std::make_shared<ASTNode>(2.0)); 
        mult->children.push_back(std::make_shared<ASTNode>(0.5)); 
        mult->children.push_back(power);
        
        return { mult, ExplanationBuilder::variableRule(node, mult) };
    }

    // Rule 3: Linearity rule (+)
    if (node->type == NodeType::ADD) {
        auto leftResult = integrate(node->children[0]);
        auto rightResult = integrate(node->children[1]);
        
        auto result = std::make_shared<ASTNode>(NodeType::ADD);
        result->children.push_back(leftResult.ast);
        result->children.push_back(rightResult.ast);
        
        std::string steps = ExplanationBuilder::linearityRule(node, leftResult.steps, rightResult.steps, result);
        return { result, steps };
    }

    // Rule 4: Multiplication (Handling Constants & LIATE Automated IBP)
    if (node->type == NodeType::MULTIPLY) {
        auto leftChild = node->children[0];
        auto rightChild = node->children[1];

        // Constant Factor Pullout (Left Constant)
        if (leftChild->type == NodeType::NUMBER) {
            auto baseIntegral = integrate(rightChild);
            auto result = std::make_shared<ASTNode>(NodeType::MULTIPLY);
            result->children.push_back(std::make_shared<ASTNode>(leftChild->value));
            result->children.push_back(baseIntegral.ast);
            
            std::string steps = ExplanationBuilder::constantMultipleRule(node, leftChild, rightChild, baseIntegral.steps, result);
            return { result, steps };
        }

        // Constant Factor Pullout (Right Constant)
        if (rightChild->type == NodeType::NUMBER) {
            auto baseIntegral = integrate(leftChild);
            auto result = std::make_shared<ASTNode>(NodeType::MULTIPLY);
            result->children.push_back(std::make_shared<ASTNode>(rightChild->value));
            result->children.push_back(baseIntegral.ast);
            
            std::string steps = ExplanationBuilder::constantMultipleRule(node, rightChild, leftChild, baseIntegral.steps, result);
            return { result, steps };
        }

        // --- LIATE AUTOMATION SORTING LAYER ---
        std::shared_ptr<ASTNode> u = leftChild;
        std::shared_ptr<ASTNode> dv = rightChild;
        std::string sortLog = "";

        int leftPriority = getLIATEPriority(leftChild->type);
        int rightPriority = getLIATEPriority(rightChild->type);

        // If right child holds a higher priority on LIATE, swap them automatically
        if (rightPriority < leftPriority) {
            u = rightChild;
            dv = leftChild;
            sortLog = "  ■ [LIATE Auto-Sort]: Rearranged expression order for Integration by Parts into standard form: ∫ " 
                      + Printer::toFormulaString(u) + " * " + Printer::toFormulaString(dv) + " dx\n\n";
        }

        // Execute Integration by Parts using the LIATE sorted variables
        if (u->type == NodeType::VARIABLE || u->type == NodeType::POWER || u->type == NodeType::LN) {
            std::shared_ptr<ASTNode> du;
            if (u->type == NodeType::VARIABLE) {
                du = std::make_shared<ASTNode>(1.0);
            } else {
                du = std::make_shared<ASTNode>(1.0); // Simple base fallback case
            }
            
            auto vResult = integrate(dv);       
            auto v = vResult.ast;

            auto u_times_v = std::make_shared<ASTNode>(NodeType::MULTIPLY);
            u_times_v->children.push_back(u);
            u_times_v->children.push_back(v);

            auto du_times_v = std::make_shared<ASTNode>(NodeType::MULTIPLY);
            du_times_v->children.push_back(du);
            du_times_v->children.push_back(v);

            auto remainingIntegral = integrate(du_times_v);
            auto result = std::make_shared<ASTNode>(NodeType::SUBTRACT);
            result->children.push_back(u_times_v);
            result->children.push_back(remainingIntegral.ast);
            
            std::string steps = sortLog + ExplanationBuilder::ibpRule(node, u, du, dv, v, vResult.steps, du_times_v, remainingIntegral.steps, result);
            return { result, steps };
        }

        throw std::runtime_error("Product sequence is too complex for basic Heuristic LIATE IBP!");
    }

    // Rule 5: Power Rule (e.g., x^3)
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
            
            return { mult, ExplanationBuilder::powerRule(node, exponent, mult) };
        }
    }

    // Advanced Composite Argument Router Pass (Reverse Chain Rule matches)
    if (!node->children.empty()) {
        auto inside = node->children[0];
        
        if (inside->type == NodeType::VARIABLE) {
            auto basicRuleResult = MathRules::lookupIntegral(node->type, inside);
            if (basicRuleResult) {
                return { basicRuleResult, ExplanationBuilder::standardLookup(node, basicRuleResult) };
            }
        }
        
        if (inside->type == NodeType::MULTIPLY && inside->children.size() == 2) {
            auto coeffNode = inside->children[0];
            if (coeffNode->type == NodeType::NUMBER) {
                auto outerIntegral = MathRules::lookupIntegral(node->type, inside);
                if (outerIntegral) {
                    auto result = std::make_shared<ASTNode>(NodeType::MULTIPLY);
                    result->children.push_back(std::make_shared<ASTNode>(1.0 / coeffNode->value));
                    result->children.push_back(outerIntegral);
                    
                    return { result, ExplanationBuilder::reverseChainProduct(node, inside, coeffNode, result) };
                }
            }
        }
        
        if ((inside->type == NodeType::ADD || inside->type == NodeType::SUBTRACT) && inside->children.size() == 2) {
            auto leftPart = inside->children[0];
            double a = 1.0;
            bool isChainRuleApplicable = false;

            if (leftPart->type == NodeType::MULTIPLY && leftPart->children.size() == 2) {
                if (leftPart->children[0]->type == NodeType::NUMBER) {
                    a = leftPart->children[0]->value;
                    isChainRuleApplicable = true;
                }
            }
            else if (leftPart->type == NodeType::VARIABLE) {
                a = 1.0;
                isChainRuleApplicable = true;
            }

            if (isChainRuleApplicable) {
                auto outerIntegral = MathRules::lookupIntegral(node->type, inside);
                if (outerIntegral) {
                    auto result = std::make_shared<ASTNode>(NodeType::MULTIPLY);
                    result->children.push_back(std::make_shared<ASTNode>(1.0 / a));
                    result->children.push_back(outerIntegral);
                    
                    return { result, ExplanationBuilder::reverseChainShift(node, inside, a, result) };
                }
            }
        }
    }

    throw std::runtime_error("Expression format too complex or rule mapping missing!");
}
