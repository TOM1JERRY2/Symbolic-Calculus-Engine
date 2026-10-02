#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include "ASTNode.hpp"
#include "lexer.hpp"   // Clean lowercase inclusion

// CRITICAL FIX: Explicit forward map so the compiler knows these types exist
enum class TokenType;
struct Token;

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::shared_ptr<ASTNode> parse();

private:
    std::vector<Token> tokens;
    size_t pos;

    Token peek() const;
    Token advance();
    bool match(TokenType type);

    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseTerm();
    std::shared_ptr<ASTNode> parseFactor();
    std::shared_ptr<ASTNode> parsePrimary();
};

#endif // PARSER_HPP
