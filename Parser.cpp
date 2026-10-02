#include "Parser.hpp"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

Token Parser::peek() const {
    if (pos >= tokens.size()) return Token{TokenType::END, ""};
    return tokens[pos];
}

Token Parser::advance() {
    if (pos >= tokens.size()) return Token{TokenType::END, ""};
    return tokens[pos++];
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

// Hierarchical Level 1: Handles Addition and Subtraction (+, -)
std::shared_ptr<ASTNode> Parser::parseExpression() {
    auto left = parseTerm();

    while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
        Token op = advance();
        NodeType nt = (op.type == TokenType::PLUS) ? NodeType::ADD : NodeType::SUBTRACT;
        auto binaryNode = std::make_shared<ASTNode>(nt);
        binaryNode->children.push_back(left);
        binaryNode->children.push_back(parseTerm());
        left = binaryNode;
    }
    return left;
}

// Hierarchical Level 2: Handles Multiplication and Division (*, /)
std::shared_ptr<ASTNode> Parser::parseTerm() {
    auto left = parseFactor();

    while (peek().type == TokenType::MULTIPLY || peek().type == TokenType::DIVIDE) {
        Token op = advance();
        NodeType nt = (op.type == TokenType::MULTIPLY) ? NodeType::MULTIPLY : NodeType::DIVIDE;
        auto binaryNode = std::make_shared<ASTNode>(nt);
        binaryNode->children.push_back(left);
        binaryNode->children.push_back(parseFactor());
        left = binaryNode;
    }
    return left;
}

// Hierarchical Level 3: Handles Exponents (^)
std::shared_ptr<ASTNode> Parser::parseFactor() {
    auto left = parsePrimary();

    if (match(TokenType::POWER)) {
        auto powerNode = std::make_shared<ASTNode>(NodeType::POWER);
        powerNode->children.push_back(left);
        powerNode->children.push_back(parseFactor()); // Right-associative exponentiation
        left = powerNode;
    }
    return left;
}

// Hierarchical Level 4: Handles Base Elements (Numbers, Variables, Functions, Parentheses)
std::shared_ptr<ASTNode> Parser::parsePrimary() {
    Token current = peek();

    if (match(TokenType::NUMBER)) {
        return std::make_shared<ASTNode>(std::stod(current.value));
    }
    
    if (match(TokenType::VARIABLE)) {
        return std::make_shared<ASTNode>(NodeType::VARIABLE, current.value);
    }
    
    // Process all 15 trigonometric, inverse trigonometric, exponential, and log functions
    if (match(TokenType::FUNCTION)) {
        NodeType ft = NodeType::COS; // Default fallback node assignment
        
        if (current.value == "sin")          ft = NodeType::SIN;
        else if (current.value == "cos")     ft = NodeType::COS;
        else if (current.value == "tan")     ft = NodeType::TAN;
        else if (current.value == "sec")     ft = NodeType::SEC;
        else if (current.value == "cosec")   ft = NodeType::COSEC;
        else if (current.value == "cot")     ft = NodeType::COT;
        else if (current.value == "arcsin")  ft = NodeType::ARCSIN;
        else if (current.value == "arccos")  ft = NodeType::ARCCOS;
        else if (current.value == "arctan")  ft = NodeType::ARCTAN;
        else if (current.value == "arcsec")  ft = NodeType::ARCSEC;
        else if (current.value == "arccosec") ft = NodeType::ARCCOSEC;
        else if (current.value == "arccot")  ft = NodeType::ARCCOT;
        else if (current.value == "exp")     ft = NodeType::EXP;
        else if (current.value == "ln")      ft = NodeType::LN;
        else if (current.value == "log")     ft = NodeType::LOG;
        
        auto funcNode = std::make_shared<ASTNode>(ft);
        
        if (!match(TokenType::LPAREN)) {
            throw std::runtime_error("Expected '(' after function keyword");
        }
        funcNode->children.push_back(parseExpression());
        if (!match(TokenType::RPAREN)) {
            throw std::runtime_error("Expected ')' after function argument");
        }
        return funcNode;
    }

    if (match(TokenType::LPAREN)) {
        auto expr = parseExpression();
        if (!match(TokenType::RPAREN)) {
            throw std::runtime_error("Mismatched parentheses");
        }
        return expr;
    }

    throw std::runtime_error("Unexpected token encountered during parsing: " + current.value);
}

std::shared_ptr<ASTNode> Parser::parse() {
    return parseExpression();
}
