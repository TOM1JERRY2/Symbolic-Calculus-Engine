#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

// The universal tags for your calculus engine grammar
enum class TokenType {
    NUMBER,
    VARIABLE,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER,
    LPAREN,   
    RPAREN,   
    FUNCTION, 
    END       
};

struct Token {
    TokenType type;
    std::string value;
};

#endif // TOKEN_HPP
