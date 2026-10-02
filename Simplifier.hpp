#ifndef SIMPLIFIER_HPP
#define SIMPLIFIER_HPP

#include <memory>
#include <cmath>
#include "ASTNode.hpp"

class Simplifier {
public:
    static std::shared_ptr<ASTNode> simplify(std::shared_ptr<ASTNode> node) {
        if (!node) return nullptr;

        // 1. Recursively clean up children branches first
        for (size_t i = 0; i < node->children.size(); ++i) {
            node->children[i] = simplify(node->children[i]);
        }

        // 2. Multiplication Cleanups
        if (node->type == NodeType::MULTIPLY && node->children.size() == 2) {
            auto left = node->children[0];
            auto right = node->children[1];

            // 1 * anything = anything
            if (left->type == NodeType::NUMBER && left->value == 1.0) return right;
            if (right->type == NodeType::NUMBER && right->value == 1.0) return left;

            // 0 * anything = 0
            if ((left->type == NodeType::NUMBER && left->value == 0.0) || 
                (right->type == NodeType::NUMBER && right->value == 0.0)) {
                return std::make_shared<ASTNode>(0.0);
            }

            // Collapse raw numbers: constant * constant (e.g., 3 * 0.5 -> 1.5)
            if (left->type == NodeType::NUMBER && right->type == NodeType::NUMBER) {
                double val = left->value * right->value;
                if (std::abs(val - 1.0) < 0.0001) val = 1.0;
                return std::make_shared<ASTNode>(val);
            }

            // Advanced Structuring: Pull out nested negative constants (e.g., x * (-1 * cos(x)))
            // Rewrites it to: (-1 * (x * cos(x)))
            if (right->type == NodeType::MULTIPLY && right->children.size() == 2) {
                auto rLeft = right->children[0];
                auto rRight = right->children[1];
                if (rLeft->type == NodeType::NUMBER && rLeft->value == -1.0) {
                    auto innerMult = std::make_shared<ASTNode>(NodeType::MULTIPLY);
                    innerMult->children.push_back(left);
                    innerMult->children.push_back(rRight);
                    
                    auto outerNegative = std::make_shared<ASTNode>(NodeType::MULTIPLY);
                    outerNegative->children.push_back(std::make_shared<ASTNode>(-1.0));
                    outerNegative->children.push_back(simplify(innerMult));
                    return outerNegative;
                }
            }
        }

        // 3. Subtraction Cleanups
        if (node->type == NodeType::SUBTRACT && node->children.size() == 2) {
            auto left = node->children[0];
            auto right = node->children[1];

            // double negative cleanup: something - (-1 * anything) -> something + anything
            if (right->type == NodeType::MULTIPLY && right->children.size() == 2) {
                auto rLeft = right->children[0];
                auto rRight = right->children[1];
                if (rLeft->type == NodeType::NUMBER && rLeft->value == -1.0) {
                    auto addNode = std::make_shared<ASTNode>(NodeType::ADD);
                    addNode->children.push_back(left);
                    addNode->children.push_back(rRight);
                    return addNode;
                }
            }
        }

        return node;
    }
};

#endif // SIMPLIFIER_HPP
