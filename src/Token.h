#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    Keyword,
    Identifier,
    Number,
    StringLiteral,
    Symbol,
    EndOfFile // Changed from EOF to EndOfFile
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};

#endif // TOKEN_H
