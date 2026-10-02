#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

// 1. Core Grammar Tags (Note the Capital T and T)
enum class TokenType {
    NUMBER,
    VARIABLE,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER,
    LPAREN,   // (
    RPAREN,   // )
    FUNCTION, // sin, cos, ln
    END       // End of input string
};

// 2. Token Struct (Note the Capital T)
struct Token {
    TokenType type;       // Matches TokenType exactly
    std::string value;
};

// 3. Lexer Class
class Lexer {
public:
    explicit Lexer(const std::string& input);
    std::vector<Token> tokenize(); // Matches Token exactly

private:
    std::string src;
    size_t pos;

    char peek() const;
    char advance();
    void skipWhitespace();
    Token readNumber();
    Token readIdentifier();
};

#endif // LEXER_HPP
