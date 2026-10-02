#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include <memory>
#include <string>
#include "ASTNode.hpp"

// Object mapping containing calculation steps bundled alongside math results
struct IntegrationResult {
    std::shared_ptr<ASTNode> ast;
    std::string steps;
};

class Integrator {
public:
    // Takes an expression tree and returns a structure containing both the integrated 
    // result tree and the descriptive step-by-step logs.
    static IntegrationResult integrate(const std::shared_ptr<ASTNode>& node);
};

#endif // INTEGRATOR_HPP
