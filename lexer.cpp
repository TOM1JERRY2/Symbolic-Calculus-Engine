#include "Lexer.hpp"
#include <cctype>

Lexer::Lexer(const std::string& input) : src(input), pos(0) {}

char Lexer::peek() const {
    if (pos >= src.length()) return '\0';
    return src[pos];
}

char Lexer::advance() {
    if (pos >= src.length()) return '\0';
    return src[pos++];
}

void Lexer::skipWhitespace() {
    while (pos < src.length() && std::isspace(src[pos])) {
        pos++;
    }
}

Token Lexer::readNumber() {
    std::string res = "";
    while (pos < src.length() && (std::isdigit(src[pos]) || src[pos] == '.')) {
        res += advance();
    }
    return Token{TokenType::NUMBER, res};
}

Token Lexer::readIdentifier() {
    std::string res = "";
    while (pos < src.length() && std::isalpha(src[pos])) {
        res += advance();
    }
    
    // Check if the string matches a known math function
    if (res == "sin" || res == "cos" || res == "ln" ||
    res == "tan" || res == "cot" || res == "sec" || res == "cosec"
    || res == "arccot" || res == "arccosec" || res == "arccos" || res == "arcsin"
    || res == "arctan" || res == "arcsec" || res == "exp" || res == "ln" || res == "log") {
        return Token{TokenType::FUNCTION, res};
    }
    return Token{TokenType::VARIABLE, res};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (pos < src.length()) {
        skipWhitespace();
        if (pos >= src.length()) break;
        
        char current = peek();
        
        if (std::isdigit(current) || current == '.') {
            tokens.push_back(readNumber());
        } else if (std::isalpha(current)) {
            tokens.push_back(readIdentifier());
        } else {
            advance(); // Consume operator character
            switch (current) {
                case '+': tokens.push_back(Token{TokenType::PLUS, "+"}); break;
                case '-': tokens.push_back(Token{TokenType::MINUS, "-"}); break;
                case '*': tokens.push_back(Token{TokenType::MULTIPLY, "*"}); break;
                case '/': tokens.push_back(Token{TokenType::DIVIDE, "/"}); break;
                case '^': tokens.push_back(Token{TokenType::POWER, "^"}); break;
                case '(': tokens.push_back(Token{TokenType::LPAREN, "("}); break;
                case ')': tokens.push_back(Token{TokenType::RPAREN, ")"}); break;
                default: break; // Ignore unknown characters gently
            }
        }
    }
    tokens.push_back(Token{TokenType::END, ""});
    return tokens;
}
