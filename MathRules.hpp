#ifndef MATH_RULES_HPP
#define MATH_RULES_HPP

#include <memory>
#include "ASTNode.hpp"

class MathRules {
public:
    // Dynamically injects the inner 'inside' node argument into the textbook formulas
    static std::shared_ptr<ASTNode> lookupIntegral(NodeType type, std::shared_ptr<ASTNode> inside) {
        if (!inside) return nullptr;

        switch (type) {
            // 1. ∫ sin(u) du = -1 * cos(u)
            case NodeType::SIN: {
                auto mult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
                mult->children.push_back(std::make_shared<ASTNode>(-1.0));
                auto cosNode = std::make_shared<ASTNode>(NodeType::COS);
                cosNode->children.push_back(inside);
                mult->children.push_back(cosNode);
                return mult;
            }
            // 2. ∫ cos(u) du = sin(u)
            case NodeType::COS: {
                auto sinNode = std::make_shared<ASTNode>(NodeType::SIN);
                sinNode->children.push_back(inside);
                return sinNode;
            }
            // 3. ∫ tan(u) du = ln(sec(u))
            case NodeType::TAN: {
                auto lnNode = std::make_shared<ASTNode>(NodeType::LN);
                auto secNode = std::make_shared<ASTNode>(NodeType::SEC);
                secNode->children.push_back(inside);
                lnNode->children.push_back(secNode);
                return lnNode;
            }
            // 4. ∫ sec(u) du = ln(sec(u) + tan(u))
            case NodeType::SEC: {
                auto lnNode = std::make_shared<ASTNode>(NodeType::LN);
                auto addNode = std::make_shared<ASTNode>(NodeType::ADD);
                auto secNode = std::make_shared<ASTNode>(NodeType::SEC);
                auto tanNode = std::make_shared<ASTNode>(NodeType::TAN);
                
                secNode->children.push_back(inside);
                tanNode->children.push_back(inside);
                
                addNode->children.push_back(secNode);
                addNode->children.push_back(tanNode);
                lnNode->children.push_back(addNode);
                return lnNode;
            }
            // 5. ∫ cosec(u) du = -1 * ln(cosec(u) + cot(u))
            case NodeType::COSEC: {
                auto mult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
                auto lnNode = std::make_shared<ASTNode>(NodeType::LN);
                auto addNode = std::make_shared<ASTNode>(NodeType::ADD);
                auto cosecNode = std::make_shared<ASTNode>(NodeType::COSEC);
                auto cotNode = std::make_shared<ASTNode>(NodeType::COT);
                
                cosecNode->children.push_back(inside);
                cotNode->children.push_back(inside);
                
                addNode->children.push_back(cosecNode);
                addNode->children.push_back(cotNode);
                lnNode->children.push_back(addNode);
                
                mult->children.push_back(std::make_shared<ASTNode>(-1.0));
                mult->children.push_back(lnNode);
                return mult;
            }
            // 6. ∫ cot(u) du = ln(sin(u))
            case NodeType::COT: {
                auto lnNode = std::make_shared<ASTNode>(NodeType::LN);
                auto sinNode = std::make_shared<ASTNode>(NodeType::SIN);
                sinNode->children.push_back(inside);
                lnNode->children.push_back(sinNode);
                return lnNode;
            }
            // 7. ∫ exp(u) du = exp(u)
            case NodeType::EXP: {
                auto expNode = std::make_shared<ASTNode>(NodeType::EXP);
                expNode->children.push_back(inside);
                return expNode;
            }
            // 8. ∫ ln(u) du = u * ln(u) - u
            case NodeType::LN: {
                auto subNode = std::make_shared<ASTNode>(NodeType::SUBTRACT);
                auto multNode = std::make_shared<ASTNode>(NodeType::MULTIPLY);
                auto lnNode = std::make_shared<ASTNode>(NodeType::LN);
                
                lnNode->children.push_back(inside);
                multNode->children.push_back(inside);
                multNode->children.push_back(lnNode);
                
                subNode->children.push_back(multNode);
                subNode->children.push_back(inside);
                return subNode;
            }
            default:
                return nullptr;
        }
    }
};

#endif // MATH_RULES_HPP
