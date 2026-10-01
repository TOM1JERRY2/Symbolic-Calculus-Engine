#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include <memory>
#include "ASTnode.hpp"

class Integrator {
public:
    // Takes an expression tree and returns a brand new tree representing its integral
    static std::shared_ptr<ASTNode> integrate(const std::shared_ptr<ASTNode>& node);
};

#endif // INTEGRATOR_HPP
